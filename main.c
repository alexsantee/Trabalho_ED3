#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "helper.h"
#include "functions.h"
#include "funcionalidades.h"

int main()
{
    int op;
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
        scan_quote_string(valor);
        funcionalidade3(nomeBIN, nomecampo, valor);
    }
    return 1;
}