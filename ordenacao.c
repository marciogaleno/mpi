#include <stdio.h>
#include <stdlib.h>


void mergeSort( int *vetorDesorndeado, int posicaoInicio, int posicaoFim ) 
{
   int i,j,k,metadeTamanho,*vetorTemp;
   if ( posicaoInicio == posicaoFim ) return;
   
   // ordenacao recursiva das duas metades
   metadeTamanho = ( posicaoInicio+posicaoFim )/2;
   mergeSort( vetorDesorndeado, posicaoInicio, metadeTamanho);
   mergeSort( vetorDesorndeado, metadeTamanho+1,posicaoFim );

   // intercalacao no vetor temporario t
   i = posicaoInicio;
   j = metadeTamanho+1;
   k = 0;
   vetorTemp = (int *) malloc(sizeof(int) * (posicaoFim-posicaoInicio+1));
   
   while( i < metadeTamanho+1 || j  < posicaoFim+1 )
   { 
      if ( i == metadeTamanho+1 )
      { // i passou do final da primeira metade, pegar v[j]
         vetorTemp[k] = vetorDesorndeado[j];
         j++;
         k++;
      } 
      else
      {
         if (j==posicaoFim+1) 
         { 
            // j passou do final da segunda metade, pegar v[i]
            vetorTemp[k] = vetorDesorndeado[i];
            i++;
            k++;
         } 
         else 
         {
            if (vetorDesorndeado[i] < vetorDesorndeado[j]) 
            { 
               vetorTemp[k] = vetorDesorndeado[i];
               i++;
               k++;
            } 
            else
            { 
              vetorTemp[k] = vetorDesorndeado[j];
              j++;
              k++;
            }
         }
      }
      
   }
   // copia vetor intercalado para o vetor original
   for( i = posicaoInicio; i <= posicaoFim; i++ )
   {
      vetorDesorndeado[i] = vetorTemp[i-posicaoInicio];
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

	pivo = separa(vetor, posicaoInicio, posicaoFim);
	
	mergeSort(vetor, posicaoInicio, pivo);
	quickSort(vetor, pivo + 1, posicaoFim);
}
