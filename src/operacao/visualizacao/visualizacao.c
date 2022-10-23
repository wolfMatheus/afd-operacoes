#include <stdlib.h>
#include <string.h>
#include "../operacoes.h"
#include "../../strings/stringutil.h"

char **getDoubleCircles(AFD *afd)
{
  char **double_circles = malloc(sizeof(char *) * (*afd->numero_estado_final));
  for (int i = 0; i < *afd->numero_estado_final; i++)
  {
    double_circles[i] = copyString(afd->estados[afd->estado_final[i]]);
  }
  return double_circles;
}

DOTTransition *getSingleTransition(AFD *afd, Transition *transicao)
{
  char *from = afd->estados[*transicao->from];
  char *to = afd->estados[*transicao->to];
  char *read = afd->alphabet[*transicao->read];

  DOTTransition *dotTransition = getDOTTransition(from, to, read);
  return dotTransition;
}

DOTTransition **getTransitionArray(AFD *afd)
{
  int tamanho = *afd->transicoes_numerica;
  DOTTransition **transicoes = malloc(sizeof(DOTTransition *) * tamanho);
  for (int i = 0; i < tamanho; i++)
  {
    DOTTransition *transicao = getSingleTransition(afd, afd->transicoes[i]);
    transicoes[i] = transicao;
  }
  return transicoes;
}

DOTFormat *visualizacao(AFD *afd)
{
  char **double_circles = getDoubleCircles(afd);
  DOTTransition **transicoes = getTransitionArray(afd);
  char *estado_inicial = copyString(afd->estados[*afd->estado_inicial]);
  DOTFormat *dotFormat = getDOTFormat(double_circles, *afd->numero_estado_final, transicoes, *afd->transicoes_numerica, estado_inicial);
  filterTransitions(dotFormat);
  return dotFormat;
}
