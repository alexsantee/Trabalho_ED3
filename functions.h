#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

int leregistro(FILE *, struct registro *);

void escreve_registro(FILE *, struct registro *);

int leregbin(FILE *, struct registro *);

int leCampoVar(FILE *, char * string);

int buscaporCampo(FILE *, char * nomedocampo, char * buscado, struct registro *);

int buscaRRN(FILE *, int RRN, struct registro *);

void preenche_cabecalho(struct cabecalho *cab, FILE *);

void limpa_reg(struct registro *);

void print_reg(int RRN, struct registro *);

void compacta_arq(FILE *, FILE *);