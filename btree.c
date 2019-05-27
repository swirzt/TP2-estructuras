#include "btree.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

BTree btree_crear() { return NULL; }

void btree_destruir(BTree nodo) {
  if (nodo != NULL) {
    btree_destruir(nodo->left);
    btree_destruir(nodo->right);
    free(nodo);
  }
}

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
  if (encontre)
    return 1;
  else
    return 0;
}

BTree btree_insertar(BTree arbol, void* string) {
  wchar_t* palabra = string;
  size_t largo = wcslen(palabra);
  wprintf(L"%ld\n", largo);
  if (arbol == NULL) {
    BTree nuevoNodo = malloc(sizeof(BTNodo));
    nuevoNodo->dato = malloc(sizeof(wchar_t) * (largo - 1));
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
  nuevoNodo->dato = malloc(sizeof(wchar_t) * (largo - 1));
  nuevoNodo->dato = wcscpy(nuevoNodo->dato, palabra);
  nuevoNodo->right = NULL;
  nuevoNodo->left = NULL;
  if (wcscmp(padre->dato, palabra) < 0)
    padre->right = nuevoNodo;
  else
    padre->left = nuevoNodo;
  return origen;
}