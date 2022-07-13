#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define SIZE_DISK 16777216

#define BYTES_PER_SECTOR 256
#define SECTORS_PER_CLUSTER 2
#define RESERVED_SECTOR 2

typedef struct BootRecord
{
  unsigned short    bytes_per_sector;
  unsigned char     sectors_per_cluster;
  unsigned short    reserved_sector;
  unsigned short    sector_directory_blocks;
  unsigned short    sector_section_data;
  unsigned int      pointer_to_space_free;
  unsigned int      total_directory_entries;
}__attribute__((packed)) BootRecord;

bool setBR(FILE * img){
    BootRecord br;

    // seta os valores padrões do boot record
    br.bytes_per_sector = BYTES_PER_SECTOR;
    br.sectors_per_cluster = SECTORS_PER_CLUSTER;
    br.reserved_sector = RESERVED_SECTOR;
    br.sector_directory_blocks = (SIZE_DISK/BYTES_PER_SECTOR)*0.1;
    br.sector_section_data = (SIZE_DISK/BYTES_PER_SECTOR)*0.9;
    br.pointer_to_space_free = 0;
    br.total_directory_entries = 0;

    // posiciona ponteiro no inicio da imagem
    fseek(img, 0, SEEK_SET);

    // escreve o bootrecord na imagem
    fwrite(&br, 1 , sizeof(br) , img);
        
    return true;
}

BootRecord readBR(FILE *img){
    BootRecord br;

    // posiciona o ponteiro
    fseek(img, 0, SEEK_SET);

    //lê todo o bootrecord da imagem e armazena
    fread(&br, sizeof(BootRecord), 1, img);
    
    printf("%d", br.bytes_per_sector);
    return br;
}

void createImage(){
    FILE * img;
    char byte[1] = {0};

    img = fopen( "file.img" , "wb" );

    if(img == NULL){
        printf("Error opening image file.\n");
    }

    for(int i=0 ; i<= SIZE_DISK ; i++){
        fwrite(byte, 1 , sizeof(byte) , img);
    }
    
    if(setBR(img)){
        printf("[!] Nenhuma imagem especificada, logo uma nova imagem foi criada com sucesso e está pronta para uso.\n");
    }
    readBR(img);
    fclose(img);
}

int main(int argc, char const *argv[])
{
    createImage();
    return(0);
}