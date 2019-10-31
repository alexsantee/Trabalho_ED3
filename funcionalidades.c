#include <stdio.h>
#include <stdlib.h>
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
}

void funcionalidade2(char * nomebin)
{
    struct registro * reg = (struct registro *)calloc(1,sizeof(struct registro));
    int i = 0;
    
    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no carregamento do arquivo.");
        return;
    }

    while(leregbin(fp, reg))
    {
        printf("%d %s %s %d %s %s %s\n", i, reg->estadoOrigem, reg->estadoDestino, reg->distancia, reg->cidadeOrigem, reg->cidadeDestino, reg->tempoViagem);
        i++;
        free(reg);
        reg = (struct registro *)calloc(1,sizeof(struct registro));
    }
}

void funcionalidade3(char * nomebin, char * nomecampo, char * buscado)
{
    int rrn = 0;
    int flag;
    struct registro *reg = (struct registro *)calloc(1,sizeof(struct registro));

    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no carregamento do arquivo.");
        return;
    }

    flag = buscaporCampo(fp, nomecampo, buscado, reg);
    while(flag != -1)
    {
        if(flag == 1)
             printf("%d %s %s %d %s %s %s\n", rrn, reg->estadoOrigem, reg->estadoDestino, reg->distancia, reg->cidadeOrigem, reg->cidadeDestino, reg->tempoViagem);
        free(reg);
        reg = (struct registro *)calloc(1,sizeof(struct registro));
        flag = buscaporCampo(fp, nomecampo, buscado, reg);
        rrn++;
    }
}