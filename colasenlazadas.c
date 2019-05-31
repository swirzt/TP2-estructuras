#include "colasenlazadas.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Cola cola_crear() {
  Cola nuevaCola = malloc(sizeof(struct _Cola));
  nuevaCola->primero = NULL;
  nuevaCola->ultimo = NULL;
  return nuevaCola;
}

int cola_es_vacia(Cola cola) {
  assert(cola != NULL);
  return cola->primero == NULL;
}

void* cola_primero(Cola cola) {
  assert(cola != NULL);
  assert(!cola_es_vacia(cola));
  return cola->primero->dato;
}

void cola_encolar(Cola cola, void* dato) {
  assert(cola != NULL);
  SList nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = NULL;
  if (cola_es_vacia(cola)) {
    cola->primero = nuevoNodo;
  } else {
    cola->ultimo->sig = nuevoNodo;
  }
  cola->ultimo = nuevoNodo;
}

void cola_desencolar(Cola cola, FuncionDestruir d) {
  assert(cola != NULL);
  if (!cola_es_vacia(cola)) {
    SList buffer = cola->primero->sig;
    if (buffer == NULL) cola->ultimo = NULL;
    d(cola->primero->dato);
    free(cola->primero);
    cola->primero = buffer;
  }
}

void cola_destruir(Cola cola, FuncionDestruir d) {
  assert(cola != NULL);
  for (SList buffer = cola->primero; buffer != NULL;) {
    SList tmp = buffer->sig;
    d(buffer->dato);
    free(buffer);
    buffer = tmp;
  }
  free(cola);
}
