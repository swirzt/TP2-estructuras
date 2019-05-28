#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/**
 * Devuelve un arbol vacío.
 */
BTree btree_crear() { return NULL; }

/**
 * Destruccion del árbol.
 */
void btree_destruir(BTree nodo) {
  if (nodo != NULL) {
    btree_destruir(nodo->left);
    btree_destruir(nodo->right);
    free(nodo->dato);
    free(nodo);
  }
}

/**
 * Busca el dato en la lista, devuelve un Booleano.
 */
int btree_obtener_dato(BTree arbol, void* string) {
  wchar_t* palabra = string;
  int encontre = 0;
  while (!encontre && arbol != NULL) {
    if (!wcscmp(arbol->dato, palabra))
      encontre = 1;
    else if (wcscmp(arbol->dato, palabra) < 0)
      arbol = arbol->right;
    else
      arbol = arbol->left;
  }
  return encontre;
}

/**
 * Inserta un dato en el arbol.
 */
BTree btree_insertar(BTree arbol, void* string, size_t strlen) {
  wchar_t* palabra = string;
  // size_t strlen = wcslen(palabra) + 1;
  if (arbol == NULL) {
    BTree nuevoNodo = malloc(sizeof(BTNodo));
    nuevoNodo->dato = malloc(sizeof(wchar_t) * strlen);
    nuevoNodo->dato = wcscpy(nuevoNodo->dato, palabra);
    nuevoNodo->right = NULL;
    nuevoNodo->left = NULL;
    return nuevoNodo;
  }
  BTree padre, origen = arbol;
  while (arbol != NULL) {
    if (!(wcscmp(arbol->dato, palabra))) return origen;
    padre = arbol;
    if (wcscmp(arbol->dato, palabra) < 0)
      arbol = arbol->right;
    else if (wcscmp(arbol->dato, palabra) > 0)
      arbol = arbol->left;
  }
  BTree nuevoNodo = malloc(sizeof(BTNodo));
  nuevoNodo->dato = malloc(sizeof(wchar_t) * strlen);
  nuevoNodo->dato = wcscpy(nuevoNodo->dato, palabra);
  nuevoNodo->right = NULL;
  nuevoNodo->left = NULL;
  if (wcscmp(padre->dato, palabra) < 0)
    padre->right = nuevoNodo;
  else
    padre->left = nuevoNodo;
  return origen;
}