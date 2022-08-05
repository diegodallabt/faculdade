#include "functions.h"

bool setBR(int TOTAL_SECTORS){
    FILE * img;
    BootRecord br;

    img = fopen( "safs.img" , "r+b" );

    if(img == NULL){
        printf("[setBR] Error opening image file.\n");
    }
    // seta os valores padrões do boot record
    br.bytes_per_sector = BYTES_PER_SECTOR;
    br.sectors_per_cluster = SECTORS_PER_CLUSTER;
    br.reserved_sector = RESERVED_SECTOR;
    br.sector_directory_blocks = (TOTAL_SECTORS-1) * 0.1;
    br.sector_section_data = TOTAL_SECTORS * 0.9;
    br.pointer_to_space_free = 0;
    br.total_main_directory_entries = 0;

    // posiciona ponteiro no inicio da imagem
    fseek(img, 0, SEEK_SET);

    // escreve o bootrecord na imagem
    fwrite(&br, sizeof(br), 1, img);

    fclose(img);
        
    return true;
}

BootRecord readBR(){
    FILE * img;
    BootRecord br;

    img = fopen( "safs.img" , "r+b" );

    if(img == NULL){
        printf("[readBR] Error opening image file.\n");
    }

    // posiciona o ponteiro
    fseek(img, 0, SEEK_SET);

    //lê todo o bootrecord da imagem e armazena
    fread(&br, sizeof(BootRecord), 1, img);

    fclose(img);
    return br;
}

int startDirectoryBlocks(){
    BootRecord br;
    int offset;

    br = readBR();
    offset = br.bytes_per_sector * br.reserved_sector;

    return offset;
}

int startDataSection(){
    BootRecord br;
    int offset;

    br = readBR();

    offset = (br.bytes_per_sector * br.reserved_sector) + (br.bytes_per_sector * br.sector_directory_blocks);

    return offset;
}

int searchCluster(int cluster){
    BootRecord br;
    int offset;

    br = readBR();

    offset = (br.bytes_per_sector * br.reserved_sector) + (br.bytes_per_sector * br.sector_directory_blocks) + (br.bytes_per_sector * br.sectors_per_cluster * cluster);
    
    return offset;
}

void insertBlocksFree(int qtClusters, int firstCluster){
    FILE * img;
    BootRecord br;
    int cluster=0, aux;
    int j=0;

    br = readBR();
    img = fopen( "safs.img" , "r+b" );

    if(img == NULL){
        printf("Error opening image file.\n");
    }
    cluster = br.pointer_to_space_free;
    while(cluster!=0xFFFF){
        fseek(img, searchCluster(cluster), SEEK_SET);
        aux = cluster;
        fread(&cluster, sizeof(unsigned int), 1, img);
    }

    fseek(img, searchCluster(aux), SEEK_SET);
    fwrite(&firstCluster, 1, sizeof(unsigned int), img);

    aux = firstCluster;

    int i = 0;
    while (i<qtClusters)
    {
        fseek(img, searchCluster(aux), SEEK_SET);
        i++;
        aux++;
        if(i == qtClusters){
            aux = 0xFFFF;
            fwrite(&aux, sizeof(unsigned int), 1, img);
            break;
        }
        fwrite(&aux, sizeof(unsigned int), 1, img);
    }
    
    
    fclose(img);
}

