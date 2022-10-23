#include <stdlib.h>
#include "../reconhecimento/reconhecimento.h"

void executeOperation(ApplicationContext validacao)
{
  AFD *afd = readAFD(validacao.input1);
  AFD *resultado = NULL;
  if (validacao.dot)
  {
    DOTFormat *dotFormat = visualizacao(afd);
    writeDot(dotFormat, validacao.output);
    freeDot(dotFormat);
  }
  if (validacao.complemento)
  {
    resultado = complemento(afd);
  }
  if (validacao.intersecao)
  {
    AFD *afd2 = readAFD(validacao.input2);
    resultado = intersecao(afd, afd2);
    freeAFD(afd2);
  }
  if (validacao.uniao)
  {
    AFD *afd2 = readAFD(validacao.input2);
    resultado = uniao(afd, afd2);
    freeAFD(afd2);
  }
  if (validacao.minimizacao)
  {
    resultado = minimizacao(afd);
  }
  if (validacao.reconhecer)
  {
    int tamanho;
    char **words = readWords(validacao.input2, &tamanho);
    if (!words)
    {
      return;
    }
    int *resultados = reconhecer(afd, words, tamanho);
    writeReconhecerResult(resultados, validacao.output, tamanho);
    free(resultados);
    freeWords(words, tamanho);
  }
  if (resultado)
  {
    writeAFD(*resultado, validacao.output);
    freeAFD(resultado);
  }
  freeAFD(afd);
}