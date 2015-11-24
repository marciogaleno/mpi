#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ordenacao.c"
#include "numerosAleatorios.c"

#define TAM 200

int main(int argc,char** argv)
{
   MPI_Status status;
   MPI_Request request;
   int     rank;
   int     posicao;
   char    buffer[TAM];
   int vet[TAM];
   int *vet1, *vet2, *vet3, *vet4;
   int pivo01, pivo02, pivo03;
   int tag;
 
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   //MPI_Buffer_attach(buffer, TAM * 4);
  int i;
   if (rank == 0){

      posicao = 0;

      // Preeche vetor com números aleatórios
      preencheVetor(vet, TAM);

      // Separa um vetor em dois e retorna um pivô. 


      pivo02 = separa(vet, 0, TAM);

      printf("Pivo 02 %d\n", pivo02);

      pivo01 = separa(vet, 0, pivo02);

      printf("Pivo 01 %d\n", pivo01);
      
      pivo03 = separa(vet, pivo02 + 1, TAM);


     
     printf("Pivo 03 %d\n", pivo03);

      // Empacgota o vetor para ser enviados aos processos
      //MPI_Pack(vet, TAM, MPI_INT, buffer, TAM * 4, &posicao, MPI_COMM_WORLD);
     MPI_Send(vet, pivo01 , MPI_INT, 1, tag, MPI_COMM_WORLD);
     MPI_Send(&vet[pivo01 + 1], (pivo02 - pivo01), MPI_INT, 2, tag, MPI_COMM_WORLD);


     MPI_Send(&vet[pivo02] + 1, (pivo03 - pivo02) , MPI_INT, 3, tag, MPI_COMM_WORLD);
     MPI_Send(&vet[pivo03 + 1], (TAM - pivo03), MPI_INT, 4, tag, MPI_COMM_WORLD);

    // Envia o vetor para os outros processos
     //MPI_Bcast(buffer, TAM * 4, MPI_PACKED, 0, MPI_COMM_WORLD);

     // Envia o pivê para os outros processos
     MPI_Bcast(&pivo01, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     MPI_Bcast(&pivo02, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     MPI_Bcast(&pivo03, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     //MPI_Send(buffer, TAM * 4, MPI_PACKED, 1, tag, MPI_COMM_WORLD);
 
   }else{
      // 
      //MPI_Bcast(buffer, TAM * 4, MPI_PACKED, 0, MPI_COMM_WORLD);
     MPI_Bcast(&pivo01, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     MPI_Bcast(&pivo02, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     MPI_Bcast(&pivo03, 1, MPI_INT, 0, MPI_COMM_WORLD); 
        //MPI_Bcast(buffer, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
      posicao = 0;
    
      //printf("Pivo = %d\n", pivo );

      // Desempacota o vetor
      //MPI_Unpack(buffer, TAM * 4, &posicao, vet, TAM, MPI_INT, MPI_COMM_WORLD);

      // Se processo 1, ordena os valores do lado esquerdo do pivô
      if (rank == 1){

          vet1 = (int *)malloc(sizeof(int) * TAM);

          MPI_Recv(vet1, pivo01, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
          ordenacao(vet1, 0, pivo01);

             // printf("\nVetor 01 => ");
             //  for(i = 0; i <= pivo; i++){
             //    printf("%d - ", vet1[i]);
             //  }
             //  printf("\n");
          //
          //free(vet1);

          // Envia os valores ordenados para o processo 0
          MPI_Send(vet1, pivo01, MPI_INT, 0, tag, MPI_COMM_WORLD);

      }else if(rank == 2){// Se processo 2, ordena os valores do lado direto do pivô
          vet2 = (int *)malloc(sizeof(int) * (pivo02 - pivo01));

          MPI_Recv(vet2, (pivo02 - pivo01), MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

          //     printf("\nVetor 02 => ");
          //       for(i = 0; i < (TAM - pivo); i++){
          //         printf("%d - ", vet2[i]);
          //     }
          // printf("\n");
          
          ordenacao(vet2, 0, (pivo02 - pivo01));

          //free(vet2);

          // Envia os valores ordenados para o processo 0
          MPI_Send(vet2, (pivo02 - pivo01), MPI_INT, 0, tag, MPI_COMM_WORLD);

      }else if(rank == 3){// Se processo 2, ordena os valores do lado direto do pivô
          vet3 = (int *)malloc(sizeof(int) * (pivo03 - pivo02));

          MPI_Recv(vet3, (pivo03 - pivo02), MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

          //     printf("\nVetor 02 => ");
          //       for(i = 0; i < (TAM - pivo); i++){
          //         printf("%d - ", vet2[i]);
          //     }
          // printf("\n");
          
          ordenacao(vet3, 0, (pivo03 - pivo02));

          //free(vet2);

          // Envia os valores ordenados para o processo 0
          MPI_Send(vet3, (pivo03 - pivo02), MPI_INT, 0, tag, MPI_COMM_WORLD);

      }else if(rank == 4){// Se processo 2, ordena os valores do lado direto do pivô
          vet4 = (int *)malloc(sizeof(int) * (TAM - pivo03));

          MPI_Recv(vet4, (TAM - pivo03), MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

            
          
          ordenacao(vet4, 0, (TAM - pivo03));

          //free(vet2);

          // Envia os valores ordenados para o processo 0
          MPI_Send(vet4, (TAM - pivo03), MPI_INT, 0, tag, MPI_COMM_WORLD);
      }
      
   }
;

   if (rank == 0){

     //Alocando novos vetores que armazenarão os números ordenados
     vet1 = (int *)malloc(sizeof(int) * pivo01);
     vet2 = (int *)malloc(sizeof(int) * (pivo02 - pivo01));
     vet3 = (int *)malloc(sizeof(int) * pivo03 - pivo02);
     vet4 = (int *)malloc(sizeof(int) * TAM - pivo03);

    
     // memcpy(vet3 + pivo, vet2, (TAM - pivo) * sizeof(int));

      //MPI_Wait(&request, &status);
      MPI_Irecv(vet4, (TAM - pivo03), MPI_INT, 4, tag, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);
      // printf("Vetor 04 => ");
      //   for(i = 0; i < TAM - pivo03; i++){
      //     printf("%d - ", vet4[i]);
      //   }
      // printf("\n\n");
      //MPI_Wait(&request, &status);



      MPI_Irecv(vet1, pivo01, MPI_INT, 1, tag, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);
      //memcpy(vet3, vet1, pivo * sizeof(int));
   // printf("Vetor 01 => ");
   //      for(i = 0; i < pivo01; i++){
   //        printf("%d - ", vet1[i]);
   //      }
   //    printf("\n\n");




      MPI_Irecv(vet2, (pivo02 - pivo01), MPI_INT, 2, tag, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);

      //   printf("Vetor 02 => ");
      // for(i = 0; i < pivo02 - pivo01; i++){
      //   printf("%d - ", vet2[i]);
      // }
      // printf("\n\n");

      MPI_Irecv(vet3, (pivo03 - pivo02), MPI_INT, 3, tag, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);
      //   printf("Vetor 03 => ");
      // for(i = 0; i < pivo03 - pivo02; i++){
      //   printf("%d - ", vet3[i]);
      // }
      // printf("\n\n");

      

   

    

    

      



      

      //memcpy(&vet3[pivo], vet2, (TAM - pivo) * sizeof(int));

      // printf("Vetor 02 => ");
      // for(i = 1; i < (TAM - pivo); i++){
      //   printf("%d - ", vet2[i]);
      // }
      // printf("\n\n");

      // MPI_Recv(vet, TAM, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);

     
      // pivo = pivo + 2;
      // //printf("\nPivo 2 = %d\n\n", pivo );
      // printf("Vetor 02 => ");
      // for(i = pivo; i < TAM; i++){
      //   printf("%d - ", vet[i]);
      // }
      // printf("\n\n");
   }
   
   


   MPI_Finalize();
}
