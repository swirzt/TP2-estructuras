#ifndef __COLA_H__
#define __COLA_H__

#include "slist.h"

// typedef struct _SNodo {
//   void* dato;
//   struct _SNodo* sig;
// } SNodo;

// typedef SNodo* SList;

typedef struct _Cola {
  SList primero;
  SList ultimo;
} * Cola;

// typedef void (*FuncionDestruir)(void* clave);

/**
 * Crea una nueva Cola.
 */
Cola cola_crear();

/**
 * Devuelve 1 si la Cola es vacia, de lo contrario, devuelve 0.
 */
int cola_es_vacia(Cola cola);

/**
 * Devuelve el primer elemento de la Cola.
 */
void* cola_primero(Cola cola);

/**
 * Agrefa un elemento al final de la Cola.
 */
void cola_encolar(Cola cola, void* dato);

/**
 * Elimina el primer elemento de la Cola.
 */
void cola_desencolar(Cola cola);

/**
 * Destruye la Cola.
 */
void cola_destruir(Cola cola);

#endif /* __COLA_H__ */
