#include "elfGenerate.h"

#include "context.h"

#include "core/DSL.h"

#include "elf.h"
#include <assert.h>
#include <string.h>

const Elf64_Addr START_VIRTUAL_ADDR  = 0x400000;
const Elf64_Addr  START_PHYSICAL_ADDR = START_VIRTUAL_ADDR;

const Elf64_Xword MEM_PAGE_SIZE       = 0x1000;

const size_t DATA_SECTION_SIZE        = 16;

static void genElfHeader(codeGenContext* context);
static void genProgramHeaders(codeGenContext* context);
static void genSectionTable(codeGenContext* context);

enum segment_t{
    SEG_LOAD_RX = 0,
    SEG_LOAD_RW,
    SEG_INTERP,
    SEG_DYNAMIC,

    SEG_COUNT
};

struct Segment{
    segment_t type;

    Elf64_Phdr phdr;

    size_t file_offset;
    size_t file_size;

    size_t vaddr;
    size_t mem_size;

    uint32_t flags;
    size_t align;
};

static Segment segments[SEG_COUNT];

enum section_t{
    SEC_NULL = 0,
    SEC_TEXT,
    SEC_DATA,
    SEC_INTERP,
    SEC_DYNAMIC,
    SEC_DYNSTR,
    SEC_SHSTRTAB,

    SEC_COUNT
};

struct StringTable{
    char   buf[1024];
    size_t size;
};

struct Section{
    section_t type;

    const char* name;

    Elf64_Shdr shdr;

    uint8_t* data;
    size_t   size;

    size_t file_offset;
    size_t vaddr;

    size_t name_offset;

    segment_t segment;
};

static Section sections[SEC_COUNT];

static StringTable shstrtab;
static StringTable dynstr;

static Elf64_Dyn dynamicEntries[4];

static void strtabInit(StringTable* tab);
static size_t strtabAdd(StringTable* tab, const char* str);
static void initSections();
static void buildShStrTab();
static void layoutSections(size_t phnum);
static void buildSectionHeaders();
static Section* getSection(section_t sec);

static void initSegments();
static void buildSegments();

static void buildDynStr();
static void buildDynamicSection();

void genPrologueX86Elf(codeGenContext* context){
    assert(context);

    initSections();
    initSegments();
    
    sections[SEC_TEXT].data    = BIN_BUFFER_DATA(_CONTEXT_ELF_CODE_BUFFER(context));
    sections[SEC_TEXT].size    = BIN_BUFFER_SIZE(_CONTEXT_ELF_CODE_BUFFER(context)) * sizeof(uint8_t);
    sections[SEC_TEXT].segment = SEG_LOAD_RX;
    
    static uint8_t raw_data[]  = {'%', 'd', '\0'};
    sections[SEC_DATA].data    = raw_data;
    sections[SEC_DATA].size    = sizeof(raw_data);
    sections[SEC_DATA].segment = SEG_LOAD_RW;
    
    static uint8_t interp[]      = "/lib64/ld-linux-x86-64.so.2";
    sections[SEC_INTERP].data    = interp;
    sections[SEC_INTERP].size    = sizeof(interp);
    sections[SEC_INTERP].segment = SEG_INTERP;

    sections[SEC_DYNAMIC].size   = sizeof(dynamicEntries);
    sections[SEC_DYNAMIC].segment = SEG_DYNAMIC;

    buildShStrTab();
    buildDynStr();
    layoutSections(SEG_COUNT);

    buildDynamicSection();

    buildSegments();
    buildSectionHeaders();

    genElfHeader(context);
    genProgramHeaders(context);

    fwrite(sections[SEC_TEXT].data,     sections[SEC_TEXT].size,     1, _CONTEXT_FILE_PTR(context));
    fwrite(sections[SEC_DATA].data,     sections[SEC_DATA].size,     1, _CONTEXT_FILE_PTR(context));
    fwrite(sections[SEC_INTERP].data,   sections[SEC_INTERP].size,   1, _CONTEXT_FILE_PTR(context));
    fwrite(sections[SEC_DYNAMIC].data,  sections[SEC_DYNAMIC].size,  1, _CONTEXT_FILE_PTR(context));
    fwrite(sections[SEC_DYNSTR].data,   sections[SEC_DYNSTR].size,   1, _CONTEXT_FILE_PTR(context));
    fwrite(sections[SEC_SHSTRTAB].data, sections[SEC_SHSTRTAB].size, 1, _CONTEXT_FILE_PTR(context));

    genSectionTable(context);
}

