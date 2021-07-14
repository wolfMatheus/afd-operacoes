#include <stdlib.h>
#include <string.h>
#include "./afd.h"

AFD *getEmptyAFD()
{
  AFD *afd = malloc(sizeof(AFD));
  afd->number_states = malloc(sizeof(int));
  afd->number_symbols = malloc(sizeof(int));
  afd->number_transitions = malloc(sizeof(int));
  afd->number_final_states = malloc(sizeof(int));
  afd->initial_state = malloc(sizeof(int));
}

Transition *getTransition(AFD afd, char *from, char *to, char *read)
{
  int fromPosition = getStatePosition(afd, from);
  int toPosition = getStatePosition(afd, to);
  int readPosition = getSymbolPosition(afd, read);
  Transition *transition = malloc(sizeof(Transition));
  transition->from = malloc(sizeof(int));
  transition->to = malloc(sizeof(int));
  transition->read = malloc(sizeof(int));

  *transition->from = fromPosition;
  *transition->to = toPosition;
  *transition->read = readPosition;
  return transition;
}

int getStatePosition(AFD afd, char *state)
{
  for (int i = 0; i < *afd.number_states; i++)
  {
    if (!strcmp(afd.states[i], state))
      return i;
  }
  return -1;
}

int getSymbolPosition(AFD afd, char *symbol)
{
  for (int i = 0; i < *afd.number_symbols; i++)
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
  readInt(afd->number_transitions, file);
  afd->transitions = malloc(sizeof(Transition *) * (*afd->number_transitions));
  for (int i = 0; i < *afd->number_transitions; i++)
  {
    char buffer1[100], buffer2[100], buffer3[100];
    fscanf(file, "%s %s %s\n", buffer1, buffer2, buffer3);

    int fromPosition = getStatePosition(*afd, buffer1);
    int readPosition = getSymbolPosition(*afd, buffer2);
    int toPosition = getStatePosition(*afd, buffer3);

    Transition *transition = malloc(sizeof(Transition));
    transition->from = malloc(sizeof(int));
    transition->to = malloc(sizeof(int));
    transition->read = malloc(sizeof(int));
    *transition->from = fromPosition;
    *transition->read = readPosition;
    *transition->to = toPosition;
    afd->transitions[i] = transition;
  }
}

void readInitialState(AFD *afd, FILE *file)
{
  char buffer[100];
  fscanf(file, "%s\n", buffer);
  int statePosition = getStatePosition(*afd, buffer);
  *afd->initial_state = statePosition;
}

void readFinalStates(AFD *afd, FILE *file)
{
  readInt(afd->number_final_states, file);
  afd->final_states = malloc(sizeof(int *) * (*afd->number_final_states));
  for (int i = 0; i < *afd->number_final_states; i++)
  {
    char buffer[100];
    fscanf(file, "%s\n", buffer);
    int statePosition = getStatePosition(*afd, buffer);
    afd->final_states[i] = statePosition;
  }
}

AFD *readAFD(char *fileName)
{
  FILE *file = fopen(fileName, "r");
  if (!file)
  {
    return NULL;
  }
  AFD *afd = getEmptyAFD();
  readChars(&afd->states, afd->number_states, file);
  printf("%d\n", *afd->number_states);
  readChars(&afd->alphabet, afd->number_symbols, file);
  printf("%d\n", *afd->number_states);
  readTransitions(afd, file);
  printf("%d\n", *afd->number_states);
  readInitialState(afd, file);
  printf("%d\n", *afd->number_states);
  readFinalStates(afd, file);

  printf("%d\n", *afd->number_states);
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

void writeChars(char **list, int size, FILE *file)
{
  writeInt(size, file);
  puts("escreveu");
  for (int i = 0; i < size; i++)
  {
    puts("passou 1");
    writeChar(list[i], file);
  }
}

void writeTransitions(AFD afd, FILE *file)
{
  int size = *afd.number_transitions;
  printf("%d\n", size);
  writeInt(size, file);
  puts("wrote size");
  for (int i = 0; i < size; i++)
  {
    Transition *element = afd.transitions[i];
    puts("GOT HERE");
    printf("%d\n", *element->from);
    char *from = afd.states[*element->from];
    puts(from);
    char *read = afd.alphabet[*element->read];
    puts(read);
    char *to = afd.states[*element->to];
    puts(to);
    printf("transitions %d\n", i);
    fprintf(file, "%s %s %s\n", from, read, to);
  }
}

void writeFinalStates(AFD afd, FILE *file)
{
  int size = *afd.number_final_states;
  char *finalStates[size];
  for (int i = 0; i < size; i++)
  {
    finalStates[i] = afd.states[afd.final_states[i]];
  }
  writeChars(finalStates, size, file);
}

void writeAFD(AFD afd, char *fileName)
{
  FILE *file = fopen(fileName, "w");
  if (!file)
  {
    return;
  }
  printf("%d\n", *afd.number_states);
  writeChars(afd.states, *afd.number_states, file);
  puts("escreveu estados");
  writeChars(afd.alphabet, *afd.number_symbols, file);
  puts("escreveu alfabeto");
  writeTransitions(afd, file);
  puts("escreveu transitions");
  writeChar(afd.states[*afd.initial_state], file);
  puts("escreveu estados iniciais");
  writeFinalStates(afd, file);
  puts("escreveu estados finais");
}