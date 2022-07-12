#include "functions.c"

/*
    DISCO DE 16MB

    <--- BOOT RECORD --->
    256 BYTES POR SETOR
    4   SETOR POR CLUSTER

    <--- INFORMAÇÃO DE SETORES --->
    1       SETOR   (256        BYTES)   BOOT_RECORD
    6.553   SETORES (1.677.568  BYTES)   ROOT_DIR      (~10% DO TAMANHO TOTAL)
    58.982  SETORES (15.099.392 BYTES)   DATA_SECTION   (~90% DO TAMANHO TOTAL)

    <--- TOTAL --->
    65.536  SETORES
    16.384  CLUSTERS

*/

int main(int argc, char const *argv[])
{
    char filename[13];
    if(argc == 2)
        strcpy(filename, argv[1]);
    else if (argc > 2){
        printf("Argumentos Invalidos.\n");
        return 1;
    }
    else{
        createImg();
    }
        
        
    openFile(filename);
    



    return 0;
}
