#include "slist.h"
#include <assert.h>
#include <stdlib.h>

SHead slist_crear() {
  SHead salida = malloc(sizeof(struct _SHead));
  salida->inicio = NULL;
  salida->final = NULL;
  return salida;
}

int slist_vacia(SHead header) {
  assert(header != NULL);
  return header->inicio == NULL;
}

void slist_insertar_final(SHead header, void* dato) {
  assert(header != NULL);
  SList nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = NULL;
  if (slist_vacia(header))
    header->inicio = nuevoNodo;
  else
    header->final->sig = nuevoNodo;
  header->final = nuevoNodo;
}

void slist_destruir(SHead header, FuncionDestruir d) {
  SList i = header->inicio;
  while (i != NULL) {
    SList buffer = i->sig;
    d(i->dato);
    free(i);
    i = buffer;
  }
  free(header);
}