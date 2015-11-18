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
   int     rank;
   int     posicao;
   char    buffer[TAM * 4];
   int vet[TAM];
   int *vet1, *vet2, *vet3;
   int pivo;
   int tag;
 
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int i;
   if (rank == 0){

      posicao = 0;

      // Preeche vetor com números aleatórios
      preencheVetor(vet, TAM);

      // Separa um vetor em dois e retorna um pivô. 
      pivo = separa(vet, 0, TAM -1);

      // Empacgota o vetor para ser enviados aos processos
      MPI_Pack(vet, TAM, MPI_INT, buffer, TAM * 4, &posicao, MPI_COMM_WORLD);
 
    // Envia o vetor para os outros processos
     MPI_Bcast(buffer, TAM * 4, MPI_PACKED, 0, MPI_COMM_WORLD);

     // Envia o pivê para os outros processos
     MPI_Bcast(&pivo, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     //MPI_Send(buffer, TAM * 4, MPI_PACKED, 1, tag, MPI_COMM_WORLD);
 
   }else{
      // 
      MPI_Bcast(buffer, TAM * 4, MPI_PACKED, 0, MPI_COMM_WORLD);
      MPI_Bcast(&pivo, 1, MPI_INT, 0, MPI_COMM_WORLD); 
        //MPI_Bcast(buffer, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
      posicao = 0;
    
      //printf("Pivo = %d\n", pivo );

      // Desempacota o vetor
      MPI_Unpack(buffer, TAM * 4, &posicao, vet, TAM, MPI_INT, MPI_COMM_WORLD);

      // Se processo 1, ordena os valores do lado esquerdo do pivô
      if (rank == 1){

          ordenacao(vet, 0, pivo);

          // Envia os valores ordenados para o processo 0
          MPI_Send(&vet, TAM, MPI_INT, 0, tag, MPI_COMM_WORLD);
      }else if(rank == 2){// Se processo 2, ordena os valores do lado direto do pivô
          
          ordenacao(vet, pivo + 1, TAM);

          // Envia os valores ordenados para o processo 0
          MPI_Send(&vet, TAM, MPI_INT, 0, tag, MPI_COMM_WORLD);
      }
      
   }
;

   if (rank == 0){

     //Alocando novos vetores que armazenarão os números ordenados
     // vet1 = (int *)malloc(sizeof(int) * pivo);
     // vet2 = (int *)malloc(sizeof(int) * (TAM - pivo));
     // vet3 = (int *)malloc(sizeof(int) * TAM);

     // memcpy(vet3, vet1, pivo * sizeof(int));
     // memcpy(vet3 + pivo, vet2, (TAM - pivo) * sizeof(int));

    //printf("Pivo = %d\n", pivo );
      MPI_Recv(vet, TAM, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(vet, TAM, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);

      // for(i = 0; i < pivo; i++){
      //   printf("%d - ", vet[i]);
      // }
      // printf("\n\n\n");

      // for(i = pivo + 1; i < TAM; i++){
      //   printf("%d - ", vet[i]);
      // }
      // printf("\n");
   }

   MPI_Finalize();
}
