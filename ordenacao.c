#include <stdio.h>
#include <stdlib.h>


void mergeSort(int *vetor, int posicaoInicio, int posicaoFim) {
    int i, j, k, metadeTamanho, *vetorTemp;

    if(posicaoInicio == posicaoFim) return;

    // ordenacao recursiva das duas metades
    metadeTamanho = (posicaoInicio + posicaoFim ) / 2;
    mergeSort(vetor, posicaoInicio, metadeTamanho);
    mergeSort(vetor, metadeTamanho + 1, posicaoFim);

    // intercalacao no vetor temporario t
    i = posicaoInicio;
    j = metadeTamanho + 1;
    k = 0;
    vetorTemp = (int *) malloc(sizeof(int) * (posicaoFim - posicaoInicio + 1));

    while(i < metadeTamanho + 1 || j  < posicaoFim + 1) {
        if (i == metadeTamanho + 1 ) { // i passou do final da primeira metade, pegar v[j]
            vetorTemp[k] = vetor[j];
            j++;
            k++;
        } 
        else {
            if (j == posicaoFim + 1) { // j passou do final da segunda metade, pegar v[i]
                vetorTemp[k] = vetor[i];
                i++;
                k++;
            } 
            else {
                if (vetor[i] < vetor[j]) { 
                    vetorTemp[k] = vetor[i];
                    i++;
                    k++;
                } 
                else { 
                    vetorTemp[k] = vetor[j];
                    j++;
                    k++;
                }
            }
        }

    }
    // copia vetor intercalado para o vetor original
    for(i = posicaoInicio; i <= posicaoFim; i++) {
        vetor[i] = vetorTemp[i - posicaoInicio];
    }
    free(vetorTemp);
}

int separa (int *v, int p, int r)
{
   int c = v[p], i = p+1, j = r, t;
   while (/*A*/ i <= j) {
      if (v[i] <= c) ++i;
      else if (c < v[j]) --j; 
      else {
         t = v[i], v[i] = v[j], v[j] = t;
         ++i; --j;
      }
   }
   // agora i == j+1                 
   v[p] = v[j], v[j] = c;
   return j; 
}

void quickSort(int v[], int p, int r)
{
   int j;
   while (p < r) {      
      j = separa (v, p, r); 

      if (j - p < r - j) {     
         quickSort (v, p, j-1);
         p = j + 1;            
      } else {                 
         quickSort (v, j+1, r);
         r = j - 1;
      }
   }
}

void ordenacao(int *vetor, int posicaoInicio, int posicaoFim){
	int pivo;
   // int i, j;
   // int aux;
	// pivo = separa(vetor, posicaoInicio, posicaoFim);
  
	// mergeSort(vetor, posicaoInicio, posicaoFim);
   quickSort(vetor, posicaoInicio, posicaoFim);


   

  // for(i = posicaoFim; i >= posicaoInicio; i--) {  
        
  //   for(j=0; j < i ; j++) {
          
  //     if(vetor[j]>vetor[j+1]) {
                
  //       aux = vetor[j];
  //       vetor[j] = vetor[j+1];
  //       vetor[j+1] = aux;
        
  //       }
  //     }
  //   }

	
}