void deleteFile(int root, int adress, int entries){
    FILE * img;
    BootRecord br;
    int i=0, j=0, k=0, l=0;
    int qtClusters=0;
    int firstCluster=0, fileSize=0, status=0, lengthName=0, lengthExt=0, atributte=0, addrDir=0, statusEntries=0, firstCFile=0, fileSizeDir=0, qtClustersDir=0;
    unsigned int entriesDir=0;
    char filename[12], filenameInDirectory[12], exte[4], exteInDirectory[4], copy[12], copyExt[4];

    img = fopen( "safs.img" , "r+b" );

    if(img == NULL){
        printf("Error opening image file.\n");
    }
    
    br = readBR();
    printf("Digite o nome do arquivo que quer apagar: ");
    scanf("%s", &filename);
    printf("Digite a extensão do arquivo que quer apagar: ");
    scanf("%s", &exte);
    lengthName = strlen(filename);
    lengthExt = strlen(exte);
    strcpy(copyExt, exte);
    char byte_nulo[1] = {0};

    switch (entries)
    {
    case 0:
        printf("[!] O diretório está vazio.\n");
        break;
    default:
        while (j<entries)
        {
            // posiciona o ponteiro no status do arquivo/diretório
            fseek(img, adress + i, SEEK_SET);
            //lê o status do arquivo
            fread(&status, sizeof(char), 1, img);
            
            if(status == 0){
                
                // lê o nome da imagem para comparar se é igual ao que foi digitado
                fseek(img, adress + 1 + i, SEEK_SET);
                fread(&filenameInDirectory, sizeof(char), 12, img);
                strcpy(copy, filenameInDirectory);
                fseek(img, adress + 13 + i, SEEK_SET);
                fread(&exteInDirectory, sizeof(char), 4, img);
                
                // retira o lixo de memória da variável obtida da imagem
                copy[lengthName + 1] = '\0';
                exteInDirectory[lengthExt + 1] = '\0';
                
                
                if(!strcmp(filename, copy) && !strcmp(copyExt, exteInDirectory)){
                    
                    // escreve 0xFF no status para definir que ele foi apagado
                    status = 0xFF;
                    fseek(img, adress + i, SEEK_SET);
                    fwrite(&status, sizeof(char), 1, img);

                    // armazena o primeiro cluster do arquivo
                    fseek(img, adress + 18 + i, SEEK_SET);
                    fread(&firstCluster, sizeof(unsigned int), 1, img);
                    
                    // armazena o tamanho do arquivo
                    fseek(img, adress + 22 + i, SEEK_SET);
                    fread(&fileSize, sizeof(unsigned int), 1, img);
                    
                    
                    /*
                    // posiciona no primeiro cluster do arquivo
                    fseek(img, searchCluster(firstCluster), SEEK_SET);
                    
                    // apaga todo conteúdo do arquivo
                    for(int k=0; k<fileSize; k++){
                        fwrite(&byte_nulo, sizeof(char), 1, img);
                    }*/

                    if(!strcmp("dir", copyExt)){
                        addrDir = searchCluster(firstCluster);
                        // posiciona na sessao de dados do diretorio
                        fseek(img, addrDir + 2047, SEEK_SET);
                        fread(&entriesDir, sizeof(unsigned char), 1, img);                  
                        
                        fseek(img, addrDir, SEEK_SET);
                        
                        while(k<entriesDir){
                            //printf("APAGANDO ENTRADAS\n");
                            // posiciona o ponteiro no status do arquivo/diretório
                            fseek(img, addrDir + l, SEEK_SET);
                            //lê o status do arquivo
                            fread(&statusEntries, sizeof(char), 1, img);

                            if(statusEntries == 0){
                                fseek(img, addrDir + 18 + l, SEEK_SET);
                                fread(&firstCFile, sizeof(unsigned int), 1, img);
                                // armazena o tamanho do arquivo
                                fseek(img, addrDir + 22 + l, SEEK_SET);
                                fread(&fileSizeDir, sizeof(unsigned int), 1, img);
                                
                                qtClustersDir = ((fileSizeDir/512)/4)+1;
                                //printf("QT CLUSTER: %d | FIRSTCLUSTER: %d \n", qtClustersDir, firstCFile);
                                insertBlocksFree(qtClustersDir, firstCFile);
                            }
                            l+=28;
                            k++;
                        }

                    }
                    qtClusters = ((fileSize/512)/4)+1;
                    insertBlocksFree(qtClusters, firstCluster);

                    if(root){
                        unsigned int attBR=0;
                        fseek(img, 12, SEEK_SET);
                        fread(&attBR, sizeof(unsigned int), 1, img);
                        attBR--;
                        fseek(img, 12, SEEK_SET);
                        fwrite(&attBR, sizeof(unsigned int), 1, img);
                    }else{
                        unsigned int attDIR=0;
                        fseek(img, adress + 2047, SEEK_SET);
                        fread(&attDIR, sizeof(unsigned int), 1, img);
                        attDIR--;
                        fseek(img, adress + 2047, SEEK_SET);
                        fwrite(&attDIR, sizeof(unsigned int), 1, img);
                    }
                    
                }
            } 
            
            // vai para a próxima entrada
            i+=28;
            // incrementa indicando que mais uma entrada foi lida
            j++;
        }

        break;
    }

    fclose(img);
}

