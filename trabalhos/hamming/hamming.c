#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// formata para exibição de binários em um caracter
void BinFormat(char character, char *text) {
	text[14] = '\0';
    for (int i = 13; i >= 0; i--) text[i] = ((character >> i) & 1) + '0';
}

// formata para exibição de binários para um short int
void BinFormatInt(short int inteiro, char *text) {
	text[14] = '\0';
    for (int i = 13; i >= 0; i--) text[i] = ((inteiro >> i) & 1) + '0';
}

short int encodandoLimpo(char a){
    short int saida = 0;
    char * binario = malloc(14);
    char c[4]={0,0,0,0};
    char g = 0;
    short int aux = 0;

    // setando bits da palavra exceto nas posições reservadas
    int j = 0;
    for(int i = 3; i <= 13; i++){
        if(!(i&(i-1)))
            continue;
        aux = a & (1<<j);
        if(aux) saida |= (1<<i);
        else saida |= (0<<i);
        j++;
    }
    

    // paridade dos bits das palavras reservadas
    for (j = 0; j < 4; j++){
        for (int i = 0; i <= 13; i++){
            if(i & (1<<j))
                c[j] ^= (saida & (1<<i))? 1 : 0;
        }
    }

    // setando os bits de paridade nas palavras reservadas
    for (j = 0; j < 4; j++){
        saida |= (c[j]<<(int)pow(2,j));
    }

    // calcula paridade global
    for (int i = 0; i <= 13; i++){
        g ^= (saida & (1<<i))? 1 : 0;
    }
    // seta bit g
    saida |= g;

    return saida;
}

short int encodando(char a){
    short int saida = 0;
    char * binario = malloc(14);
    char c[4]={0,0,0,0};
    char g = 0;
    short int aux = 0;

    BinFormat(a, binario);
    printf("Encodando o byte `%c` > %c%c%c%c%c%c%c%c\n", a, binario[7],binario[6],binario[5],binario[4], binario[3], binario[2], binario[1], binario[0]);
    // setando bits da palavra exceto nas posições reservadas
    int j = 0;
    for(int i = 3; i <= 13; i++){
        if(!(i&(i-1)))
            continue;
        aux = a & (1<<j);
        if(aux) saida |= (1<<i);
        else saida |= (0<<i);
        j++;
    }
    
    BinFormatInt(saida, binario);
    printf("Copiando bits M para a palavra de hamming > %c%c%c%c%c%c%c%c%c%c%c%c%c\n",  binario[12], binario[11], binario[10], binario[9],binario[8],binario[7],binario[6],binario[5],binario[4], binario[3], binario[2], binario[1], binario[0]);

    // paridade dos bits das palavras reservadas
    for (j = 0; j < 4; j++){
        for (int i = 0; i <= 13; i++){
            if(i & (1<<j))
                c[j] ^= (saida & (1<<i))? 1 : 0;
        }
    }

    // setando os bits de paridade nas palavras reservadas
    for (j = 0; j < 4; j++){
        saida |= (c[j]<<(int)pow(2,j));
    }
    printf("Extraindo bits de paridade: \n C1 > %d\n C2 > %d\n C4 > %d\n C8 > %d\n", c[0], c[1], c[2], c[3]);

    // calcula paridade global
    for (int i = 0; i <= 13; i++){
        g ^= (saida & (1<<i))? 1 : 0;
    }
    g? printf("Calculando a paridade global > 1\n") : printf("Calculando a paridade global > 0\n");
    // seta bit g
    saida |= g;

    BinFormatInt(saida, binario);
    printf("Palavra de hamming gerada > %c%c%c%c%c%c%c%c%c%c%c%c%c\n",  binario[12], binario[11], binario[10], binario[9],binario[8],binario[7],binario[6],binario[5],binario[4], binario[3], binario[2], binario[1], binario[0]);

    printf("Byte `%c` encodado\n\n", a);
    return saida;
}

