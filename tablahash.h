#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__
#include <stdlib.h>
#include "btree.h"

/**
 * Tipo de las funciones hash a ser consideradas por las tablas hash.
 */
typedef unsigned (*FuncionHash)(void* clave, size_t lenclave);

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  BTNodo* nodo;
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
typedef struct {
  CasillaHash* tabla;
  unsigned capacidad;
  FuncionHash hash;
} TablaHash;

/**
 * Crea una nueva tabla Hash vacía, con la capacidad dada.
 */
TablaHash* tablahash_crear(unsigned capacidad, FuncionHash fun);

/**
 * Inserta el dato en la tabla, asociado a la clave dada.
 */
void tablahash_insertar(TablaHash* tabla, void* string, size_t strlen);

/**
 * Busca un elemento dado en la tabla, y retorna un puntero al mismo.
 * En caso de no existir, se retorna un puntero nulo.
 */
int tablahash_buscar(TablaHash* tabla, void* clave, size_t strlen);

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash* tabla);
/**
 * Comparar datos
 */
int comparar_clave(void* clave1, void* clave2);

#endif /* __TABLAHASH_H__ */
