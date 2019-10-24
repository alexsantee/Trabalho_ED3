#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "contagem.h"

void inicializa_lista(struct lista *lista){
    lista->tamanho = 0;
    return;
}

//Retorna ENCONTRADO se encontrou e NAO_ENCONTRADO caso contrario,
//Retorna na variavel pos a posicao da cidade ou a melhor posicao para inserir
int busca_cidade(char *nome, int *pos ,struct lista *lista){
    //busca binaria
    int min = 0;
    int max = lista->tamanho-1;
    int mid;

    while(min <= max){
        mid = (max+min)/2;   //Divisao truncada
        if( strncmp(nome, lista->cidades[mid].nome, TAM_VAR) > 0){ //nome > comparado
            min = mid+1;
        }
        else if( strncmp(nome, lista->cidades[mid].nome, TAM_VAR) < 0){//nome < comparado
            max = mid-1;
        }
        else{   //nome == comparado
            //retorno por variavel e da funcao
            //se a busca for somente para saber se a cidade existe, nao eh
            //necessario retornar a posicao
            if (pos != NULL) *pos = mid;
            return ENCONTRADO;
        }
    }

    //retorno por variavel e da funcao
    if (pos != NULL){
        //Verifica se deve colocar antes ou depois do atual
        if (strncmp(nome, lista->cidades[mid].nome, TAM_VAR) < 0){
            *pos = mid;
        }
        else{
            *pos = mid+1;
        }
    }
    return NAO_ENCONTRADO;
}

void insere_cidade(char *nome, struct lista *lista){
    if(lista->tamanho == 0){
        strncpy ( lista->cidades[0].nome, nome, TAM_VAR );
        lista->cidades[0].repeticoes = 1;
        (lista->tamanho)++;
        return;
    }

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
            strncpy ( lista->cidades[i].nome, lista->cidades[i-1].nome, TAM_VAR );
            lista->cidades[i].repeticoes = lista->cidades[i-1].repeticoes;
        }
        //insercao de fato
        strncpy ( lista->cidades[pos].nome, nome, TAM_VAR);
        lista->cidades[pos].repeticoes = 1;
        (lista->tamanho)++;
    }
    return;
}

//retorna ENCONTRADO caso removido com sucesso e NAO_ENCONTRADO caso nao encontrado
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
                strncpy(lista->cidades[i-1].nome, lista->cidades[i].nome, TAM_VAR);
                lista->cidades[i-1].repeticoes = lista->cidades[i].repeticoes;
            }
            (lista->tamanho)--;
        }
        return ENCONTRADO;
    }
}

//Retorna 0 se funcionar e 1 se falha ao abrir arquivo
int grava_arquivo(char *nome, struct lista *lista){
    FILE *fp;
    if ( (fp = fopen(nome, "wb")) == NULL) return 1;
    grava_header(fp, 0);
    grava_registros(fp, lista);
    grava_header(fp, 1);
    fclose(fp);
    return 0;
}

//TROCAR ISSO POR UM ENUM
//status = 0 indica inconsistente e status = 1 indica consistente
void grava_header(FILE *fp, char status){
    fseek(fp, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, fp);
}

void grava_registros(FILE *fp, struct lista *lista){
    int i, j;
    int len;
    //Para toda cidade
    for(i=0; i < lista->tamanho; i++){
        //Grava nome
        len = strnlen(lista->cidades[i].nome, TAM_VAR);
        fwrite(lista->cidades[i].nome, sizeof(char), len, fp);
        for(j=0; j < TAM_VAR-len; j++){
            fwrite(LIXO_STR, sizeof(char), 1, fp);  //PODE SER OTIMIZADO?
        }
        //Grava numero de repeticoes
        fwrite( &(lista->cidades[i].repeticoes), sizeof(int), 1, fp);
    }
}

//Retorna 0 se funcionar e 1 se falha ao abrir arquivo
int le_arquivo(char *nome, struct lista *lista){
    FILE *fp;
    if ( (fp = fopen(nome, "rb")) == NULL) return 1;
    if( le_header(fp) != 1){
        return 1;  //Verificar se o arquivo esta valido
        fclose(fp);
    }
    le_registros(fp, lista);
    fclose(fp);
    return 0;
}

//TROCAR ISSO POR UM ENUM
//retorna status
//status = 0 indica inconsistente e status = 1 indica consistente
char le_header(FILE *fp){
    char status;
    return fread(&status, sizeof(char), 1, fp);
}

//Recebe lista inicializada e retorna com os registros do arquivo
void le_registros(FILE *fp, struct lista *lista){
    int i;
    char nome[TAM_VAR];
    for(i=0; 1;i++){
        fread(nome, sizeof(char), TAM_VAR, fp);
        if(feof(fp)) break;
        sscanf(nome, TAM_VAR_SCANF, lista->cidades[i].nome);
        fread( &(lista->cidades[i].repeticoes), sizeof(int), 1, fp);
    }
}
