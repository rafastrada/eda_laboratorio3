#ifndef REBALSEAC_H
#define REBALSEAC_H

#include "Envio.h"
#include "Costos.h"

// Constantes
#define RAC_M 401

//N = 300
//p = 0.74
//P = N/M => M = N/p = 300/0.75 = 400 = 401


// Definicion de rebalse
typedef struct {
    Envio arreglo[RAC_M];
    int cantidad;
} RebalseAC;

// --- INICIALIZAR
void RAC_init(RebalseAC *);

// --- LOCALIZAR
int RAC_localizar(RebalseAC *,char [], int *, int *);

// --- ALTA
int RAC_alta(RebalseAC *,Envio *, Costos_estructura *);

// --- BAJA
int RAC_baja(RebalseAC *,Envio *, Costos_estructura *);

// --- EVOCAR
int RAC_evocar(RebalseAC *, char [], Envio *, Costos_estructura *);

void RAC_mostrarLista(RebalseAC *);

#endif // REBALSEAC_H
