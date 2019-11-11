#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

int leregistro(FILE *, struct registro *);

void escreve_registro(FILE *, struct registro *);

void escreve_registro_sem_lixo(FILE *, struct registro *);

int leregbin(FILE *, struct registro *);

int leCampoVar(FILE *, char * string);

int buscaporCampo(FILE *, char * nomedocampo, char * buscado, struct registro *);

int buscaRRN(FILE *, int RRN, struct registro *);

void preenche_cabecalho(struct cabecalho *cab, FILE *);

void limpa_reg(struct registro *);

void print_reg(int RRN, struct registro *);

void compacta_arq(FILE *, FILE *);

void le_reg_do_teclado(struct registro *);

void verifica_leitura(struct registro *);

void modifica_reg(struct registro *, char*, char*);

void verifica_leitura_single(char*);

void zera_regt(char *);
