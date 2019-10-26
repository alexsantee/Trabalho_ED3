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
    fscanf(fp,"%77[^,]", reg->cidadeOrigem);
    fscanf(fp,"%c", &lixo);
    fscanf(fp,"%77[^,]", reg->cidadeDestino);
    fscanf(fp,"%c", &lixo);
    fscanf(fp,"%77[^\n]", reg->tempoViagem);
    fscanf(fp,"%c", &lixo);

}

/*
    Reescreve no arquivo binário um registro lido de um arquivo .csv.
    Considera que as discrepâncias entre os registros (campos vazios 
    e afins) foram solucionadas por funções que a antecederam.
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

    do
    {   fwrite(&(reg->cidadeOrigem[j]), sizeof(char), 1, fp);
        j++;
    } while (reg->cidadeOrigem[j] != '\0');

    fwrite(&(SEPARADOR_CAMPO), sizeof(char), 1, fp);

    j=0;

    do
    {   fwrite(&(reg->cidadeDestino[j]), sizeof(char), 1, fp);
        j++;
    }while (reg->cidadeDestino[j] != '\0');

    fwrite(&(SEPARADOR_CAMPO), sizeof(char), 1, fp);

    j=0;

    do
    {   fwrite(&(reg->tempoViagem[j]), sizeof(char), 1, fp);
        j++;
    }while (reg->tempoViagem[j] != '\0');

    fwrite(&(SEPARADOR_CAMPO), sizeof(char), 1, fp);
    fwrite(&(SEPARADOR_REGISTRO), sizeof(char), 1, fp);

    fim = ftell(fp);

    k = fim - ini;

    while(k < TAMANHO_REGISTRO)
    {
        fwrite(&(LIXO), sizeof(char), 1, fp);
        k++;
    }

    return;
}