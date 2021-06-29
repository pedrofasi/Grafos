#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista_ponteiro.h"

typedef struct Vert{
  bool aresta; // se verdadeiro existirá um aresta na posição especificada da matriz de adj
  int capacidade; // se existir aresta a capacidade será de 1, caso contrário 0
}Vert;

struct Vert **CreateMadjText(struct Vert **Madj){
  int i, j, V, vi, vf; // 'V' quantidade de arestas 'vi' vertice inicial, 'vf' vertice final

  FILE *Gtext;

  Gtext = fopen("grafo.txt", "r"); // abre o arquivo

  if(Gtext == NULL) {
    printf("\nNão foi possível abrir o arquivo.\n");
    exit(1);
  }

  fscanf(Gtext, "%d\n", &V); // le quantos vertices
  
  Madj = (struct Vert **)malloc(V*sizeof(struct Vert *)); // aloca primeira dimensão da matriz
  if(Madj == NULL){
    printf("\nMemoria nao suficiente\n");
    exit(1);
  }

  for(i=0; i < V; i++){ // aloca a segunda dimensão da matriz
    Madj[i] = (struct Vert *)malloc(V*sizeof(struct Vert));
    if(Madj[i] == NULL){
      printf("\nMemoria nao suficiente\n");
      exit(1);
    }
  }

  for(i=0; i < V; i++){ // inicializa todos como não possuindo arestas
    for(j=0; j < V; j++){
     	Madj[i][j].aresta = false;
    }
  }

  while(!feof(Gtext)){ // adiciona 'true' onde há arestas e coloca a capacidade com valor 1
    fscanf(Gtext, "%d %d\n", &vi, &vf);
    Madj[vi][vf].aresta = true;
    Madj[vi][vf].capacidade = 1;
  }

  fclose(Gtext); // fecha o arquivo

  return Madj; // retorna a matriz de adj com base no arquivo lido
}

int n_vertices(){ // le o arquivo e retorna o número de vertices
  
  int V;

  FILE *Gtext;

  Gtext = fopen("grafo.txt", "r"); // abre o arquivo

  if(Gtext == NULL) {
    printf("\nNão foi possível abrir o arquivo.\n");
    exit(1);
  }

  fscanf(Gtext, "%d\n", &V); // le quantos vertices

  fclose(Gtext);

  return V; // retorna a quantidade de vertices
}
 
void printMadj(struct Vert **Madj, int V){ // imprime a matriz
  int i, j;
  for(i=0; i < V; i++){
    for(j=0; j < V; j++){
      if(Madj[i][j].aresta)
        printf("1");
      else
        printf("0");
    }
    printf("\n");
  }
}

void desalocacao(struct Vert **Madj, int V){ // Desaloca toda a matriz
    for(int i=0; i<V; i++){
      free(Madj[i]);
    }
    free(Madj);
    printf("\nMemoria liberada!\n");
}


void n_max_cda(struct Vert **Madj, Lista *l1, int V, int va, int vf, int *quant){
  // encontrará a quantidade de caminhos disjuntos por arestas
    bool achou = false;
  for(int i=0; i < V; i++){
    if(Madj[va][vf].aresta && Madj[va][vf].capacidade == 1){ // verifica se o aresta tem uma aresta
      Madj[va][vf].capacidade = 0;                            // direta para o aresta final
      *quant = *quant + 1; // incrementa a quantidade de caminhos disjuntos de arestas encontrados
      add(l1, va); // adiciona o vertice atual na lista
      add(l1, vf); // adiciona o vertice final na lista
      printf("\nCaminho disjunto de aresta Nº %d\n", *quant);
      print_lista(*l1); // imprime a lista contendo o caminho encontrado
      printf("\n");
      destroy_lista(l1);
      achou = true;
    }
    if(Madj[va][i].aresta && Madj[va][i].capacidade == 1){
      Madj[va][i].capacidade = 0; // atribui 0 para que essa aresta não seja utilizada mais de uma vez
      add(l1, va); // adiciona o vertice atual na lista
      n_max_cda(Madj, l1, V, i, vf, quant); // chama novamente a função em que 'i' é o vertice atual
      break; // o break é usado para que no retorno da função recursiva o for externo não continue a ser executado
    }
    if(achou){
        break;
    }
  }
}


int main(){
  
  struct Vert **Madj; // matriz usada para representar o grafo
  
  int V; // 'V' quantidade de arestas do grafo
  int vi, vf, quant=0; // 'vi' vertice inical; 'vf' vertice final; 'quant' quantidade de caminhos

  Madj = CreateMadjText(Madj); // preenche a matiz tendo como entrada o arquivo com pares de vertices do grafo
  
  V = n_vertices();

  printf("\nA matriz de adjacencia do grafo lido\n");
  printf("\n====================================\n\n");
  printMadj(Madj, V);
  printf("\n====================================\n");
 
  printf("\nDigite o vertice inicial\n");
  scanf("%d", &vi);
  printf("\nDigite o vertice final\n");
  scanf("%d", &vf);
  printf("\n");

  for (int i=0; i < V; i++){
    if(Madj[vi][i].aresta && Madj[vi][i].capacidade == 1){
      Lista l1; // declara a lista a ser usada na função 'n_max_cda'
      create_lista(&l1); // cria a lista
      n_max_cda(Madj, &l1, V, vi, vf, &quant); // chama a função que encontra os caminhos disjuntos por arestas
    }
  }

  if(quant == 0){ // caso não haja caminhos entre os dois vertices
    printf("\nNão há caminhos entre os vertices %d e %d\n\n", vi, vf);
  }
  
  printf("\n");

  desalocacao(Madj, V); // desaloca a matriz de adjacencia criada a partir do arquivo lido

  return 0;

}
