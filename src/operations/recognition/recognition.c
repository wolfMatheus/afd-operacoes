#include <stdlib.h>
#include "./recognition.h"
#include "../../strings/stringutil.h"

char **readWords(char *fileName, int *size_ptr)
{
  FILE *file = fopen(fileName, "r");
  if (!file)
    return NULL;
  int tamanho = 0;
  char **words = NULL;
  char buffer[100];
  while (fscanf(file, "%s\n", buffer) != EOF)
  {
    char *word = copyString(buffer);
    tamanho++;
    if (words)
    {
      words = realloc(words, sizeof(char *) * tamanho);
    }
    else
    {
      words = malloc(sizeof(char *));
    }
    words[tamanho - 1] = word;
  }
  *size_ptr = tamanho;
  fclose(file);
  return words;
}

void freeWords(char **words, int tamanho)
{
  for (int i = 0; i < tamanho; i++)
  {
    free(words[i]);
  }
  free(words);
}

void writeReconhecerResult(int *resultado, char *fileName, int tamanho)
{

  FILE *file = fopen(fileName, "w");
  if (!file)
    return;
  for (int i = 0; i < tamanho; i++)
  {
    fprintf(file, "%d\n", resultado[i]);
  }
  fclose(file);
}