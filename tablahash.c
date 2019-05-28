#include "tablahash.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "btree.h"

/**
 * Crea una nueva tabla Hash vacía, con la capacidad dada.
 */
TablaHash* tablahash_crear(unsigned capacidad, FuncionHash hash) {
  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash* tabla = malloc(sizeof(TablaHash));
  tabla->hash = hash;
  tabla->capacidad = capacidad;
  tabla->tabla = malloc(sizeof(CasillaHash) * capacidad);

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->tabla[idx].nodo = btree_crear();
  }

  return tabla;
}

/**
 * Inserta el dato en la tabla, asociado a la clave dada.
 */
void tablahash_insertar(TablaHash* tabla, void* string, size_t strlen) {
  // Calculamos la posición de la clave dada, de acuerdo a la función hash.
  unsigned idx = tabla->hash(string, strlen);
  idx = idx % tabla->capacidad;
  // printf("valor hash %d\n", idx);

  // insertamos el valor en el arbol correspondiente
  tabla->tabla[idx].nodo =
      btree_insertar(tabla->tabla[idx].nodo, string, strlen);
}

/**
 * Busca un elemento dado en la tabla, y retorna un puntero al mismo.
 * En caso de no existir, se retorna un puntero nulo.
 */
int tablahash_buscar(TablaHash* tabla, void* string, size_t strlen) {
  // Calculamos la posición de la clave dada, de acuerdo a la función hash.
  unsigned idx = tabla->hash(string, strlen);
  idx = idx % tabla->capacidad;

  // Si el lugar esta vacío, retornamos un puntero nulo.
  if (tabla->tabla[idx].nodo == NULL) return 0;

  return btree_obtener_dato(tabla->tabla[idx].nodo, string);
}

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash* tabla) {
  for (unsigned int i = 0; i < tabla->capacidad; i++)
    btree_destruir(tabla->tabla->nodo + i);
  free(tabla);
}

/**
 * Comparar datos
 */
int comparar_clave(void* clave1, void* clave2) {
  int* claveoriginal = clave1;
  int* claveocomparar = clave2;
  return *claveocomparar == *claveoriginal;
}
