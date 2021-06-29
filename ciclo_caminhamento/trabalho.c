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

    int vetor[100];
    int tam;
}Ciclos;

int qntd_ciclos=0;
Ciclos ciclo[50000]; // uma "matriz" ciclos para auxiliar na não repetição de ciclos (a cada ciclo novo encontrado, é adicionado nessa matriz, portanto a medida que vai achando novos ciclos, é comparado nessa matriz para ver se o ciclo já existe, se já existir, o ciclo não é adicionado)
Grafo gf;
 
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


bool novo(int *caminho){ // funçao para identificar se um ciclo já existe dentro da matriz ciclos (onde armazena os ciclos encontrados)
    int tam = 0;
    for(int i = 0 ; i < 10000 ; i++){ // for para achar o tamanho do caminho
        if(caminho[i]!=0)
            tam++; // tamanho do caminho
    }

    int cont;
    for(int i = 0 ; i<qntd_ciclos; i++){ // percorre qnts ciclos q foram encontrados tem até agora
        cont = 0;
        for(int j = 0 ; j< tam && cont == j ; j++){ 
            for(int k = 0 ; k< ciclo[i].tam ; k++){
                if(ciclo[i].vetor[k] == caminho[j] && ciclo[i].tam == tam){ // faz uma comparação para ver se a matriz ciclo já possui os vertices que vieram do vetor caminho
                    cont++; // contador incrementa
                    break;
                }
            }
        }
        if(cont == tam){ // ao final do primeiro for, significa que o primeiro ciclo encontrado na matriz ciclo já foi concluido, e só saberemos se ele é igual ao ciclo do vetor caminho se ele possui cont == inicio, pois inicio é o tamanho do caminho. portanto se inicio == cont então significa que todos os elementos encontrados no caminho, já estão presentes na matriz ciclo, portanto retorna falso.
            return false; 
        }
    }

    return true; // ciclo novo, então retorna true.

}


void conta_ciclo(int pai, int filho, int *visitado, int *caminho, int *aux)
{
    //vertice já completado
    if (visitado[pai] == 2)
    {
        return; // retorna a função pra dar continuidade ao restante dos vertices
    }
    if (visitado[pai] == 1) // ciclo "encontrado", verifica os ultimos vertices para encontarr o ciclo 
    {
        
        int atual = filho; 
        caminho[atual] = 1; // marca a posição atual com 1 no vetor caminho, para identificar que ela faz parte do ciclo
        
        while (atual != pai)
        {
            atual = aux[atual];
            caminho[atual] = 1;

        }
        if(true){
            for(int i = 0; i < gf.vertices ; i++)
            {
                if(caminho[i]!=0){ // vai adicionando as posições desse vetor caminho que são diferentes de 0, na nossa matriz de ciclos para armazenar o ciclo encontrado
                
                    ciclo[qntd_ciclos-1].vetor[ciclo[qntd_ciclos-1].tam] = i; // add o vertice que faz parte do ciclo X
                    ciclo[qntd_ciclos-1].tam++; // aumenta o tamanho do ciclo X em 1
                }
                   
            }
            qntd_ciclos += 1; // adiciona +1 ciclo nessa variavel
        }

        for(int i = 0 ; i < gf.vertices ; i++){
            caminho[i] = 0; // reseta o caminho para a proxima interação
        } 
        return;
        
    }
    
    aux[pai] = filho; // vai guardando nesse vetor aux o caminho que está percorrendo o filho para chegar ao pai
    visitado[pai] = 1; // marca como "visitando"
    

    for (int i = 0; i < gf.grau[pai] ; i++) // confere os vetores conetados presentes na lista do vertice pai
    {
        
        if (gf.arestas[pai][i] == aux[pai]) // checa se os valores batem
        {
            continue;
        }
        
        conta_ciclo(gf.arestas[pai][i], pai, visitado, caminho, aux); // função recursiva, agora levando o primeiro "filho" do pai, se tornando o novo pai agora, e o pai antigamente se tornando filho agora.
    }
    visitado[pai] = 2; // apos o final marca como visitado, finalizando todo percurso desse vertice existente.

}

int main()
{
    int visitado[10000];
    int aux[10000];
    int caminho[10000];

    inicializa(&gf); // função pra inicializar o grafo
    
    for(int i = 0 ; i< 50000 ; i++){
        ciclo[i].tam = 0; // setando o tamanho = 0 para todos os ciclos que serão encontrados e armazenados nesse vetor
    }

    for(int i = 0 ; i< gf.vertices; i++){
        conta_ciclo(i+1,i,visitado,caminho,aux); // funcao para identificar os ciclos, tendo a origem do caminhamento revezada por cada vertice do grafo  
    }

    for(int i = 0 ; i < qntd_ciclos; i++) // funcao para printar os  ciclos encontrados e armazenados na matriz ciclo
    {
        if(ciclo[i].tam > 2){
            printf("Ciclo %d: ",i+1);
            for(int j = 0; j< ciclo[i].tam; j++) 
            {
                printf("%d ",ciclo[i].vetor[j]);
            }
            printf("\n");
        }
        
    }

    return 0;

}
