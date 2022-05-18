#include <stdio.h>
#include <stdlib.h>

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

int main()
{

    FILE *fp;
    fat_BS_t  boot_record;
	char nomeArquivo[11];

    fp= fopen("./fat16.img", "rb");
    fseek(fp, 0, SEEK_SET);
    fread(&boot_record, sizeof(fat_BS_t),1, fp);

 
    printf("Bytes per sector %hd \n", boot_record.bytes_per_sector);
    printf("Sector per cluster %x \n\n", boot_record.sectors_per_cluster);
	printf("BootRecord: %hd\n", boot_record.reserved_sector_count);
	for(int i = 1; i<=boot_record.table_count; i++)
		printf("FAT%d: %hd\n", i, boot_record.table_size_16);

	long int root = (boot_record.table_size_16*2) + boot_record.reserved_sector_count;
	long int startRoot = root * boot_record.bytes_per_sector;

	printf("Root: %ld\nStart Root: %ld\n", root, startRoot);
	fseek(fp, startRoot + 43, SEEK_SET);
		
	char remove; 
	fread(&remove, sizeof(char), 1, fp);
	
	if(remove == 0x0f){//0x0f){
		printf("É longfile.\n");
	}
	

	
    return 0;
}
