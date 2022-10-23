#include <stdlib.h>
#include "../reconhecimento/reconhecimento.h"

void executeOperation(ApplicationContext context)
{
  AFD *afd = readAFD(context.input1);
  AFD *resultado = NULL;
  if (context.dot)
  {
    DOTFormat *dotFormat = visualizacao(afd);
    writeDot(dotFormat, context.output);
    freeDot(dotFormat);
  }
  if (context.complemento)
  {
    resultado = complemento(afd);
  }
  if (context.intersecao)
  {
    AFD *afd2 = readAFD(context.input2);
    resultado = intersecao(afd, afd2);
    freeAFD(afd2);
  }
  if (context.uniao)
  {
    AFD *afd2 = readAFD(context.input2);
    resultado = uniao(afd, afd2);
    freeAFD(afd2);
  }
  if (context.minimizacao)
  {
    resultado = minimizacao(afd);
  }
  if (context.reconhecer)
  {
    int tamanho;
    char **words = readWords(context.input2, &tamanho);
    if (!words)
    {
      return;
    }
    int *resultados = reconhecer(afd, words, tamanho);
    writeReconhecerResult(resultados, context.output, tamanho);
    free(resultados);
    freeWords(words, tamanho);
  }
  if (resultado)
  {
    writeAFD(*resultado, context.output);
    freeAFD(resultado);
  }
  freeAFD(afd);
}