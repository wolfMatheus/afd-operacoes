#include "../operations.h"
#include <stdlib.h>
#include "../../strings/stringutil.h"

AFD *complemento(AFD *afd)
{
  AFD *novoAfd = getEmptyAFD();
  novoAfd->estado = malloc((*afd->estadoNumerico) * sizeof(char *));
  for (int i = 0; i < *afd->estadoNumerico; i++)
  {
    char *estado = afd->estado[i];
    char *copiaDoEstado = copyString(estado);
    novoAfd->estado[i] = copiaDoEstado;
  }
  *novoAfd->estadoNumerico = *afd->estadoNumerico;

  novoAfd->alphabet = malloc((*afd->numero_simbolos) * sizeof(char *));
  for (int i = 0; i < *afd->numero_simbolos; i++)
  {
    char *simbolo = afd->alphabet[i];
    char *copiaSimbolo = copyString(simbolo);
    novoAfd->alphabet[i] = copiaSimbolo;
  }
  *novoAfd->numero_simbolos = *afd->numero_simbolos;

  novoAfd->transicoes = malloc((*afd->transicoes_numerica) * sizeof(transicao *));
  for (int i = 0; i < *afd->transicoes_numerica; i++)
  {
    Transition *transicao = afd->transicoes[i];
    Transition *novaTransicao = getEmptyTransition();
    *novaTransicao->from = *transicao->from;
    *novaTransicao->to = *transicao->to;
    *novaTransicao->read = *transicao->read;
    novoAfd->transicoes[i] = novaTransicao;
  }
  *novoAfd->transicoes_numerica = *afd->transicoes_numerica;
  *novoAfd->estado_inicial = *afd->estado_inicial;

  int numeroEstadoFinal = (*afd->estadoNumerico) - (*afd->numero_estado_final);
  novoAfd->estado_final = malloc(sizeof(int) * numeroEstadoFinal);
  *novoAfd->numero_estado_final = numeroEstadoFinal;
  int estadoFinalAtual = 0;
  for (int i = 0; i < *afd->estadoNumerico; i++)
  {
    int isFinal = 0;
    for (int j = 0; j < *afd->numero_estado_final; j++)
    {
      int estadoFinal = afd->estado_final[j];
      if (i == estadoFinal)
      {
        isFinal = 1;
        break;
      }
    }
    if (!isFinal)
    {
      novoAfd->estado_final[estadoFinalAtual] = i;
      estadoFinalAtual++;
    }
  }

  return novoAfd;
}