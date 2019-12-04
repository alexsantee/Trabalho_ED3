#include <bits/stdc++.h>

using namespace std;

struct aresta{
    string destino;  //Nome da cidade destino (id do vertice)
    int distancia;
    string tempo;
};

//estrutura de vertice para armazenar no grafo
struct vertice{
    string origem;                //Nome da cidade de origem (id do vertice)
    vector<aresta> arestas;       //Cidades destino do vertice (LISTA ORDENADA)
};

//Define uma estrutura de grafo por lista de adjacencia
//A lista esta ORDENADA por nome da cidade de origem
struct grafo{
       vector<vertice> vertices; 
};
