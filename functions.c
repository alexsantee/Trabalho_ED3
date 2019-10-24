#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "helper.h"
#include "functions.h"
/*
    OBSERVAÇÃO IMPORTANTE
    Essa função serve para pegar um registro do arquivo, porém não leva
    em consideração a linha inicial, dessa forma é necessário removê-la
    antes de utilizar essa função
*/

void leregistro(FILE * fp, struct registro * reg)
{
    char lixo = ' ';

    fscanf(fp,"%[^,]", reg->estadoOrigem);
    fscanf(fp,"%c", &lixo);
    fscanf(fp,"%[^,]", reg->estadoDestino);
    fscanf(fp,"%c", &lixo);
    fscanf(fp,"%d", &reg->distancia);
    fscanf(fp,"%c", &lixo);
    fscanf(fp,"%[^,]", reg->cidadeOrigem);
    fscanf(fp,"%c", &lixo);
    fscanf(fp,"%[^,]", reg->cidadeDestino);
    fscanf(fp,"%c", &lixo);
    fscanf(fp,"%[^\n]", reg->tempoViagem);
    fscanf(fp,"%c", &lixo);
}
