#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "colasenlazadas.h"
#include "tablahash.h"

typedef struct {
  int linea;
  void* palabra;
  int sizesugerencias;
  void** sugerencias;
} PalabraMal;

#define NOMBREDICCIONARIO "universo.txt"

#define SIZEHASH 1689
#define SIZEBUFFER 30

#define SALTO_N(c) (c == L'\n')
#define SALTO_R(c) (c == L'\r')
#define ESPACIO(c) (c == L' ')
#define ES_WEOF(c) (c == WEOF)

/*
 * Devuelve una espacio de memoria alocado para una palabra.
 */
void* malloc_wpalabra(size_t size) {
  wchar_t* alocado = malloc(sizeof(wchar_t) * size);
  return alocado;
}

/*
 *Eleva base a la exp-ésima potencia.
 */
unsigned potencia(int base, int exp) {
  unsigned valor = 1;
  for (int i = 0; i < exp; i++) valor = valor * base;
  return valor;
}

/*
 *Proceso:
 -Multiplica el valor Int de cada caracter por 27 elevado a la i-ésima potencia.
 -Aplica módulo del máximo tamaño del Hash.
 -Suma el resultado de cada caracter de la palabra y lo devuelve.
 */
unsigned funcion_hasheo(void* string, size_t strlen) {
  wchar_t* palabra = string;
  unsigned contador = 0;
  int largo = strlen;
  for (int i = 0; i < largo; i++)
    contador += (palabra[i] * potencia(27, i)) % SIZEHASH;
  return contador;
}

/*
 *Reemplaza los caracteres que no funcionan con towlower.
 *Para los demás. aplica towlower.
 */
wchar_t disminuye_wchar(wchar_t caracter) {
  wchar_t base[] = L"ÁÉÍÓÚÖÜÑ";
  wchar_t basereemplazo[] = L"áéíóúöüñ";
  for (int i = 0; i < 8; i++)
    if (caracter == base[i]) caracter = basereemplazo[i];
  return towlower(caracter);
}

/*
 *Recibe una tabla hash.
 *Almacena todas las palabras del universo en el Hash.
 */
void leer_diccionario(TablaHash* tabla) {
  FILE* archivo = fopen(NOMBREDICCIONARIO, "r");
  wchar_t* palabra = malloc_wpalabra(SIZEBUFFER);
  while (fgetws(palabra, SIZEBUFFER, archivo) != NULL) {
    size_t largo = wcslen(palabra);
    while (SALTO_N(palabra[largo - 1]) || SALTO_R(palabra[largo - 1])) {
      palabra[largo - 1] = L'\0';  // Quito el caracter '\n' o '\r'
      largo--;
    }
    for (int i = 0; i < largo; i++) palabra[i] = disminuye_wchar(palabra[i]);
    tablahash_insertar(tabla, palabra, largo);
  }
  free(palabra);
}

Cola palabras_incorrectas(char* archivoEntrada, TablaHash* universo) {
  Cola palabras = cola_crear();
  FILE* archivo = fopen(archivoEntrada, "r");
  wchar_t *palabra = malloc_wpalabra(SIZEBUFFER), bufferc = fgetwc(archivo);
  int linea = 1;
  while (!ES_WEOF(bufferc)) {
    wcsncpy(palabra, L"", 1);
    int i = 0;
    while (!ESPACIO(bufferc) && !SALTO_N(bufferc) && !ES_WEOF(bufferc)) {
      bufferc = disminuye_wchar(bufferc);
      if (iswalpha(bufferc)) {
        palabra = wcsncat(palabra, &bufferc, 1);
        i++;
      }
      bufferc = fgetwc(archivo);
    }
    palabra[i] = L'\0';

    if (!tablahash_buscar(universo, palabra, i)) {
      PalabraMal* structmal = malloc(sizeof(PalabraMal));
      structmal->linea = linea;
      structmal->palabra = malloc_wpalabra(i);
      wcsncpy(structmal->palabra, palabra, i);
      structmal->sizesugerencias = 0;
      structmal->sugerencias = NULL;
      cola_encolar(palabras, structmal);
    }

    if (SALTO_N(bufferc)) linea++;

    while ((SALTO_N(bufferc) || ESPACIO(bufferc)) && !ES_WEOF(bufferc))
      bufferc = fgetwc(archivo);
  }
  wprintf(L"salí\n");
  free(palabra);
  return palabras;
}

void escribe_sugerencias(char* archivoSalida, Cola palabras, TablaHash* dicc) {
  while (!cola_es_vacia(palabras)) {
  }
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");
  TablaHash* diccionario = tablahash_crear(SIZEHASH, funcion_hasheo);
  leer_diccionario(diccionario);
  Cola incorrectas = palabras_incorrectas(argv[1], diccionario);
  wprintf(L"%d\n", cola_es_vacia(incorrectas));
  return 0;
}