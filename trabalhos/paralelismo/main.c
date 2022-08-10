#include "matriz.c"
#include "matriz.h"

void execSequencial(int n) {
    double resultado = 0;
    double **A, **B, **C;
    
    double time_spent_total, time_start, time_end, time_spent_op = 0.0;

    A = inicializaMatriz(n);
    B = inicializaMatriz(n);
    C = inicializaMatriz(n);

    time_start = omp_get_wtime();
    preencheMatriz(A, n);
    preencheMatriz(B, n);
    time_end = omp_get_wtime();

    time_spent_op = time_end - time_start;
    time_spent_total += time_spent_op;
    
    if(feature_flag_print == 1)
    {
        printf("MATRIZ A\n");
        imprimeMatriz(A, n);
        printf("\nMATRIZ B\n");
        imprimeMatriz(B, n);
    }

    if(feature_flag_tempo == 1)
        printf("Tempo de inicialização das matrizes: %f segundos\n", time_spent_op);
    
    time_start = omp_get_wtime();
    quadrado(A, n);
    quadrado(B, n);
    time_end = omp_get_wtime();

    time_spent_op = time_end - time_start;
    time_spent_total += time_spent_op;
    
    if(feature_flag_print == 1)
    {
        printf("\nMATRIZ A QUADRADA\n");
        imprimeMatriz(A, n);
        printf("\nMATRIZ B QUADRADA\n");
        imprimeMatriz(B, n);
    }

    if(feature_flag_tempo == 1)
        printf("Tempo de cálculo do quadrado das matrizes: %f segundos\n", time_spent_op);
    
    time_start = omp_get_wtime();
    diferenca(A, B, C, n);
    time_end = omp_get_wtime();

    time_spent_op = time_end - time_start;
    time_spent_total += time_spent_op;

    if(feature_flag_print == 1)
    {
        printf("\nMATRIZ DE DIFERENÇA\n");
        imprimeMatriz(C, n);
    }

    if(feature_flag_tempo == 1)
        printf("Tempo de cálculo da diferença das matrizes: %f segundos\n", time_spent_op);
    
    time_start = omp_get_wtime();
    resultado = soma(C, n);
    time_end = omp_get_wtime();

    time_spent_op = time_end - time_start;
    time_spent_total += time_spent_op;

    if(feature_flag_print == 1)
    {
        printf("\nResultado: %.3f\n", resultado);
    }
    
    if(feature_flag_tempo == 1)
    {
        printf("Tempo de cálculo da soma: %f segundos\n", time_spent_op);
        printf(" \nTempo de execução do programa %f segundos\n", time_spent_total);
    }

    liberaMatriz(A, n);
    liberaMatriz(B, n);
    liberaMatriz(C, n);
}

void execParalela(int n) {
    double resultado = 0;
    double **A, **B, **C;
    
    double time_spent_total, time_start, time_end, time_spent_op = 0.0;

    A = inicializaMatriz(n);
    B = inicializaMatriz(n);
    C = inicializaMatriz(n);

    time_start = omp_get_wtime();
    preencheMatrizP(A, n);
    preencheMatrizP(B, n);
    time_end = omp_get_wtime();

    time_spent_op = time_end - time_start;
    time_spent_total += time_spent_op;
    
    if(feature_flag_print == 1)
    {
        printf("MATRIZ A\n");
        imprimeMatriz(A, n);
        printf("\nMATRIZ B\n");
        imprimeMatriz(B, n);
    }

    if(feature_flag_tempo == 1)
        printf("Tempo de inicialização das matrizes: %f segundos\n", time_spent_op);
    
    time_start = omp_get_wtime();
    quadradoP(A, n);
    quadradoP(B, n);
    time_end = omp_get_wtime();

    time_spent_op = time_end - time_start;
    time_spent_total += time_spent_op;
    
    if(feature_flag_print == 1)
    {
        printf("\nMATRIZ A QUADRADA\n");
        imprimeMatriz(A, n);
        printf("\nMATRIZ B QUADRADA\n");
        imprimeMatriz(B, n);
    }

    if(feature_flag_tempo == 1)
        printf("Tempo de cálculo do quadrado das matrizes: %f segundos\n", time_spent_op);
    
    time_start = omp_get_wtime();
    diferencaP(A, B, C, n);
    time_end = omp_get_wtime();

    time_spent_op = time_end - time_start;
    time_spent_total += time_spent_op;

    if(feature_flag_print == 1)
    {
        printf("\nMATRIZ DE DIFERENÇA\n");
        imprimeMatriz(C, n);
    }

    if(feature_flag_tempo == 1)
        printf("Tempo de cálculo da diferença das matrizes: %f segundos\n", time_spent_op);
    
    time_start = omp_get_wtime();
    resultado = somaP(C, n);
    time_end = omp_get_wtime();

    time_spent_op = time_end - time_start;
    time_spent_total += time_spent_op;

    if(feature_flag_print == 1)
    {
        printf("\nResultado: %.3f\n", resultado);
    }
    
    if(feature_flag_tempo == 1)
    {
        printf("Tempo de cálculo da soma: %f segundos\n", time_spent_op);
        printf(" \nTempo de execução do programa %f segundos\n", time_spent_total);
    }
    
    liberaMatriz(A, n);
    liberaMatriz(B, n);
    liberaMatriz(C, n);
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    if(argc < 3 || argv[1] <= 0)
    {
        printf("[!] Parâmetros inválidos\n");
        printf("Use ./out.x < linhas | colunas > < s | p >\n");
        return 0;
    }

    int n = atoi(argv[1]);    
    char modo;

    modo = argv[2][0];

    if(argc == 4)
    {
        num_threads = atoi(argv[3]);
    }
    else num_threads = 1;
    
    if(modo == 's')
    {
        printf("<= EXECUÇÃO SEQUENCIAL =>\n");
        execSequencial(n);
    }
    else if(modo == 'p')
    {
        printf("<= EXECUÇÃO PARALELA =>\n");
        execParalela(n);
    }
    else
    {
        printf("[!] Parâmetro de execução não indetificado\n");
    }

    // execParalela(n);
    return 0;
}
