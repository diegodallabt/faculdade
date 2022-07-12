#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short get_next_cluster(FILE *fp, unsigned short cluster, int fatAdress)
{
    unsigned short clusterResult;
    fseek(fp,fatAdress + (cluster * 2) , SEEK_SET);
	fread(&clusterResult, 1, sizeof(clusterResult), fp);
    // read_bytes(fp, fatAdress + cluster*2, &clusterResult, sizeof(clusterResult));
    return clusterResult;
}

struct dir_t {
    unsigned char name[11];
    unsigned char attribute;
    unsigned char NTRes;
    unsigned char createTimeTenth;
    unsigned short createTime;

    unsigned short createDate;
    unsigned short lastAcessDate;
    unsigned short highFirstCluster;
    unsigned short writeTime;
    unsigned short writeDate;
    unsigned short firstCluster;
    unsigned int fileSize;
} __attribute__((packed)) dir_t;

typedef struct fat_BS
{
	unsigned char 		bootjmp[3];
	unsigned char 		oem_name[8];
	unsigned short 	    bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short		reserved_sector_count;
	unsigned char		table_count;
	unsigned short		root_entry_count;
	unsigned short		total_sectors_16;
	unsigned char		media_type;
	unsigned short		table_size_16;
	unsigned short		sectors_per_track;
	unsigned short		head_side_count;
	unsigned int 		hidden_sector_count;
	unsigned int 		total_sectors_32;
 
	//this will be cast to it's specific type once the driver actually knows what type of FAT this is.
	unsigned char		extended_section[54];
 
}__attribute__((packed)) fat_BS_t;

int main(int argc, char const *argv[])
{
    FILE *fp;
    fat_BS_t boot_record;
	int i;
	char nomeArquivo[13];

	if(argc != 2) return 0;

	strcpy(nomeArquivo, argv[1]);

    fp= fopen("./fat16.img", "rb");

	if(fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fseek(fp, 0, SEEK_SET);
    fread(&boot_record, sizeof(fat_BS_t),1, fp);

 
    printf("Bytes per sector %hd \n", boot_record.bytes_per_sector);
    printf("Sector per cluster %x \n\n", boot_record.sectors_per_cluster);
	printf("BootRecord: %hd\n", boot_record.reserved_sector_count);
	for(int i = 1; i<=boot_record.table_count; i++)
		printf("FAT%d: %hd\n", i, boot_record.table_size_16);

	long int root = (boot_record.table_size_16*2) + boot_record.reserved_sector_count;
	long int startRoot = root * boot_record.bytes_per_sector;
	int fatAdress = boot_record.bytes_per_sector * boot_record.reserved_sector_count;
	long int dateAdress = (boot_record.reserved_sector_count + (boot_record.table_count * boot_record.table_size_16) + boot_record.sectors_per_track);

	printf("Root: %ld\nStart Root: %ld\n", root, startRoot);
	// fseek(fp, startRoot, SEEK_SET);
	
	for ( i = 0; i < boot_record.root_entry_count; i++)	{
		struct dir_t dir;
		unsigned int offset = startRoot + i*32;

        fseek(fp, offset, SEEK_SET);
		fread(&dir, 1, sizeof(dir), fp);


		if(dir.name[0] == 0) {
            break;
        } else if(dir.name[0] == 0xE5) {
            printf("Espaco apagado.\n");
            continue;
        }

		printf("\n%s ", dir.name);

		if(dir.attribute & 0x10)
			printf("DIRETORIO\n");
		else if(dir.attribute & 0xF)	
			printf("LONG FILE NAME\n");
		else if(dir.attribute & 0x20)
			printf("ARQUIVO\n");
    

		if(!strcmp(dir.name, nomeArquivo)){
			unsigned short c;
			const unsigned int cluster_size = boot_record.bytes_per_sector * boot_record.sectors_per_cluster;
			
			fseek(fp, fatAdress + (dir.firstCluster * 2), SEEK_SET);
			
			for(c = dir.firstCluster; c<0xFFF8; c = get_next_cluster(fp, c, fatAdress)){
				char buf[cluster_size];

				//-----------116       +  10-2   *   512
				fseek(fp, ((dateAdress + (c -2)) * boot_record.bytes_per_sector), SEEK_SET);
				fread(&buf, 1, sizeof(cluster_size), fp);

				//read_bytes(f, BPB_Data_addr(bpb) + (c - 2) * cluster_size, buf, sizeof(buf));
				printf("%s", buf);
			}
			
	/*
			bool read_bytes(FILE *f, unsigned int offset, void *buf, unsigned int length)
{
    if(fseek(f, offset, SEEK_SET != 0)) {
        printf("Erro ao posicionar local, SEEK_SET.\n");
        return false;
    }

    if(fread(buf, 1, length, f) != length && ferror(f)) {
        printf("Erro ao ler arquivo.\n");
        return false;
    }

    return true;
}
*/
      


/*
      void print_cluster(FILE *f, struct BPB_t *bpb, struct dir_t *dir)
{
    const unsigned int cluster_size = boot_record.bytes_per_sector * boot_record.sectors_per_cluster;

    for(unsigned short c = dir->FstClusLO; c < 0xFFF8; c = get_next_cluster(f, bpb, c)) {
        char buf[cluster_size];
        //FAT16 subtrair os 2 primeiros do cluster, para encontrar a posicao inicial do conteudo
        read_bytes(f, BPB_Data_addr(bpb) + (c - 2) * cluster_size, buf, sizeof(buf));

        printf("%.*s", (int)(sizeof(buf)), buf);
    }
}



unsigned short get_next_cluster(FILE *f, struct BPB_t *bpb, unsigned short cluster)
{
    unsigned short result;
    read_bytes(f, BPB_FAT_addr(bpb) + cluster*2, &result, sizeof(result));
    return result;
}

unsigned int BPB_Data_addr(struct BPB_t *bpb)
{
    return BPB_Root_addr(bpb) + bpb->RootEntCnt * 32;
                                //tamanho root
}


unsigned int BPB_Root_addr(struct BPB_t *bpb)
{
  return BPB_FAT_addr(bpb) + bpb->NumFATs * bpb->FATSz16 * bpb->BytsPerSec;
}


*/






		}
	}
	
	
	fclose(fp);
    return 0;
}
