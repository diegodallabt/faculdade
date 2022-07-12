#include "functions.h"

void createImg(){
    char nulo[1] = {0};
    FILE * img = fopen("safs.img", "wb");
    if(img == NULL) {
        printf("Erro ao criar a imagem.\n");
        return 0;
    }

    for(int i=0 ; i<= SIZE_DISK ; i++){
        fwrite(nulo, 1 , sizeof(nulo) , fp );
    }

    writeBR();

}


BootRecord writeBR(){
    
}

void openFile(char * filename){
    BootRecord BR;
    
    FILE * img = fopen(filename, "rb");
	if(img == NULL) {
        printf("Erro ao abrir a imagem.\n");
    }
}

void closeFile(FILE * img){
    fclose(img);
}