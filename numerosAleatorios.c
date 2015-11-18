#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//FUNÇÃO PARA VERIFICAR SE A ALGUM NUMERO REPETIDO
int existe(int *vet,  int n, int tam){
	int i; 
	for(i = 0; i<= tam; i++){
		if(vet[i] == n){
			return 1;	
		}

	}
	return 0;

}

//FUNÇÃO PARA GERAR O VALOR ALEATORIO

int proximoValor(int *vet, int tam){
	int r;
	srand((unsigned int)time((time_t*)NULL));
	do{
		r = rand() % (tam + 1);
	}while(existe(vet,r, tam));
	return r;
}

int preencheVetor(int *vet, int tam){
	int i; 
	
	for(i = 0; i <= tam; i++){
		vet[i] = proximoValor(vet, tam);
		//fprintf(vetor, "%d \n", vet[i]);
		//putc("\n", vetor);	
		//printf("%d - ", vet[i]);

	}
	//printf("\n\n");
	
		
}
