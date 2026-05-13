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
static void genProgramHeader(codeGenContext* context);
static void genSectionTable(codeGenContext* context);

typedef enum {
    SEC_NULL = 0,
    SEC_TEXT,
    SEC_DATA,
    SEC_INTERP,
    SEC_SHSTRTAB,

    SEC_COUNT
} section_t;

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
};

static Section sections[SEC_COUNT];

static StringTable shstrtab;

static void strtabInit(StringTable* tab);
static size_t strtabAdd(StringTable* tab, const char* str);
static void initSections();
static void buildShStrTab();
static void layoutSections(size_t phnum);
static void buildSectionHeaders();
static Section* getSection(section_t sec);

void genPrologueX86Elf(codeGenContext* context){
    assert(context);

    initSections();
    
    sections[SEC_TEXT].data = BIN_BUFFER_DATA(_CONTEXT_ELF_CODE_BUFFER(context));

    sections[SEC_TEXT].size = BIN_BUFFER_SIZE(_CONTEXT_ELF_CODE_BUFFER(context)) * sizeof(uint8_t);

    static uint8_t raw_data[] = {
        '%', 'd', '\0'
    };

    sections[SEC_DATA].data = raw_data;
    sections[SEC_DATA].size = sizeof(raw_data);

    static uint8_t dynamicINt[] = {
        '%', 'd', '\0'
    };    

    buildShStrTab();
    layoutSections(1);
    buildSectionHeaders();

    _CONTEXT_ELF_SIZE_EH(context)            = sizeof(Elf64_Ehdr);
    _CONTEXT_ELF_SIZE_PH(context)            = sizeof(Elf64_Phdr);
    _CONTEXT_ELF_DATA_SECTION_SIZE(context)  = sizeof(raw_data);
    _CONTEXT_ELF_SECTION_TABLE_SIZE(context) = sizeof(Elf64_Shdr);

    genElfHeader(context);
    genProgramHeader(context);

    fwrite(sections[SEC_TEXT].data, sections[SEC_TEXT].size, 1, _CONTEXT_FILE_PTR(context));

    fwrite(sections[SEC_DATA].data, sections[SEC_DATA].size, 1, _CONTEXT_FILE_PTR(context));

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
    ehdr.e_ehsize = _CONTEXT_ELF_SIZE_EH(context);    

    ehdr.e_phoff     = _CONTEXT_ELF_SIZE_EH(context); 
    ehdr.e_phentsize = _CONTEXT_ELF_SIZE_PH(context);
    ehdr.e_phnum     = 1;               

    ehdr.e_shoff = sections[SEC_SHSTRTAB].file_offset + sections[SEC_SHSTRTAB].size;;  
    ehdr.e_shentsize = _CONTEXT_ELF_SECTION_TABLE_SIZE(context);
    ehdr.e_shnum     = SEC_COUNT; 
    ehdr.e_shstrndx  = SEC_SHSTRTAB;  

    fwrite(&ehdr, sizeof(ehdr), 1, _CONTEXT_FILE_PTR(context));
}

static void genProgramHeader(codeGenContext* context){
    assert(context);

    Elf64_Phdr phdr;
    memset(&phdr, 0, sizeof(phdr));

    phdr.p_type   = PT_LOAD; 
    phdr.p_offset = 0;                 
    phdr.p_vaddr  = START_VIRTUAL_ADDR;         
    phdr.p_paddr  = START_PHYSICAL_ADDR;

    phdr.p_filesz = _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) + _CONTEXT_ELF_CODE_SIZE(context) + _CONTEXT_ELF_DATA_SECTION_SIZE(context) + _CONTEXT_ELF_TABLE_NAMES_TS(context) ;
    phdr.p_memsz  = phdr.p_filesz;
    phdr.p_flags  = PF_R | PF_X | PF_W; 
    phdr.p_align  = MEM_PAGE_SIZE;   

    fwrite(&phdr, sizeof(phdr), 1, _CONTEXT_FILE_PTR(context));
}

static void genSectionTable(codeGenContext* context){
    assert(context);

    Elf64_Shdr shdr[SEC_COUNT] = {0};

    for(size_t i = 0; i < SEC_COUNT; i++){
        shdr[i] = sections[i].shdr;
    }

    fwrite(
        shdr,
        sizeof(Elf64_Shdr),
        SEC_COUNT,
        _CONTEXT_FILE_PTR(context)
    );
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

    sections[SEC_TEXT].type = SEC_TEXT;
    sections[SEC_TEXT].name = ".text";

    sections[SEC_DATA].type = SEC_DATA;
    sections[SEC_DATA].name = ".data";

    sections[SEC_INTERP].type = SEC_INTERP;
    sections[SEC_INTERP].name = ".interp";

    sections[SEC_SHSTRTAB].type = SEC_SHSTRTAB;
    sections[SEC_SHSTRTAB].name = ".shstrtab";
}

static void buildShStrTab(){
    strtabInit(&shstrtab);

    for(size_t i = 1; i < SEC_COUNT; i++){
        sections[i].name_offset =
            strtabAdd(&shstrtab, sections[i].name);
    }

    sections[SEC_SHSTRTAB].data =
        (uint8_t*)shstrtab.buf;

    sections[SEC_SHSTRTAB].size =
        shstrtab.size;
}

static void layoutSections(size_t phnum){
    size_t cur =
        sizeof(Elf64_Ehdr) +
        phnum * sizeof(Elf64_Phdr);

    for(size_t i = 1; i < SEC_COUNT; i++){

        sections[i].file_offset = cur;

        sections[i].vaddr =
            START_VIRTUAL_ADDR + cur;

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

    sections[SEC_TEXT].shdr.sh_type      = SHT_PROGBITS;
    sections[SEC_TEXT].shdr.sh_flags     = SHF_ALLOC | SHF_EXECINSTR;
    sections[SEC_TEXT].shdr.sh_addralign = 16;

    sections[SEC_DATA].shdr.sh_type      = SHT_PROGBITS;
    sections[SEC_DATA].shdr.sh_flags     = SHF_ALLOC | SHF_WRITE;
    sections[SEC_DATA].shdr.sh_addralign = 8;

    sections[SEC_INTERP].shdr.sh_type  = SHT_PROGBITS;
    sections[SEC_INTERP].shdr.sh_flags = SHF_ALLOC;
    
    sections[SEC_SHSTRTAB].shdr.sh_type      = SHT_STRTAB;
    sections[SEC_SHSTRTAB].shdr.sh_addralign = 1;
}

static Section* getSection(section_t sec){
    return &sections[sec];
}






