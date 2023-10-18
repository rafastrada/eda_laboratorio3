#include <stdlib.h>
#include "RebalseS.h"

void RS_init(RebalseS *rs) {
    // inicia los baldes apuntando a NULL
    for (int i = 0; i < RS_M; i++) rs->punteros[i] = NULL;
}

int RS_localizar(RebalseS *rs, char codigo_envio[], RS_Nodo **elemento, RS_Nodo ***elemento_padre, int *celdas_consultadas) {
    int contador = 0;
    *posicion = hashing(codigo_envio, RS_M);
    RS_Nodo *cursor = rs->punteros[*posicion], **cursor_padre = &cursor;

    while (cursor != NULL) {
        // mientras el elemento apuntado no sea el buscado
        if (strcmpi(cursor->envio.codigo_envio, codigo_envio) != 0) {
            cursor_padre = &(cursor->siguiente);
            cursor = cursor->siguiente;
            contador++;
        }
    }

    *celdas_consultadas = contador;

    *elemento = cursor; *elemento_padre = cursor_padre;

    if (cursor != NULL) {
        // se suma la comparacion que detuvo el bucle
        (*celdas_consultadas)++;

        return LOCALIZACION_EXITOSA;
    } else return LOCALIZACION_ERROR_NO_EXISTE;
}

int RS_alta(RebalseS *rs, Envio *envio, Costos_estructura *costos) {
    int celdas_consultadas;
    RS_Nodo *elemento, **elemento_padre;

    if (RS_localizar(rs,envio->codigo_envio,&elemento, &elemento_padre, &celdas_consultadas)
        == LOCALIZACION_ERROR_NO_EXISTE) {
            // se pide memoria para el nuevo elemento
            RS_Nodo *nuevo = (RS_Nodo*) malloc(sizeof(RS_Nodo));

            // si hay memoria disponible
            if (nuevo != NULL) {
                nuevo->envio = *envio;                  // se copia el envio nuevo
                nuevo->siguiente = elemento;  // se define el puntero al siguiente
                *elemento_padre = nuevo;             // se actualiza el balde

                // se guarda el costo
                (costos->Alta.cantidad)++;

            }
            else return ALTA_ERROR_LISTA_LLENA;
        }
    else return ALTA_ERROR_CODIGO_EXISTENTE;
}

int RS_baja(RebalseS *rs, Envio *envio, Costos_estructura *costos) {
    int celdas_consultadas;
    RS_Nodo *elemento, **elemento_padre;

    if (RS_localizar(rs, envio->codigo_envio, &elemento, &elemento_padre, &celdas_consultadas)
        == LOCALIZACION_EXITOSA) {
            *elemento_padre = elemento->siguiente;
            free(elemento); // se libera la memoria

            // se guarda el costo
            (costos->Baja.cantidad)++;
        }
    else return BAJA_ERROR_NO_EXISTE;
}

int RS_evocar(RebalseS *rs, char codigo_envio[], Envio *envio, Costos_estructura *costos) {
    int celdas_consultadas;
    RS_Nodo *elemento, **elemento_padre;

    if (RS_localizar(rs, codigo_envio, &elemento, &elemento_padre, &celdas_consultadas)
        == LOCALIZACION_EXITOSA) {
            // se devuelve la nupla por parametro
            *envio = elemento->envio;

            // se guarda el costo de la operacion
            (costos->Evocacion_exitosa.cantidad)++;
            costos->Evocacion_exitosa.sumatoria_vector += celdas_consultadas;
            if (costos->Evocacion_exitosa.maximo < celdas_consultadas) costos->Evocacion_exitosa.maximo = celdas_consultadas;

            return EVOCAR_EXITOSO;
        }
    else {
        // se guarda el costo de la evocacion fallida
        (costos->Evocacion_fallida.cantidad)++;
        costos->Evocacion_fallida.sumatoria_vector += celdas_consultadas;
        if (costos->Evocacion_fallida.maximo < celdas_consultadas) costos->Evocacion_fallida.maximo = celdas_consultadas;

        return EVOCAR_NO_EXISTE;
    }
}


void RS_liberarMemoria(RebalseS *rs) {
    RS_Nodo *cursor, *cursor_auxiliar;

    for (int i=0; i<RS_M; i++) {
        cursor = rs->punteros[i];

        while (cursor != NULL) {
            cursor_auxiliar = cursor->siguiente;
            free(cursor);
            cursor = cursor_auxiliar;
        }
    }
}