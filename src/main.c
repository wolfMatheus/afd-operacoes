#include <stdio.h>
#include <string.h>
#include "./operacao/operacoes.h"

/**
 * Registra os parametros necessarios para o funcionamento da CLI da aplicacao.
 */
CLI_Params *criaCLIParams()
{
  CLI_Params *parametros = getParams(10); 

  registerParam(parametros, DOT, 1);
  registerParam(parametros, OUTPUT, 1);
  registerParam(parametros, COMPLEMENTO, 1);
  registerParam(parametros, INTERSECAO, 2);
  registerParam(parametros, UNIAO, 2);
  registerParam(parametros, MINIMIZACAO, 1);
  registerParam(parametros, RECONHECER, 2);

  return parametros;
}

/**
 * Programa principal, executa o modulo de CLI, depois aplica o modulo de contexto,
 * e ao final aplica o modulo de operacoes, para retornar o resultado esperado.
 */
int main(int argc, char *argv[])
{
  CLI_Params *parametros = criaCLIParams();
  CLI_Result *resultado = readCLI(parametros, argc, argv);

  ApplicationContext *appContext = getAppContext(*resultado);
  if (appContext->err)
  {
    printf("Invalid input provided. Error log: %s\n", appContext->message);
    return 1;
  }

  validateAppContext(appContext);
  if (appContext->err)
  {
    printf("Invalid input provided. Error log: %s\n", appContext->message);
    return 1;
  }

  executeOperation(*appContext);

  freeAppContext(appContext);
  freeCLI(resultado);
  return 0;
}