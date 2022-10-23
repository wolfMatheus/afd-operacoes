#include <stdlib.h>
#include <string.h>
#include "./afd.h"

AFD *getEmptyAFD()
{
  AFD *afd = malloc(sizeof(AFD));
  afd->estados_numericos = malloc(sizeof(int));
  afd->numero_simbolos = malloc(sizeof(int));
  afd->transicoes_numerica = malloc(sizeof(int));
  afd->numero_estado_final = malloc(sizeof(int));
  afd->estado_inicial = malloc(sizeof(int));
}

Transition *getTransition(AFD afd, char *from, char *to, char *read)
{
  int fromPosition = getStatePosition(afd, from);
  int toPosition = getStatePosition(afd, to);
  int readPosition = getSymbolPosition(afd, read);
  Transition *transicao = malloc(sizeof(Transition));
  transicao->from = malloc(sizeof(int));
  transicao->to = malloc(sizeof(int));
  transicao->read = malloc(sizeof(int));

  *transicao->from = fromPosition;
  *transicao->to = toPosition;
  *transicao->read = readPosition;
  return transicao;
}

int getStatePosition(AFD afd, char *estado)
{
  for (int i = 0; i < *afd.estados_numericos; i++)
  {
    if (!strcmp(afd.estados[i], estado))
      return i;
  }
  return -1;
}

int getSymbolPosition(AFD afd, char *symbol)
{
  for (int i = 0; i < *afd.numero_simbolos; i++)
  {
    if (!strcmp(afd.alphabet[i], symbol))
      return i;
  }
  return -1;
}

void readInt(int *int_ptr, FILE *file)
{
  int value;
  fscanf(file, "%d\n", &value);
  *int_ptr = value;
}

void readString(char **str_ptr, FILE *file)
{
  char buffer[100];
  fscanf(file, "%s\n", buffer);
  *str_ptr = malloc(sizeof(char) * (strlen(buffer) + 1));
  strcpy(*str_ptr, buffer);
}

void readChars(char ***list_ptr, int *size_ptr, FILE *file)
{
  readInt(size_ptr, file);
  *list_ptr = malloc(sizeof(char *) * (*size_ptr));
  for (int i = 0; i < (*size_ptr); i++)
  {
    readString(&(*list_ptr)[i], file);
  }
}

void readTransitions(AFD *afd, FILE *file)
{
  readInt(afd->transicoes_numerica, file);
  afd->transicoes = malloc(sizeof(Transition *) * (*afd->transicoes_numerica));
  for (int i = 0; i < *afd->transicoes_numerica; i++)
  {
    char buffer1[100], buffer2[100], buffer3[100];
    fscanf(file, "%s %s %s\n", buffer1, buffer2, buffer3);

    int fromPosition = getStatePosition(*afd, buffer1);
    int readPosition = getSymbolPosition(*afd, buffer2);
    int toPosition = getStatePosition(*afd, buffer3);

    Transition *transicao = malloc(sizeof(Transition));
    transicao->from = malloc(sizeof(int));
    transicao->to = malloc(sizeof(int));
    transicao->read = malloc(sizeof(int));
    *transicao->from = fromPosition;
    *transicao->read = readPosition;
    *transicao->to = toPosition;
    afd->transicoes[i] = transicao;
  }
}

void readInitialState(AFD *afd, FILE *file)
{
  char buffer[100];
  fscanf(file, "%s\n", buffer);
  int statePosition = getStatePosition(*afd, buffer);
  *afd->estado_inicial = statePosition;
}

void readFinalStates(AFD *afd, FILE *file)
{
  readInt(afd->numero_estado_final, file);
  afd->estado_final = malloc(sizeof(int *) * (*afd->numero_estado_final));
  for (int i = 0; i < *afd->numero_estado_final; i++)
  {
    char buffer[100];
    fscanf(file, "%s\n", buffer);
    int statePosition = getStatePosition(*afd, buffer);
    afd->estado_final[i] = statePosition;
  }
}

Transition *getEmptyTransition()
{
  Transition *transicao = malloc(sizeof(Transition));
  transicao->from = malloc(sizeof(int));
  transicao->to = malloc(sizeof(int));
  transicao->read = malloc(sizeof(int));
  return transicao;
}

AFD *readAFD(char *fileName)
{
  FILE *file = fopen(fileName, "r");
  if (!file)
  {
    return NULL;
  }
  AFD *afd = getEmptyAFD();
  readChars(&afd->estados, afd->estados_numericos, file);
  readChars(&afd->alphabet, afd->numero_simbolos, file);
  readTransitions(afd, file);
  readInitialState(afd, file);
  readFinalStates(afd, file);

  fclose(file);
  return afd;
}

void writeInt(int value, FILE *file)
{
  fprintf(file, "%d\n", value);
}

void writeChar(char *value, FILE *file)
{
  fprintf(file, "%s\n", value);
}

void writeChars(char **list, int tamanho, FILE *file)
{
  writeInt(tamanho, file);
  for (int i = 0; i < tamanho; i++)
  {
    writeChar(list[i], file);
  }
}

void writeTransitions(AFD afd, FILE *file)
{
  int tamanho = *afd.transicoes_numerica;
  writeInt(tamanho, file);
  for (int i = 0; i < tamanho; i++)
  {
    Transition *element = afd.transicoes[i];
    char *from = afd.estados[*element->from];
    char *read = afd.alphabet[*element->read];
    char *to = afd.estados[*element->to];
    fprintf(file, "%s %s %s\n", from, read, to);
  }
}

void writeFinalStates(AFD afd, FILE *file)
{
  int tamanho = *afd.numero_estado_final;
  char *estadosFinais[tamanho];
  for (int i = 0; i < tamanho; i++)
  {
    estadosFinais[i] = afd.estados[afd.estado_final[i]];
  }
  writeChars(estadosFinais, tamanho, file);
}

void writeAFD(AFD afd, char *fileName)
{
  FILE *file = fopen(fileName, "w");
  if (!file)
  {
    return;
  }
  writeChars(afd.estados, *afd.estados_numericos, file);
  writeChars(afd.alphabet, *afd.numero_simbolos, file);
  writeTransitions(afd, file);
  writeChar(afd.estados[*afd.estado_inicial], file);
  writeFinalStates(afd, file);
}

void freeTransition(Transition *transicao)
{
  free(transicao->from);
  free(transicao->to);
  free(transicao->read);
}

void freeAFD(AFD *afd)
{
  for (int i = 0; i < *afd->estados_numericos; i++)
  {
    free(afd->estados[i]);
  }
  for (int i = 0; i < *afd->numero_simbolos; i++)
  {
    free(afd->alphabet[i]);
  }
  for (int i = 0; i < *afd->transicoes_numerica; i++)
  {
    freeTransition(afd->transicoes[i]);
  }

  free(afd->transicoes);
  free(afd->alphabet);
  free(afd->estados);
  free(afd->estado_final);
  free(afd->estados_numericos);
  free(afd->numero_simbolos);
  free(afd->numero_estado_final);
  free(afd->transicoes_numerica);
  free(afd->estado_inicial);
}