static void genElfHeader(codeGenContext* context){
    assert(context);

    Elf64_Ehdr ehdr;
    memset(&ehdr, 0, sizeof(ehdr));

    ehdr.e_ident[EI_MAG0] = ELFMAG0;          
    ehdr.e_ident[EI_MAG1] = ELFMAG1;          /// memcpy
    ehdr.e_ident[EI_MAG2] = ELFMAG2;          
    ehdr.e_ident[EI_MAG3] = ELFMAG3;          

    ehdr.e_ident[EI_CLASS]   = ELFCLASS64;      
    ehdr.e_ident[EI_DATA]    = ELFDATA2LSB;      
    ehdr.e_ident[EI_VERSION] = EV_CURRENT;    
    ehdr.e_ident[EI_OSABI]   = ELFOSABI_NONE; 

    ehdr.e_type    = ET_EXEC;                    
    ehdr.e_machine = EM_X86_64;               
    ehdr.e_version = EV_CURRENT;

    ehdr.e_entry  = sections[SEC_TEXT].vaddr;
    ehdr.e_ehsize = sizeof(Elf64_Ehdr);;    

    ehdr.e_phoff     = sizeof(Elf64_Ehdr); 
    ehdr.e_phentsize = sizeof(Elf64_Phdr);;
    ehdr.e_phnum     = SEG_COUNT;               

    ehdr.e_shoff = sections[SEC_SHSTRTAB].file_offset + sections[SEC_SHSTRTAB].size;
    ehdr.e_shentsize = sizeof(Elf64_Shdr);
    ehdr.e_shnum     = SEC_COUNT; 
    ehdr.e_shstrndx  = SEC_SHSTRTAB;  

    fwrite(&ehdr, sizeof(ehdr), 1, _CONTEXT_FILE_PTR(context));
}

static void genProgramHeaders(codeGenContext* context){
    assert(context);

    segments[SEG_LOAD_RX].phdr.p_type  = PT_LOAD;
    segments[SEG_LOAD_RX].phdr.p_flags = PF_R | PF_X;

    segments[SEG_LOAD_RW].phdr.p_type  = PT_LOAD;
    segments[SEG_LOAD_RW].phdr.p_flags = PF_R | PF_W;

    segments[SEG_INTERP].phdr.p_type   = PT_INTERP;
    segments[SEG_INTERP].phdr.p_flags  = PF_R;

    segments[SEG_DYNAMIC].phdr.p_type  = PT_DYNAMIC;
    segments[SEG_DYNAMIC].phdr.p_flags = PF_R | PF_W;;

    for(size_t i = 0; i < SEG_COUNT; i++){
        segments[i].phdr.p_offset = segments[i].file_offset;
        segments[i].phdr.p_vaddr  = segments[i].vaddr;
        segments[i].phdr.p_paddr  = segments[i].vaddr;
        segments[i].phdr.p_filesz = segments[i].file_size;
        segments[i].phdr.p_memsz  = segments[i].mem_size;
        segments[i].phdr.p_align  = MEM_PAGE_SIZE;

        if(segments[i].phdr.p_type == 0)
            continue;

        fwrite(&segments[i].phdr, sizeof(Elf64_Phdr), 1, _CONTEXT_FILE_PTR(context));
    }
    
}

static void genSectionTable(codeGenContext* context){
    assert(context);

    Elf64_Shdr shdr[SEC_COUNT] = {0};

    for(size_t i = 0; i < SEC_COUNT; i++){
        shdr[i] = sections[i].shdr;
    }

    fwrite(shdr, sizeof(Elf64_Shdr), SEC_COUNT, _CONTEXT_FILE_PTR(context));
}

static void initSegments(){
    memset(segments, 0, sizeof(segments));

    segments[SEG_LOAD_RX].type   = SEG_LOAD_RX;
    segments[SEG_LOAD_RW].type   = SEG_LOAD_RW;
    segments[SEG_INTERP].type    = SEG_INTERP;
    sections[SEC_DYNSTR].segment = SEG_LOAD_RW;
}

static void buildSegments(){
    for(size_t s = 0; s < SEG_COUNT; s++){

        size_t min_off = SIZE_MAX;
        size_t max_off = 0;

        size_t min_vaddr = SIZE_MAX;
        size_t max_vaddr = 0;

        for(size_t i = 1; i < SEC_COUNT; i++){

            if(sections[i].segment != s)
                continue;

            if(sections[i].file_offset < min_off)
                min_off = sections[i].file_offset;

            size_t end = sections[i].file_offset + sections[i].size;

            if(end > max_off)
                max_off = end;

            if(sections[i].vaddr < min_vaddr)
                min_vaddr = sections[i].vaddr;

            size_t vend =
                sections[i].vaddr +
                sections[i].size;

            if(vend > max_vaddr)
                max_vaddr = vend;
        }

        segments[s].file_offset = min_off;
        segments[s].file_size   = max_off - min_off;

        segments[s].vaddr       = min_vaddr;
        segments[s].mem_size    = max_vaddr - min_vaddr;
    }
}

static void strtabInit(StringTable* tab){
    tab->buf[0] = '\0';
    tab->size   = 1;
}

static size_t strtabAdd(StringTable* tab, const char* str){
    size_t off = tab->size;

    size_t len = strlen(str) + 1;

    memcpy(tab->buf + tab->size, str, len);

    tab->size += len;

    return off;
}

