#include "RebalseAL.h"
#include "stdlib.h"
#include "Codigos_retornos.h"

//VIRGEN - OCUPADO (dato) - LIBRE

void RAL_init(RebalseAL *ral)
{
    int i;
    for(i = 0; i < TAM_RAL; i++)
        strcpy(ral->arreglo[i]->codigo_envio, "VIRGEN");
    ral->limite_superior = -1;
}

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

// Localizar
int RAL_localizar(RebalseAL *ral, char codigo[], int *posicion, int *baldes_consultados)
{
//    int primera_posicion_libre = -1;
//    *posicion = hashing(codigo, TAM_RAL);
//    *baldes_consultados = 0;
//    while(*baldes_consultados < TAM_RAL && strcmp(ral.arr[*posicion].codigo, "VIRGEN") != 0 && strcmp(ral.arr[*posicion].codigo, codigo) != 0)
//    {
//        if(primera_posicion_libre == -1 && strcmp(ral.arr[*posicion].codigo, "LIBRE") == 0)
//            primera_posicion_libre = *posicion;
//        (*baldes_consultados)++;
//        *posicion = (*posicion + AVANCE_RAL) % TAM_RAL;
//    }
//    if(*baldes_consultados >= TAM_RAL)
//    {
//        if(primera_posicion_libre != -1)
//            *posicion = primera_posicion_libre;
//        return FRACASO_POR_FALTA_ESPACIO;
//    }
//    (*baldes_consultados)++;
//    if(strcmp(ral.arr[*posicion].codigo, codigo) == 0)
//        return EXITO;
//    if(primera_posicion_libre != -1)
//        *posicion = primera_posicion_libre;
//    return FRACASO;
}

#endif // LISTASO_H
