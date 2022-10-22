#include "../../strings/stringutil.h"
#include "./recognition.h"
#include <string.h>
#include <stdlib.h>

char *transitionFunction(AFD *afd, char *simbolo, char *estadoAtual)
{
  for (int i = 0; i < *afd->numero_transicoes; i++)
  {
    Transition *transicao = afd->transicoes[i];
    char *fromState = afd->estados[*transicao->from];
    char *read = afd->alphabet[*transicao->read];
    if (!strcmp(estadoAtual, fromState) && !strcmp(simbolo, read))
    {
      return afd->estados[*transicao->to];
    }
  }
}

char *extendedTransitionFunction(AFD *afd, char *word, char *estadoAtual)
{
  int tamanho = strlen(word);
  if (tamanho == 0)
  {
    return estadoAtual;
  }
  else
  {
    char simbolo[2] = {word[tamanho - 1], '\0'};
    char *copy = copyString(word);
    copy[tamanho - 1] = '\0';
    char *estado= transitionFunction(afd, simbolo, extendedTransitionFunction(afd, copy, estadoAtual));
    free(copy);
    return estado;
  }
}

int isFinalState(AFD *afd, char *estado)
{
  for (int i = 0; i < *afd->numeroEstadoFinal; i++)
  {
    char *estadoFinal = afd->estados[afd->final_states[i]];
    if (!strcmp(estadoFinal, estado))
      return 1;
  }
  return 0;
}

int *reconhecer(AFD *afd, char **words, int tamanho)
{
  int *resultados = calloc(tamanho, sizeof(int));
  for (int i = 0; i < tamanho; i++)
  {
    char *estado_inicial = afd->estados[*afd->estado_inicial];
    char *wordCopy = copyString(words[i]);
    char *estado= extendedTransitionFunction(afd, wordCopy, estado_inicial);
    resultados[i] = isFinalState(afd, estado);
    free(wordCopy);
  }
  return resultados;
}