#include <stdio.h>
#define DOT_H

/**
 * Transicao do DOT format. Basicamente igual a transicao do AFD, porem no contexto de transformacao DOT.
*/
typedef struct dotformat_transition_est
{
  char *from;
  char *to;
  char *label;
} DOTTransition;

/**
 * Representacao do DOT format. Essa estrutura serve para armazenar todas as informacoes necessarias para escrever um
 * arquivo DOT referente a um AFD.
*/
typedef struct dotformat_est
{
  char **double_circles;
  int numero_circulos_duplos;
  int transicoes_numerica;
  char *estado_inicial;
  DOTTransition **transicoes;
} DOTFormat;

/**
 * Factory que gera um DOTTransition a partir dos parametros informados.
 * @param from: de onde sai a transicao.
 * @param to: para onde vai a transicao.
 * @param read: simbolo lido.
*/
DOTTransition *getDOTTransition(char *from, char *to, char *label);

/**
 * Factory que retorna um DOTFormat a partir dos parametros recebidos.
 * @param double_circles: estados finais
 * @param numero_circulos_duplos: numero de estados finais
 * @param transicoes: transicoes do AFD traduzidas para DOTTransitions.
 * @param transicoes_numerica: numero de transicoes.
 * @param estado_inicial: estado inicial.
 * @return instancia de DOTFormat correspondente.
*/
DOTFormat *getDOTFormat(char **double_circles, int numero_circulos_duplos, DOTTransition **transicoes, int transicoes_numerica, char *estado_inicial);

/**
 * Libera a memoria alocada por uma DOTTransition.
 * @param transicao: transicao a ser liberada.
*/
void freeDotTransition(DOTTransition *transition);

/**
 * Libera toda a memoria alocada por um DOTFormat.
 * @param DOTFormat: objeto cuja memoria sera liberada.
*/
void freeDot(DOTFormat *DOTFormat);

/**
 * Escreve um DOTFormat no arquivo .dot correspondente, com nome fileName.
 * @param DOTFormat: informacoes a serem escritas no arquivo.
 * @param fileName: nome do arquivo que vai receber as informacoes.
*/
void writeDot(DOTFormat *DOTFormat, char *fileName);