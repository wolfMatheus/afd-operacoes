#include <stdlib.h>
#include <string.h>
#include "./filter.h"

void filterTransitions(DOTFormat *dotFormat)
{
  int tamanho = dotFormat->transicoes_numerica;
  int removed[tamanho];
  for (int i = 0; i < tamanho; i++)
  {
    removed[i] = 0;
  }

  int indexes[tamanho];
  int indexesSize = 0;
  int newSize = 0;
  DOTTransition *newTransitions[tamanho];
  for (int i = 0; i < tamanho; i++)
  {
    if (removed[i])
      continue;
    DOTTransition *outerTransition = dotFormat->transicoes[i];
    int labelSize = strlen(outerTransition->label + 1);
    for (int j = 0; j < tamanho; j++)
    {
      if (i == j || removed[j])
        continue;
      DOTTransition *innerTransition = dotFormat->transicoes[j];
      if (!strcmp(outerTransition->from, innerTransition->from) && !strcmp(outerTransition->to, innerTransition->to))
      {
        indexes[indexesSize] = j;
        indexesSize++;
        removed[j] = 1;
        labelSize += strlen(innerTransition->label + 2);
      }
    }

    if (indexesSize)
    {
      char *newLabel = malloc(sizeof(char) * labelSize);
      sprintf(newLabel, "%s, ", outerTransition->label);
      free(outerTransition->label);
      int tamanhoAtual = strlen(newLabel);
      for (int k = 0; k < indexesSize; k++)
      {
        DOTTransition *transition = dotFormat->transicoes[indexes[k]];
        for (int j = 0; j < strlen(transition->label); j++, tamanhoAtual++)
        {
          newLabel[tamanhoAtual] = transition->label[j];
        }
        if (k != indexesSize - 1)
        {
          newLabel[tamanhoAtual] = ',';
          newLabel[tamanhoAtual + 1] = ' ';
          tamanhoAtual += 2;
        }
        freeDotTransition(transition);
      }
      newLabel[tamanhoAtual] = '\0';
      outerTransition->label = newLabel;
      newTransitions[newSize] = outerTransition;
      newSize++;
    }
    else
    {
      newTransitions[newSize] = outerTransition;
      newSize++;
    }

    removed[i] = 1;
    indexesSize = 0;
  }
  DOTTransition **newTransitionsPointer = malloc(sizeof(DOTTransition *) * newSize);
  for (int i = 0; i < newSize; i++)
  {
    newTransitionsPointer[i] = newTransitions[i];
  }
  free(dotFormat->transicoes);
  dotFormat->transicoes = newTransitionsPointer;
  dotFormat->transicoes_numerica = newSize;
}