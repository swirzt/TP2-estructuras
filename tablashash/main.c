#include <stdio.h>
#include <stdlib.h>
#include "tablahash.h"

unsigned hash(void* string) {
  char* p = string;
  int i = 0;
  for (int j = 0; *p;) i = i + (*p++);
  return i;
}

void leer_diccionario(TablaHash* tabla) {
  FILE* archivo = fopen("diccionario.txt", "r");
  char* palabra = malloc(sizeof(char) * 256);
  while (!feof(archivo)) {
    printf("Voy a ingresar algo\n");
    fscanf(archivo, "%s", palabra);
    printf("Lei algo\n");
    tablahash_insertar(tabla, palabra);
    printf("Inserte algo\n");
  }
}

int main() {
  FILE* archivo = fopen("diccionario.txt", "r");
  TablaHash* diccionario = tablahash_crear(1689, hash);
  leer_diccionario(diccionario);
  return 0;
}
