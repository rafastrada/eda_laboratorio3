#include <stdlib.h>
#include <string.h>
#include "RebalseAL.h"
#include "Hashing.h"
#include "Codigos_retornos.h"

//VIRGEN - OCUPADO (dato) - LIBRE

void RAL_init(RebalseAL *ral)
{
    for(int i = 0; i < RAL_M; i++)
        strcpy(ral->arreglo[i]->codigo_envio, "VIRGEN");
}


// Localizar
int RAL_localizar(RebalseAL *ral, char codigo_envio[], int *posicion, int *baldes_consultados)
{
    int ubicacion = hashing(codigo_envio), contador = 0;

    while (strcmpi(ral->arreglo[ubicacion], "VIRGEN") != 0 &&
           strcmpi(ral->arreglo[ubicacion], codigo_envio) != 0) {
               ubicacion = (ubicacion + 1) % RAL_M;
               contador++;
           }

    *baldes_consultados = contador;

    // se devuelve la posicion por parametro
    // (la variable contiene el INDICE correspondiente del elemento en el arreglo)
    *posicion = ubicacion;

    if (strcmpi(ral->arreglo[ubicacion]->codigo_envio, "VIRGEN") != 0) {
        // se suma la ultima celda consultada
        // (en este caso, distinta de VIRGEN)
        (*baldes_consultados)++;

        return LOCALIZACION_EXITOSA;
    } else {
        return LOCALIZACION_ERROR_NO_EXISTE;
    }
}

#endif // LISTASO_H
