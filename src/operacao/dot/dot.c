#include <stdlib.h>
#include "../../strings/stringutil.h"
#include "./dot.h"

void writeDot(DOTFormat *dotFormat, char *fileName)
{
  FILE *file = fopen(fileName, "w");
  if (!file)
  {
    return;
  }
  fprintf(file, "digraph finite_state_machine {\nrankdir=LR;\nsize=\"8,5\"\n");
  if (dotFormat->numero_circulos_duplos)
  {
    fprintf(file, "node [shape = doublecircle]; ");
    for (int i = 0; i < dotFormat->numero_circulos_duplos; i++)
    {
      fprintf(file, "\"%s\" ", dotFormat->double_circles[i]);
    }
    fprintf(file, ";");
  }
  fprintf(file, "\nnode [shape = circle];\ninit [shape = point];\n");
  fprintf(file, "init -> \"%s\";\n", dotFormat->estado_inicial);
  for (int i = 0; i < dotFormat->transicoes_numerica; i++)
  {
    DOTTransition *element = dotFormat->transicoes[i];
    fprintf(file, "\"%s\" -> \"%s\" [label = \"%s\"];\n", element->from, element->to, element->label);
  }
  fprintf(file, "}");
}

DOTTransition *getDOTTransition(char *from, char *to, char *label)
{
  DOTTransition *transicao = malloc(sizeof(DOTTransition));
  transicao->from = copyString(from);
  transicao->to = copyString(to);
  transicao->label = copyString(label);
  return transicao;
}

DOTFormat *getDOTFormat(char **double_circles, int numero_circulos_duplos, DOTTransition **transicoes, int transicoes_numerica, char *estado_inicial)
{
  DOTFormat *dotFormat = malloc(sizeof(DOTFormat));
  dotFormat->double_circles = double_circles;
  dotFormat->numero_circulos_duplos = numero_circulos_duplos;
  dotFormat->transicoes = transicoes;
  dotFormat->transicoes_numerica = transicoes_numerica;
  dotFormat->estado_inicial = estado_inicial;
  return dotFormat;
}

void freeDotTransition(DOTTransition *transicao)
{
  free(transicao->from);
  free(transicao->to);
  free(transicao->label);
  free(transicao);
}

void freeDot(DOTFormat *dotFormat)
{
  for (int i = 0; i < dotFormat->transicoes_numerica; i++)
  {
    freeDotTransition(dotFormat->transicoes[i]);
  }
  for (int i = 0; i < dotFormat->numero_circulos_duplos; i++)
  {
    free(dotFormat->double_circles[i]);
  }
  free(dotFormat->double_circles);
  free(dotFormat->transicoes);
  free(dotFormat);
}
