#ifndef REBALSEAL_H
#define REBALSEAL_H

#include "Envio.h"
#include "Costos.h"

// Constantes
#define RAL_M 359

//N = 300
//p = 0.84
//P = N/M => M = N/p = 300/0.84 = 357.14 = 359


// Definicion de rebalse
typedef struct {
    Envio arreglo[RAL_M];
    int cantidad;
} RebalseAL;

// --- INICIALIZAR
void RAL_init(RebalseAL *);

// --- LOCALIZAR
int RAL_localizar(RebalseAL *,char [], int *, int *);

// --- ALTA
int RAL_alta(RebalseAL *,Envio *, Costos_estructura *);

// --- BAJA
int RAL_baja(RebalseAL *,Envio *, Costos_estructura *);

// --- EVOCAR
int RAL_evocar(RebalseAL *, char [], Envio *, Costos_estructura *);

void RAL_mostrarLista(RebalseAL *);

#endif // REBALSEAL_H
