#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

const char SEPARADOR_CAMPO = '|';
const char SEPARADOR_REGISTRO = '#';
const char LIXO = '#';
const char VIRGULA = ',';

//REGISTROS

//Struct para guardar registro hobrido em RAM
struct registro{
    //Tamanho fixo
    char estadoOrigem[3];  //2 chars + \0
    char estadoDestino[3];
    unsigned int distancia;
    //Tamanho variavel (registro hibrido)
    char cidadeOrigem[77];  //76 reservado para tamanho fixo + \0
    char cidadeDestino[77];
    char tempoViagem[77];
    //Auxiliar (nao eh gravado em disco)
    //unsigned int RRN; //TALVEZ SEJA NECESSARIO
};
//Tamanho no arquivo
const unsigned int TAMANHO_REGISTRO = 85;
//Define as posicoes de cada campo dos registros no arquivo
enum campo {
    //Acesso direto
    ESTADO_ORIG = 0, //2 bytes
    ESTADO_DEST = 2, //2 bytes
    DISTANCIA = 4,   //4 bytes
    //No epaco de campos de tamanho variavel
    CIDADE_ORIG = 8,
    CIDADE_DEST,
    TEMPO_VIAJEM
};

//CABECALHO

//Struct para facilitar manuseio do cabecalho
struct cabecalho{
    char status; //Booleano de validade do arquivo
    int numeroVertices;
    int numeroArestas;
    char dataUltimaCompactacao[11]; //10 chars + \0
};
//Tamanho no arquivo
const unsigned int TAMANHO_CABECALHO = 19;
//Define as posicoes de cada campo do cabecalho no arquivo
enum campoH {
    STATUS = 0,             // 1 byte
    NUMERO_VERTICES = 1,    // 4 bytes
    NUMERO_ARESTAS = 5,     // 4 bytes
    ULTIMA_COMPILACAO = 9   // 10 bytes
};

#endif
