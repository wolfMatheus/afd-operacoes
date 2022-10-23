#include "../reconhecimento/reconhecimento.h"
#include "../../strings/stringutil.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int isContained(int *values, int value, int tamanho)
{
  for (int i = 0; i < tamanho; i++)
  {
    if (value == values[i])
    {
      return 1;
    }
  }
  return 0;
}

int getEquivalenceGroup(int **grupos, int tamanhoGrupos, int *tamanhos, int value)
{
  for (int i = 0; i < tamanhoGrupos; i++)
  {
    for (int j = 0; j < tamanhos[i]; j++)
    {
      if (grupos[i][j] == value)
      {
        return i;
      }
    }
  }
  return -1;
}

char *mergeStates(AFD *afd, int *estados, int tamanho)
{
  int separadoresDeNumeros = tamanho - 1;
  int tamanhoAtual = separadoresDeNumeros + 1;
  char *novoEstado = malloc(sizeof(char) * tamanhoAtual);
  int indiceAtual = 0;
  for (int i = 0; i < tamanho; i++)
  {
    int indiceEstado = estados[i];
    char *estado= afd->estados[indiceEstado];
    int comprimentoDoEstado = strlen(estado);
    tamanhoAtual += comprimentoDoEstado;
    novoEstado = realloc(novoEstado, sizeof(char) * tamanhoAtual);
    for (int j = 0; j < comprimentoDoEstado; j++)
    {
      novoEstado[indiceAtual] = estado[j];
      indiceAtual++;
    }
    if (i != tamanho - 1)
    {
      novoEstado[indiceAtual] = '_';
      indiceAtual++;
    }
  }
  novoEstado[indiceAtual] = '\0';
  return novoEstado;
}

Transition *getTransitionFromIndexes(AFD *afd, int from, int read)
{
  for (int i = 0; i < *afd->transicoes_numerica; i++)
  {
    if (*afd->transicoes[i]->from == from && *afd->transicoes[i]->read == read)
    {
      return afd->transicoes[i];
    }
  }
  return NULL;
}

void updateReachableList(AFD *afd, int **alcancaveis, int *tamanhoAlcancavel, int elemento)
{
  for (int i = 0; i < *afd->numero_simbolos; i++)
  {
    Transition *transicao = getTransitionFromIndexes(afd, elemento, i);
    if (!isContained(*alcancaveis, *transicao->to, *tamanhoAlcancavel))
    {
      (*tamanhoAlcancavel)++;
      *alcancaveis = realloc(*alcancaveis, *tamanhoAlcancavel);
      (*alcancaveis)[(*tamanhoAlcancavel) - 1] = *transicao->to;
      updateReachableList(afd, alcancaveis, tamanhoAlcancavel, *transicao->to);
    }
  }
}

AFD *copyAFDWithoutUnreachableStates(AFD *afd)
{
  AFD *novoAfd = getEmptyAFD();
  int *alcancaveis = malloc(sizeof(int));
  int tamanhoAlcancavel = 1;
  alcancaveis[0] = *afd->estado_inicial;
  updateReachableList(afd, &alcancaveis, &tamanhoAlcancavel, *afd->estado_inicial);

  *novoAfd->estados_numericos = tamanhoAlcancavel;
  novoAfd->estados= malloc(sizeof(char *) * tamanhoAlcancavel);
  novoAfd->estado_final = malloc(sizeof(int));
  int tamanhoFinalDeAlocacaoEstado = 1;
  int estadosFinais = 0;
  for (int i = 0; i < tamanhoAlcancavel; i++)
  {
    char *estado= afd->estados[alcancaveis[i]];
    novoAfd->estados[i] = copyString(estado);
    if (isContained(afd->estado_final, alcancaveis[i], *afd->numero_estado_final))
    {
      if (tamanhoFinalDeAlocacaoEstado == estadosFinais)
      {
        tamanhoFinalDeAlocacaoEstado++;
        novoAfd->estado_final = realloc(novoAfd->estado_final, sizeof(int) * tamanhoFinalDeAlocacaoEstado);
      }
      novoAfd->estado_final[estadosFinais] = i;
      estadosFinais++;
    }
  }
  *novoAfd->numero_estado_final = estadosFinais;

  int numeroTransicoes = 0;
  int alocarTamanho = 1;
  novoAfd->transicoes = malloc(sizeof(Transition *));
  for (int i = 0; i < tamanhoAlcancavel; i++)
  {
    int elemento= alcancaveis[i];
    for (int j = 0; j < *afd->transicoes_numerica; j++)
    {
      Transition *transicao = afd->transicoes[j];
      if (*transicao->to == elemento)
      {
        Transition *novaTransicao = getEmptyTransition();
        char *estado= afd->estados[*transicao->from];
        int posicao = getStatePosition(*novoAfd, estado);
        if (posicao == -1)
        {
          continue;
        }
        *novaTransicao->from = posicao;
        *novaTransicao->read = *transicao->read;
        *novaTransicao->to = i;
        if (alocarTamanho == numeroTransicoes)
        {
          alocarTamanho++;
          novoAfd->transicoes = realloc(novoAfd->transicoes, sizeof(Transition *) * alocarTamanho);
        }
        novoAfd->transicoes[numeroTransicoes] = novaTransicao;
        numeroTransicoes++;
      }
    }
  }
  *novoAfd->transicoes_numerica = numeroTransicoes;

  *novoAfd->numero_simbolos = *afd->numero_simbolos;
  novoAfd->alphabet = malloc(sizeof(char *) * (*novoAfd->numero_simbolos));
  for (int i = 0; i < *novoAfd->numero_simbolos; i++)
  {
    novoAfd->alphabet[i] = copyString(afd->alphabet[i]);
  }
  *novoAfd->estado_inicial = *afd->estado_inicial;

  return novoAfd;
}