static void initSections(){
    memset(sections, 0, sizeof(sections));

    sections[SEC_TEXT].type     = SEC_TEXT;
    sections[SEC_TEXT].name     = ".text";

    sections[SEC_DATA].type     = SEC_DATA;
    sections[SEC_DATA].name     = ".data";

    sections[SEC_INTERP].type   = SEC_INTERP;
    sections[SEC_INTERP].name   = ".interp";

    sections[SEC_DYNAMIC].type  = SEC_DYNAMIC;
    sections[SEC_DYNAMIC].name  = ".dynamic";

    sections[SEC_DYNSTR].type   = SEC_DYNSTR;
    sections[SEC_DYNSTR].name   = ".dynstr";

    sections[SEC_SHSTRTAB].type = SEC_SHSTRTAB;
    sections[SEC_SHSTRTAB].name = ".shstrtab";
}

static void buildShStrTab(){
    strtabInit(&shstrtab);

    for(size_t i = 1; i < SEC_COUNT; i++){
        sections[i].name_offset = strtabAdd(&shstrtab, sections[i].name);
    }

    sections[SEC_SHSTRTAB].data = (uint8_t*)shstrtab.buf;

    sections[SEC_SHSTRTAB].size = shstrtab.size;
}

static void layoutSections(size_t phnum){
    size_t cur =
        sizeof(Elf64_Ehdr) +
        phnum * sizeof(Elf64_Phdr);

    for(size_t i = 1; i < SEC_COUNT; i++){

        sections[i].file_offset = cur;

        sections[i].vaddr = START_VIRTUAL_ADDR + cur;

        cur += sections[i].size;
    }
}

static void buildSectionHeaders(){
    for(size_t i = 1; i < SEC_COUNT; i++){

        sections[i].shdr.sh_name   = sections[i].name_offset;

        sections[i].shdr.sh_offset = sections[i].file_offset;

        sections[i].shdr.sh_addr   = sections[i].vaddr;

        sections[i].shdr.sh_size   = sections[i].size;
    }

    sections[SEC_TEXT].shdr.sh_type           = SHT_PROGBITS;
    sections[SEC_TEXT].shdr.sh_flags          = SHF_ALLOC | SHF_EXECINSTR;
    sections[SEC_TEXT].shdr.sh_addralign      = 16;

    sections[SEC_DATA].shdr.sh_type           = SHT_PROGBITS;
    sections[SEC_DATA].shdr.sh_flags          = SHF_ALLOC | SHF_WRITE;
    sections[SEC_DATA].shdr.sh_addralign      = 8;

    sections[SEC_INTERP].shdr.sh_type         = SHT_PROGBITS;
    sections[SEC_INTERP].shdr.sh_flags        = SHF_ALLOC;
    sections[SEC_INTERP].shdr.sh_addralign    = 1;

    sections[SEC_DYNAMIC].shdr.sh_type        = SHT_DYNAMIC;
    sections[SEC_DYNAMIC].shdr.sh_flags       = SHF_ALLOC | SHF_WRITE;
    sections[SEC_DYNAMIC].shdr.sh_link        = SEC_DYNSTR;
    sections[SEC_DYNAMIC].shdr.sh_addralign   = 8;
    
    sections[SEC_DYNSTR].shdr.sh_type         = SHT_STRTAB;
    sections[SEC_DYNSTR].shdr.sh_flags        = SHF_ALLOC;
    sections[SEC_DYNSTR].shdr.sh_addralign    = 1;

    sections[SEC_SHSTRTAB].shdr.sh_type       = SHT_STRTAB;
    sections[SEC_SHSTRTAB].shdr.sh_addralign  = 1;
}

static Section* getSection(section_t sec){
    return &sections[sec];
}

static void buildDynStr(){
    strtabInit(&dynstr);

    strtabAdd(&dynstr, "libc.so.6");
    strtabAdd(&dynstr, "printf");
    strtabAdd(&dynstr, "scanf");

    sections[SEC_DYNSTR].data = (uint8_t*)dynstr.buf;
    sections[SEC_DYNSTR].size = dynstr.size;
}

static void buildDynamicSection(){
    memset(dynamicEntries, 0, sizeof(dynamicEntries));

    dynamicEntries[0].d_tag      = DT_STRTAB;
    dynamicEntries[0].d_un.d_ptr = sections[SEC_DYNSTR].vaddr;

    dynamicEntries[1].d_tag      = DT_STRSZ;
    dynamicEntries[1].d_un.d_val = sections[SEC_DYNSTR].size;

    dynamicEntries[2].d_tag      = DT_NEEDED;
    dynamicEntries[2].d_un.d_val = 1;

    dynamicEntries[3].d_tag      = DT_NULL;
    dynamicEntries[3].d_un.d_val = 0;

    sections[SEC_DYNAMIC].data    = (uint8_t*) dynamicEntries;
    sections[SEC_DYNAMIC].size    = sizeof(dynamicEntries);
    sections[SEC_DYNAMIC].segment = SEG_DYNAMIC;
}