void reallocateFirstPointer(int qtClusters){
    FILE * img;
    BootRecord br;
    int cluster = 0;
    int allocate = 0, aux = 0;

    br = readBR();
    img = fopen( "safs.img" , "r+b" );

    if(img == NULL){
        printf("Error opening image file.\n");
    }

    cluster = br.pointer_to_space_free;
    aux = br.pointer_to_space_free;

    while (allocate == 0)
    {
        for (int i = 0; i < qtClusters; i++)
        {
            fseek(img, searchCluster(aux), SEEK_SET);
            fread(&aux, sizeof(unsigned int), 1, img);
            if(aux == 0xFFFF){
                printf("[!] Espaço cheio ou fragmentado, não é possível alocar esse arquivo.\n");
                break;
            }
            if(cluster == aux-1){
                allocate = 1;
                cluster++;
            } else {
                allocate = 0;
                cluster = aux;
            }
        }
        if(aux == 0xFFFF)
            break;
    }

    if(allocate == 1){
        cluster = br.pointer_to_space_free;
    }
    
    for(int i = 0; i < qtClusters; i++){
        fseek(img, searchCluster(cluster), SEEK_SET);
        fread(&cluster, sizeof(unsigned int), 1, img);
    }

    fseek(img, 8, SEEK_SET);
    fwrite(&aux, 1, sizeof(unsigned int), img);

    fclose(img);
}

bool inicializeLinkList(){
    FILE * img;
    BootRecord br;
    int i = 0;
    
    br = readBR();
    img = fopen( "safs.img" , "r+b" );

    if(img == NULL){
        printf("[spaceFree] Error opening image file.\n");
    }
    
    int qtTotalClusters = (br.sector_section_data /4);
    int final = 0xFFFF;
    while(i < qtTotalClusters){
        fseek(img, searchCluster(i), SEEK_SET);
        i++;
        if(i == qtTotalClusters){
            fwrite(&final, 1, sizeof(unsigned int), img);
        } else{
            fwrite(&i, 1, sizeof(unsigned int), img);
        }
        
    }
    
    fclose(img);
    return true;
}

void createImage(){
    FILE * img;
    int n;
    printf("Digite quantos setores gostaria de reservar para a partição formatada: ");
    scanf("%d", &n);

    if(n<6){
        printf("[!] Erro: O tamanho minímo para uma partição formatada é de 6 setores.\n");
        return;
    }
    
    img = fopen( "safs.img" , "wb" );

    if(img == NULL){
        printf("[!] Erro ao abrir o arquivo.\n");
    }

    char byte[1] = {0};

    for(int i=0 ; i<= (n*512); i++){
        fwrite(byte, 1 , sizeof(byte) , img);
    }
    
    if(setBR(n) && inicializeLinkList()){
        printf("[!] Uma nova imagem foi criada com sucesso e está pronta para uso.\n");
    }

    fclose(img);
}

void createDir(int adress, int total_entries){
    FILE * img;
    unsigned char nome[12], exte[4];
    BootRecord br;
    br = readBR();
    printf("Digite o nome do diretório que quer criar: ");
    scanf("%s", &nome);
    strcpy(exte, "dir");
    
    img = fopen("safs.img", "r+b");
    if(img == NULL){
        printf("[!] Erro ao abrir o arquivo.\n");
    }

    FileFormat file;
    
    file.status = 0x00;
    strcpy(file.name, nome);
    strcpy(file.ext, exte);
    file.fileSize = 0;   
    file.attribute = 0x10;     
    file.first_cluster = br.pointer_to_space_free; 
    file.total_entries = 0;

    reallocateFirstPointer(1);

    fseek(img, adress + (28 * total_entries), SEEK_SET);
    fwrite(&file, sizeof(FileFormat), 1, img);

    unsigned int attBR=0;
    fseek(img, 12, SEEK_SET);
    fread(&attBR, sizeof(unsigned int), 1, img);
    attBR++;
    fseek(img, 12, SEEK_SET);
    fwrite(&attBR, sizeof(unsigned int), 1, img);

    fclose(img);

}

