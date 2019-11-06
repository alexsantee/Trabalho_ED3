#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "helper.h"
#include "functions.h"

/*
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

/*
    Função que lê um registro do arquivo binario passado atraves
    do ponteiro FILE e o salva na struct registro passada como
    parâmetro.
*/

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

    return 1;
}

/*
    Função que lê um campo de tamanho variável
    e retorna 1 se a leitura for bem sucedida
    ou 0 caso não consiga ler.
*/

int leCampoVar(FILE * fp, char *str)
{
    char c;
    int i = 0;
    
    if(fread(&c, sizeof(char), 1, fp) == 0)
        return 0;
    
    while(c != SEPARADOR_CAMPO)
    {
        str[i] = c;
        i++;
        fread(&c, sizeof(char), 1, fp);
    }

    return 1;
}

/*
    Função que realiza uma busca no arquivo binario passado
    pelo ponteiro FILE, relativa a um determinado campo, e
    retorna -1 caso termine o arquivo, -2 caso não encontre,
    ou o RRN correspondente caso encontre o campo buscado.
*/

int buscaporCampo(FILE *fp, char * nomecamp, char * buscado, struct registro *reg)
{
    char str[TAM_VAR] = "";
    long ini = ftell(fp);
    int RRN = ini/TAMANHO_REGISTRO;

    if(strcmp(nomecamp, "estadoOrigem") == 0)
    {
        if(fseek(fp, ESTADO_ORIG, SEEK_CUR) != 0)
            return -1;

        if(fread(str, sizeof(char), 2, fp) == 0)
            return -1;

        fseek(fp, ini, SEEK_SET);

        if(strcmp(str, buscado) == 0)
        {
            leregbin(fp, reg);
            fseek(fp, (ini + TAMANHO_REGISTRO), SEEK_SET);
            return RRN;
        }
        else
        {
            if(fseek(fp, (ini + TAMANHO_REGISTRO), SEEK_SET) != 0)
                return -1;
            return -2;
        }
    }

    if(strcmp(nomecamp, "estadoDestino") == 0)
    {
        if(fseek(fp, ESTADO_DEST, SEEK_CUR) != 0)
            return -1;

        if(fread(str, sizeof(char), 2, fp) == 0)
            return -1;

        fseek(fp, ini, SEEK_SET);

        if(strcmp(str, buscado) == 0)
        {
            leregbin(fp, reg);
            fseek(fp,(ini + TAMANHO_REGISTRO), SEEK_SET);
            return RRN;
        }
        else
        {
            if(fseek(fp,(ini + TAMANHO_REGISTRO), SEEK_SET) != 0)
                return -1;
            return -2;
        }
    }

    if(strcmp(nomecamp, "distancia") == 0)
    {
        int numb = atoi(buscado);
        int num;
        if(fseek(fp, DISTANCIA, SEEK_CUR) != 0)
            return -1;

        if(fread(&num, sizeof(int), 1, fp) == 0)
            return -1;

        fseek(fp, ini, SEEK_SET);

        if(num == numb)
        {
            leregbin(fp, reg);
            fseek(fp,(ini + TAMANHO_REGISTRO), SEEK_SET);
            return RRN;
        }
        else
        {
            if(fseek(fp,(ini + TAMANHO_REGISTRO), SEEK_SET) != 0)
                return -1;
            return -2;
        }
    }

    if(strcmp(nomecamp, "cidadeOrigem") == 0)
    {
        if(fseek(fp, CIDADE_ORIG, SEEK_CUR) != 0)
            return -1;
        
        if(!leCampoVar(fp, str))
            return -1;

        fseek(fp, ini, SEEK_SET);

        if(strcmp(str, buscado) == 0)
        {
            leregbin(fp, reg);
            fseek(fp, (ini + TAMANHO_REGISTRO), SEEK_SET);
            return RRN;
        }
        else
        {
            if(fseek(fp,(ini + TAMANHO_REGISTRO), SEEK_SET) != 0)
                return -1;
            return -2;
        }
    }

    if(strcmp(nomecamp, "cidadeDestino") == 0)
    {
        char lixo[TAM_VAR];
        
        if(fseek(fp, CIDADE_ORIG, SEEK_CUR) != 0)
            return -1;
        
        if(!leCampoVar(fp, lixo))
            return -1;

        if(!leCampoVar(fp, str))
            return -1;

        fseek(fp, ini, SEEK_SET);

        if(strcmp(str, buscado) == 0)
        {
            leregbin(fp, reg);
            fseek(fp, (ini+TAMANHO_REGISTRO), SEEK_SET);
            return RRN;
        }
        else
        {
            if(fseek(fp, (ini + TAMANHO_REGISTRO) , SEEK_SET) != 0)
                return -1;
            
            return -2;
        }
    }

    if(strcmp(nomecamp, "tempoViagem") == 0)
    {
        char lixo[TAM_VAR];
        
        if(fseek(fp, CIDADE_ORIG, SEEK_CUR) != 0)
            return -1;

        if(!leCampoVar(fp, lixo))
            return -1;

        if(!leCampoVar(fp, lixo))
            return -1;

        if(!leCampoVar(fp, str))
            return -1;

        fseek(fp, ini, SEEK_SET);

        if(strcmp(str, buscado) == 0)
        {
            leregbin(fp, reg);
            fseek(fp, (ini + TAMANHO_REGISTRO), SEEK_SET);
            return RRN;
        }
        else
        {
            if(fseek(fp, (ini + TAMANHO_REGISTRO), SEEK_SET) != 0)
                return -1;

            return -2;
        }
    }

    return -1;
}

/*
    Função que busca um determinado registro
    identificado pelo RRN passado como parâmetro
    no arquivo binario passado pelo ponteiro FILE.
    Retorna 0 se não achar e 1 se achar.
*/

int buscaRRN(FILE *fp, int RRN, struct registro *reg)
{
    fseek(fp, RRN*TAMANHO_REGISTRO, SEEK_SET);
    if(!leregbin(fp, reg) || (reg->estadoOrigem[0] == INDICA_REMOVIDO))
    {
        printf("Registro inexistente.");
        return 0;
    }
    else
    {
        return 1;
    }
}

void preenche_cabecalho(struct cabecalho * cab, FILE * arq)
{
    fseek(arq,0,SEEK_SET);
    fwrite("0",sizeof(char),1,arq);
    fwrite(&(cab->numeroVertices), sizeof(int),1,arq);
    fwrite(&(cab->numeroArestas),sizeof(int),1,arq);
    for(int i = 0; i < 10; i++)
        {
            fwrite(&(cab->dataUltimaCompactacao[i]),sizeof(char),1,arq);
        }
}