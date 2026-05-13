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

void genPrologueX86Elf(codeGenContext* context){
    assert(context);

    const char* shstrtab = "\0.text\0.data\0.shstrtab";
    size_t shstrtabSize  = 23;

    uint8_t raw_data[] = {
        '%', 'd', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
    };

    _CONTEXT_ELF_SIZE_EH(context)            = sizeof(Elf64_Ehdr);
    _CONTEXT_ELF_SIZE_PH(context)            = sizeof(Elf64_Phdr);
    _CONTEXT_ELF_CODE_SIZE(context)          = BIN_BUFFER_SIZE(_CONTEXT_ELF_CODE_BUFFER(context)) * sizeof(uint8_t);
    _CONTEXT_ELF_SECTION_TABLE_SIZE(context) = sizeof(Elf64_Shdr);
    _CONTEXT_ELF_TABLE_NAMES_TS(context)     = shstrtabSize;
    _CONTEXT_ELF_DATA_SECTION_SIZE(context)  = sizeof(raw_data);

    genElfHeader(context);
    genProgramHeader(context);

    fwrite(raw_data, _CONTEXT_ELF_DATA_SECTION_SIZE(context), 1, _CONTEXT_FILE_PTR(context));

    fwrite(BIN_BUFFER_DATA(_CONTEXT_ELF_CODE_BUFFER(context)), _CONTEXT_ELF_CODE_SIZE(context), 1, _CONTEXT_FILE_PTR(context));
    fwrite(shstrtab, shstrtabSize, 1, _CONTEXT_FILE_PTR(context));

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

    ehdr.e_entry  = START_VIRTUAL_ADDR + _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context);              /// лучше + 0x1000
    ehdr.e_ehsize = _CONTEXT_ELF_SIZE_EH(context);    

    ehdr.e_phoff     = _CONTEXT_ELF_SIZE_EH(context); 
    ehdr.e_phentsize = _CONTEXT_ELF_SIZE_PH(context);
    ehdr.e_phnum     = 1;               

    ehdr.e_shoff     = _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) + _CONTEXT_ELF_CODE_SIZE(context) + _CONTEXT_ELF_DATA_SECTION_SIZE(context) + _CONTEXT_ELF_TABLE_NAMES_TS(context);  
    ehdr.e_shentsize = _CONTEXT_ELF_SECTION_TABLE_SIZE(context);
    ehdr.e_shnum     = 4; 
    ehdr.e_shstrndx  = 3;  

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
    phdr.p_flags  = PF_R | PF_X || PF_W; 
    phdr.p_align  = MEM_PAGE_SIZE;   

    fwrite(&phdr, sizeof(phdr), 1, _CONTEXT_FILE_PTR(context));
}

static void genSectionTable(codeGenContext* context){
    assert(context);

    Elf64_Shdr shdr[4] = {0};

    shdr[1].sh_name   = 1;                
    shdr[1].sh_type   = SHT_PROGBITS;
    shdr[1].sh_flags  = SHF_ALLOC | SHF_EXECINSTR;
    shdr[1].sh_addr   = START_VIRTUAL_ADDR + _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context);
    shdr[1].sh_offset = _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) ;
    shdr[1].sh_size   = _CONTEXT_ELF_CODE_SIZE(context);

    shdr[2].sh_name      = 7;
    shdr[2].sh_type      = SHT_PROGBITS;
    shdr[2].sh_flags     = SHF_ALLOC | SHF_WRITE;
    shdr[2].sh_addr = START_VIRTUAL_ADDR + _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) + _CONTEXT_ELF_CODE_SIZE(context);
    shdr[2].sh_offset    = _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) + _CONTEXT_ELF_CODE_SIZE(context);
    shdr[2].sh_size      = _CONTEXT_ELF_DATA_SECTION_SIZE(context);
    shdr[2].sh_addralign = 8;

    shdr[3].sh_name   = 13;
    shdr[3].sh_type   = SHT_STRTAB;
    shdr[3].sh_offset = _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) + _CONTEXT_ELF_CODE_SIZE(context) + _CONTEXT_ELF_DATA_SECTION_SIZE(context);
    shdr[3].sh_size   = _CONTEXT_ELF_TABLE_NAMES_TS(context);

    fwrite(&shdr, sizeof(shdr), 1, _CONTEXT_FILE_PTR(context));
}







