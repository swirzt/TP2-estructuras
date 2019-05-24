#ifndef __BTREE_H__
#define __BTREE_H__

typedef struct _BTNodo {
  char* dato;
  struct _BTNodo *left;
  struct _BTNodo *right;
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

int btree_obtener_dato(BTree arbol, void* string);

BTree btree_insertar(BTree arbol, void* string);

#endif /* __BTREE_H__ */
