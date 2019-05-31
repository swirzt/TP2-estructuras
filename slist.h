#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>

typedef struct _SNodo {
  void* dato;
  struct _SNodo* sig;
} SNodo;

typedef SNodo* SList;

typedef struct _SHead {
  SList inicio, final;
} * SHead;

typedef void (*FuncionDestruir)(void* clave);

/**
 * Crea una nueva Lista.
 */
SHead slist_crear();

/**
 * Devuelve 1 si la Lista es vacia, de lo contrario, devuelve 0.
 */
int slist_vacia(SHead header);

/**
 * Agrega un elemento al final de la Lista.
 */
void slist_insertar_final(SHead header, void* dato);

/**
 * Destruye la Lista.
 */
void slist_destruir(SHead header, FuncionDestruir d);

#endif /* __SLIST_H__ */