void copyFile(char * filename, int root, int adress, int entries){
    unsigned char nome[12], exte[4], filenameInDirectory[12], copy[12], exteInDirectory[4];
    BootRecord br;
    int qtdClusters=0, firstCluster=0, already=0, lengthName=0, lengthExt=0, status=0, j=0, i=0;
    printf("\nDigite o nome do arquivo a ser copiado:\t\t");
    scanf("%s", &nome);
    printf("Digite a extensao do arquivo a ser copiado:\t");
    scanf("%s", &exte);
    
    br = readBR();

    char buffer[100];
    strcat(strcpy(buffer, nome), ".");
    strcat(buffer, exte); 
    lengthName = strlen(nome);
    lengthExt = strlen(exte);

    FILE *arq_origem = fopen(buffer, "rb+");
    FILE *arq_destino = fopen("safs.img", "rb+");
    if(arq_origem == NULL){
        printf("Erro ao abrir o arquivo a copiar.\n");
        return;
    }
    if(arq_destino == NULL){
        printf("Erro ao abrir o arquivo destino.\n");
        return;
    }

    while (j<entries)
    {
        fseek(arq_destino, adress + 1 + i, SEEK_SET);
        fread(&filenameInDirectory, sizeof(unsigned char), 12, arq_destino);
        filenameInDirectory[lengthName + 1] = '\0';
        strcpy(copy, filenameInDirectory);

        fseek(arq_destino, adress + 13 + i, SEEK_SET);
        fread(&exteInDirectory, sizeof(unsigned char), 4, arq_destino);
        exteInDirectory[lengthExt + 1] = '\0'; 

        fseek(arq_destino, adress + i, SEEK_SET);
        fread(&status, sizeof(char), 1, arq_destino);

        
        if(!strcmp(filenameInDirectory, nome) && !strcmp(exteInDirectory, exte)){
            if(status != 0xFF)
                already = 1;
        }    

        i+=28;
        j++; 
    }
    if(already == 1){
        printf("[!] Arquivo já existe neste local. \n");
    } else {
        unsigned int tamanho=0;
        unsigned char conteudo[1048576];    
        while(!feof(arq_origem)){
            fread(&conteudo[tamanho], sizeof(unsigned char), 1, arq_origem);
            tamanho++;
        }
        tamanho-=1;
        
        qtdClusters = (tamanho/512)/4;
        firstCluster = br.pointer_to_space_free;

        reallocateFirstPointer(qtdClusters+1);

        fseek(arq_destino, searchCluster(firstCluster), SEEK_SET);     
        for(int i=0 ; i< tamanho; i++)
            fwrite(&conteudo[i], sizeof(unsigned char), 1, arq_destino);

        FileFormat file_origem;
        file_origem.status = 0x00;
        strcpy(file_origem.name, nome);
        strcpy(file_origem.ext, exte);
        file_origem.fileSize = tamanho;
        file_origem.attribute = 0x20;
        file_origem.first_cluster = firstCluster;
        file_origem.total_entries = 0;  
        
        if(entries == 0){
            fseek(arq_destino, adress, SEEK_SET); 
            fwrite(&file_origem, sizeof(FileFormat), 1, arq_destino);
        }else{
            fseek(arq_destino, adress + (entries * 28), SEEK_SET);
            fwrite(&file_origem, sizeof(FileFormat), 1, arq_destino);
        }

        if(root){
            unsigned int attBR=0;
            fseek(arq_destino, 12, SEEK_SET);
            fread(&attBR, sizeof(unsigned int), 1, arq_destino);
            attBR++;
            fseek(arq_destino, 12, SEEK_SET);
            fwrite(&attBR, sizeof(unsigned int), 1, arq_destino);
        }else{
            unsigned int attDIR=0;
            fseek(arq_destino, adress + 2047, SEEK_SET);
            fread(&attDIR, sizeof(unsigned int), 1, arq_destino);
            attDIR++;
            fseek(arq_destino, adress + 2047, SEEK_SET);
            fwrite(&attDIR, sizeof(unsigned int), 1, arq_destino);
        }
        

        printf("\nO arquivo \"%s\" foi copiado com sucesso.\n", buffer);
    }

    
    fclose(arq_origem);
    fclose(arq_destino);
}

