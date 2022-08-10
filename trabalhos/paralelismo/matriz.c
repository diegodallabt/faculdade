#include "matriz.h"
#include "omp.h"

// inicializa matriz
double ** inicializaMatriz(double n)
{
    int i;

    double **M = (double **)malloc(n * sizeof(double *));

    for (i = 0; i < n; i++)
        M[i] = (double *)malloc(n * sizeof(double));

    return M;
}

// printa matriz
void imprimeMatriz(double **M, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            printf("%.3f     ", M[i][j]);
        printf("\n");
    }
}

// FUNÇÕES SEQUENCIAIS

// preenche matriz
void preencheMatriz(double **M, int n)
{
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            M[i][j] = rand() % 10 + ((double)(rand() % 10) / 100);
}

// calcula o quadrado da matriz
void quadrado(double ** M, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            M[i][j] = M[i][j] * M[i][j];
}

// calcula a diferença entre duas matrizes
void diferenca(double ** A, double ** B, double ** D, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            D[i][j] = A[i][j] - B[i][j];
}

// soma os elementos da matriz
double soma(double ** M, int n)
{
    int i, j;
    double soma = 0;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            soma += M[i][j];
    return soma;
}

// FUNÇÕES PARALELAS

// preenche matriz usando paralelismo
void preencheMatrizP(double **M, int n)
{
    omp_set_num_threads(num_threads);
    int i, j;

    #pragma omp parallel shared(i,M) private(j)
    {
        #pragma omp for
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                M[i][j] = rand() % 10 + ((double)(rand() % 10) / 100);
    }
}

// calcula o quadrado da matriz usando paralelismo
void quadradoP(double ** M, int n)
{
    omp_set_num_threads(num_threads);
    int i, j;

    #pragma omp parallel shared(i,M) private(j)
    {
        #pragma omp for
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                M[i][j] = M[i][j] * M[i][j];
    }
}

// calcula a diferença entre duas matrizes usando paralelismo
void diferencaP(double ** A, double ** B, double ** D, int n)
{
    omp_set_num_threads(num_threads);
    int i, j;
    #pragma omp parallel shared(i,A, B, D)
    {
        #pragma omp for
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                D[i][j] = A[i][j] - B[i][j];
    }
}

// soma os elementos da matriz usando paralelismo
double somaP(double ** M, int n)
{
    omp_set_num_threads(num_threads);
    int i, j;
    double soma = 0;
    #pragma omp parallel shared (M,i) reduction(+:soma)
    {
        #pragma omp for
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                soma += M[i][j];
    }
    return soma;
}