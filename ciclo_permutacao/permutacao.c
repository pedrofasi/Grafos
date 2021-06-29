#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct grafo
{

    int vertices;
    int n_arestas;
    int **arestas;
    int *grau;
    int grau_max;

} Grafo;

typedef struct Ciclos{

    int vetor[1000];
    int tam;
}Ciclos;

Grafo gf;
int q_ciclos=0;
Ciclos ciclo[10000]; // uma "matriz" ciclos para auxiliar na não repetição de ciclos (a cada ciclo novo encontrado, é adicionado nessa matriz, portanto a medida que vai achando novos ciclos, é comparado nessa matriz para ver se o ciclo já existe, se já existir, o ciclo não é adicionado)
 
Grafo* inicializa(Grafo *gf)
{

    int origem, destino;
    FILE *fp;

    fp = fopen("grafos.txt","r"); // abre o arquivo grafos.txt

    fscanf(fp,"%d\n",&gf->vertices); // primeira linha lida, pega os vertices
    fscanf(fp,"%d\n",&gf->n_arestas); // pega o numero de arestas

    gf->grau = (int*)calloc(gf->vertices,sizeof(int)); // seta grau max de cada elemento = o tamanho de vertices
    for(int i = 0 ; i< gf->vertices ; i++)
    {
        gf->grau[i]=0; // inicializa todos elementos com grau = 0, já que não possui elementos ainda
    }
    gf->arestas = (int**)malloc(gf->vertices * sizeof(int*)); // alocação do espaço da memoria para criar a lista de vertices

    for(int i=0; i<gf->vertices; i++)
    {
        gf->arestas[i] = (int*)malloc(gf->vertices * sizeof(int)); // alocação do espaço da memoria pra criar a lista de vertices conectados ao vertice i.
    }

    while(!feof(fp))
    {
        fscanf(fp,"%d %d\n",&origem,&destino); // le origem e destino dos vertices
        gf->arestas[origem][gf->grau[origem]] = destino; // aloca na lista da origem, com base no seu grau, o vertice de destino 
        gf->grau[origem]++; // aumenta o grau

        gf->arestas[destino][gf->grau[destino]] = origem; // faz a mesma coisa só que ao contrario(grafo não direcionado)
        gf->grau[destino]++;
    }

    fclose(fp);

}

bool novo(int *caminho,int *inicio){ // funçao para identificar se um ciclo já existe dentro da matriz ciclos (onde armazena os ciclos encontrados)

    int cont;
    for(int i = 0 ; i<q_ciclos; i++){ // percorre qnts ciclos q foram encontrados tem até agora
        cont = 0;
        for(int j = 0 ; j< *inicio && cont == j ; j++){ 
            for(int k = 0 ; k< ciclo[i].tam ; k++){
                if(ciclo[i].vetor[k] == caminho[j] && ciclo[i].tam == *inicio){ // faz uma comparação para ver se a matriz ciclo já possui os vertices que vieram do vetor caminho
                    cont++; // contador incrementa
                    break;
                }
            }
        }
        if(cont == *inicio){ // ao final do primeiro for, significa que o primeiro ciclo encontrado na matriz ciclo já foi concluido, e só saberemos se ele é igual ao ciclo do vetor caminho se ele possui cont == inicio, pois inicio é o tamanho do caminho. portanto se inicio == cont então significa que todos os elementos encontrados no caminho, já estão presentes na matriz ciclo, portanto retorna falso.
            return false; 
        }
    }

    return true; // ciclo novo, então retorna true.

}


void permutacao(int a, int b, bool *visitado, int *caminho, int *inicio){

    visitado[a] = true; // indica que agora o vertice A foi visitado.
    
    caminho[*inicio]=a; // começa o caminho
    
    *inicio += 1; // aumenta o tamanho do caminho

    if(a==b){ // se o inicio for igual ao fim, significa que concluimos o caminho e vamos analisar uma possibilidade de ciclo
        if(*inicio > 2){ // k > 2, inicio é quantas vezes o caminho foi andado
            for(int i = 0 ; i< gf.grau[caminho[0]];i++){  // percorre todo o grau do vertice primario do caminho
                if(gf.arestas[caminho[0]][i]==caminho[(*inicio-1)]){ // Confere se a ultima posição do caminho, tem conexão com a primeira, gerando assim um ciclo. 
                    if(novo(caminho,inicio)){ // função para saber se já tem o ciclo já encontrado anteriormente
                        printf("Ciclo %d: ",q_ciclos+1);
                        for(int i = 0 ; i < *inicio ; i++){
                            printf("%d ",caminho[i]); // printa novo ciclo encontrado
                            ciclo[q_ciclos].vetor[i] = caminho[i]; // adiciona novo ciclo encontrado à matriz ciclo na posição q_ciclos
                            ciclo[q_ciclos].tam++; // aumenta o tamanho do ciclo na posição q_ciclos (que indica quantos ciclos possui até o momento)
                        }
                        printf("\n");
                        q_ciclos++; // apos adicionar o ciclo, é aumentado +1 nessa variavel para indicar a proxima posição da memoria para adicionar o ciclo
                    }
                }   
            }     
        }
    }
    

    else{ // caso não seja, continua percorrendo o caminho
        int aux;
        for(int i = 0 ; i < gf.grau[a]; i++){
            aux = gf.arestas[a][i]; // checa qual o proximo vetor a ser explorado (o vetor precisa estar conectado a A)
            if(!visitado[aux]){
                permutacao(aux,b,visitado,caminho,inicio); // função recursiva passando agora a proxima posição do caminho atual
            }
        }

    }
    *inicio -= 1; // finalização do percurso, reduzindo todas as interações (como é recursivo), para a preparação de outra nova chamada da função.
    visitado[a]= false; // voltando o visitado para falso, para futuras interações.

}

void gera_caminhos(int a, int b){ // gerador de caminhos

    bool visitado[gf.vertices]; // criação de um vetor com tamanho igual ao vertices do grafo
    int caminho[gf.vertices]; // criaçao de uma variavel caminho com o tamanho igual ao vertices do grafo
    int inicio = 0;

    for(int i = 0 ; i< gf.vertices ; i++){
        visitado[i]=false; // setando todas os vertices como não visitados
    }

    permutacao(a,b,visitado,caminho,&inicio); // chamada da função para achar todos os caminhos de a até b

}


int main()
{
	inicializa(&gf); // função para inicializar o grafo

	for(int i = 0 ; i< 10000 ; i++){
        ciclo[i].tam = 0;  // setando o tamanho = 0 para todos os ciclos que serão encontrados e armazenados nesse vetor
    }

    for(int i = 0 ; i< gf.vertices; i++){
        for(int j = 0 ; j< gf.grau[i]; j++){
            gera_caminhos(i,j); // função para achar todos os caminhos de um vértice para o outro (iremos achar todos os caminhos possiveis dentro do grafo para analisar futuramente se esse caminho é um ciclo)
        }
    }

	return 0;
}