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
    int i = 0;
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