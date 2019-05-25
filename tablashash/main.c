#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablahash.h"

#define SIZEHASH 1689

void reemplazachar(char* palabra, int pos, int valorcambio) {
  char cambio;
  switch (valorcambio) {
    case 1:
      cambio = 'a';
      break;
    case 3:
      cambio = 'e';
      break;
    case 5:
      cambio = 'i';
      break;
    case 7:
      cambio = 'o';
      break;
    case 9:
      cambio = 'u';
      break;
    case 11:
      cambio = 'a';
      break;
    case 13:
      cambio = 'e';
      break;
    case 15:
      cambio = 'i';
      break;
    case 17:
      cambio = 'o';
      break;
    case 19:
      cambio = 'u';
      break;
    case 21:
      cambio = 'n';
      break;
    case 23:
      cambio = 'n';
      break;
    case 25:
      cambio = 'o';
      break;
    case 27:
      cambio = 'o';
      break;
    case 29:
      cambio = 'u';
      break;
    case 31:
      cambio = 'u';
      break;
  }
  palabra[pos] = cambio;
  pos++;
  int largo = strlen(palabra);
  for (; pos < largo; pos++) palabra[pos] = palabra[pos + 1];
}

void eliminarchar(char* palabra, int pos) {
  int nuevolargo = strlen(palabra) - 1;
  for (int i = pos; i < nuevolargo; i++) palabra[i] = palabra[i + 1];
}

int rangoascii(char c) { return ((c > 64 && c < 91) || (c > 96 && c < 123)); }

void simplificador(char* palabra) {
  char base[33] = "áéíóúÁÉÍÓÚñÑöÖüÜ";
  int i = 0;
  char it = palabra[i];
  while (it != '\0') {
    if (it == base[0]) {
      char its = palabra[i + 1];
      int valorcambio = 0;
      for (int j = 1; j < 32 && !valorcambio; j = j + 2)
        if (its == base[j]) valorcambio = j;
      reemplazachar(palabra, i, valorcambio);
    } else if (!rangoascii(it)) {
      printf("borrealgo jeje\n");
      eliminarchar(palabra, i);
    } else
      palabra[i] = tolower(palabra[i]);
    i++;
    it = palabra[i];
  }
}

unsigned potencia(int base, int exp) {
  unsigned valor = 1;
  for (int i = 0; i < exp; i++) valor = valor * base;
  return valor;
}

unsigned hash(void* string) {
  char* palabra = string;
  unsigned contador = 0;
  int largo = strlen(palabra);
  for (int i = 0; i < largo; i++)
    contador += (palabra[i] * potencia(27, i)) % SIZEHASH;
  return contador;
}

void leer_diccionario(TablaHash* tabla, char* nombrearchivo) {
  FILE* archivo = fopen(nombrearchivo, "r");
  char* palabra = malloc(sizeof(char) * 256);
  while (!feof(archivo)) {
    fscanf(archivo, "%s", palabra);
    simplificador(palabra);
    tablahash_insertar(tabla, palabra);
  }
}

int main() {
  TablaHash* diccionario = tablahash_crear(SIZEHASH, hash);
  leer_diccionario(diccionario, "diccionario.txt");
  return 0;
}
