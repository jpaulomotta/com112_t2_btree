#include "btree.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define N 6
#define ORDEM 4

clock_t inicio_cpu;
time_t inicio_time;
void iniciar_cronometro(char* msg)
{
  printf("\n\n\n\n********************\nMedindo tempo: %s\n", msg);
  inicio_time = time(NULL);
  inicio_cpu = clock();
}

void parar_cronometro(char* msg)
{
  clock_t fim_cpu = (clock()-inicio_cpu);
  time_t fim_time = time(NULL) - inicio_time;

  printf("Tempo da operacao:\nSegundos: %ld\nClock Ticks: %ld\n%s\n*******************\n\n\n\n", fim_time, fim_cpu, msg);
}

BTree* inicializar_arvore(FILE* pf, int* ordem) {
  BTree* a;
  int count_elementos = 0;
  int elemento;
  char saida[100];

  iniciar_cronometro("Iniciando Arvore");
  fscanf(pf, "%d", ordem);

  printf("Ordem %d\n", (*ordem));
  a = bt_create((*ordem));

  while(fscanf(pf, "%d, ", &elemento) != EOF) {
    a = bt_insert(a, elemento);
    count_elementos++;
  }
  sprintf(saida, "%d elementos inseridos", count_elementos);
  parar_cronometro(saida);
  return a;
}



void opcao_imprimir(BTree* a) {
  printf("\n\n");
  bt_print(a, 0);
  printf("0 1 2 3 4 5 6 7 8 9\n");
}

void opcao_remover(BTree** a)
{
  int valor;
  printf("Entre com um valor inteiro:\n=> ");
  scanf("%d", &valor);

  iniciar_cronometro("Removendo Elemento");
  (*a) = bt_remove((*a), valor);
  parar_cronometro("Removendo Elemento [Concluido]\n\n\n\n");
}

void opcao_inserir(BTree** a) {
  int valor;
  printf("Entre com um valor inteiro:\n=> ");
  scanf("%d", &valor);

  iniciar_cronometro("Inserindo Elemento");
  (*a) = bt_insert((*a), valor);
  parar_cronometro("Inserindo Elemento [Concluido]\n\n\n\n");
}

void entrar_modo_operacoes(BTree* a) {
  int opcao;

  do {
    printf("Menu de Operacoes\n===================\n\n1. Inserir elemento\n2. Remover elemento\n3. Imprimir\n4. Sair\n=> ");
    scanf("%d", &opcao);

    switch(opcao) {
      case 1:
        opcao_inserir(&a);
        break;
      case 2:
        opcao_remover(&a);
        break;
      case 3:
        opcao_imprimir(a);
        break;
      case 4:
        return;
    }
  } while(opcao != 4);

}


int main (int argc, char** argv) {
  int ordem;
  FILE* pf;

  if(argc != 2) {
    printf("Erro. Arquivo de entrada nao expecificado nos parametros.\n");
    return -1;
  }
  pf = fopen(argv[1], "r");


  BTree* a = inicializar_arvore(pf, &ordem);

  entrar_modo_operacoes(a);

  bt_destroy(a);
  return 0;
}
