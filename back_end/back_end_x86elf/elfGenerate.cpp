#include "elfGenerate.h"

#include "context.h"

#include "core/DSL.h"

#include "elf.h"
#include <assert.h>
#include <string.h>

static void genElfHeader(codeGenContext* context);
static void genProgramHeader(codeGenContext* context);
static void genSectionTable(codeGenContext* context);

void genPrologueX86Elf(codeGenContext* context){
    assert(context);
    // unsigned char code[] = {
    //     // 1. Прыгаем через строку (на 12 байт вперед)
    //     0xeb, 0x0c,                         // jmp +12 (смещение к началу кода)

    //     // 2. Сама строка "Hello, ELF!\n" (12 байт)
    //     0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x45, 0x4c, 0x46, 0x21, 0x0a,

    //     // 3. Код вывода (точка входа должна указывать сюда или на jmp)
    //     // Вычисляем адрес строки через RIP-relative адресацию (строка выше на 14 байт отсюда)
    //     0x48, 0x8d, 0x35, 0xed, 0xff, 0xff, 0xff, // lea rsi, [rip - 19]

    //     // Системный вызов write(stdout, rsi, 12)
    //     0x48, 0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00, // mov rax, 1
    //     0x48, 0xc7, 0xc7, 0x01, 0x00, 0x00, 0x00, // mov rdi, 1
    //     0x48, 0xc7, 0xc2, 0x0c, 0x00, 0x00, 0x00, // mov rdx, 12 (длина строки)
    //     0x0f, 0x05,                               // syscall

    //     // Системный вызов exit(0)
    //     0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00, // mov rax, 60
    //     0x48, 0x31, 0xff,                         // xor rdi, rdi
    //     0x0f, 0x05                                // syscall
    // };

    const char* shstrtab = "\0.text\0.shstrtab";
    size_t shstrtabSize = 17;

    _CONTEXT_ELF_SIZE_EH(context)            = sizeof(Elf64_Ehdr);
    _CONTEXT_ELF_SIZE_PH(context)            = sizeof(Elf64_Phdr);
    _CONTEXT_ELF_CODE_SIZE(context)          = BIN_BUFFER_SIZE(_CONTEXT_ELF_CODE_BUFFER(context)) * sizeof(uint8_t);
    _CONTEXT_ELF_SECTION_TABLE_SIZE(context) = sizeof(Elf64_Shdr);
    _CONTEXT_ELF_TABLE_NAMES_TS(context)     = shstrtabSize;

    genElfHeader(context);
    genProgramHeader(context);

    fwrite(BIN_BUFFER_DATA(_CONTEXT_ELF_CODE_BUFFER(context)), 1, _CONTEXT_ELF_CODE_SIZE(context), _CONTEXT_FILE_PTR(context));
    fwrite(shstrtab, 1, shstrtabSize, _CONTEXT_FILE_PTR(context));

    genSectionTable(context);
}

static void genElfHeader(codeGenContext* context){
    assert(context);

    Elf64_Ehdr ehdr;
    memset(&ehdr, 0, sizeof(ehdr));

    ehdr.e_ident[EI_MAG0] = ELFMAG0;          
    ehdr.e_ident[EI_MAG1] = ELFMAG1;          
    ehdr.e_ident[EI_MAG2] = ELFMAG2;          
    ehdr.e_ident[EI_MAG3] = ELFMAG3;          

    ehdr.e_ident[EI_CLASS] = ELFCLASS64;      
    ehdr.e_ident[EI_DATA] = ELFDATA2LSB;      
    ehdr.e_ident[EI_VERSION] = EV_CURRENT;    
    ehdr.e_ident[EI_OSABI] = ELFOSABI_NONE; 

    ehdr.e_type    = ET_EXEC;                    
    ehdr.e_machine = EM_X86_64;               
    ehdr.e_version = EV_CURRENT;

    ehdr.e_entry = 0x400000 + _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context);                  
    ehdr.e_ehsize = _CONTEXT_ELF_SIZE_EH(context);    

    ehdr.e_phoff = _CONTEXT_ELF_SIZE_EH(context); 
    ehdr.e_phentsize = _CONTEXT_ELF_SIZE_PH(context);
    ehdr.e_phnum = 1;               

    ehdr.e_shoff = _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) + _CONTEXT_ELF_CODE_SIZE(context) + _CONTEXT_ELF_TABLE_NAMES_TS(context);  
    ehdr.e_shentsize = _CONTEXT_ELF_SECTION_TABLE_SIZE(context);
    ehdr.e_shnum = 3; 
    ehdr.e_shstrndx = 2;  


    fwrite(&ehdr, 1, sizeof(ehdr), _CONTEXT_FILE_PTR(context));
}

static void genProgramHeader(codeGenContext* context){
    assert(context);

    Elf64_Phdr phdr;
    memset(&phdr, 0, sizeof(phdr));

    phdr.p_type   = PT_LOAD; 
    phdr.p_offset = 0;                 
    phdr.p_vaddr  = 0x400000;         
    phdr.p_paddr  = 0x400000;

    phdr.p_filesz = _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) + _CONTEXT_ELF_CODE_SIZE(context) + _CONTEXT_ELF_TABLE_NAMES_TS(context);
    phdr.p_memsz  = phdr.p_filesz;
    phdr.p_flags  = PF_R | PF_X; 
    phdr.p_align  = 0x1000;   

    fwrite(&phdr, 1, sizeof(phdr), _CONTEXT_FILE_PTR(context));
}

static void genSectionTable(codeGenContext* context){
    assert(context);

    Elf64_Shdr shdr[3] = {0};

    shdr[1].sh_name = 1;                
    shdr[1].sh_type = SHT_PROGBITS;
    shdr[1].sh_flags = SHF_ALLOC | SHF_EXECINSTR;
    shdr[1].sh_addr = 0x400000 + _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context);
    shdr[1].sh_offset = _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) ;
    shdr[1].sh_size = _CONTEXT_ELF_CODE_SIZE(context);

    shdr[2].sh_name = 7;
    shdr[2].sh_type = SHT_STRTAB;
    shdr[2].sh_offset = _CONTEXT_ELF_SIZE_EH(context) + _CONTEXT_ELF_SIZE_PH(context) + _CONTEXT_ELF_CODE_SIZE(context);
    shdr[2].sh_size = _CONTEXT_ELF_TABLE_NAMES_TS(context);

    fwrite(&shdr, 1, sizeof(shdr), _CONTEXT_FILE_PTR(context));
}