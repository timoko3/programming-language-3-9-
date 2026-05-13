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
    SEG_LOAD_RX = 1ull << 0,
    SEG_LOAD_RW = 1ull << 1,
    SEG_INTERP  = 1ull << 2,
    SEG_DYNAMIC = 1ull << 3,
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

const size_t SEG_COUNT = 2;

static Segment segments[SEG_COUNT];

static const uint64_t segmentFlags[] = {
    SEG_LOAD_RX,
    SEG_LOAD_RW,
    // SEG_INTERP,
    // SEG_DYNAMIC
};

enum section_t{
    SEC_NULL = 0,
    SEC_TEXT,
    SEC_DATA,
    // SEC_INTERP,
    // SEC_DYNAMIC,
    // SEC_DYNSTR,
    // SEC_DYNSYM,
    // SEC_RELA_PLT,
    // SEC_PLT,
    // SEC_GOT_PLT,
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

    uint64_t segment_mask;
};

static Section sections[SEC_COUNT];

static StringTable shstrtab;
static StringTable dynstr;

static Elf64_Dyn  dynamicEntries[9];
static Elf64_Sym  dynsymEntries[3];
static Elf64_Rela relaPltEntries[2];
static uint8_t    pltCode[48];
static uint64_t   gotPlt[4];

static void strtabInit(StringTable* tab);
static size_t strtabAdd(StringTable* tab, const char* str);
static void initSections();
static void buildShStrTab();
static void layoutSections(size_t phnum);
static void buildSectionHeaders();
static Section* getSection(section_t sec);

static void initSegments();
static void buildSegments();

// static void buildDynStr();
// static void buildDynamicSection();
// static void buildDynSym();
// static void buildRelaPlt();
// static void buildPlt();
// static void buildGotPlt();

void genPrologueX86Elf(codeGenContext* context){
    assert(context);

    initSections();
    initSegments();
    
    sections[SEC_TEXT].data             = BIN_BUFFER_DATA(_CONTEXT_ELF_CODE_BUFFER(context));
    sections[SEC_TEXT].size             = BIN_BUFFER_SIZE(_CONTEXT_ELF_CODE_BUFFER(context)) * sizeof(uint8_t);
    sections[SEC_TEXT].segment_mask     = SEG_LOAD_RX;
    
    static uint8_t raw_data[]           = {'%', 'd', '\0'};
    sections[SEC_DATA].data             = raw_data;
    sections[SEC_DATA].size             = sizeof(raw_data);
    sections[SEC_DATA].segment_mask     = SEG_LOAD_RW;
    
    // static uint8_t interp[]             = "/lib64/ld-linux-x86-64.so.2";
    // sections[SEC_INTERP].data           = interp;
    // sections[SEC_INTERP].size           = sizeof(interp);
    // sections[SEC_INTERP].segment_mask   = SEG_INTERP | SEG_LOAD_RX;

    // sections[SEC_DYNAMIC].size          = sizeof(dynamicEntries);
    // sections[SEC_DYNAMIC].segment_mask  = SEG_DYNAMIC | SEG_LOAD_RW;

    // sections[SEC_PLT].size = 48;
    // sections[SEC_GOT_PLT].size = sizeof(gotPlt);

    buildShStrTab();
    // buildDynStr();
    // buildDynSym();
    // buildGotPlt();
    // buildRelaPlt();
    // buildPlt();
    layoutSections(SEG_COUNT);

    // buildDynamicSection();

    buildSegments();
    buildSectionHeaders();

    genElfHeader(context);
    genProgramHeaders(context);

    for(size_t i = 1; i < SEC_COUNT; i++){
        if(sections[i].size == 0)
            continue;
        fwrite(sections[i].data, sections[i].size, 1, _CONTEXT_FILE_PTR(context));
    }

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

    segments[0].phdr.p_type  = PT_LOAD;
    segments[0].phdr.p_flags = PF_R | PF_X;

    segments[1].phdr.p_type  = PT_LOAD;
    segments[1].phdr.p_flags = PF_R | PF_W;

    // segments[2].phdr.p_type   = PT_INTERP;
    // segments[2].phdr.p_flags  = PF_R;

    // segments[3].phdr.p_type  = PT_DYNAMIC;
    // segments[3].phdr.p_flags = PF_R | PF_W;;

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

    segments[0].type   = SEG_LOAD_RX;
    segments[1].type   = SEG_LOAD_RW;
    // segments[2].type   = SEG_INTERP;
    // segments[3].type   = SEG_DYNAMIC;
}

