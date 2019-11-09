#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "helper.h"
#include "functions.h"
#include "funcionalidades.h"

int main()
{
    int op;
    int num;
    char nomeCSV[77];
    char nomeBIN[77];
    char nomecampo[77];
    char valor[77];
    scanf("%d", &op);

    switch (op)
    {
    case 0:
        return 1;
        break;
    
    case 1:
        scanf("%s", nomeCSV);
        scanf("%s", nomeBIN);
        funcionalidade1(nomeCSV, nomeBIN);
        break;

    case 2:
        scanf("%s", nomeBIN);
        funcionalidade2(nomeBIN);
        break;

    case 3:
        scanf("%s", nomeBIN);
        scanf("%s", nomecampo);
        if(strcmp(nomecampo, "distancia") == 0)
            scanf("%s", valor);
        else
            scan_quote_string(valor);
        funcionalidade3(nomeBIN, nomecampo, valor);
        break;

    case 4:
        scanf("%s", nomeBIN);
        scanf("%d", &num);
        funcionalidade4(nomeBIN, num);
        break;

    case 5:
        scanf("%s", nomeBIN);
        scanf("%d", &num);
        for(int i = 0; i < num; i++)
            funcionalidade5(nomeBIN);
        binarioNaTela1(nomeBIN);
        break;

    case 6:
        scanf("%s", nomeBIN);
        scanf("%d", &num);
        funcionalidade6(nomeBIN, num);
        break;

    case 7:
        scanf("%s", nomeBIN);
        scanf("%d", &num);
        funcionalidade7(nomeBIN, num);
        break;

    case 8:
        scanf("%s", nomeBIN);
        scanf("%s", nomeCSV);
        funcionalidade8(nomeBIN, nomeCSV);
        break;
    }
    return 0;
}