void extractArq(char * filename, int adress, int entries){
    unsigned char nome[12], exte[4], nameInDirectory[12];
    int i=0, j=0, length, firstCluster, fileSize, already = 0;

    BootRecord br;
    br = readBR();

    printf("\nDigite o nome do arquivo a ser extrair:\t\t");
    scanf("%s", &nome);
    printf("Digite a extensao do arquivo a ser extrair:\t");
    scanf("%s", &exte);

    length = strlen(nome);

    char buffer[100];
    strcat(strcpy(buffer, nome), ".");
    strcat(buffer, exte); 
    
    FILE *arq_destino = fopen(buffer, "wb+");
    FILE *arq_origem = fopen("safs.img", "rb+");
    if(arq_destino == NULL){
        printf("Erro ao abrir o arquivo destino.\n");
        return;
    }
    if(arq_origem == NULL){
        printf("Erro ao abrir o arquivo de origem.\n");
        return;
    }
    unsigned char conteudo[1048576]; 

    while (j<entries){
        fseek(arq_origem, adress + i + 1, SEEK_SET);
        fread(&nameInDirectory, sizeof(unsigned char), 12, arq_origem);
        nameInDirectory[length + 1] = '\0';
        if(!strcmp(nome, nameInDirectory)){
            fseek(arq_origem, adress + i + 18, SEEK_SET);
            fread(&firstCluster, sizeof(unsigned int), 1, arq_origem);
            fseek(arq_origem, adress + i + 22, SEEK_SET);
            fread(&fileSize, sizeof(unsigned int), 1, arq_origem);
            
            printf("Extraindo o arquivo...\n");

            fseek(arq_origem, searchCluster(firstCluster), SEEK_SET);
            for (int k = 0; k < fileSize; k++)
            {
                fread(&conteudo[k], sizeof(unsigned char), 1, arq_origem);
            }
            fseek(arq_destino, 0, SEEK_SET);
            for(int k = 0; k < fileSize; k++){
                fwrite(&conteudo[k], sizeof(unsigned char), 1, arq_destino);
            }
            already = 1;
             printf("O arquivo \"%s\" foi extraido com sucesso.\n", buffer);
        }else if(j == entries){
            printf("[!] Arquivo não encontrado.\n");
        }
        if(already == 1){
            break;
        }
        i+=28;
        j++;
    }

    fclose(arq_destino);
    fclose(arq_origem);
}

void showFiles(char * filename, int adress, int root, int entries){          
    FILE * img = fopen(filename, "rb+");
    if(img == NULL){
        printf("Erro ao abrir o arquivo para listar os itens.\n");
        return;
    }
    
    unsigned char status;
    FileFormat file;
    int i=0, j=0;
    BootRecord br = readBR();

    while(j<entries){
        //printf("Entrou no while\n");
        fseek(img, adress + i, SEEK_SET);
        fread(&status, sizeof(unsigned char), 1, img);
        if(status == 0x00){
            fseek(img, adress + i, SEEK_SET);
            fread(&file, sizeof(FileFormat), 1, img);
            if(file.attribute == 0x20){
                printf("\nArquivo\n");
                printf("Nome  : %hs\n", file.name);
                printf("Ext   : %hs\n", file.ext);
                printf("Atb   : %hx\n", file.attribute);
                printf("FClst : %d\n", file.first_cluster);
                printf("Fsize : %d\n", file.fileSize);
            }
            else if(file.attribute == 0xF){
                printf("\nLong File Name : %hs\n", file.name);
            }
            else if(file.attribute == 0x10){
                printf("\nDiretorio : %hs\n", file.name);
            }
        }
        i+=28;
        j++;
    }
    //printf("Saiu do while\n");


    if(entries == 0){
        printf("[!] Nenhum arquivo encontrado.\n");
    }
    
    fclose(img);
}

int next(int endAtual, int entries){
    FILE * img;
    BootRecord br;
    int i=0, j=0, status=0, atributte=0, firstCluster, total_directory_entries;

    br = readBR();

    img = fopen( "safs.img" , "r+b" );

    if(img == NULL){
        printf("Error opening image file.\n");
    }

    char dirName[12], dirInDirectory[12];
    int length;
    printf("Qual o nome do diretório que quer acessar?\n");
    scanf("%s", &dirName);
    length = strlen(dirName);
        if(entries==0)
            printf("[!] O diretório atual não possui esse arquivo.\n");
        else{
            while(j<entries){
                fseek(img, endAtual + i, SEEK_SET);
                fread(&status, sizeof(unsigned char), 1, img);
                fseek(img, endAtual + 17 + i, SEEK_SET);
                fread(&atributte, sizeof(unsigned char), 1, img);

                if(atributte == 0x10){
                    
                    fseek(img, endAtual + 1 + i, SEEK_SET);
                    fread(&dirInDirectory, sizeof(char), 12, img);
                    dirInDirectory[length + 1] = '\0';

                    if(!strcmp(dirName, dirInDirectory)){
                        fseek(img, endAtual + 18 + i, SEEK_SET);
                        fread(&firstCluster, sizeof(unsigned int), 1, img);

                        fseek(img, endAtual + 26 + i, SEEK_SET);
                        fread(&total_directory_entries, sizeof(unsigned short), 1, img);

                        endAtual = searchCluster(firstCluster);
                        
                    }
                }
                i+= 28;
                j++;
            }
        }

    fclose(img);

    return endAtual;
}

