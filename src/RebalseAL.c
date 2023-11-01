#include <stdlib.h>
#include <string.h>
#include "RebalseAL.h"
#include "Hashing.h"
#include "Codigos_retornos.h"

//VIRGEN - OCUPADO (dato) - LIBRE

void RAL_init(RebalseAL *ral)
{
    for(int i = 0; i < RAL_M; i++)
        strcpy(ral->arreglo[i].codigo_envio, "VIRGEN");

    // 'cantidad' contiene la cantidad de elementos de la estructura
    ral->cantidad = 0;
}


// Localizar
int RAL_localizar(RebalseAL *ral, char codigo_envio[], int *posicion, int *celdas_consultadas)
{
    int ubicacion = hashing(codigo_envio, RAL_M), contador = 0, primer_libre = -1;

    while (contador < RAL_M &&
           strcmpi(ral->arreglo[ubicacion].codigo_envio, "VIRGEN") != 0 &&
           strcmpi(ral->arreglo[ubicacion].codigo_envio, codigo_envio) != 0) {
                // se guarda la posicion de la primera celda libre encontrada
                if (strcmpi(ral->arreglo[ubicacion].codigo_envio, "LIBRE") == 0
                   && primer_libre == -1) primer_libre = ubicacion;
               // se calcula el siguiente
               ubicacion = (ubicacion + 1) % RAL_M;
               contador++;
           }

    // se suma la ultima celda consultada
    if (contador < RAL_M) contador++;

    *celdas_consultadas = contador;

    // se devuelve la posicion por parametro
    // (la variable contiene el INDICE correspondiente del elemento en el arreglo)
    if (primer_libre > -1) *posicion = primer_libre;
    else *posicion = ubicacion;

    if (strcmpi(ral->arreglo[ubicacion].codigo_envio, codigo_envio) == 0) return LOCALIZACION_EXITOSA;
    else return LOCALIZACION_ERROR_NO_EXISTE;
}

int RAL_alta(RebalseAL *ral, Envio *envio, Costos_estructura *costos) {
    int posicion, celdas_consultadas;

    // si la estructura NO esta llena
    // (se deja una celda para al menos una marca de fin 'VIRGEN')
    if (ral->cantidad < RAL_M ) {
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
                if (Envio_sonIguales(&(ral->arreglo[posicion]), envio)) {

                    // se vuelve los valores de ENVIO a default
                    Envio_init(&(ral->arreglo[posicion]));
                    // luego se marca como libre
                    strcpy(ral->arreglo[posicion].codigo_envio,"LIBRE");

                    // se reduce la cantidad
                    (ral->cantidad)--;

                    // se guarda el costo de la operacion
                    (costos->Baja.cantidad)++;

                    return BAJA_EXITOSA;
                }
                else return BAJA_CANCELADA;
            }
    }

    return BAJA_ERROR_NO_EXISTE;
}

int RAL_evocar(RebalseAL *ral, char codigo_envio[], Envio *envio, Costos_estructura *costos) {
    int posicion, celdas_consultadas;

    if (RAL_localizar(ral, codigo_envio, &posicion, &celdas_consultadas) == LOCALIZACION_EXITOSA) {
        // se copia los datos de envio
        *envio = ral->arreglo[posicion];

        // se guardan los costos
        (costos->Evocacion_exitosa.cantidad)++;
        costos->Evocacion_exitosa.sumatoria_vector += celdas_consultadas;
        if (costos->Evocacion_exitosa.maximo < celdas_consultadas) costos->Evocacion_exitosa.maximo = celdas_consultadas;

        return EVOCAR_EXITOSO;
    }
    else {
        // se guarda el costo de evocacion fallida
        (costos->Evocacion_fallida.cantidad)++;
        costos->Evocacion_fallida.sumatoria_vector += celdas_consultadas;
        if (costos->Evocacion_fallida.maximo < celdas_consultadas) costos->Evocacion_fallida.maximo = celdas_consultadas;

        return EVOCAR_NO_EXISTE;
    }
}

void RAL_mostrarLista(RebalseAL *ral) {
    for (int i = 0; i < RAL_M; i++){
        printf("\nPOSICION %d\n",i);
        if(strcmpi(ral->arreglo[i].codigo_envio, "VIRGEN") == 0){
            printf("CELDA VIRGEN\n\n");
        }else{
            if (strcmpi(ral->arreglo[i].codigo_envio, "LIBRE") == 0){
                printf("CELDA LIBRE\n");
            }else{
                Envio_imprimir(ral->arreglo[i]);
                }
            }
        // muestra de a 5 elementos en la lista
        if (((i+1) % 5) == 0) system("pause");
    }
}
