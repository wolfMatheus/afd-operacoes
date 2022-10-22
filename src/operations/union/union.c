#include "../product/product.h"
#include <stdlib.h>
#include <stdio.h>

int isAfdFinalState(AFD *afd, int indice)
{
  for (int i = 0; i < *afd->numero_estado_final; i++)
  {
    if (indice == afd->estado_final[i])
      return 1;
  }
  return 0;
}

AFD *uniao(AFD *afd1, AFD *afd2)
{
  AFD *produto = afdProduct(afd1, afd2);
  int numeroEstadoFinal = ((*afd1->numero_estado_final) * (*afd2->estadoNumerico)) +
                          ((*afd2->numero_estado_final) * (*afd1->estadoNumerico)) - (*afd1->numero_estado_final * (*afd2->numero_estado_final));
  produto->estado_final = malloc(sizeof(int) * numeroEstadoFinal);
  *produto->numero_estado_final = numeroEstadoFinal;
  int indiceAtual = 0;
  for (int i = 0; i < *afd1->numero_estado_final; i++)
  {
    int estadoFinal = afd1->estado_final[i];
    int startingPoint = estadoFinal * (*afd2->estadoNumerico);
    for (int j = 0; j < *afd2->estadoNumerico; j++)
    {
      int indice = startingPoint + j;
      produto->estado_final[indiceAtual] = indice;
      indiceAtual++;
    }
  }

  for (int i = 0; i < *afd2->numero_estado_final; i++)
  {
    int estadoFinal = afd2->estado_final[i];
    for (int j = 0; j < *afd1->estadoNumerico; j++)
    {
      int indice = (j * (*afd2->estadoNumerico)) + estadoFinal;
      if (isAfdFinalState(afd1, j))
      {
        continue;
      }
      produto->estado_final[indiceAtual] = indice;
      indiceAtual++;
    }
  }

  return produto;
}