char decodando(short int saida){
    char c=0;
    char c1=0;
    char * binario = malloc(14);
    char g1=0;
    char g2=0;
    int sindrome=0;
    short int entrada=0;
    short int novaSaida=0;
    short int aux=0;
    short int aux2=0;

    // extraindo bits de dados M
    int j=0;
    for(int i = 3; i <= 13; i++){
        if(!(i&(i-1)))
            continue;

        aux = saida & (1<<i);
        if(aux) entrada |= (1<<j);
        else entrada |= (0<<j);
        j++;
    }
    
    aux=0;
    j=0;
    // extraindo bits de dados C
    for (int i = 1; i <= 8; i++){
        if(!(i&(i-1)) && j<4){
            aux = (saida & (1<<(int)pow(2,j)));
            if(aux) c |= (1<<j);
            else c |= (0<<j);
            j++;
        }
    }

    // encodando a palavra de entrada para gerar uma nova saida e extrair bits c'
    novaSaida = encodandoLimpo(entrada);

    // extraindo bits c' da nova palavra
    j=0;
    for (int i = 1; i <= 8; i++){
        if(!(i&(i-1)) && j<4){
            aux2 = (novaSaida & (1<<(int)pow(2,j)));
            if(aux2) c1 |= (1<<j);
            else c1 |= (0<<j);
            j++;
        }
    }
    printf("Decodando o byte `%c`\n", entrada);
    BinFormat(c, binario);
    printf("C original > %c%c%c%c\n", binario[3], binario[2], binario[1], binario[0]);

    BinFormat(c1, binario);
    printf("Novo C > %c%c%c%c\n", binario[3], binario[2], binario[1], binario[0]);
    
    // encontrando a palavra sindrome
    sindrome = c^c1;

    BinFormat(sindrome, binario);
    printf("Palavra sindrome > %c%c%c%c\n", binario[3], binario[2], binario[1], binario[0]);
    
    if(sindrome!=0 && sindrome<=13){
        saida ^= (1<<sindrome);
        int j=0;
        entrada = 0;
        for(int i = 3; i <= 13; i++){
            if(!(i&(i-1)))
               continue;

            aux = saida & (1<<i);
            if(aux) entrada |= (1<<j);
            else entrada |= (0<<j);
            j++;
        }
        printf("Status > A palavra possuia um bit incorreto na posição: %d.\n", sindrome);
    } else if(sindrome>13){
        printf("Status > Não foi possível recuperar os dados.\n");
    } else {
        printf("Status > Palavra correta.\n");
    }

    // calcula o g antigo
    for (int i = 0; i <= 13; i++){
        g1 ^= (saida & (1<<i))? 1 : 0;
    }

    // calcula o novo g
    for (int i = 0; i <= 13; i++){
        g2 ^= (novaSaida & (1<<i))? 1 : 0;
    }

    // verifica se tem mais de um bit errado
    if(g1 != g2){
        printf("Status > Dois erros foram detectados, impossível recuperar dados.\n");
    }

    return entrada;
}

void leArquivo(const char * entrada, const char * saida);
void escreveArquivo(const char * entrada, const char * saida);

int main(int argc, char const *argv[])
{
    if(argc != 4) return 0;
    
    if(!strcmp(argv[1], "-w")){
        escreveArquivo(argv[2], argv[3]);
    }else if(!strcmp(argv[1], "-r")){
        leArquivo(argv[2], argv[3]);
    }
    
    return 0;
}
void leArquivo(const char * entrada, const char * saida){
    char palavra;
    short int pEncodada;
    FILE * file;
    FILE * file2;

    // abre os arquivos
    file = fopen(entrada, "rb");
    file2 = fopen(saida, "wb+");
    
    // verifica se os arquivos abriram corretamente
    if(file == NULL){
        printf("ERRO: Não foi possível abrir o arquivo de entrada.\n");
        exit(1);
    }

    if(file2 == NULL){
        printf("ERRO: Não foi possível abrir o arquivo de saída.\n");
        exit(1);
    }

    // lê cada caracter do arquivo encodado e decoda para o arquivo de saída
    while(palavra != EOF){
        fread(&pEncodada, sizeof(short int), 1, file);
        if (feof(file))
        {
            break;
        }
        palavra = decodando(pEncodada);
        
        printf("Byte `%c` decodado\n\n", palavra);
        fwrite(&palavra, sizeof(char), 1, file2);
    }
    
    // fecha os arquivos
    fclose(file);
    fclose(file2);
}

void escreveArquivo(const char * entrada, const char * saida){
    char palavra;
    short int pEncodada, pDecodada = 0;
    FILE * file;
    FILE * file2;

    // abre os arquivos
    file = fopen(entrada, "rb");
    file2 = fopen(saida, "wb+");

    // verifica se os arquivos abriram corretamente
    if(file == NULL){
        printf("ERRO: Não foi possível abrir o arquivo de entrada.\n");
        exit(1);
    }

    if(file2 == NULL){
        printf("ERRO: Não foi possível abrir o arquivo de saída.\n");
        exit(1);
    }

    // lê cada caracter do arquivo, encoda e depois decoda
    while(palavra != EOF){
        fread(&palavra, sizeof(char), 1, file);
        if (feof(file))
        {
            break;
        }
        if(palavra == 10){
            continue;
        }
        
        pEncodada = encodando(palavra);
        fwrite(&pEncodada, sizeof(short int), 1, file2);
    }
    
    // fecha os arquivos
    fclose(file);
    fclose(file2);
}
