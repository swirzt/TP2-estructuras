#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "btree.h"
#include "tablahash.h"

#define SIZEHASH 1689
#define SIZEBUFFER 128

/*
 *Eleva la base a exp.
 */
unsigned potencia(int base, int exp) {
  unsigned valor = 1;
  for (int i = 0; i < exp; i++) valor = valor * base;
  return valor;
}

/*
 *Sumo el valor ascii del caracter multiplicado por el largo del abecedario
 *elevado a la posicion actual en la matriz y luego aplico modulo SIZEHASH.
 */
unsigned hash(void* string, size_t strlen) {
  wchar_t* palabra = string;
  unsigned contador = 0;
  int largo = strlen;
  for (int i = 0; i < largo; i++)
    contador += (palabra[i] * potencia(27, i)) % SIZEHASH;
  return contador;
}

/*
 *-Recibe una tabla hash y el nombre del diccionario a leer.
 *-Almacena todas las palabras del diccionario en el Hash.
 */
void leer_diccionario(TablaHash* tabla, char* nombrearchivo) {
  FILE* archivo = fopen(nombrearchivo, "r");
  wchar_t* palabra = malloc(sizeof(wchar_t) * SIZEBUFFER);
  while (fgetws(palabra, SIZEBUFFER, archivo) != NULL) {
    size_t largo = wcslen(palabra);
    palabra[largo - 1] = palabra[largo];  // Quito el caracter '\n'
    tablahash_insertar(tabla, palabra, largo);
  }
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");
  TablaHash* diccionario = tablahash_crear(SIZEHASH, hash);
  leer_diccionario(diccionario, argv[1]);
  tablahash_destruir(diccionario);
  wchar_t busqueda[90] = L"péname";
  wprintf(L"%d\n", tablahash_buscar(diccionario, busqueda, 6));
  return 0;
}
