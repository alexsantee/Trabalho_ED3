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
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    FILE * destino = fopen(nomebin, "wb");
    if(destino == NULL)
    {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    inicializa_lista(&list);
    
    strcpy((&cab)->dataUltimaCompactacao, DATA_INI);
    (&cab)->status = '0';
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
            printf("Falha no carregamento do arquivo.\n");
            return;
        }

    (&cab)->numeroArestas = i;
    (&cab)->numeroVertices = (&list)->tamanho;
    (&cab)->status = '1';
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
    char Status;
    struct registro reg;
    int i = 0;
    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fread(&Status, sizeof(char), 1, fp);
    if(Status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    fseek(fp, TAMANHO_CABECALHO, SEEK_SET);

    int RRN;
    for(RRN = 0; leregbin(fp, &reg); RRN++)
    {
        if(reg.estadoOrigem[0] != INDICA_REMOVIDO)
            {
                print_reg(RRN, &reg);
                i++;
            }
        limpa_reg(&reg);
    }

    if(i == 0)
    {
        printf("Registro inexistente.\n");
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
    char Status;
    int rrn = 0;
    int achou = 0;
    struct registro reg;

    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fread(&Status, sizeof(char), 1, fp);
    if(Status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    fseek(fp, TAMANHO_CABECALHO, SEEK_SET);

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
        printf("Registro inexistente.\n");
    
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
    char Status;
    struct registro reg;
    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fread(&Status, sizeof(char), 1, fp);
    if(Status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    fseek(fp, TAMANHO_CABECALHO, SEEK_SET);

    if(!buscaRRN(fp, rrn, &reg))
        {   
			fclose(fp);
            return;
        }
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

int funcionalidade5(char *nomebin) // CRIAR LISTA DE CIDADES A PARTIR DO ARQUIVO BINARIO
{
    char Status;
    char nomecampo[TAM_VAR];
    char valor[TAM_VAR];
    struct registro reg;
    struct lista list;
    struct cabecalho cab;
    int RRN;
    int aresta;
    
    FILE * fp = fopen(nomebin, "rb+");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    
    fread(&Status, sizeof(char), 1, fp);
    if(Status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    fseek(fp, STATUS, SEEK_SET);
    
    fwrite("0", sizeof(char), 1, fp);

    inicializa_lista(&list);
    le_arquivo(NOME_CITY, &list);
    fseek(fp, NUMERO_ARESTAS, SEEK_SET);
    fread(&aresta, sizeof(int), 1, fp);
    fseek(fp, STATUS, SEEK_SET);

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
            fseek(fp, ((RRN*TAMANHO_REGISTRO) + TAMANHO_CABECALHO), SEEK_SET);
            fwrite(&INDICA_REMOVIDO, sizeof(char), 1, fp);
            fseek(fp, (((RRN+1)*TAMANHO_REGISTRO) + TAMANHO_CABECALHO), SEEK_SET);
            aresta--;
            remove_cidade((&reg)->cidadeDestino, &list);
            remove_cidade((&reg)->cidadeOrigem, &list);
        }
        RRN = buscaporCampo(fp, nomecampo, valor, &reg);
    }

    fseek(fp, STATUS, SEEK_SET);
    strcpy((&cab)->dataUltimaCompactacao, DATA_INI);
    (&cab)->numeroArestas = aresta;
    (&cab)->numeroVertices = (&list)->tamanho;
    (&cab)->status = '1';
    preenche_cabecalho(&cab, fp);

    fclose(fp);
    return 1;
}

void funcionalidade6(char * arq, int n)
{
    struct registro reg;
    struct cabecalho cab;
    struct lista list;
    FILE * fp;
    char Status;
    int arestas;

    fp = fopen(arq, "rb+");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fread(&Status, sizeof(char), 1, fp);
    if(Status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(fp, NUMERO_ARESTAS, SEEK_SET);
    fread(&arestas, sizeof(int), 1, fp);

    fseek(fp, STATUS, SEEK_SET);
    fwrite("0", sizeof(char), 1, fp);
    fseek(fp, STATUS, SEEK_SET);

    inicializa_lista(&list);
    criaListaBin(&list, fp);

    fseek(fp, STATUS, SEEK_END);

    for(; n > 0; n--)
    {
        le_reg_do_teclado(&reg);

        verifica_leitura(&reg);

        escreve_registro(fp, &reg);

        if(strlen((&reg)->cidadeOrigem) != 0)
            insere_cidade((&reg)->cidadeOrigem, &list);

        if(strlen((&reg)->cidadeDestino) != 0)
            insere_cidade((&reg)->cidadeDestino, &list);

        limpa_reg(&reg);

        arestas++;
    }

    (&cab)->status = '1';
    strcpy((&cab)->dataUltimaCompactacao, DATA_INI);
    (&cab)->numeroArestas = arestas;
    (&cab)->numeroVertices = (&list)->tamanho;

    preenche_cabecalho(&cab, fp);

    fclose(fp);

    binarioNaTela1(arq);

    return;
}

void funcionalidade7(char *nomebin)
{
    //Abre o arquivo
    FILE * fp = fopen(nomebin, "rb+");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    //Verifica integridade do arquivo
    char Status;
    fseek(fp, STATUS, SEEK_SET);
    fread(&Status, sizeof(char), 1, fp);
    if(Status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(fp);
        return;
    }

    //Registra status do arquivo como erro
    fseek(fp, STATUS, SEEK_SET);
    fwrite("0", sizeof(char), 1, fp);

    //recebe entradas
    int RRN;
    char nomecampo[TAM_VAR];
    char valor[TAM_VAR];
    //le valor de RRN da entrada
    scanf(" %d", &RRN);
    //le nome e valor do campo a ser alterado
    scanf(" %s", nomecampo);
    if(strcmp(nomecampo, "distancia") == 0)
        scanf(" %s", valor);
    else
        scan_quote_string(valor);

    //enconta variavel e guarda valor original em reg
    struct registro reg;
    if(!buscaRRN(fp, RRN, &reg))
    {   
	    fseek(fp, STATUS, SEEK_SET);
    	fwrite("1", sizeof(char), 1, fp);
        fclose(fp);
        return;
    }
    //Substitui valor novo 
    modifica_reg(&reg, nomecampo, valor);
    //Reinsere no arquivo
    fseek(fp, ((RRN*TAMANHO_REGISTRO)+TAMANHO_CABECALHO), SEEK_SET);
    escreve_registro_sem_lixo(fp, &reg);
    
    //Registra status do arquivo como OK
    fseek(fp, STATUS, SEEK_SET);
    fwrite("1", sizeof(char), 1, fp);

    fclose(fp);
    return;
}


/*
void funcionalidade7(char * arq, int n)
{
    struct registro reg;
    struct cabecalho cab;
    struct lista list;
    FILE * fp;
    char Status;
    int arestas;
    int RRN;
    char Field[15];
    char newValue[TAM_VAR];

    fp = fopen(arq, "rb+");
    if(fp == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fread(&Status, sizeof(char), 1, fp);
    if(Status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(fp, NUMERO_ARESTAS, SEEK_SET);
    fread(&arestas, sizeof(int), 1, fp);

    fseek(fp, STATUS, SEEK_SET);
    fwrite("0", sizeof(char), 1, fp);

    for(; n > 0; n--)
    {
        scanf("%d", &RRN);
        scan_quote_string(Field);
        scan_quote_string(newValue);
        buscaRRN(fp, RRN, &reg);
        fseek(fp, ((RRN*TAMANHO_REGISTRO)+TAMANHO_CABECALHO), SEEK_SET);
        verifica_leitura_single(newValue);
        modifica_reg(&reg, Field, newValue);
        escreve_registro(fp, &reg);
        limpa_reg(&reg);
        zera_regt(Field);
        zera_regt(newValue);
    }

    (&cab)->status = '1';
    strcpy((&cab)->dataUltimaCompactacao, DATA_INI);
    (&cab)->numeroArestas = arestas;
    (&cab)->numeroVertices = (&list)->tamanho;

    preenche_cabecalho(&cab, fp);

    fclose(fp);

    binarioNaTela1(arq);

    return;
}
*/

void funcionalidade8(char * origem, char * destino)
{
    char Status;
    FILE * fp1 = fopen(origem, "rb");
    FILE * fp2 = fopen(destino, "wb");

    if(fp1 == NULL || fp2 == NULL)
    {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    fread(&Status, sizeof(char), 1, fp1);
    if(Status != '1')
    {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }
    fseek(fp1, STATUS, SEEK_SET);

    compacta_arq(fp1, fp2);

    remove(origem);

    binarioNaTela1(destino);
}

void funcionalidade9(char * nomebin)
{
    struct registro reg;
    struct cabecalho cab;
    int i = 0;
    FILE * fp = fopen(nomebin, "rb");
    if(fp == NULL)
    {
        printf("Falha na execução da funcionalidade.\n");
        return;
    }

    fread(&cab.status, sizeof(char), 1, fp);
    if(cab.status != '1')
    {
        printf("Falha na execução da funcionalidade.\n");
        return;
    }
    
    fread(&cab.numeroVertices, sizeof(int), 1, fp);
    fread(&cab.numeroArestas, sizeof(int), 1, fp);
    fread(cab.dataUltimaCompactacao, sizeof(char), 10, fp);

    struct grafo * grafo = calloc(cab.numeroVertices, sizeof(struct grafo));

    int RRN;
    for(RRN = 0; leregbin(fp, &reg); RRN++)
    {
        if(reg.estadoOrigem[0] != INDICA_REMOVIDO)
        {
            inserereggrafo(reg, grafo, cab.numeroVertices);
        }
        limpa_reg(&reg);
    }

    fclose(fp);
    print_grafo(grafo, cab.numeroVertices);
    return;
}