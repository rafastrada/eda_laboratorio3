#include <stdlib.h>
#include <string.h>
#include "RebalseAC.h"
#include "Hashing.h"
#include "Codigos_retornos.h"

//VIRGEN - OCUPADO (dato) - LIBRE

void RAC_init(RebalseAC *rac)
{
    for(int i = 0; i < RAC_M; i++)
        strcpy(rac->arreglo[i].codigo_envio, "VIRGEN");

    // 'cantidad' contiene la cantidad de elementos de la estructura
    rac->cantidad = 0;
}


// Localizar
int RAC_localizar(RebalseAC *rac, char codigo_envio[], int *posicion, int *baldes_consultados)
{
    int ubicacion = hashing(codigo_envio, RAC_M), contador = 0, primer_libre = -1, paso = 1;

    while (contador < RAC_M &&
           strcmpi(rac->arreglo[ubicacion].codigo_envio, "VIRGEN") != 0 &&
           strcmpi(rac->arreglo[ubicacion].codigo_envio, codigo_envio) != 0) {
                // se guarda la posicion de la primera celda libre encontrada
                if (strcmpi(rac->arreglo[ubicacion].codigo_envio, "LIBRE") == 0
                   && primer_libre == -1) primer_libre = ubicacion;
               // se calcula el siguiente
               ubicacion = (ubicacion + paso) % RAC_M;
               paso++;
               contador++;
           }

    // se suma la ultima celda consultada
    *baldes_consultados = contador + 1;

    // se devuelve la posicion por parametro
    // (la variable contiene el INDICE correspondiente del elemento en el arreglo)
    if (primer_libre > -1) *posicion = primer_libre;
    else *posicion = ubicacion;

    if (strcmpi(rac->arreglo[ubicacion].codigo_envio, codigo_envio) == 0) return LOCALIZACION_EXITOSA;
    else return LOCALIZACION_ERROR_NO_EXISTE;
}

int RAC_alta(RebalseAC *rac, Envio *envio, Costos_estructura *costos) {
    int posicion, celdas_consultadas;

    // si la estructura NO esta llena
    // (se deja una celda para al menos una marca de fin 'VIRGEN')
    if (rac->cantidad < RAC_M - 1) {
        if (RAC_localizar(rac, envio->codigo_envio, &posicion, &celdas_consultadas)
            == LOCALIZACION_ERROR_NO_EXISTE) {

                // se guarda el envio en la estructura
                rac->arreglo[posicion] = *envio;
                (rac->cantidad)++;  // aumenta la cantidad de elementos en la RAL

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

int RAC_baja(RebalseAC *rac, Envio *envio, Costos_estructura *costos) {
    int posicion, celdas_consultadas;

    // si la estructura NO esta vacia
    if (rac->cantidad > 0) {
        if (RAC_localizar(rac, envio->codigo_envio, &posicion, &celdas_consultadas)
            == LOCALIZACION_EXITOSA) {

                // confirmacion de baja por comparacion de campo por campo
                if (Envio_sonIguales(&(rac->arreglo[posicion]), envio)) {

                    // se vuelve los valores de ENVIO a default
                    Envio_init(&(rac->arreglo[posicion]));
                    // luego se marca como libre
                    strcpy(rac->arreglo[posicion].codigo_envio,"LIBRE");

                    // se reduce la cantidad
                    (rac->cantidad)--;

                    // se guarda el costo de la operacion
                    (costos->Baja.cantidad)++;

                    return BAJA_EXITOSA;
                }
                else return BAJA_CANCELADA;
            }
    }

    return BAJA_ERROR_NO_EXISTE;
}

int RAC_evocar(RebalseAC *rac, char codigo_envio[], Envio *envio, Costos_estructura *costos) {
    int posicion, celdas_consultadas;

    if (RAC_localizar(rac, codigo_envio, &posicion, &celdas_consultadas) == LOCALIZACION_EXITOSA) {
        // se copia los datos de envio
        *envio = rac->arreglo[posicion];

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

void RAC_mostrarLista(RebalseAC *rac) {
    for (int i = 0; i < RAC_M; i++){
        printf("POSICION %d\n",i);
        if(strcmpi(rac->arreglo[i].codigo_envio, "VIRGEN") == 0){
            printf("CELDA VIRGEN\n\n");
        }else{
            if (strcmpi(rac->arreglo[i].codigo_envio, "LIBRE") == 0){
                printf("CELDA LIBRE\n\n");
            }else{
                Envio_imprimir(rac->arreglo[i]);
                }
            }
        // muestra de a 5 elementos en la lista
        if (((i+1) % 25) == 0) system("pause");
    }
}
