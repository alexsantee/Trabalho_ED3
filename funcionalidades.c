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
    char * str = (char*)calloc(90, sizeof(char)); //DE ONDE VEIO ESSE 90? USAR CONSTANTES DO CONSTANTS.H
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

    preenche_cabecalho(cab, destino); // CABECALHO DEVE SER DETERMINADO AINDA

    fgets(str, 90, origem); //90 APARECE AQUI TAMBEM
    while(leregistro(origem, reg))
    {   
        fseek(destino, (i * TAMANHO_REGISTRO) + TAMANHO_CABECALHO, SEEK_SET);
        escreve_registro(destino, reg);
        free(reg);  //USO DESNECESSARIO DO FREE
        reg = (struct registro *)calloc(1,sizeof(struct registro));
    }

    fseek(destino,0,SEEK_SET);
    fwrite("1", sizeof(char),1,destino);
    
    //FALTOU DAR FREE EM REG E CAB NO FINAL
    //EH REALMENTE NECESSARIO USAR ALOCACAO DINAMICA?
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
    struct registro * reg = (struct registro *)calloc(1,sizeof(struct registro));
    
    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }

    int RRN;
    for(RRN = 0; leregbin(fp, reg); RRN++)
    {
        if(reg->estadoOrigem[0] != INDICA_REMOVIDO)
            //ESSE PRINTF FOI USADO VARIAS VEZES, NAO SERIA BOM FAZER UMA FUNCAO SEPARADA?
            printf("%d %s %s %d %s %s %s\n", RRN, reg->estadoOrigem, reg->estadoDestino, reg->distancia, reg->cidadeOrigem, reg->cidadeDestino, reg->tempoViagem);
        free(reg);  //USO DESNECESSARIO DO FREE
        reg = (struct registro *)calloc(1,sizeof(struct registro));
    }

    //FALTOU FREE FINAL
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
    //AQUI -1 APARECE COMO UM NUMERO MAGICO! SERIA BOM DEFINIR UMA CONSTANTE
    //COM NOME EXPLICATIVO (FIM_BUSCA?) E DEFINI-LA COMO -1 PARA MELHORAR A
    //LEGIBILIDADE
    while(rrn != -1)
    {
        if(rrn >= 0 && (reg->estadoOrigem[0] != INDICA_REMOVIDO))
        {
                printf("%d %s %s %d %s %s %s\n", rrn, reg->estadoOrigem, reg->estadoDestino, reg->distancia, reg->cidadeOrigem, reg->cidadeDestino, reg->tempoViagem);
                achou = 1;
        }
        free(reg);  //FREE DESNECESSARIO
        reg = (struct registro *)calloc(1,sizeof(struct registro));
        rrn = buscaporCampo(fp, nomecampo, buscado, reg);
    }

    if(!achou)
        printf("Registro Inexistente.");
    
    //FALTOU FREE FINAL
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

    //FALTOU FREE FINAL
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
