#ifndef __CONTAGEM_H__
#define __CONTAGEM_H__

#define TAM_MAX_LISTA 1000

struct cidade{
    char nome[TAM_VAR];
    int repeticoes;
};

struct lista{
    struct cidade cidades[TAM_MAX_LISTA];
    int tamanho;
};

void inicializa_lista(struct lista *lista);
//Retorna 0 se encontrou e NAO_ENCONTRADO caso contrario,
//Retorna na variavel pos a posicao da cidade ou a melhor posicao para inserir
int busca_cidade(char *nome, int *pos ,struct lista *lista);
void insere_cidade(char *nome, struct lista *lista);
int remove_cidade(char *nome, struct lista *lista);

int grava_arquivo(char *nome, struct lista *lista);
void grava_header(FILE *fp, char status);
void grava_registros(FILE *fp, struct lista *lista);

int le_arquivo(char *nome, struct lista *lista);
char le_header(FILE *fp);
void le_registros(FILE *fp, struct lista *lista);

#endif