AFD *minimizacao(AFD *initialAfd)
{
  AFD *afd = copyAFDWithoutUnreachableStates(initialAfd);

  for (int i = 0; i < *afd->transicoes_numerica; i++)
  {
    Transition *transicao = afd->transicoes[i];
  }

  int **grupoEquivalencia = malloc(sizeof(int *) * 2);
  int *tamanhos = malloc(sizeof(int) * 2);
  tamanhos[0] = *afd->numero_estado_final;
  tamanhos[1] = *afd->estados_numericos - (*afd->numero_estado_final);
  int totalDeGrupos = !tamanhos[0] || !tamanhos[1] ? 1 : 2;
  grupoEquivalencia[0] = tamanhos[0] ? malloc(sizeof(int) * tamanhos[0]) : NULL;
  grupoEquivalencia[1] = tamanhos[1] ? malloc(sizeof(int) * tamanhos[1]) : NULL;

  if (!tamanhos[0])
  {
    tamanhos[0] = tamanhos[1];
    grupoEquivalencia[0] = grupoEquivalencia[1];
  }

  for (int i = 0; i < *afd->numero_estado_final; i++)
  {
    grupoEquivalencia[0][i] = afd->estado_final[i];
  }
  int indiceEstadoInicialAtual = 0;
  for (int i = 0; i < *afd->estados_numericos; i++)
  {
    if (isContained(afd->estado_final, i, *afd->numero_estado_final))
    {
      continue;
    }
    grupoEquivalencia[1][indiceEstadoInicialAtual] = i;
    indiceEstadoInicialAtual++;
  }
  int changed = 0;
  do
  {
    changed = 0;
    for (int i = 0; i < totalDeGrupos; i++)
    {
      int *x = NULL;
      int xTamanho = 0;
      int elemento= grupoEquivalencia[i][0];
      int *grupoEquivalenciaDeElementos = malloc(sizeof(int) * (*afd->numero_simbolos));
      for (int k = 0; k < *afd->numero_simbolos; k++)
      {
        char *estado= transitionFunction(afd, afd->alphabet[k], afd->estados[elemento]);
        int indiceEstado = getStatePosition(*afd, estado);
        int group = getEquivalenceGroup(grupoEquivalencia, totalDeGrupos, tamanhos, indiceEstado);
        grupoEquivalenciaDeElementos[k] = group;
      }

      for (int j = 0; j < tamanhos[i]; j++)
      {
        for (int l = 0; l < *afd->numero_simbolos; l++)
        {
          char *estado= transitionFunction(afd, afd->alphabet[l], afd->estados[grupoEquivalencia[i][j]]);
          int indiceEstado = getStatePosition(*afd, estado);
          int group = getEquivalenceGroup(grupoEquivalencia, totalDeGrupos, tamanhos, indiceEstado);
          if (group != grupoEquivalenciaDeElementos[l])
          {
            xTamanho++;
            if (!xTamanho)
            {
              x = malloc(sizeof(int));
            }
            else
            {
              x = realloc(x, sizeof(int) * xTamanho);
            }
            x[xTamanho - 1] = grupoEquivalencia[i][j];
            for (int k = j; k < tamanhos[i] - 1; k++)
            {
              grupoEquivalencia[i][k] = grupoEquivalencia[i][k + 1];
            }
            tamanhos[i]--;
            break;
          }
        }
      }
      if (xTamanho)
      {
        changed = 1;
        totalDeGrupos++;
        grupoEquivalencia = realloc(grupoEquivalencia, sizeof(int *) * totalDeGrupos);
        grupoEquivalencia[totalDeGrupos - 1] = x;
        tamanhos = realloc(tamanhos, sizeof(int) * totalDeGrupos);
        tamanhos[totalDeGrupos - 1] = xTamanho;
      }
    }
  } while (changed);

  AFD *novoAfd = getEmptyAFD();
  *novoAfd->estados_numericos = totalDeGrupos;
  novoAfd->estados= malloc(sizeof(char *) * totalDeGrupos);
  for (int i = 0; i < totalDeGrupos; i++)
  {
    novoAfd->estados[i] = mergeStates(afd, grupoEquivalencia[i], tamanhos[i]);
  }
  *novoAfd->numero_simbolos = *afd->numero_simbolos;
  novoAfd->alphabet = malloc(sizeof(char *) * (*afd->numero_simbolos));
  for (int i = 0; i < *afd->numero_simbolos; i++)
  {
    novoAfd->alphabet[i] = copyString(afd->alphabet[i]);
  }

  int estadoInicial = getEquivalenceGroup(grupoEquivalencia, totalDeGrupos, tamanhos, *afd->estado_inicial);
  *novoAfd->estado_inicial = estadoInicial;

  novoAfd->estado_final = malloc(sizeof(int));
  int tamanhoAlocado = 1;
  int estadosFinaisTotais = 0;

  for (int i = 0; i < totalDeGrupos; i++)
  {
    int contained = 1;
    for (int j = 0; j < tamanhos[i]; j++)
    {
      int elemento= grupoEquivalencia[i][j];
      if (!isContained(afd->estado_final, elemento, *afd->numero_estado_final))
      {
        contained = 0;
        break;
      }
    }
    if (contained)
    {
      if (estadosFinaisTotais == tamanhoAlocado)
      {
        tamanhoAlocado++;
        novoAfd->estado_final = realloc(novoAfd->estado_final, sizeof(int) * tamanhoAlocado);
      }
      novoAfd->estado_final[estadosFinaisTotais] = i;
      estadosFinaisTotais++;
    }
  }
  *novoAfd->numero_estado_final = estadosFinaisTotais;

  int numeroTransicoes = *novoAfd->estados_numericos * (*afd->numero_simbolos);
  novoAfd->transicoes = malloc(sizeof(Transition *) * numeroTransicoes);
  *novoAfd->transicoes_numerica = numeroTransicoes;
  int indiceTransicaoAtual = 0;
  for (int i = 0; i < totalDeGrupos; i++)
  {
    if (!tamanhos[i])
    {
      continue;
    }
    for (int j = 0; j < *afd->numero_simbolos; j++)
    {
      Transition *transicao = getEmptyTransition();
      *transicao->from = i;
      char *estado= transitionFunction(afd, afd->alphabet[j], afd->estados[grupoEquivalencia[i][0]]);
      int indiceEstado = getStatePosition(*afd, estado);
      int group = getEquivalenceGroup(grupoEquivalencia, totalDeGrupos, tamanhos, indiceEstado);
      *transicao->to = group;
      *transicao->read = j;
      novoAfd->transicoes[indiceTransicaoAtual] = transicao;
      indiceTransicaoAtual++;
    }
  }

  for (int i = 0; i < totalDeGrupos; i++)
  {
    free(grupoEquivalencia[i]);
  }
  free(grupoEquivalencia);
  free(tamanhos);

  freeAFD(afd);
  for (int i = 0; i < *novoAfd->transicoes_numerica; i++)
  {
    Transition *transicao = novoAfd->transicoes[i];
  }

  return novoAfd;
}
