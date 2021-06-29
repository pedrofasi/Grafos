#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Cel{
  int dado;
  struct Cel *prox;
}Cel;

typedef struct Lista{
  Cel *inicio, *fim;
  int tam;
}Lista;

void create_lista(Lista *l){
  Cel *temp = (Cel*)malloc(sizeof(Cel));
  temp->prox = NULL;
  l->inicio = l->fim = temp;
  l->tam = 0;
}

void add(Lista *l, int dado){
  Cel *temp = (Cel*)malloc(sizeof(Cel));
  temp->prox = NULL;
  temp->dado = dado;
  
  l->fim->prox = temp;
  l->fim = temp;
  l->tam++;
}

int remove_at(Lista *l, int pos){

  if(pos < 0 || pos >= l->tam )
    return -1;

  Cel *ant = l->inicio;
  for(int i=0; i<pos;i++)
    ant = ant->prox;

  Cel *temp = ant->prox;
  int dado = temp->dado;

  ant->prox = temp->prox;

  if(ant->prox == NULL)
      l->fim = ant;

  free(temp);

  l->tam--;

  return dado;
}

void print_lista(Lista l){
  Cel *temp = l.inicio->prox;
  int i=0;
  while(temp!=NULL){
    printf("-%d-",  temp->dado);
    temp = temp->prox;
    i++;
  }
}

bool verifica_lista(Lista *l, int dado){
  Cel *temp = l->inicio->prox;
  
  while(temp!=NULL){
    if(temp->dado = dado)
      return false;
    temp = temp->prox;
  }
  return true;
}

Lista clonar_lista(Lista l1, Lista l2){
  Cel *temp =l1.inicio->prox;

  while(temp!=NULL){
    add(&l2, temp->dado);
    temp = temp->prox;
  }
  return l2;  
}

void destroy_lista(Lista *l){
  while(l->inicio != l->fim)
    remove_at(l, 0);
  free(l->inicio);
}
