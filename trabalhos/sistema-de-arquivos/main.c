#include "functions.c"


int main(int argc, char const *argv[])
{
int opt;
    
    BootRecord br;
    while (opt!=8)
    {
        printf("\n1. Formatar imagem.\n2. Listar arquivos.\n3. Copiar arquivo do disco para o sistema de arquivos.\n4. Copiar do sistema de arquivos para o disco.\n5. Apagar arquivo.\n6. Criar diretório.\n7. Navegar.\n8. Sair\n\nDigite o número da opção que deseja: ");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            createImage();
            break;
        case 2:
            br = readBR();
            showFiles("safs.img", startDirectoryBlocks(), 1, br.total_main_directory_entries);
            break;
        case 3:
            br = readBR();
            copyFile("safs.img", 1, startDirectoryBlocks(), br.total_main_directory_entries);
            break;
        case 4:
            br = readBR();
            extractArq("safs.img", startDirectoryBlocks(), br.total_main_directory_entries);
            break;
        case 5:
            br = readBR();
            deleteFile(1, startDirectoryBlocks(), br.total_main_directory_entries);
            break;
        case 6:
            br = readBR();
            createDir(startDirectoryBlocks(), br.total_main_directory_entries);
            break;
        case 7:
            navigator("safs.img");
            break;
        case 8:
            break;
        }
    }
    
    
    
    return(0);
}
