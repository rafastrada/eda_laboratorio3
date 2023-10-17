#include "Hashing.h"

int hashing(char* x, int M)
{
    int longitud, i;
    int contador = 0;
    longitud = strlen(x);
    contador = 0;
    for(i = 0; i < longitud; i++)
        contador += ((int)x[i]) * (i+1));
    return (contador % M );
}
