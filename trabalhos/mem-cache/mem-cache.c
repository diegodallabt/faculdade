#include <stdio.h>
#include <stdlib.h>

int main()
{
	int r = 3, c = 3, i, j, count;

	int* M1[r];
   int* M2[r];
   int* MR[r];

	for (i = 0; i < r; i++)
		M1[i] = (int*)malloc(c * sizeof(int));

   for (i = 0; i < r; i++)
		M2[i] = (int*)malloc(c * sizeof(int));

   for (i = 0; i < r; i++)
      MR[i] = (int*)malloc(c * sizeof(int));
   
	count = 0;
	for (i = 0; i < r; i++)
		for (j = 0; j < c; j++)
			M1[i][j] = ++count; 

   count = 0;
   for (i = 0; i < r; i++)
		for (j = 0; j < c; j++)
			M2[i][j] = ++count;

   printf("M1: \n");
	for (i = 0; i < r; i++){
		for (j = 0; j < c; j++)
			printf("%d ", M1[i][j]);
      printf("\n");
   }

   printf("M2: \n");
   for (i = 0; i < r; i++){
		for (j = 0; j < c; j++)
			printf("%d ", M2[i][j]);
      printf("\n");
   }

   for (int i = 0; i < r; i++) {
      for (int j = 0; j < c; j++) {
         MR[i][j] = 0;
         for (int k = 0; k < c; k++) {
            MR[i][j] += M1[i][k] * M2[k][j];
         }
      }
   }

   printf("MR: \n");
   for (i = 0; i < r; i++){
		for (j = 0; j < c; j++)
			printf("%d ", MR[i][j]);
      printf("\n");
   }

	for (int i = 0; i < r; i++)
   {
      free(M1[i]);
      free(M2[i]);
      free(MR[i]);
   }
		
	return 0;
}
