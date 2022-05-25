#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void transposeM2(double ** MT, double ** M2, int r, int c){
   for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			MT[i][j] = M2[j][i];
        }
   }
}

void mulM1M2(double ** M1, double ** M2, double ** MR, int r, int c){
   for (int i = 0; i < r; i++) {
      for (int j = 0; j < c; j++) {
         MR[i][j] = 0;
         for (int k = 0; k < c; k++) {
            MR[i][j] += M1[i][k] * M2[k][j];
         }
      }
   }
}

void mulM1M2T(double ** M1, double ** MT, double ** MRMT, int r, int c){
   for (int i = 0; i < r; i++) {
      for (int j = 0; j < c; j++) {
         MRMT[i][j] = 0;
         for (int k = 0; k < c; k++) {
            MRMT[i][j] += M1[i][k] * MT[j][k];
         }
      }
   }
}

void printMatrix(double ** M, int r, int c){
	for (int i = 0; i < r; i++){
		for (int j = 0; j < c; j++)
			printf("%.2lf ", M[i][j]);
      printf("\n");
   }
}


int main(int argc, char const *argv[])
{
	int r1, c1, r2, c2, i, j;
   float tempo = 0.0;
   clock_t inicio, fim;

   if(argc != 6) return 0;

   r1 = atoi(argv[1]);
   c1 = atoi(argv[2]);
   r2 = atoi(argv[3]);
   c2 = atoi(argv[4]);
   
   if(c1 != r2){
      printf("Impossível multiplicar matrizes com essas dimensões.\n");
      exit(1);
   }

	double* M1[r1];
   double* M2[r2];
   double* MR[r1];
   double* MT[c2];
   double* MRMT[c1];

   srand(time(NULL));

   /* dynamic allocation */
	for (i = 0; i < r1; i++)
		M1[i] = (double*)malloc(c1 * sizeof(double));

   for (i = 0; i < r2; i++)
		M2[i] = (double*)malloc(c2 * sizeof(double));

   for (i = 0; i < r1; i++)
      MR[i] = (double*)malloc(c2 * sizeof(double));

   for (i = 0; i < c2; i++)
      MT[i] = (double*)malloc(r2 * sizeof(double));
   
   for (i = 0; i < c1; i++)
      MRMT[i] = (double*)malloc(r2 * sizeof(double));
   
   /* generate M1 */
	for (i = 0; i < r1; i++)
		for (j = 0; j < c1; j++)
			M1[i][j] = rand() % 100 + ((float)(rand() % 100)/100); 

   /* generate M2 */
   for (i = 0; i < r2; i++)
		for (j = 0; j < c2; j++)
			M2[i][j] = rand() % 100 + ((float)(rand() % 100)/100);
   

   
   /*

   printf("\nM1\n");
   printMatrix(M1, r1, c1);

   printf("\nM2\n");
   printMatrix(M2, r2, c2);
   
   */
   

   if (!strcmp(argv[5], "o"))
   { 
      inicio = clock();
      mulM1M2(M1, M2, MR, r1, c2);
      fim = clock();
      /*

      printf("\nMR: \n");
      printMatrix(MR, r1, c2);

      */
     tempo = (float) (((fim - inicio ) + 0.0) / CLOCKS_PER_SEC);

     printf("%fs\n", tempo);
   }else if(!strcmp(argv[5], "t"))
   {
      inicio = clock();
      transposeM2(MT, M2, c2, r2);
      mulM1M2T(M1, MT, MRMT, r1, c2);
      fim = clock();

      tempo = (float) (((fim - inicio ) + 0.0) / CLOCKS_PER_SEC);

      printf("%fs\n", tempo);
      /*
      
      printf("\nT[M2]: \n");
      printMatrix(MT, c2, r2);
   
      printf("\nMRMT: \n");
      printMatrix(MRMT, r1, c2);
      
      */
   }


   /* memory release */
	for (int i = 0; i < r1; i++)
   {
      free(M1[i]);
      free(MR[i]);
      free(MRMT[i]);
   }

   for (int i = 0; i < r2; i++)
      free(M2[i]);

   for (int i = 0; i < c2; i++)
      free(MT[i]);
		
	return 0;
}