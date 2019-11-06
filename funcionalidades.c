#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "constants.h"
#include "helper.h"
#include "funcionalidades.h"
#include "contagem.h"

/*
    Essa funcionalidade recebe o nome do arquivo CSV
    e o nome do binario como parâmetros e apenas lê todos
    os registros contidos no CSV transcrevendo-os para
    o binário que será criado.
*/

void funcionalidade1(char * nomecsv, char * nomebin)
{
    int i = 0;
    struct cabecalho cab;
    struct registro reg;
    struct lista list;
    char * str = (char*)calloc(TAMANHO_REGISTRO, sizeof(char));
    FILE * origem = fopen(nomecsv, "rt");
    if(origem == NULL)
    {
        printf("Falha no carregamento do arquivo.");
        return;
    }

    FILE * destino = fopen(nomebin, "wb");
    if(destino == NULL)
    {
        printf("Falha no carregamento do arquivo.");
        return;
    }

    inicializa_lista(&list);
    
    strcpy((&cab)->dataUltimaCompactacao, "##########");
    (&cab)->status = "0";
    preenche_cabecalho(&cab, destino);

    fgets(str, TAMANHO_REGISTRO, origem);
    while(leregistro(origem, &reg))
    {   
        fseek(destino, (i * TAMANHO_REGISTRO) + TAMANHO_CABECALHO, SEEK_SET);
        insere_cidade((&reg)->cidadeDestino, &list);
        insere_cidade((&reg)->cidadeOrigem, &list);
        escreve_registro(destino, &reg);
        limpa_reg(&reg);
        i++;
    }

    if(grava_arquivo("Cities.bin", &list))
        {
            printf("Falha no carregamento do arquivo.");
            return;
        }

    (&cab)->numeroArestas = i;
    (&cab)->numeroVertices = (&list)->tamanho;
    (&cab)->status = "1";
    preenche_cabecalho((&cab),destino);
    
    fclose(origem);
    fclose(destino);

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
    struct registro reg;
    
    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }

    int RRN;
    for(RRN = 0; leregbin(fp, &reg); RRN++)
    {
        if(reg.estadoOrigem[0] != INDICA_REMOVIDO)
            print_reg(RRN, &reg);
        limpa_reg(&reg);
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
    struct registro reg;

    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }

    rrn = buscaporCampo(fp, nomecampo, buscado, &reg);
    while(rrn != FIM_BUSCA)
    {
        if(rrn >= 0 && (reg.estadoOrigem[0] != INDICA_REMOVIDO))
        {
                print_reg(rrn, &reg);
                achou = 1;
        }
        limpa_reg(&reg);
        rrn = buscaporCampo(fp, nomecampo, buscado, &reg);
    }

    if(!achou)
        printf("Registro Inexistente.");
    
    fclose(fp);
    return;
}

/*
    Essa funcionalidade recebe o nome de um arquivo binario
    e um RRN de um registro que deseja ser buscado.
    Ela busca o registro no arquivo e o printa na tela
    se for encontrado.
*/

void funcionalidade4(char * nomebin, int rrn)
{
    struct registro reg;
    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }

    if(!buscaRRN(fp, rrn, &reg))
        return;
    else
        print_reg(rrn, &reg);  

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
    struct registro reg;
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
    RRN = buscaporCampo(fp, nomecampo, valor, &reg);
    while(RRN != -1)
    {
        if(RRN >= 0)
        {
            fseek(fp, RRN*TAMANHO_REGISTRO, SEEK_SET);
            fwrite(&INDICA_REMOVIDO, sizeof(char), 1, fp);
            fseek(fp, (RRN+1)*TAMANHO_REGISTRO, SEEK_SET);
        }
        RRN = buscaporCampo(fp, nomecampo, valor, &reg);
    }

    fclose(fp);

    binarioNaTela1(nomebin);

    return;
}
