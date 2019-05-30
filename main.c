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
  void** sugerencias;  // POR DEFECTO ES NULL, TENES QUE HACER DOBLE MALLOC.
} PalabraMal;          // GURVICH GAY

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

void swap_char(wchar_t* palabra){
  int largo = wcslen(palabra);
  wchar_t temp;
  for (int i = 0; i<largo-1; i++){
    palabra[i] = temp;
    palabra[i] = palabra[i+1];
    palabra[i+1] = temp;
    funcionGrande(palabra);
    //revierto la operacion
    palabra[i] = temp;
    palabra[i] = palabra[i+1];
    palabra[i+1] = temp;
  }
}

void replace_char(wchar_t* palabra){
  int largo = wcslen(palabra);
  for (int j = 0; j < largo; j++){
    for (int i = 'a'; i < 'z'; i++) {
      wchar_t viejochar;
      viejochar = palabra[j];
      palabra[j] = i;
      funcionGrande(palabra);
      palabra[j] = viejochar;

      }
  }
}

void remove_char(wchar_t* palabra){
  int largo = wcslen(palabra);
  for (int j = 0; j < largo; j++){
    for(int i = j; i <= largo; i++){
      palabra[i] = palabra [i+1];
    }
  }
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
  fclose(archivo);
}

PalabraMal* palabramal_crear(wchar_t* palabra, int size, int linea) {
  PalabraMal* generado = malloc(sizeof(PalabraMal));
  generado->linea = linea;
  generado->palabra = malloc_wpalabra(size);
  wcsncpy(generado->palabra, palabra, size);
  generado->sizesugerencias = 0;
  generado->sugerencias = NULL;
  return generado;
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
      PalabraMal* structMal = palabramal_crear(palabra, i, linea);
      cola_encolar(palabras, structMal);
    }

    if (SALTO_N(bufferc)) linea++;

    while ((SALTO_N(bufferc) || ESPACIO(bufferc)) && !ES_WEOF(bufferc))
      bufferc = fgetwc(archivo);
  }
  free(palabra);
  fclose(archivo);
  return palabras;
}

void crea_correciones(Cola palabras, TablaHash* dicc) {}  // hacer

void palabramal_destruir(void* dato) {
  PalabraMal* destruido = dato;
  int max = destruido->sizesugerencias;
  for (int i = 0; i < max; i++) free(destruido->sugerencias[i]);
  free(destruido->sugerencias);
  free(destruido->palabra);
  free(destruido);
}

void imprime_correciones(char* archivoSalida, Cola palabras) {
  FILE* archivo = fopen(archivoSalida, "w");
  while (!cola_es_vacia(palabras)) {
    // printf("holi\n");
    PalabraMal* dato = cola_primero(palabras);
    fwprintf(archivo, L"Línea %d,", dato->linea);
    fwprintf(archivo, L"\"%ls\" no está en el diccionario.\n", dato->palabra);
    fwprintf(archivo, L"Quizá quiso decir:");
    for (int i = 0; i < dato->sizesugerencias; i++)
      fwprintf(archivo, L" %ls", dato->sugerencias[i]);
    fwprintf(archivo, L"\n");
    cola_desencolar(palabras, palabramal_destruir);
  }
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");
  TablaHash* diccionario = tablahash_crear(SIZEHASH, funcion_hasheo);
  leer_diccionario(diccionario);
  Cola incorrectas = palabras_incorrectas(argv[1], diccionario);
  // wprintf(L"%d\n", cola_es_vacia(incorrectas));
  imprime_correciones(argv[2], incorrectas);
  return 0;
}
