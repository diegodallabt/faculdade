#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int r, c, i, j, count;

   scanf("%d %d", &r, &c);

	double* M1[r];
   double* M2[r];
   double* MR[r];
   double* MT[r];

   srand(time(NULL));

   /* dynamic allocation */
	for (i = 0; i < r; i++)
		M1[i] = (double*)malloc(c * sizeof(double));

   for (i = 0; i < r; i++)
		M2[i] = (double*)malloc(c * sizeof(double));

   for (i = 0; i < r; i++)
      MR[i] = (double*)malloc(c * sizeof(double));

   for (i = 0; i < r; i++)
      MT[i] = (double*)malloc(c * sizeof(double));
   
   /* generate M1 */
	for (i = 0; i < r; i++)
		for (j = 0; j < c; j++)
			M1[i][j] = rand() % 100 + ((float)(rand() % 100)/100); 

   /* generate M2 */
   for (i = 0; i < r; i++)
		for (j = 0; j < c; j++)
			M2[i][j] = rand() % 100 + ((float)(rand() % 100)/100);
   
   /* generate transpose matrix */
   for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			MT[i][j] = M1[j][i];
        }
   }  

   /* print M1 */
   printf("\nM1: \n");
	for (i = 0; i < r; i++){
		for (j = 0; j < c; j++)
			printf("%.2lf ", M1[i][j]);
      printf("\n");
   }

   /* print M2 */
   printf("M2: \n");
   for (i = 0; i < r; i++){
		for (j = 0; j < c; j++)
			printf("%.2lf ", M2[i][j]);
      printf("\n");
   }

   /* mutiplication M1 x M2 */
   for (int i = 0; i < r; i++) {
      for (int j = 0; j < c; j++) {
         MR[i][j] = 0;
         for (int k = 0; k < c; k++) {
            MR[i][j] += M1[i][k] * M2[k][j];
         }
      }
   }

   /* print MR */
   printf("MR: \n");
   for (i = 0; i < r; i++){
		for (j = 0; j < c; j++)
			printf("%.2lf ", MR[i][j]);
      printf("\n");
   }

   /* memory release */
	for (int i = 0; i < r; i++)
   {
      free(M1[i]);
      free(M2[i]);
      free(MR[i]);
   }
		
	return 0;
}
