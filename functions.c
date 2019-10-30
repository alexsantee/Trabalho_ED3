#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "helper.h"
#include "functions.h"

/*
    OBSERVAÇÃO IMPORTANTE
    Essa função serve para pegar um registro do arquivo, porém não leva
    em consideração a linha inicial, dessa forma é necessário removê-la
    antes de utilizar essa função
*/

int leregistro(FILE * fp, struct registro * reg)
{
    if(fscanf(fp," %[^,]%*c", reg->estadoOrigem) == EOF)
        return 0;
    fscanf(fp,"%[^,]%*c", reg->estadoDestino);
    fscanf(fp,"%d%*c", &reg->distancia);
    fscanf(fp,"%77[^,]%*c", reg->cidadeOrigem);
    fscanf(fp,"%77[^,]%*c", reg->cidadeDestino);
    fscanf(fp,"%77[^\n]%*c", reg->tempoViagem);

    return 1;
}

/*
    Função que recebe um registro lido do arquivo .csv e a escreve
    no arquivo binário de dados. (Usar calloc please)
*/

void escreve_registro(FILE * fp, struct registro * reg)
{
    int i, j = 0, k;
    int ini, fim;

    ini = ftell(fp);

    for(i = 0; i < (sizeof(reg->estadoOrigem) - 1); i++)
        fwrite(&(reg->estadoOrigem[i]), sizeof(char), 1, fp);

    for(i = 0; i < (sizeof(reg->estadoDestino) - 1); i++)
        fwrite(&(reg->estadoDestino[i]), sizeof(char), 1, fp);

    fwrite(&(reg->distancia), sizeof(int), 1, fp);

    while (reg->cidadeOrigem[j] != '\0')
    {
        fwrite(&(reg->cidadeOrigem[j]), sizeof(char), 1, fp);
        j++;
    }

    fwrite(&(SEPARADOR_CAMPO), sizeof(char), 1, fp);

    j=0;

    while (reg->cidadeDestino[j] != '\0')
    {
        fwrite(&(reg->cidadeDestino[j]), sizeof(char), 1, fp);
        j++;
    }

    fwrite(&(SEPARADOR_CAMPO), sizeof(char), 1, fp);

    j=0;

    while (reg->tempoViagem[j] != '\0')
    {
        fwrite(&(reg->tempoViagem[j]), sizeof(char), 1, fp);
        j++;
    }

    fwrite(&(SEPARADOR_CAMPO), sizeof(char), 1, fp);
    fwrite(&(SEPARADOR_REGISTRO), sizeof(char), 1, fp);

    fim = ftell(fp);

    k = fim - ini;

    while(k < TAMANHO_REGISTRO)
    {
        fwrite(&(LIXO), sizeof(char), 1, fp);
        k++;
    }
}

int leregbin(FILE *fp, struct registro * reg)
{
    char c = SEPARADOR_REGISTRO;
    int i = 0;
    
    while(c == SEPARADOR_REGISTRO)
        if(fread(&c, sizeof(char), 1, fp) == 0)
            return 0;
    
    reg->estadoOrigem[0] = c;
    fread(&(reg->estadoOrigem[1]), sizeof(char), 1, fp);
    fread(reg->estadoDestino, sizeof(char), 2, fp);
    fread(&(reg->distancia), sizeof(int), 1, fp);
    
    fread(&c, sizeof(char), 1, fp);
    while (c != SEPARADOR_CAMPO)
    {
        reg->cidadeOrigem[i] = c;
        fread(&c, sizeof(char), 1, fp);
        i++;
    }

    i = 0;
    fread(&c, sizeof(char), 1, fp);
    while (c != SEPARADOR_CAMPO)
    {
        reg->cidadeDestino[i] = c;
        fread(&c, sizeof(char), 1, fp);
        i++;
    }

    i = 0;
    fread(&c, sizeof(char), 1, fp);
    while(c != SEPARADOR_CAMPO)
    {
        reg->tempoViagem[i] = c;
        fread(&c, sizeof(char), 1, fp);
        i++;
    }
}
