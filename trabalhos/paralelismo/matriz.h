#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "omp.h"

double ** inicializaMatriz(double n);
void imprimeMatriz(double ** M, int n);

void preencheMatriz(double ** M, int n);
void quadrado(double ** M, int n);
void diferenca(double ** A, double ** B, double ** D, int n);
double soma(double ** M, int n);

unsigned int num_threads;
void preencheMatrizP(double **M, int n);
void quadradoP(double ** M, int n);
void diferencaP(double ** A, double ** B, double ** D, int n);
double somaP(double ** M, int n);