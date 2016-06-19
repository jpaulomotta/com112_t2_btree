#include "btree.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_TESTES 1
#define TAB "\t\t"
#define LBREAK "\n"



void inicializar_arvore(char* nome_arquivo, int ordem, clock_t* tempo_cpu, time_t* tempo_s, char is_memoria, unsigned long long* memoria) {
  BTree* a;
  int elemento;
  int ordem_arquivo_ignorar;
  char saida[100];

  clock_t inicio_cpu;
  time_t inicio_time;

  FILE* pf = fopen(nome_arquivo, "r");
  if(!pf) {
    printf("ero porra");
    return;
  }
  inicio_time = time(NULL);
  inicio_cpu = clock();

  fscanf(pf, "%d", &ordem_arquivo_ignorar);

  a = bt_create(ordem);

  while(fscanf(pf, "%d, ", &elemento) != EOF) {
    a = bt_insert(a, elemento);
  }

  (*tempo_cpu) = clock() - inicio_cpu;
  (*tempo_s) = time(NULL) - inicio_time;

  if(is_memoria) {
    (*memoria) = bt_size_memory(a);
  }

  bt_destroy(a);
  fclose(pf);
}



int main (void) {
  int ordem;
  FILE* pf;
  int i_arquivo;
  int i_teste;
  int i_ordem;

  int num_arquivos = 5;
  char prefixo_arquivo[] = "entradas/entrada_";
  char sufixo_arquivo[] = ".txt";
  char* arquivos[5] = {"150k", "250k", "1000k", "1500k", "10000k"};

  char nome_arquivo[100];

  int num_ordens = 3;
  int ordens[3] = {21, 256, 512};

  unsigned long long memoria;

  clock_t tempo_cpu = 0;
  time_t tempo_s = 0;
  clock_t tempo_cpu_total;
  clock_t tempo_s_total;

  /**
  CABECARIO
  */
  printf("TEMPO\n===================\n");
  printf("ORD%s", TAB);
  for(i_arquivo = 0; i_arquivo < num_arquivos; i_arquivo++)
  {
    printf("%s%s", arquivos[i_arquivo], TAB);
  }
  printf(LBREAK);

  for(i_ordem = 0; i_ordem < num_ordens; i_ordem++)
  {
    printf("%d%s", ordens[i_ordem], TAB);
    for(i_arquivo = 0; i_arquivo < num_arquivos; i_arquivo++)
    {
      tempo_cpu_total = 0;
      tempo_s_total = 0;
      sprintf(nome_arquivo, "%s%s%s", prefixo_arquivo, arquivos[i_arquivo], sufixo_arquivo);
      for(i_teste = 0; i_teste < NUM_TESTES; i_teste++)
      {

        inicializar_arvore(nome_arquivo, ordens[i_ordem], &tempo_cpu, &tempo_s, 0, NULL);
        tempo_cpu_total += tempo_cpu;
      }
      printf("%ldms%s", (tempo_cpu_total/NUM_TESTES)/1000, TAB);
    }
    printf(LBREAK);
  }





  /**
  CABECARIO
  */
  printf("MEMORIA\n===================\n");
  printf("ORD%s", TAB);
  for(i_arquivo = 0; i_arquivo < num_arquivos; i_arquivo++)
  {
    printf("%s%s", arquivos[i_arquivo], TAB);
  }
  printf(LBREAK);

  for(i_ordem = 0; i_ordem < num_ordens; i_ordem++)
  {
    printf("%d%s", ordens[i_ordem], TAB);
    for(i_arquivo = 0; i_arquivo < num_arquivos; i_arquivo++)
    {
      tempo_cpu_total = 0;
      tempo_s_total = 0;
      sprintf(nome_arquivo, "%s%s%s", prefixo_arquivo, arquivos[i_arquivo], sufixo_arquivo);


      inicializar_arvore(nome_arquivo, ordens[i_ordem], &tempo_cpu, &tempo_s, 1, &memoria);
      printf("%llu%s", memoria, TAB);
    }
    printf(LBREAK);
  }


  return 0;
}
