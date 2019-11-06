#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "constants.h"
#include "helper.h"
#include "funcionalidades.h"

/*
    Essa funcionalidade recebe o nome do arquivo CSV
    e o nome do binario como parâmetros e apenas lê todos
    os registros contidos no CSV transcrevendo-os para
    o binário que será criado.
*/

void funcionalidade1(char * nomecsv, char * nomebin)
{
    int i = 0;
    struct cabecalho * cab = (struct cabecalho *)calloc(1,sizeof(struct cabecalho));
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

    preenche_cabecalho(cab, fp2); // CABECALHO DEVE SER DETERMINADO AINDA

    fgets(str, 90, fp);
    while(leregistro(fp, reg))
    {   
        fseek(fp2, (i * TAMANHO_REGISTRO) + TAMANHO_CABECALHO, SEEK_SET);
        escreve_registro(fp2, reg);
        free(reg);
        reg = (struct registro *)calloc(1,sizeof(struct registro));
    }

    fseek(fp2,0,SEEK_SET);
    fwrite("1", sizeof(char),1,fp2);

    fclose(fp);
    fclose(fp2);

    binarioNaTela1(nomebin);
    return;
}

/*
    Essa funcionalidade recebe o nome de um arquivo binário
    como parâmetro e percorre todo o arquivo printando o conteúdo
    dos registros na tela, junto ao RRN correspondente de cada um deles
*/

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
        if(reg->estadoOrigem[0] != INDICA_REMOVIDO)
            printf("%d %s %s %d %s %s %s\n", i, reg->estadoOrigem, reg->estadoDestino, reg->distancia, reg->cidadeOrigem, reg->cidadeDestino, reg->tempoViagem);
        i++;
        free(reg);
        reg = (struct registro *)calloc(1,sizeof(struct registro));
    }

    fclose(fp);
    return;
}

/*
    Essa funcionalidade recebe o nome de um arquivo binario, o nome
    de um campo e o valor do mesmo e realiza uma busca por todo o
    arquivo binario por campos com o nome igual ao do valor especificado
    printando-os caso encontre.
*/

void funcionalidade3(char * nomebin, char * nomecampo, char * buscado)
{
    int rrn = 0;
    int achou = 0;
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
        if(rrn >= 0 && (reg->estadoOrigem[0] != INDICA_REMOVIDO))
        {
                printf("%d %s %s %d %s %s %s\n", rrn, reg->estadoOrigem, reg->estadoDestino, reg->distancia, reg->cidadeOrigem, reg->cidadeDestino, reg->tempoViagem);
                achou = 1;
        }
        free(reg);
        reg = (struct registro *)calloc(1,sizeof(struct registro));
        rrn = buscaporCampo(fp, nomecampo, buscado, reg);
    }

    if(!achou)
        printf("Registro Inexistente.");

    fclose(fp);
    return;
}

/*
    Essa funcionalidade recebe o nome de um arquivo binario
    e um RRN de um registro que deseja ser buscado.
    Ela busca o registro por todo o arquivo e o printa na tela
    se for encontrado.
*/

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

/*
    Essa funcionalidade recebe o nome de um arquivo binario,
    além de o nome de um campo e de um valor correspondente.
    A partir disso, são buscados os registros que contém o
    campo desejado com o valor igual ao buscado e exclui
    logicamente todos esses registros.
*/

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

    binarioNaTela1(nomebin);

    return;
}