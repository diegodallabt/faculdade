#include <stdio.h>
#include <stdlib.h>
#include <math.h>

short int encodando(char a){
    short int saida = 0;
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

char decodando(short int saida){
    char c=0;
    char c1=0;
    char index=0;
    short int entrada=0;
    short int novaSaida=0;
    short int aux, aux2, aux3=0;

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

    // extraindo bits de dados C
    // 111101010101
    for (j = 0; j < 4; j++){
        for (int i = 0; i <= 13; i++){
            c |= ((saida & (1<<(int)pow(2,j)))<<j);
        }
    }
    /*j = 0;
    for (int i = 1; i <= 8; i++){
        if(!(i&(i-1)) && j<4){
            aux2 = (saida & (1<<(int)pow(2, j)));
            if(aux2) c |= (1<<j);
            else c |= (0<<j);
            j++;
        }
    }*/

    novaSaida = encodando(entrada);

    for (j = 0; j < 4; j++){
        for (int i = 0; i <= 13; i++){
            c1 |= ((novaSaida & (1<<(int)pow(2,j)))<<j);
        }
    }
   /* j=0;
    for (int i = 1; i <= 8; i++){
        if(!(i&(i-1)) && j<4){
            aux3 = (novaSaida & (1<<(int)pow(2, j)));
            if(aux3) c1 |= (1<<j);
            else c1 |= (0<<j);
            j++;
        }
    }
*/
    index = c^c1;
    
    if(index!=0 && index<=13){
        saida ^= (1<<index);
    } else if(index>13){
        printf("Não foi possível recuperar os dados.\n");
    } else {
        printf("Palavra correta.\n");
    }
    
    return saida;
}
/*
void lendoArquivo(){
    char palavra;
    short int pEncodada, pDecodada = 0;
    FILE * file;

    // abre o arquivo
    file = fopen("entrada.txt", "rt");

    // verifica se o arquivo abriu corretamente
    if(file == NULL){
        printf("ERRO: Não foi possível abrir o arquivo.\n");
        exit(1);
    }

    // lê cada caracter do arquivo, encoda e depois decoda
    while(palavra != EOF){
        palavra = fgetc(file);
        printf("Caracter lido: %c\n", palavra);

        pEncodada = encodando(palavra);
        pDecodada = decodando(pEncodada);
        printf("%c\n\n", pDecodada);
        
    }
    
    // fecha o arquivo
    fclose(file);
}*/

int main(int argc, char const *argv[])
{
    char palavra='t';
    short int pEncodada, pDecodada = 0;

    // 111101010101
    pEncodada = encodando(palavra);
    pDecodada = decodando(pEncodada);
    printf("%c\n", pDecodada);

    //lendoArquivo();

    return 0;
}
