#include "functions.c"

/*
    DISCO DE 16MB

    <--- BOOT RECORD --->
    512 BYTES POR SETOR
    4   SETOR POR CLUSTER

    <--- INFORMAÇÃO DE SETORES --->
    1       SETOR   (512        BYTES)   BOOT_RECORD
    3.276   SETORES (1.677.312  BYTES)   ROOT_DIR      (~10% DO TAMANHO TOTAL)
    29.492  SETORES (15.099.904 BYTES)   DATA_SECTION   (~90% DO TAMANHO TOTAL)

    <--- TOTAL --->
    32.768  SETORES
    8.192  CLUSTERS

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
