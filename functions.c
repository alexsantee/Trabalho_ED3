#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "helper.h"

void lecampovariavel(FILE * fp, char * str)
{
    int i = -1;
    char aux;

    do
    {
        if(i != -1)
            str[i] = aux;                     //Atribui o ultimo caracter válido lido a string passada como parametro
        
        fread(&aux, 1, 1, fp);                //Lê um caracter do arquivo csv e atribui à um char auxiliar
        i++;
    }while(aux != VIRGULA || aux != '\n');    //Procura pelos caracteres virgula e quebra de linha que indicam o fim de um campo variável no csv

    if(i != 0)                                //Verifica se o campo lido é nulo e atribui null se sim, completa a string com \0 se não
        str[i] = '\0';
    else
        str = NULL;

    return;
}

void leregistro(char * nomearquivo, struct registro * reg)
{
    FILE *fp = fopen(nomearquivo, "rb");            //Abre o arquivo de nome passado como parâmetro

    fread(reg->estadoOrigem, 2, 1, fp);             //Lê os valores de tamanho fixo
    reg->estadoOrigem[2] = "\0";                    //atribuindo aos respectivos campos 
    fread(NULL,1,1,fp);                             //completando com \0 e pulando a linha
                                                    //**
    fread(reg->estadoDestino, 2, 1, fp);            //**
    reg->estadoDestino[2] = "\0";                   //**
    fread(NULL,1,1,fp);                             //**
                                                    //**
    fread(reg->distancia, 4, 1, fp);                //**
    fread(NULL,1,1,fp);                             //**

    lecampovariavel(fp, reg->cidadeOrigem);         //Função que le os valores de tamanho variável
    lecampovariavel(fp, reg->cidadeDestino);        //e atribui aos campos passados como parâmetro

}
