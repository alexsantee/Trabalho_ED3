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
    }
    return 1;
}