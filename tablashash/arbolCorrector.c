#include <stdlib.h>
#include <string.h>
#include "btree.h"

BTree btree_crear() { return NULL; }

void btree_destruir(BTree nodo) {
  if (nodo != NULL) {
    btree_destruir(nodo->left);
    btree_destruir(nodo->right);
    free(nodo);
  }
}

int btree_obtener_dato(BTree arbol, void* string) {
  char* palabra = string;
  int encontre = 0;
  while (!encontre && arbol != NULL) {
    if (!strcmp(arbol->dato, palabra))
      encontre = 1;
    else if (strcmp(arbol->dato, palabra) < 0)
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
  char* palabra = string;
  if (arbol == NULL) {
    BTree nuevoNodo = malloc(sizeof(BTNodo));
    nuevoNodo->dato = malloc(sizeof(char) * 256);
    nuevoNodo->dato = strcpy(nuevoNodo->dato, palabra);
    nuevoNodo->right = NULL;
    nuevoNodo->left = NULL;
    return nuevoNodo;
  }
  BTree padre, origen = arbol;
  while (arbol != NULL) {
    padre = arbol;
    if (strcmp(arbol->dato, palabra) < 0)
      arbol = arbol->right;
    else if (strcmp(arbol->dato, palabra) > 0)
      arbol = arbol->left;
  }
  BTree nuevoNodo = malloc(sizeof(BTNodo));
  nuevoNodo->dato = malloc(sizeof(char) * 256);
  nuevoNodo->dato = strcpy(nuevoNodo->dato, palabra);
  nuevoNodo->right = NULL;
  nuevoNodo->left = NULL;
  if (strcmp(padre->dato, palabra) < 0)
    padre->right = nuevoNodo;
  else
    padre->left = nuevoNodo;
  return origen;
}
