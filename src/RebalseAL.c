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

    // 'cantidad' contiene la cantidad de elementos de la estructura
    ral->cantidad = 0;
}


// Localizar
int RAL_localizar(RebalseAL *ral, char codigo_envio[], int *posicion, int *baldes_consultados)
{
    int ubicacion = hashing(codigo_envio, RAL_M), contador = 0, primer_libre = -1;

    while (strcmpi(ral->arreglo[ubicacion], "VIRGEN") != 0 &&
           strcmpi(ral->arreglo[ubicacion], codigo_envio) != 0) {
                // se guarda la posicion de la primera celda libre encontrada
                if (strcmpi(ral->arreglo[ubicacion], "LIBRE") == 0
                   && primer_libre == -1) primer_libre = ubicacion;
               // se calcula el siguiente
               ubicacion = (ubicacion + 1) % RAL_M;
               contador++;
           }

    *baldes_consultados = contador;

    // se devuelve la posicion por parametro
    // (la variable contiene el INDICE correspondiente del elemento en el arreglo)
    if (primer_libre > -1) *posicion = primer_libre;
    else *posicion = ubicacion;

    if (strcmpi(ral->arreglo[ubicacion]->codigo_envio, "VIRGEN") != 0) {
        // se suma la ultima celda consultada
        // (en este caso, distinta de VIRGEN)
        (*baldes_consultados)++;

        return LOCALIZACION_EXITOSA;
    } else {
        return LOCALIZACION_ERROR_NO_EXISTE;
    }
}

int RAL_alta(RebalseAL *ral, Envio *envio, Costos_estructura *costos) {
    int posicion, celdas_consultadas;

    // si la estructura NO esta llena
    if (ral->cantidad < RAL_M) {
        if (RAL_localizar(ral, envio->codigo_envio, &posicion, &celdas_consultadas)
            == LOCALIZACION_ERROR_NO_EXISTE) {

                // se guarda el envio en la estructura
                ral->arreglo[posicion] = *envio;
                (ral->cantidad)++;  // aumenta la cantidad de elementos en la RAL

                // se guarda el costo de la alta
                // NOTA: aunque el laboratorio no pide los costos de Alta y de Baja, ya que estos
                // son siempre constantes, estos se guardan para mantener la uniformidad de funcionamiento
                // con los trabajos anteriores
                (costos->Alta.cantidad)++;
                // SUMATORIA_VECTOR += 0, suponiendo que se observara las celdas desplazadas, pero
                // dicha variable sigue igual al sumarle cero, por lo que sigue igual

                // el MAXIMO tampoco modifica, ya que se inicia en cero, y el costo de este rebalse nunca puede superar
                // ese valor
            }
        else return ALTA_ERROR_CODIGO_EXISTENTE;
    }
    else return ALTA_ERROR_LISTA_LLENA;
}

int RAL_baja(RebalseAL *ral, Envio *envio, Costos_estructura *costos) {
    int posicion, celdas_consultadas;

    // si la estructura NO esta vacia
    if (ral->cantidad > 0) {
        if (RAL_localizar(ral, envio->codigo_envio, &posicion, &celdas_consultadas)
            == LOCALIZACION_EXITOSA) {

                // confirmacion de baja por comparacion de campo por campo
                if (Envio_sonIguales(ral->arreglo[posicion], envio)) {

                    // se vuelve los valores de ENVIO a default
                    Envio_init(ral->arreglo[posicion]);
                    // luego se marca como libre
                    strcpy(ral->arreglo[posicion].codigo_envio,"LIBRE");

                    // se reduce la cantidad
                    (ral->cantidad)--;

                    return BAJA_EXITOSA;
                }
                else return BAJA_CANCELADA;
            }
    }

    return return BAJA_ERROR_NO_EXISTE;
}

#endif // LISTASO_H