static void buildSegments(){
    for(size_t j = 0; j < SEG_COUNT; j++){

        uint64_t seg = segmentFlags[j];

        size_t min_off   = SIZE_MAX;
        size_t max_off   = 0;

        size_t min_vaddr = SIZE_MAX;
        size_t max_vaddr = 0;

        bool found = false;
        for(size_t i = 1; i < SEC_COUNT; i++){
            if(!(sections[i].segment_mask & seg)){
                continue;
            }

            if(sections[i].size == 0){
                continue;
            }

            found = true;

            if(sections[i].file_offset < min_off){
                min_off = sections[i].file_offset;
            }

            size_t end = sections[i].file_offset + sections[i].size;

            if(end > max_off){
                max_off = end;
            }

            if(sections[i].vaddr < min_vaddr){
                min_vaddr = sections[i].vaddr;
            }

            size_t vend = sections[i].vaddr + sections[i].size;

            if(vend > max_vaddr){
                max_vaddr = vend;
            }
        }

        if(!found){
            continue;
        }
        // if(seg == SEG_DYNAMIC){
        //     Section* dyn = &sections[SEC_DYNAMIC];

        //     segments[j].file_offset = dyn->file_offset;
        //     segments[j].file_size   = dyn->size;

        //     segments[j].vaddr       = dyn->vaddr;
        //     segments[j].mem_size    = dyn->size;
        // }
        segments[j].file_offset = min_off;
        segments[j].file_size   = max_off - min_off;

        segments[j].vaddr       = min_vaddr;
        segments[j].mem_size    = max_vaddr - min_vaddr;
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

    // sections[SEC_INTERP].type   = SEC_INTERP;
    // sections[SEC_INTERP].name   = ".interp";

    // sections[SEC_DYNAMIC].type  = SEC_DYNAMIC;
    // sections[SEC_DYNAMIC].name  = ".dynamic";

    // sections[SEC_DYNSTR].type   = SEC_DYNSTR;
    // sections[SEC_DYNSTR].name   = ".dynstr";

    sections[SEC_SHSTRTAB].type = SEC_SHSTRTAB;
    sections[SEC_SHSTRTAB].name = ".shstrtab";

    // sections[SEC_DYNSYM].type   = SEC_DYNSYM;
    // sections[SEC_DYNSYM].name   = ".dynsym";

    // sections[SEC_RELA_PLT].type = SEC_RELA_PLT;
    // sections[SEC_RELA_PLT].name = ".rela.plt";

    // sections[SEC_PLT].type      = SEC_PLT;
    // sections[SEC_PLT].name      = ".plt";

    // sections[SEC_GOT_PLT].type  = SEC_GOT_PLT;
    // sections[SEC_GOT_PLT].name  = ".got.plt";
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
        memset(&sections[i].shdr, 0, sizeof(Elf64_Shdr));
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

    // sections[SEC_INTERP].shdr.sh_type         = SHT_PROGBITS;
    // sections[SEC_INTERP].shdr.sh_flags        = SHF_ALLOC;
    // sections[SEC_INTERP].shdr.sh_addralign    = 1;

    // sections[SEC_DYNAMIC].shdr.sh_type        = SHT_DYNAMIC;
    // sections[SEC_DYNAMIC].shdr.sh_flags       = SHF_ALLOC | SHF_WRITE;
    // sections[SEC_DYNAMIC].shdr.sh_link        = SEC_DYNSTR;
    // sections[SEC_DYNAMIC].shdr.sh_addralign   = 8;
    // sections[SEC_DYNAMIC].shdr.sh_entsize     = sizeof(Elf64_Dyn);

    // sections[SEC_DYNSTR].shdr.sh_type         = SHT_STRTAB;
    // sections[SEC_DYNSTR].shdr.sh_flags        = SHF_ALLOC;
    // sections[SEC_DYNSTR].shdr.sh_addralign    = 1;

    // sections[SEC_DYNSYM].shdr.sh_type         = SHT_DYNSYM;
    // sections[SEC_DYNSYM].shdr.sh_flags        = SHF_ALLOC;
    // sections[SEC_DYNSYM].shdr.sh_link         = SEC_DYNSTR;
    // sections[SEC_DYNSYM].shdr.sh_info         = 1;
    // sections[SEC_DYNSYM].shdr.sh_entsize      = sizeof(Elf64_Sym);
    // sections[SEC_DYNSYM].shdr.sh_addralign    = 8;

    // sections[SEC_RELA_PLT].shdr.sh_type       = SHT_RELA;
    // sections[SEC_RELA_PLT].shdr.sh_flags      = SHF_ALLOC;
    // sections[SEC_RELA_PLT].shdr.sh_link       = SEC_DYNSYM;
    // sections[SEC_RELA_PLT].shdr.sh_info       = SEC_PLT;
    // sections[SEC_RELA_PLT].shdr.sh_entsize    = sizeof(Elf64_Rela);
    // sections[SEC_RELA_PLT].shdr.sh_addralign  = 8;

    // sections[SEC_PLT].shdr.sh_type            = SHT_PROGBITS;
    // sections[SEC_PLT].shdr.sh_flags           = SHF_ALLOC | SHF_EXECINSTR;
    // sections[SEC_PLT].shdr.sh_addralign       = 16;

    // sections[SEC_GOT_PLT].shdr.sh_type        = SHT_PROGBITS;
    // sections[SEC_GOT_PLT].shdr.sh_flags       = SHF_ALLOC | SHF_WRITE;
    // sections[SEC_GOT_PLT].shdr.sh_addralign   = 8;

    sections[SEC_SHSTRTAB].shdr.sh_type       = SHT_STRTAB;
    sections[SEC_SHSTRTAB].shdr.sh_addralign  = 1;
}

static Section* getSection(section_t sec){
    return &sections[sec];
}

// static void buildDynStr(){
//     strtabInit(&dynstr);

//     strtabAdd(&dynstr, "libc.so.6");
//     strtabAdd(&dynstr, "printf");
//     strtabAdd(&dynstr, "scanf");

//     // sections[SEC_DYNSTR].data = (uint8_t*)dynstr.buf;
//     // sections[SEC_DYNSTR].size = dynstr.size;
//     // sections[SEC_DYNSTR].segment_mask = SEG_LOAD_RX;
// }

// static void buildDynamicSection(){
//     memset(dynamicEntries, 0, sizeof(dynamicEntries));

//     dynamicEntries[0].d_tag      = DT_SYMTAB;
//     dynamicEntries[0].d_un.d_ptr = sections[SEC_DYNSYM].vaddr;

//     dynamicEntries[1].d_tag      = DT_SYMENT;
//     dynamicEntries[1].d_un.d_val = sizeof(Elf64_Sym);

//     dynamicEntries[2].d_tag      = DT_STRTAB;
//     dynamicEntries[2].d_un.d_ptr = sections[SEC_DYNSTR].vaddr;

//     dynamicEntries[3].d_tag      = DT_STRSZ;
//     dynamicEntries[3].d_un.d_val = sections[SEC_DYNSTR].size;

//     dynamicEntries[4].d_tag      = DT_NEEDED;
//     dynamicEntries[4].d_un.d_val = 1;

//     dynamicEntries[5].d_tag      = DT_JMPREL;
//     dynamicEntries[5].d_un.d_ptr = sections[SEC_RELA_PLT].vaddr;

//     dynamicEntries[6].d_tag      = DT_PLTRELSZ;
//     dynamicEntries[6].d_un.d_val = sections[SEC_RELA_PLT].size;

//     dynamicEntries[7].d_tag      = DT_PLTREL;
//     dynamicEntries[7].d_un.d_val = DT_RELA;

//     dynamicEntries[8].d_tag      = DT_NULL;
//     dynamicEntries[8].d_un.d_val = 0;

//     sections[SEC_DYNAMIC].data         = (uint8_t*) dynamicEntries;
//     sections[SEC_DYNAMIC].size         = sizeof(dynamicEntries);
//     sections[SEC_DYNAMIC].segment_mask = SEG_DYNAMIC | SEG_LOAD_RW;
// }

// static void buildDynSym(){
//     memset(dynsymEntries, 0, sizeof(dynsymEntries));

//     dynsymEntries[0].st_name  = 0;
//     dynsymEntries[0].st_info  = 0;
//     dynsymEntries[0].st_other = 0;
//     dynsymEntries[0].st_shndx = SHN_UNDEF;

//     dynsymEntries[1].st_name  = 11;
//     dynsymEntries[1].st_info  = ELF64_ST_INFO(STB_GLOBAL, STT_FUNC);
//     dynsymEntries[1].st_other = 0;
//     dynsymEntries[1].st_shndx = SHN_UNDEF;

//     dynsymEntries[2].st_name  = 18;
//     dynsymEntries[2].st_info  = ELF64_ST_INFO(STB_GLOBAL, STT_FUNC);
//     dynsymEntries[2].st_other = 0;
//     dynsymEntries[2].st_shndx = SHN_UNDEF;

//     sections[SEC_DYNSYM].data         = (uint8_t*)dynsymEntries;
//     sections[SEC_DYNSYM].size         = sizeof(dynsymEntries);
//     sections[SEC_DYNSYM].segment_mask = SEG_LOAD_RX;
// }

// static void buildRelaPlt(){
//     memset(relaPltEntries, 0, sizeof(relaPltEntries));

//     relaPltEntries[0].r_offset = sections[SEC_GOT_PLT].vaddr + 8 * 2;
//     relaPltEntries[0].r_info   = ELF64_R_INFO(1, R_X86_64_JUMP_SLOT);
//     relaPltEntries[0].r_addend = 0;

//     relaPltEntries[1].r_offset = sections[SEC_GOT_PLT].vaddr + 8 * 3;
//     relaPltEntries[1].r_info   = ELF64_R_INFO(2, R_X86_64_JUMP_SLOT);
//     relaPltEntries[1].r_addend = 0;

//     sections[SEC_RELA_PLT].data         = (uint8_t*)relaPltEntries;
//     sections[SEC_RELA_PLT].size         = sizeof(relaPltEntries);
//     sections[SEC_RELA_PLT].segment_mask = SEG_LOAD_RX;
// }

// static void buildPlt(){
//     memset(pltCode, 0x90, sizeof(pltCode));

//     sections[SEC_PLT].data         = pltCode;
//     sections[SEC_PLT].size         = sizeof(pltCode);
//     sections[SEC_PLT].segment_mask = SEG_LOAD_RX;
// }

// static void buildGotPlt(){
//     memset(gotPlt, 0, sizeof(gotPlt));

//     sections[SEC_GOT_PLT].data         = (uint8_t*)gotPlt;
//     sections[SEC_GOT_PLT].size         = sizeof(gotPlt);
//     sections[SEC_GOT_PLT].segment_mask = SEG_LOAD_RW;
// }