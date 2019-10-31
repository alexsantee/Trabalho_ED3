#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "constants.h"
#include "helper.h"
#include "funcionalidades.h"

void funcionalidade1(char * nomecsv, char * nomebin)
{
    struct registro * reg = (struct registro *)calloc(1,sizeof(struct registro));
    char * str = (char*)calloc(90, sizeof(char));
    FILE * fp = fopen(nomecsv, "rt");
    if(fp == NULL)
    {
        printf("Falha no carregamento do arquivo.");
        return;
    }

    FILE * fp2 = fopen(nomebin, "wb");
    if(fp2 == NULL)
    {
        printf("Falha no carregamento do arquivo.");
        return;
    }

    fgets(str, 90, fp);
    while(leregistro(fp, reg))
    {
        escreve_registro(fp2, reg);
        free(reg);
        reg = (struct registro *)calloc(1,sizeof(struct registro));
    }

    fclose(fp);
    fclose(fp2);

    binarioNaTela1(nomebin);
    return;
}

void funcionalidade2(char * nomebin)
{
    struct registro * reg = (struct registro *)calloc(1,sizeof(struct registro));
    int i = 0;
    
    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }

    while(leregbin(fp, reg))
    {
        printf("%d %s %s %d %s %s %s\n", i, reg->estadoOrigem, reg->estadoDestino, reg->distancia, reg->cidadeOrigem, reg->cidadeDestino, reg->tempoViagem);
        i++;
        free(reg);
        reg = (struct registro *)calloc(1,sizeof(struct registro));
    }

    fclose(fp);
    return;
}

void funcionalidade3(char * nomebin, char * nomecampo, char * buscado)
{
    int rrn = 0;
    struct registro *reg = (struct registro *)calloc(1,sizeof(struct registro));

    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }

    rrn = buscaporCampo(fp, nomecampo, buscado, reg);
    while(rrn != -1)
    {
        if(rrn >= 0)
             printf("%d %s %s %d %s %s %s\n", rrn, reg->estadoOrigem, reg->estadoDestino, reg->distancia, reg->cidadeOrigem, reg->cidadeDestino, reg->tempoViagem);
        free(reg);
        reg = (struct registro *)calloc(1,sizeof(struct registro));
        rrn = buscaporCampo(fp, nomecampo, buscado, reg);
    }

    fclose(fp);
    return;
}

void funcionalidade4(char * nomebin, int rrn)
{
    struct registro *reg = (struct registro *)calloc(1,sizeof(struct registro));
    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }

    if(!buscaRRN(fp, rrn, reg))
        return;
    else
        printf("%d %s %s %d %s %s %s\n", rrn, reg->estadoOrigem, reg->estadoDestino, reg->distancia, reg->cidadeOrigem, reg->cidadeDestino, reg->tempoViagem);  

    fclose(fp);
    return;     
}

void funcionalidade5(char *nomebin)
{
    char nomecampo[TAM_VAR];
    char valor[TAM_VAR];
    struct registro *reg = (struct registro *)calloc(1,sizeof(struct registro));
    int RRN;
    
    FILE * fp = fopen(nomebin, "rb+");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }

    scanf("%s", nomecampo);
    if(strcmp(nomecampo, "distancia") == 0)
        scanf("%s", valor);
    else
        scan_quote_string(valor);
    RRN = buscaporCampo(fp, nomecampo, valor, reg);
    while(RRN != -1)
    {
        if(RRN >= 0)
        {
            fseek(fp, RRN*TAMANHO_REGISTRO, SEEK_SET);
            fwrite(&INDICA_REMOVIDO, sizeof(char), 1, fp);
            fseek(fp, (RRN+1)*TAMANHO_REGISTRO, SEEK_SET);
        }
        RRN = buscaporCampo(fp, nomecampo, valor, reg);
    }

    fclose(fp);
    return;
}