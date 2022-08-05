#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define BYTES_PER_SECTOR 512
#define SECTORS_PER_CLUSTER 4
#define RESERVED_SECTOR 1

#pragma pack(push, 1)
typedef struct BootRecord
{
  unsigned short    bytes_per_sector;
  unsigned char     sectors_per_cluster;
  unsigned char     reserved_sector;
  unsigned short    sector_directory_blocks;
  unsigned short    sector_section_data;
  unsigned int      pointer_to_space_free;
  unsigned int      total_main_directory_entries;
}__attribute__((packed)) BootRecord;

#pragma pack(push, 1)
typedef struct FileFormat{    
    unsigned char  status;
    unsigned char  name[12];
    unsigned char  ext[4];
    unsigned char  attribute;
    unsigned int   first_cluster;
    unsigned int   fileSize;
    unsigned short total_entries;
} __attribute__((packed)) FileFormat;

typedef struct Nav
{
    int firstCluster;
    int entries;
} Nav;



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
            copyFile("safs.img");
            break;
        case 4:
            extractArq("safs.img");
            break;
        case 5:
            deleteFile();
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