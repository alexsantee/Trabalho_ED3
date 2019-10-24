#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "helper.h"

//O NOME DO ARQUIVO FICOU MUITO GENERICO,
//SERIA BOM UM MAIS DESCRITIVO, COMO "OPERA_CSV"

//ACHO QUE ESSA FUNCAO PODE SER BASTANTE SIMPLIFICADA COM O USO DE FSCANF
//VISTO QUE O CSV FUNCIONA NO MODO TEXTO
//EH BOM OLHAR A DOCUMENTACAO DO SCANF SE PRECISAR DE ALGUMA COISA DIFERENTE
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
        //ESSA VARIAVEL CHAMADA VIRGULA NAO DIZ MUITO SOBRE O QUE ELA EH
        //PREFIRA NOMES COMO "SEPARADOR_CSV"
    }while(aux != VIRGULA || aux != '\n');    //Procura pelos caracteres virgula e quebra de linha que indicam o fim de um campo variável no csv

    if(i != 0)                                //Verifica se o campo lido é nulo e atribui null se sim, completa a string com \0 se não
        str[i] = '\0';
    else
        str = NULL;

    return;
}

//ESSA FUNCAO FAZ DUAS COISAS, ABRIR E LER O ARQUIVO.
//POR UMA QUESTAO DE MODULARIDADE EH BOM QUE SEJAM DUAS SEPARADAS.
void leregistro(char * nomearquivo, struct registro * reg)
{
    FILE *fp = fopen(nomearquivo, "rb");            //Abre o arquivo de nome passado como parâmetro
    
    //OS PARAMETROS DO FREAD SAO
    //fread(destino, tamanho, numero, arquivo)
    //O TAMANHO E O NUMERO ESTAO INVERTIDOS AQUI E TAMBEM
    //EXISTE HARDCODE DO TAMANHO, O MELHOR EH USAR SIZEOF
    //EX: fread(reg->campo, sizeof(char), num, fp)
    //
    //OUTRA ALTERNATIVA E USAR FSCANF NESSES DOIS PRIMEIROS,
    //UMA VEZ QUE ESTAMOS LENDO CHARS, ASSIM COMPLETAR COM
    //O '\0' FICA AUTOMATICO
    fread(reg->estadoOrigem, 2, 1, fp);             //Lê os valores de tamanho fixo
    reg->estadoOrigem[2] = '\0';                    //atribuindo aos respectivos campos 
    //NO ARQUIVO NAO HAVERAO '\0', VISTO QUE SABEMOS O TAMANHO DA STRING
    fread(NULL,1,1,fp);                             //completando com \0 e pulando a linha
                                                    //**
    fread(reg->estadoDestino, 2, 1, fp);            //**
    reg->estadoDestino[2] = '\0';                   //**
    fread(NULL,1,1,fp);                             //**
                                                    //**
    //NO FREAD DEVE SER PASSADO PONTEIRO PARA O DESTINO,
    //AQUI ESTA PASSANDO VALOR
    fread(reg->distancia, 4, 1, fp);                //**
    fread(NULL,1,1,fp);                             //**

    lecampovariavel(fp, reg->cidadeOrigem);         //Função que le os valores de tamanho variável
    lecampovariavel(fp, reg->cidadeDestino);        //e atribui aos campos passados como parâmetro

    //O ARQUIVO NAO FOI FECHADO!!!
}

int salva_registro(FILE * fp, struct registro * reg)
{
    int i, j = 0;
    int ini, fim;

    ini = ftell(fp);

    for(i = 0; i < sizeof(reg->estadoOrigem); i++)
        fwrite(reg->estadoOrigem[i], sizeof(char), 1, fp);
    
    for(i = 0; i < sizeof(reg->estadoDestino); i++)
        fwrite(reg->estadoDestino[i], sizeof(char), 1, fp);
    
    fwrite(reg->distancia, sizeof(int), 1, fp);

    do
    {   fwrite(reg->cidadeOrigem[j], sizeof(char), 1, fp);
    } while (reg->cidadeOrigem[j] != '\0' && ++j);

    fwrite(SEPARADOR_CAMPO, sizeof(char), 1, fp);

    j=0;

    do
    {   fwrite(reg->cidadeDestino[j], sizeof(char), 1, fp);
    }while (reg->cidadeDestino[j] != '\0' && ++j);

    fwrite(SEPARADOR_CAMPO, sizeof(char), 1, fp);

    j=0;

    do
    {   fwrite(reg->tempoViagem, sizeof(char), 1, fp);
    }while (reg->tempoViagem[j] != '\0' && ++j);

    fwrite(SEPARADOR_CAMPO, sizeof(char), 1, fp);
    fwrite(SEPARADOR_REGISTRO, sizeof(char), 1, fp);
    
    fim = ftell(fp);
    
    while((fim - ini) < TAMANHO_REGISTRO)
    {
        fwrite(LIXO, sizeof(char), 1, fp);
    }
}