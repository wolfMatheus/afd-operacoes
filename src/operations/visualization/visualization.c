#include <stdlib.h>
#include <string.h>
#include "../operations.h"
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

DOTTransition *getSingleTransition(AFD *afd, Transition *transition)
{
  char *from = afd->estados[*transition->from];
  char *to = afd->estados[*transition->to];
  char *read = afd->alphabet[*transition->read];

  DOTTransition *dotTransition = getDOTTransition(from, to, read);
  return dotTransition;
}

DOTTransition **getTransitionArray(AFD *afd)
{
  int tamanho = *afd->number_transitions;
  DOTTransition **transicoes = malloc(sizeof(DOTTransition *) * tamanho);
  for (int i = 0; i < tamanho; i++)
  {
    DOTTransition *transition = getSingleTransition(afd, afd->transicoes[i]);
    transicoes[i] = transition;
  }
  return transicoes;
}

DOTFormat *visualizacao(AFD *afd)
{
  char **double_circles = getDoubleCircles(afd);
  DOTTransition **transicoes = getTransitionArray(afd);
  char *estado_inicial = copyString(afd->estados[*afd->estado_inicial]);
  DOTFormat *dotFormat = getDOTFormat(double_circles, *afd->numero_estado_final, transicoes, *afd->number_transitions, estado_inicial);
  filterTransitions(dotFormat);
  return dotFormat;
}
