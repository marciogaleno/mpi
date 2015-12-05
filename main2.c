#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ordenacao.c"
#include "numerosAleatorios.c"

#define TAM 500

int main(int argc,char** argv)
{
   MPI_Status status;
   MPI_Request request, send_request;
   int     rank;
   int     posicao;
   char    buffer[TAM];
   //int  vet[] = {1, 2, 3 , 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, 18, 19, 10};
   int  vet[TAM];

   int *vet1, *vet2, *vet3, *vet4, *vet5;
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

      // ordenacao(vet, 0, TAM);

      // for(i = 0; i < TAM; i++){
      //   printf("%d - ",vet[i]);
      // }

      // printf("\n\n");

      // Separa um vetor em dois e retorna um pivô. 


      pivo02 = separa(vet, 0, TAM);

      printf("Pivo 02 %d\n", pivo02);

      pivo01 = separa(vet, 0, pivo02);

      printf("Pivo 01 %d\n", pivo01);
      
      pivo03 = separa(vet, pivo02 + 1, TAM);

      printf("Pivo 03 %d\n", pivo03);

      // for(i = 0; i < pivo01; i++){
      //    printf("%d - ",vet[i]);
      //  }
      //  printf("\n\n");

      // for(i = pivo01; i < pivo02; i++){
      //    printf("%d - ",vet[i]);
      //  }
      // printf("\n\n");

      // for(i = pivo02; i < pivo03; i++){
      //    printf("%d - ",vet[i]);
      //  }
      //  printf("\n\n");

      // for(i = pivo02; i < TAM; i++){
      //    printf("%d - ",vet[i]);
      //  }
      
      // printf("\n\n");
    

      // Empacgota o vetor para ser enviados aos processos
      //MPI_Pack(vet, TAM, MPI_INT, buffer, TAM * 4, &posicao, MPI_COMM_WORLD);
     MPI_Isend(vet, pivo01 , MPI_INT, 1, tag, MPI_COMM_WORLD, &send_request);
     MPI_Isend(&vet[pivo01], (pivo02 - pivo01), MPI_INT, 2, tag, MPI_COMM_WORLD, &send_request);


     MPI_Isend(&vet[pivo02], (pivo03 - pivo02) , MPI_INT, 3, tag, MPI_COMM_WORLD, &send_request);
     MPI_Isend(&vet[pivo03], (TAM - pivo03), MPI_INT, 4, tag, MPI_COMM_WORLD, &send_request);

    // Envia o vetor para os outros processos
     //MPI_Bcast(buffer, TAM * 4, MPI_PACKED, 0, MPI_COMM_WORLD);

     // Envia o pivê para os outros processos
     // MPI_Bcast(&pivo01, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     // MPI_Bcast(&pivo02, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     // MPI_Bcast(&pivo03, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     //MPI_Send(buffer, TAM * 4, MPI_PACKED, 1, tag, MPI_COMM_WORLD);
 
   }

     MPI_Bcast(&pivo01, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     MPI_Bcast(&pivo02, 1, MPI_INT, 0, MPI_COMM_WORLD); 
     MPI_Bcast(&pivo03, 1, MPI_INT, 0, MPI_COMM_WORLD); 
      // 
      //MPI_Bcast(buffer, TAM * 4, MPI_PACKED, 0, MPI_COMM_WORLD);
    
        //MPI_Bcast(buffer, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
      posicao = 0;
    
      //printf("Pivo = %d\n", pivo );

      // Desempacota o vetor
      //MPI_Unpack(buffer, TAM * 4, &posicao, vet, TAM, MPI_INT, MPI_COMM_WORLD);

      // Se processo 1, ordena os valores do lado esquerdo do pivô
      if (rank == 1){

          vet1 = (int *)malloc(sizeof(int) * TAM);

          MPI_Irecv(vet, pivo01, MPI_INT, 0, tag, MPI_COMM_WORLD, &request);
          MPI_Wait(&request, &status);

      
          ordenacao(vet, 0, pivo01);

          // printf("\nVetor 01 => ");
          // for(i = 0; i <= pivo01; i++){
          //   printf("%d - ", vet1[i]);
          // }
          // printf("\n");
          
          //
          //free(vet1);
          //printf("Enviando mensagem do processo %d para o processo %d\n", rank, 0);
          // Envia os valores ordenados para o processo 0
          MPI_Isend(vet, pivo01, MPI_INT, 0, tag, MPI_COMM_WORLD, &send_request);

      }

      if(rank == 2){// Se processo 2, ordena os valores do lado direto do pivô
          vet2 = (int *)malloc(sizeof(int) * (pivo02 - pivo01));

          MPI_Irecv(vet, (pivo02 - pivo01), MPI_INT, 0, tag, MPI_COMM_WORLD, &request);
          MPI_Wait(&request, &status);
          
          ordenacao(vet, 0, (pivo02 - pivo01));

          //     printf("\nVetor 02 => ");
          //       for(i = 0; i < (pivo02 - pivo01); i++){
          //         printf("%d - ", vet2[i]);
          //     }
          // printf("\n");

          //free(vet2);

          //printf("Enviando mensagem do processo %d para o processo %d\n", rank, 0);
          // Envia os valores ordenados para o processo 0
          MPI_Isend(vet, (pivo02 - pivo01), MPI_INT, 0, tag, MPI_COMM_WORLD, &send_request);

      }


      if(rank == 3){// Se processo 2, ordena os valores do lado direto do pivô
          vet3 = (int *)malloc(sizeof(int) * (pivo03 - pivo02));

          MPI_Irecv(vet, (pivo03 - pivo02), MPI_INT, 0, tag, MPI_COMM_WORLD, &request);
          MPI_Wait(&request, &status);


          
          ordenacao(vet, 0, (pivo03 - pivo02));

          //     printf("\nVetor 03 => ");
          //       for(i = 0; i < (pivo03 - pivo02); i++){
          //         printf("%d - ", vet3[i]);
          //     }
          // printf("\n");
          //free(vet2);

          //printf("Enviando mensagem do processo %d para o processo %d\n", rank, 0);
          // Envia os valores   ordenados para o processo 0
          MPI_Isend(vet, (pivo03 - pivo02), MPI_INT, 0, tag, MPI_COMM_WORLD, &send_request);

      }


      if(rank == 4){// Se processo 2, ordena os valores do lado direto do pivô
          vet4 = (int *)malloc(sizeof(int) * (TAM - pivo03));

          MPI_Irecv(vet, (TAM - pivo03), MPI_INT, 0, tag, MPI_COMM_WORLD, &request);
          MPI_Wait(&request, &status);
            
        

          ordenacao(vet, 0, (TAM - pivo03));

          //  printf("\nvetor 4 => ");
          //       for(i = 0; i < (TAM - pivo03); i++){
          //         printf("%d - ", vet4[i]);
          //     }
          // printf("\n");

         
          //free(vet2);
          //printf("Enviando mensagem do processo %d para o processo %d - tamanho bafu: %d\n", rank, 0, (TAM-pivo03));
          // Envia os valores ordenados para o processo 0
          MPI_Isend(vet, (TAM - pivo03), MPI_INT, 0, 4, MPI_COMM_WORLD, &send_request);
          // MPI_Wait(&send_request, &status);
      }
      
   


   if (rank == 0){

     //Alocando novos vetores que armazenarão os números ordenados
     vet1 = (int *)malloc(sizeof(int) * pivo01);
     vet2 = (int *)malloc(sizeof(int) * (pivo02 - pivo01));
     vet3 = (int *)malloc(sizeof(int) * pivo03 - pivo02);
     vet4 = (int *)malloc(sizeof(int) * TAM - pivo03);
     vet5 = (int *)malloc(sizeof(int) * TAM);


    
     // memcpy(vet3 + pivo, vet2, (TAM - pivo) * sizeof(int));



      MPI_Irecv(vet1, pivo01, MPI_INT, 1, tag, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);

   // printf("Vetor 01 => ");
   //      for(i = 0; i < pivo01; i++){
   //        printf("%d - ", vet1[i]);
   //      }
   //    printf("\n\n");
      
      memcpy(vet5, vet1, pivo01 * sizeof(int));





      MPI_Irecv(vet2, (pivo02 - pivo01), MPI_INT, 2, tag, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);


      // printf("Vetor 02 => ");
      // for(i = 0; i < pivo02 - pivo01; i++){
      //   printf("%d - ", vet2[i]);
      // }
      // printf("\n\n");

      memcpy(vet5 + pivo01, vet2, (pivo02 - pivo01) * sizeof(int));


      MPI_Irecv(vet3, (pivo03 - pivo02), MPI_INT, 3, tag, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);

      //   printf("Vetor 03 => ");
      // for(i = 0; i < pivo03 - pivo02; i++){
      //   printf("%d - ", vet3[i]);
      // }
      // printf("\n\n");

      memcpy(vet5 + pivo02, vet3, (pivo03 - pivo02) * sizeof(int));

      

      //printf("Aqui\n");

      MPI_Irecv(vet4, (TAM - pivo03), MPI_INT, 4, 4, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);

      //      printf("Vetor 04 => ");
      //   for(i = 0; i < (TAM - pivo03); i++){
      //     printf("%d - ", vet4[i]);
      //   }
      // printf("\n\n");

      memcpy(vet5 + pivo03, vet4, (TAM - pivo03) * sizeof(int));


      // MPI_Irecv(vet4, (TAM - pivo03), MPI_INT, 4, tag, MPI_COMM_WORLD, &request);
      // MPI_Wait(&send_request, &status);
      
 
      //MPI_Wait(&request, &status);

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
