#include "../product/product.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

AFD *intersecao(AFD *afd1, AFD *afd2)
{
  AFD *produto = afdProduct(afd1, afd2);
  int numeroEstadoFinal = (*afd1->numero_estado_final) * (*afd2->numero_estado_final);
  produto->estado_final = malloc(sizeof(int) * numeroEstadoFinal);
  *produto->numero_estado_final = numeroEstadoFinal;
  int indiceAtual = 0;
  int n2 = *afd2->numero_estado_final;
  for (int i = 0; i < *afd1->numero_estado_final; i++)
  {
    int indice1 = afd1->estado_final[i];
    for (int j = 0; j < *afd2->numero_estado_final; j++)
    {
      int indice2 = afd2->estado_final[j];
      int indiceProduto = (indice1 * n2) + indice2;
      produto->estado_final[indiceAtual] = indiceProduto;
      indiceAtual++;
    }
  }

  return produto;
}