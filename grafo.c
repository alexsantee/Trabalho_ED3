#include "constants.h"

#define MAX_VERTICE 100
#define MAX_ARESTA 100

struct aresta{
    char destino[TAM_VAR];  //Nome da cidade destino (id do vertice)
    int peso;               //Peso da aresta
    //A especificacao afirma que o peso e uma distancia e um tempo
    //de viagem, devemos guardar dois pesos?
};

//estrutura de vertice para armazenar no grafo
struct vertice{
    char origem[TAM_VAR];               //Nome da cidade de origem (id do vertice)
    struct aresta arestas[MAX_ARESTA];  //Cidades destino do vertice (LISTA ORDENADA)
};

//Define uma estrutura de grafo por lista de adjacencia
//A lista esta ORDENADA por nome da cidade de origem
struct grafo{
       struct vertice vertices[MAX_VERTICE]; 
};