void navigator(char *filename){
    FILE * img;
    BootRecord br;
    img = fopen( "safs.img" , "r+b" );
    if(img == NULL){
        printf("Error opening image file.\n");
    }
    int endAtual = startDirectoryBlocks();
    int root=0, entries=0;
    int acessos[1000], profundidade=0;
    int total_directory_entries=0;
    int aux=0;
    int value = 0;
    br = readBR();
    acessos[profundidade] = endAtual;
    while(value != 7){
        fseek(img, endAtual, SEEK_SET);

        
        if(endAtual == startDirectoryBlocks()){
            showFiles("safs.img", endAtual, root, br.total_main_directory_entries);
        }
        else{
            fseek(img, endAtual + 2047, SEEK_SET);
            fread(&total_directory_entries, sizeof(unsigned char), 1, img);
            showFiles("safs.img", endAtual, root, total_directory_entries);
        }

        printf("\nDeseja realizar qual ação?\n1. Ir para o próximo diretório.\n2. Voltar um diretório.\n3. Criar um diretório.\n4. Copiar um arquivo do disco para o diretorio.\n5. Extrair arquivo.\n6. Apagar.\n7. Sair\n");
        scanf("%d", &value);

        switch (value)
        {
        case 1:
            acessos[profundidade] = endAtual;
            profundidade++;
            if(endAtual == startDirectoryBlocks()){
                br = readBR();
                endAtual = next(endAtual, br.total_main_directory_entries);
            }
            else{
                fseek(img, endAtual + 2047, SEEK_SET);
                fread(&total_directory_entries, sizeof(unsigned char), 1, img);
                endAtual = next(endAtual, total_directory_entries);
            }            
            break;
        case 2:
            if(endAtual == startDirectoryBlocks()){
                printf("[!] Não é possível voltar, você está no diretório raiz.\n");
                break;
            }
            profundidade--;
            fseek(img, acessos[profundidade], SEEK_SET);
            endAtual = acessos[profundidade];
            break;
        case 3:
            if(endAtual == startDirectoryBlocks()){
                createDir(startDirectoryBlocks(), br.total_main_directory_entries);
            }else{
                fseek(img, endAtual + 2047, SEEK_SET);
                fread(&total_directory_entries, sizeof(unsigned char), 1, img);
                fseek(img, endAtual, SEEK_SET);
                createDir(endAtual, total_directory_entries);
                fseek(img, endAtual + 2047, SEEK_SET);
                total_directory_entries++;
                entries = total_directory_entries;
                fwrite(&total_directory_entries, sizeof(unsigned char), 1, img);
            }
            br = readBR();
            break;
        case 4:
            br = readBR();
            if(endAtual == startDirectoryBlocks()){
                copyFile("safs.img", 1, endAtual, br.total_main_directory_entries);
                br = readBR();
            } else{
                fseek(img, endAtual + 2047, SEEK_SET);
                fread(&total_directory_entries, sizeof(unsigned char), 1, img);
                copyFile("safs.img", 0, endAtual, total_directory_entries);
            }
            break;
        case 5:
            br = readBR();
            if(endAtual == startDirectoryBlocks()){
                extractArq("safs.img", endAtual, br.total_main_directory_entries);
            }else{
                fseek(img, endAtual + 2047, SEEK_SET);
                fread(&total_directory_entries, sizeof(unsigned char), 1, img);
                extractArq("safs.img", endAtual, total_directory_entries);
            }
            break;
        case 6:
            br = readBR();
            if(endAtual == startDirectoryBlocks()){
                deleteFile(1, endAtual, br.total_main_directory_entries);
                br = readBR();
            }else{
                fseek(img, endAtual + 2047, SEEK_SET);
                fread(&total_directory_entries, sizeof(unsigned char), 1, img);
                deleteFile(0, endAtual, total_directory_entries);
            }
            break;
        case 7:
            break;
        }
    }
    fclose(img);
}