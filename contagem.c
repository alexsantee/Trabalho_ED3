#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "contagem.h"

void inicializa_lista(struct lista *lista){
    lista->tamanho = 0;
    return;
}

//Retorna 0 se encontrou e NAO_ENCONTRADO caso contrario,
//Retorna na variavel pos a posicao da cidade ou a melhor posicao para inserir
int busca_cidade(char *nome, int *pos ,struct lista *lista){
    //busca binaria
    int min = 0;
    int max = lista->tamanho;
    int mid = (max+min)/2;   //divisao truncada
    int last_mid = -1;

    while(mid != last_mid){
        if( strcmp(nome, lista->cidades[mid].nome) > 0){ //nome > comparado
            min = mid;
        }
        else if( strcmp(nome, lista->cidades[mid].nome) < 0 ){//nome < comparado
            max = mid;
        }
        else{   //nome == comparado
            //retorno por variavel e da funcao
            if (pos != NULL) *pos = mid;
            return 0;
        }

        last_mid = mid;
        mid = (max+min)/2;
    }

    //retorno por variavel e da funcao
    if (pos != NULL) *pos = mid;
    return NAO_ENCONTRADO;
}

void insere_cidade(char *nome, struct lista *lista){
    int pos;
    int encontrado;
    encontrado = busca_cidade(nome, &pos,lista);

    if (encontrado != NAO_ENCONTRADO){ 
        (lista->cidades[pos].repeticoes)++;
    }
    
    else {  //cidade nao encontrada
        //shift das cidades
        int i;
        for(i=lista->tamanho; i>pos; i--){
            strcpy ( lista->cidades[i].nome, lista->cidades[i-1].nome );
            lista->cidades[i].repeticoes = lista->cidades[i-1].repeticoes;
        }
        //insercao de fato
        strcpy ( lista->cidades[pos].nome, nome );
        lista->cidades[pos].repeticoes = 1;
        (lista->tamanho)++;
    }
    return;
}

//retorna 0 caso removido com sucesso e NAO_ENCONTRADO caso nao encontrado
int remove_cidade(char *nome, struct lista *lista){
    int pos;
    int encontrado;
    encontrado = busca_cidade(nome, &pos,lista);

    if (encontrado == NAO_ENCONTRADO){
        return NAO_ENCONTRADO;
    }
    else{   //cidade encontrada
        (lista->cidades[pos].repeticoes)--;
        if ( lista->cidades[pos].repeticoes == 0){  //removidos todos os vertices
            //shift das cidades
            int i;
            for(i=pos+1; i < lista->tamanho; i++){
                strcpy(lista->cidades[i-1].nome, lista->cidades[i].nome);
                lista->cidades[i-1].repeticoes = lista->cidades[i].repeticoes;
            }
            (lista->tamanho)--;
        }
        return 0;
    }
}
