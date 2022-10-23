#include <stdio.h>
#include <string.h>
#include "./operacao/operacoes.h"

/**
 * Registra os parametros necessarios para o funcionamento da CLI da aplicacao.
 */
CLI_Params *criaCLIParams()
{
  CLI_Params *parametros = getParams(10); 

  registraParametros(parametros, DOT, 1);
  registraParametros(parametros, OUTPUT, 1);
  registraParametros(parametros, COMPLEMENTO, 1);
  registraParametros(parametros, INTERSECAO, 2);
  registraParametros(parametros, UNIAO, 2);
  registraParametros(parametros, MINIMIZACAO, 1);
  registraParametros(parametros, RECONHECER, 2);

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

  ApplicationContext *appValidacao = getAppContext(*resultado);
  if (appValidacao->err)
  {
    printf("Invalid input provided. Error log: %s\n", appValidacao->message);
    return 1;
  }

  validateAppContext(appValidacao);
  if (appValidacao->err)
  {
    printf("Invalid input provided. Error log: %s\n", appValidacao->message);
    return 1;
  }

  executeOperation(*appValidacao);

  freeAppContext(appValidacao);
  freeCLI(resultado);
  return 0;
}