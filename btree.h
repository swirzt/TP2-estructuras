#ifndef __BTREE_H__
#define __BTREE_H__
#include <stdlib.h>

typedef struct _BTNodo {
  void* dato;
  struct _BTNodo* left;
  struct _BTNodo* right;
} BTNodo;

typedef BTNodo* BTree;

/**
 * Devuelve un arbol vacío.
 */
BTree btree_crear();

/**
 * Destruccion del árbol.
 */
void btree_destruir(BTree nodo);

/**
 * Busca el dato en la lista, devuelve un Booleano.
 */
int btree_obtener_dato(BTree arbol, void* string);

/**
 * Inserta un dato en el arbol.
 */
BTree btree_insertar(BTree arbol, void* string, size_t strlen);

#endif /* __BTREE_H__ */
