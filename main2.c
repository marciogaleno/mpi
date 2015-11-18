#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ordenacao.c"
#include "numerosAleatorios.c"

#define TAM 40

int main(int argc,char** argv)
{
   MPI_Status status;
   int     myrank;
   int     position;
   int     n;
   float   a, b;
   char    buffer[TAM * 4];
   int vet[TAM];
   int *vet1, *vet2, *vet3;
   int pivo;
   int tag;
 
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  int i;
   if (myrank == 0){
      n = 4;
      a = 1.;
      b = 2.;
      printf("Rank=%d, a= %f, b= %f, and n=%d\n", myrank, a, b, n);
      position = 0;

      preencheVetor(vet, TAM);



      pivo = separa(vet, 0, TAM -1);

        

      // vet1 = (int *)malloc(sizeof(int) * pivo);
      // memcpy(vet1, vet, pivo * sizeof(int));

      

      MPI_Pack(vet, TAM, MPI_INT, buffer, TAM * 4, &position, MPI_COMM_WORLD);
      //MPI_Pack(&pivo, 1, MPI_INT, buffer, TAM, &position, MPI_COMM_WORLD);
      //MPI_Pack(&n, 1, MPI_INT, buffer, TAM, &position, MPI_COMM_WORLD);
 
    // communication
     MPI_Bcast(buffer, TAM * 4, MPI_PACKED, 0, MPI_COMM_WORLD);
     MPI_Bcast(&pivo, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     //MPI_Send(buffer, TAM * 4, MPI_PACKED, 1, tag, MPI_COMM_WORLD);
 
   }else{
    // communication
    MPI_Bcast(buffer, TAM * 4, MPI_PACKED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&pivo, 1, MPI_INT, 0, MPI_COMM_WORLD); 
      //MPI_Bcast(buffer, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
      position = 0;
      //printf("pivo = %d \n", pivo);

      //MPI_Unpack(buffer, TAM, &position, &pivo, 1, MPI_FLOAT, MPI_COMM_WORLD);

      //vet1 = (int *)malloc(sizeof(int) * 10);
 printf("Pivo = %d\n", pivo );
    // unpacking
      //int vet2[5] = {1, 2, 3, 4 ,5};
      //MPI_Recv(buffer, TAM * 4, MPI_PACKED, 0, tag, MPI_COMM_WORLD, &status);

      MPI_Unpack(buffer, TAM * 4, &position, vet, TAM, MPI_INT, MPI_COMM_WORLD);

      if (myrank == 1){
          ordenacao(vet, 0, pivo);
          MPI_Send(&vet, TAM, MPI_INT, 0, tag, MPI_COMM_WORLD);
      }else if(myrank == 2){
          ordenacao(vet, pivo + 1, TAM);
          MPI_Send(&vet, TAM, MPI_INT, 0, tag, MPI_COMM_WORLD);
      }
      

      


      //int i;
      //for ( i = 0; i < 10; ++i)
        // printf("%d - ", vet[0]);
        //         printf("%d - ", vet[1]);

        // printf("%d - ", vet[2]);

        // printf("%d - ", vet[3]);

        // printf("%d - ", vet[4]);
     

      //{
      //}
      //printf("\n");
      //MPI_Unpack(buffer, 100, &position, &b, 1, MPI_FLOAT, MPI_COMM_WORLD);
      //MPI_Unpack(buffer, 100, &position, &n, 1, MPI_INT, MPI_COMM_WORLD);
      //printf("Rank=%d, a= %d \n", myrank, vet1[1]);
   }
 //MPI_Barrier(MPI_COMM_WORLD);

   if (myrank == 0){
     vet1 = (int *)malloc(sizeof(int) * pivo);
     vet2 = (int *)malloc(sizeof(int) * (TAM - pivo));
     vet3 = (int *)malloc(sizeof(int) * TAM);

     memcpy(vet3, vet1, pivo * sizeof(int));
     memcpy(vet3 + pivo, vet2, (TAM - pivo) * sizeof(int));

    //printf("Pivo = %d\n", pivo );
      MPI_Recv(vet1, TAM, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(vet2, TAM, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);

      for(i = 0; i < TAM; i++){
        printf("%d - ", vet3[i]);
      }
      printf("\n\n\n");

      // for(i = pivo + 1; i < TAM; i++){
      //   printf("%d - ", vet2[i]);
      // }
      // printf("\n");
   }
   free(vet1);
   free(vet2);
   MPI_Finalize();
}
