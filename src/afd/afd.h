#include <stdio.h>

/**
 * Estrutura responsavel por representar uma transicao. Utiliza os indices dos elementos em seus respectivos arrays no afd.
 * from: estado em que o afd se encontra no momento da transicao
 * read: simbolo que o afd faz a leitura para ativar a transicao
 * to: estado que o afd assume apos realizar a transicao 
 */
typedef struct transition_est
{
  int *from;
  int *read;
  int *to;
} Transition;

/** 
 * Estrutura que representa um AFD.
 * estados: array de estados de tamanho estados_numericos.
 * alphabet: array de simbolos de tamanho numero_simbolos.
 * estado_inicial: estado inicial, indice sobre o array de estados.
 * estado_final: estados finais, array de indices sobre o array de estados, de tamanho numero_estado_final
 * transicoes: array de transicoes de tamanho transicoes_numerica. 
*/
typedef struct afd_est
{

  char **estados;
  int *estados_numericos;

  char **alphabet;
  int *numero_simbolos;

  int *estado_inicial;

  int *estado_final;
  int *numero_estado_final;

  Transition **transicoes;
  int *transicoes_numerica;

} AFD;

/**
 * Factory method que retorna uma instancia de transicao, dados os parametros de transicao em sua forma string,
 * convertendo cada um deles para indices sobre os arrays de estados e simbolos no AFD recebido.
 * @param afd: AFD alvo
 * @param from: equivalente ao campo "from" da estrutura de transicao.
 * @param read: equivalente ao campo "read" da estrutura de transicao.
 * @param to: equivalente ao campo "to" da estrutura de transicao.
 * @return transicao correspondente.
*/
Transition *getTransition(AFD afd, char *from, char *to, char *read);

/**
 * Aloca memoria e retorna uma transicao sem valores.
 * @return uma transicao sem valores.
*/
Transition *getEmptyTransition();

/**
 * Busca e retorna a posicao do estado no AFD provido.
 * @param afd: afd alvo
 * @param estado: estado a ser procurado
 * @return o indice caso encontre, -1 caso contrario.
*/
int getStatePosition(AFD afd, char *estado);

/**
 * Busca e retorna a posicao do simbolo no AFD provido.
 * @param afd: afd alvo
 * @param symbol: estado a ser procurado
 * @return o indice caso encontre, -1 caso contrario.
*/
int getSymbolPosition(AFD afd, char *symbol);

/**
 * Retorna um afd vazio.
 * @return um afd vazio.
*/
AFD *getEmptyAFD();

/**
 * Faz a leitura de um AFD a partir do arquivo de nome informado.
 * @param fileName: nome do arquivo que contem o afd.
 * @return o afd correspondente.
*/
AFD *readAFD(char *fileName);

/**
 * Escreve o afd informado no arquivo com o nome provido.
 * @param afd: o afd que se quer escrever no arquivo.
 * @param fileName: nome do arquivo que vai receber o afd.
*/
void writeAFD(AFD afd, char *fileName);

/**
 * Libera a memoria alocada por um afd.
 * @param afd: afd cuja memoria alocada sera liberada.
*/
void freeAFD(AFD *afd);