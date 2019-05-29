#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>
#include "colasenlazadas.h"
#include "tablahash.h"

#define NOMBREDICCIONARIO "universo.txt"

#define SIZEHASH 1689
#define SIZEBUFFER 30

/*
 * Devuelve una espacio de memoria alocado para una palabra.
 */
void* malloc_wpalabra(size_t size) {
  wchar_t* alocado = malloc(sizeof(wchar_t) * size);
  return alocado;
}

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
void leer_diccionario(TablaHash* tabla) {
  FILE* archivo = fopen(NOMBREDICCIONARIO, "r");
  wchar_t* palabra = malloc_wpalabra(SIZEBUFFER);
  while (fgetws(palabra, SIZEBUFFER, archivo) != NULL) {
    size_t largo = wcslen(palabra);
    while (palabra[largo - 1] == L'\n' || palabra[largo - 1] == L'\r') {
      palabra[largo - 1] = L'\0';  // Quito el caracter '\n'
      largo--;
    }
    for (int i = 0; i < largo; i++) palabra[i] = towlower(palabra[i]);
    tablahash_insertar(tabla, palabra, largo);
  }
  free(palabra);
}

typedef struct {
  int linea;
  void* palabra;
  int sizesugerencias;
  void** sugerencias;
} PalabraMal;

Cola palabras_incorrectas(char* archivoEntrada, TablaHash* universo) {
  Cola palabras = cola_crear();
  FILE* archivo = fopen(archivoEntrada, "r");
  wchar_t *palabra = malloc_wpalabra(SIZEBUFFER), bufferc = fgetwc(archivo);
  int linea = 1;
  while (bufferc != WEOF) {
    wcsncpy(palabra, L"", 0);
    wprintf(L"entré linea %d\n", linea);
    int i = 0;
    while (bufferc != L' ' && bufferc != L'\n' && bufferc != WEOF) {
      // if (1) {
      palabra[i] = bufferc;
      wprintf(L"%c %d\n", bufferc, i);
      i++;
      palabra[i] = L'\0';
      // }
      wprintf(L"string %s\n", palabra);
      bufferc = fgetwc(archivo);
    }

    if (!tablahash_buscar(universo, palabra, i)) {
      PalabraMal* structmal = malloc(sizeof(PalabraMal));
      structmal->linea = linea;
      structmal->palabra = malloc_wpalabra(i);
      wcsncpy(structmal->palabra, palabra, i);
      structmal->sizesugerencias = 0;
      structmal->sugerencias = NULL;
      cola_encolar(palabras, structmal);
    }

    if (bufferc == L'\n') linea++;
    while ((bufferc == L'\n' || bufferc == L'\r' || bufferc == L' ') &&
           bufferc != WEOF)
      bufferc = fgetwc(archivo);
  }
  wprintf(L"salí\n");
  free(palabra);
  return palabras;
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");
  TablaHash* diccionario = tablahash_crear(SIZEHASH, hash);
  leer_diccionario(diccionario);
  Cola incorrectas = palabras_incorrectas(argv[1], diccionario);
  wprintf(L"%d\n", cola_es_vacia(incorrectas));
  return 0;
}