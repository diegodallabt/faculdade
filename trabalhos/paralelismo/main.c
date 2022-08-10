#include "matriz.c"

int main(int argc, char const *argv[])
{
    int n = atoi(argv[1]);
    double resultado = 0;
    double **A, **B, **C;
    
    double time_spent, time_start, time_end = 0.0;

    A = inicializaMatriz(n);
    B = inicializaMatriz(n);
    C = inicializaMatriz(n);

    
    srand(time(NULL));

    time_start = omp_get_wtime();
    preencheMatriz(A, n);
    preencheMatriz(B, n);
    
    printf("MATRIZ A\n");
    // imprimeMatriz(A, n);
    printf("\nMATRIZ B\n");
    // imprimeMatriz(B, n);

    quadrado(A, n);
    quadrado(B, n);

    printf("\nMATRIZ A QUADRADA\n");
    // imprimeMatriz(A, n);
    printf("\nMATRIZ B QUADRADA\n");
    // imprimeMatriz(B, n);

    diferenca(A, B, C, n);
    printf("\nMATRIZ DE DIFERENÇA\n");
    // imprimeMatriz(C, n);

    resultado = soma(C, n);
    time_end = omp_get_wtime();
    printf("\nSOMA = %.3f\n", resultado);
    

    time_spent = time_end - time_start;

    printf("The elapsed time is %f seconds", time_spent);
    return 0;
}
