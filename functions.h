#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

int leregistro(FILE *, struct registro *);

void escreve_registro(FILE *, struct registro *);

int leregbin(FILE *, struct registro *);

int leCampoVar(FILE *, char * string);

int buscaporCampo(FILE *, char * nomedocampo, char * buscado, struct registro *);