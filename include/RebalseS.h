#ifndef REBALSES_H
#define REBALSES_H

#include "Envio.h"
#include "Costos.h"

// Constantes
#define RS_M 211

//N = 300
//p = 1.45
//P = N/M => M = N/p = 300/1.45 = 206.89 = 211

//nodo
typedef struct nodo RS_Nodo;
struct nodo {
    Envio envio;
    RS_Nodo *siguiente;
};

// Definicion de rebalse
typedef struct {
    RS_Nodo* punteros[RS_M];
} RebalseS;

// --- INICIALIZAR
void RS_init(RebalseS *);

// --- LOCALIZAR
int RS_localizar(RebalseS *,char [], RS_Nodo **, RS_Nodo ***, int *);

// --- ALTA
int RS_alta(RebalseS *,Envio *, Costos_estructura *);

// --- BAJA
int RS_baja(RebalseS *,Envio *, Costos_estructura *);

// --- EVOCAR
int RS_evocar(RebalseS *, char [], Envio *, Costos_estructura *);

void RS_mostrarLista(RebalseS *);

void RS_liberarMemoria(RebalseS *);

#endif // REBALSES_H
