#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "colasenlazadas.h"
#include "slist.h"
#include "tablahash.h"

/*
 * Para compilar utilizar el comando "make" dentro de la carpeta del TP
 * El programa se ejecuta de la forma:
 * ./main archivoEntrada.txt archivoSalida.txt
 */

typedef struct {
  int linea;
  void* palabra;
  int sizesugerencias;
  void** sugerencias;
} PalabraMal;

typedef enum { NO, SI } limite;  // Las funciones que reciben límite

#define NOMBREDICCIONARIO "universo.txt"  // Nombre del diccionario

#define SIZEHASH 1689   // Tamaño máximo del Hash
#define SIZEBUFFER 256  // Tamaño default de buffer
#define MAXGENERADAS 5  // Máximo de palabras a generar con distancia mayor a 1

// MACROS
#define N(c) (c == L'\n')
#define R(c) (c == L'\r')
#define ESPACIO(c) (c == L' ')
#define ES_WEOF(c) (c == WEOF)

#define ABECEDARIO L"abcdefghijklmnñopqrstuvwxyzáéíóúöü"

/*
 * Devuelve una espacio de memoria alocado para una palabra.
 */
void* malloc_wpalabra(size_t size) {
  size++;
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
  int corte = 0;
  for (int i = 0; i < 8 && !corte; i++)
    if (caracter == base[i]) {
      caracter = basereemplazo[i];
      corte++;
    }
  return towlower(caracter);
}

/*
 *Recibe una tabla hash.
 *Almacena todas las palabras del universo en el Hash.
 */
void leer_diccionario(TablaHash* tabla) {
  FILE* archivo = fopen(NOMBREDICCIONARIO, "r");
  wchar_t *palabra = malloc_wpalabra(SIZEBUFFER), bufferc = fgetwc(archivo);
  while (!ES_WEOF(bufferc)) {
    int i = 0;
    while (!N(bufferc) && !ES_WEOF(bufferc)) {
      bufferc = disminuye_wchar(bufferc);
      if (iswalpha(bufferc)) {
        palabra[i] = bufferc;
        i++;
      }
      bufferc = fgetwc(archivo);
    }
    palabra[i] = L'\0';

    tablahash_insertar(tabla, palabra, i);

    while ((N(bufferc) || ESPACIO(bufferc)) && !ES_WEOF(bufferc))
      bufferc = fgetwc(archivo);
  }
  free(palabra);
  fclose(archivo);
}

/*
 * Crea y devuelve un puntero a estructura PalabraMal.
 */
PalabraMal* palabramal_crear(wchar_t* palabra, int size, int linea) {
  PalabraMal* generado = malloc(sizeof(PalabraMal));
  generado->linea = linea;
  generado->palabra = malloc_wpalabra(size);
  wcsncpy(generado->palabra, palabra, size + 1);
  generado->sizesugerencias = 0;
  generado->sugerencias = malloc(sizeof(wchar_t*) * 30);
  // Solo se esperan 30 palabras generadas máximo
  return generado;
}

/*
 * Recibe un archivo de entrada y un diccionario, devuelve una lista de
 * estructuras PalabraMal, de las palabras del texto que no se encuentran en el
 * universo.
 */
SHead palabras_incorrectas(char* archivoEntrada, TablaHash* universo) {
  SHead palabras = slist_crear();
  FILE* archivo = fopen(archivoEntrada, "r");
  if (archivo == NULL) {
    wprintf(L"No se pudo abrir el archivo de lectura,");
    wprintf(L"asegurese de que existe\n");
    return palabras;
  }
  wchar_t *palabra = malloc_wpalabra(SIZEBUFFER), bufferc = fgetwc(archivo);
  int linea = 1;
  while (!ES_WEOF(bufferc)) {
    int i = 0;
    while (!ESPACIO(bufferc) && !N(bufferc) && !ES_WEOF(bufferc)) {
      bufferc = disminuye_wchar(bufferc);
      if (iswalpha(bufferc)) {
        palabra[i] = bufferc;
        i++;
      }
      bufferc = fgetwc(archivo);
    }
    palabra[i] = L'\0';

    if (!tablahash_buscar(universo, palabra, i)) {
      PalabraMal* dato = palabramal_crear(palabra, i, linea);
      slist_insertar_final(palabras, dato);
    }

    // if (N(bufferc)) linea++;
    // int cuentalineas = 0;
    while ((N(bufferc) || ESPACIO(bufferc)) && !ES_WEOF(bufferc)) {
      if (N(bufferc)) linea++;
      bufferc = fgetwc(archivo);
    }
  }
  free(palabra);
  fclose(archivo);
  return palabras;
}

/*
 * Recibe una palabra y su largo, devuelve una copia de la misma con espacio
 * alocado.
 */
wchar_t* copiapalabra(wchar_t* palabra, int largo) {
  wchar_t* devolver = malloc_wpalabra(largo + 1);
  wcscpy(devolver, palabra);
  return devolver;
}

/*
 * Para cada par de caracteres contiguos de la palabra, si son distintos, los
 * intecambia. En caso de existir en el universo la nueva palabra se agrega a la
 * lista.
 */
void swap_char(void* origen, Cola cola, PalabraMal* palabraActual,
               TablaHash* tabla, limite l) {
  wchar_t* palabra = origen;
  int largo = wcslen(palabra);
  wchar_t temp;
  int limitar = (l == 0 ? 1 : (palabraActual->sizesugerencias < MAXGENERADAS));
  for (int i = 0; i < largo - 1 && limitar; i++) {
    if (palabra[i] != palabra[i + 1]) {
      temp = palabra[i];
      palabra[i] = palabra[i + 1];
      palabra[i + 1] = temp;
      if (tablahash_buscar(tabla, palabra, largo) &&
          wcscmp(palabra, palabraActual->palabra)) {
        int indice = palabraActual->sizesugerencias;
        palabraActual->sugerencias[indice] = copiapalabra(palabra, largo);
        palabraActual->sizesugerencias++;
        limitar =
            (l == 0 ? 1 : (palabraActual->sizesugerencias < MAXGENERADAS));
      }
      cola_encolar(cola, copiapalabra(palabra, largo));
      // revierto la operacion
      temp = palabra[i];
      palabra[i] = palabra[i + 1];
      palabra[i + 1] = temp;
    }
  }
}

/*
 * Para cada caracter de la palabra, se lo reemplaza por todas las letras del
 * ABECEDARIO. Si la palabra pertenece al universo se agrega a la lista.
 */
void replace_char(void* origen, Cola cola, PalabraMal* palabraActual,
                  TablaHash* tabla, limite l) {
  wchar_t* palabra = origen;
  int largo = wcslen(palabra);
  wchar_t abc[] = ABECEDARIO;
  int limitar = (l == 0 ? 1 : (palabraActual->sizesugerencias < MAXGENERADAS));
  for (int j = 0; j < largo && limitar; j++) {
    wchar_t viejochar = palabra[j];
    for (int i = 0; i < 34 && limitar; i++) {
      if (palabra[j] != abc[i]) {
        palabra[j] = abc[i];
        if (tablahash_buscar(tabla, palabra, largo) &&
            wcscmp(palabra, palabraActual->palabra)) {
          int indice = palabraActual->sizesugerencias;
          palabraActual->sugerencias[indice] = copiapalabra(palabra, largo);
          palabraActual->sizesugerencias++;
          limitar =
              (l == 0 ? 1 : (palabraActual->sizesugerencias < MAXGENERADAS));
        }

        cola_encolar(cola, copiapalabra(palabra, largo));
        palabra[j] = viejochar;
      }
    }
  }
}

/*
 * Elimina cada caractér de la palabra de a uno, si la nueva palabra existe en
 * el universo, esta se agrega a la lista.
 */
void remove_char(void* origen, Cola cola, PalabraMal* palabraActual,
                 TablaHash* tabla, limite l) {
  wchar_t* palabra = origen;
  int largo = wcslen(palabra);
  int limitar = (l == 0 ? 1 : (palabraActual->sizesugerencias < MAXGENERADAS));
  for (int j = 0; j < largo && limitar; j++) {
    wchar_t viejochar = palabra[j];
    for (int i = j; i < largo && limitar; i++) {
      palabra[i] = palabra[i + 1];
    }
    if (tablahash_buscar(tabla, palabra, largo) &&
        wcscmp(palabra, palabraActual->palabra)) {
      int indice = palabraActual->sizesugerencias;
      palabraActual->sugerencias[indice] = copiapalabra(palabra, largo);
      palabraActual->sizesugerencias++;
      limitar = (l == 0 ? 1 : (palabraActual->sizesugerencias < MAXGENERADAS));
    }
    cola_encolar(cola, copiapalabra(palabra, largo));
    for (int i = largo; i >= j && palabraActual->sizesugerencias < MAXGENERADAS;
         i--) {
      palabra[i + 1] = palabra[i];
    }
    palabra[j] = viejochar;
  }
}

/*
 * Recibe 1 palabra o más separadas por un espacio, si todas las palabras se
 * encuentran en el universo, se las agrega a la lista.
 */
int busca_separadas(wchar_t* palabras, TablaHash* dicc, int largo) {
  int i = 0;
  int contadorBien = 0, contadorPalabras = 0;
  wchar_t* buffer = malloc_wpalabra(largo - 1);
  while (i < largo) {
    int j = 0;
    while (palabras[i] != L' ' && i < largo) {
      buffer[j] = palabras[i];
      j++;
      i++;
    }
    i++;
    buffer[j] = L'\0';
    contadorPalabras++;
    contadorBien += tablahash_buscar(dicc, buffer, j);
  }
  free(buffer);
  return contadorBien == contadorPalabras;
}

/*
 * Inserta un caracter entre todos los espacios de la palabra, si la nueva
 * palabra se encuentra se agrega a la lista. Además separa la palabra en 2
 * agregando un espacio en las posciones posibles.
 */
void insert_char(void* origen, Cola cola, PalabraMal* palabraActual,
                 TablaHash* tabla, limite l) {
  wchar_t* palabra = origen;
  int largo = wcslen(palabra), nuevoLargo = largo + 1;
  wchar_t *buffer = malloc_wpalabra(nuevoLargo + 1), abc[] = ABECEDARIO;
  int limitar = (l == 0 ? 1 : (palabraActual->sizesugerencias < MAXGENERADAS));
  for (int j = 0; j < nuevoLargo && limitar; j++) {
    wcscpy(buffer, palabra);
    for (int i = nuevoLargo; i > j; i--) {
      buffer[i] = palabra[i - 1];
    }
    for (int k = 0; k < 34 && limitar; k++) {
      buffer[j] = abc[k];
      if (wcscmp(buffer, palabraActual->palabra) &&
          tablahash_buscar(tabla, buffer, nuevoLargo)) {
        int indice = palabraActual->sizesugerencias;
        palabraActual->sugerencias[indice] = copiapalabra(buffer, nuevoLargo);
        palabraActual->sizesugerencias++;
        limitar =
            (l == 0 ? 1 : (palabraActual->sizesugerencias < MAXGENERADAS));
      }
      cola_encolar(cola, copiapalabra(buffer, nuevoLargo));
    }
    if (j > 0 && j < largo && limitar) {
      buffer[j] = ' ';
      if (busca_separadas(buffer, tabla, nuevoLargo)) {
        int indice = palabraActual->sizesugerencias;
        palabraActual->sugerencias[indice] = copiapalabra(buffer, nuevoLargo);
        palabraActual->sizesugerencias++;
        limitar =
            (l == 0 ? 1 : (palabraActual->sizesugerencias < MAXGENERADAS));
      }
    }
  }
  free(buffer);
}

/*
 * Recibe una lista de estruxturas PalabraMal y un universo, genera al menos
 * MAXGENERADAS sugerencias para cada una.
 */
void crea_correciones(SHead palabras, TablaHash* dicc) {
  SList i = palabras->inicio;
  while (i != NULL) {
    Cola colaPalabras = cola_crear();
    PalabraMal* palabraActual = i->dato;
    int largo = wcslen(palabraActual->palabra);
    wchar_t* copia = copiapalabra(palabraActual->palabra, largo);
    replace_char(copia, colaPalabras, palabraActual, dicc, NO);
    remove_char(copia, colaPalabras, palabraActual, dicc, NO);
    swap_char(copia, colaPalabras, palabraActual, dicc, NO);
    insert_char(copia, colaPalabras, palabraActual, dicc, NO);
    free(copia);
    while (palabraActual->sizesugerencias < MAXGENERADAS) {
      wchar_t* palabraIteracion = cola_primero(colaPalabras);
      replace_char(palabraIteracion, colaPalabras, palabraActual, dicc, SI);
      remove_char(palabraIteracion, colaPalabras, palabraActual, dicc, SI);
      swap_char(palabraIteracion, colaPalabras, palabraActual, dicc, SI);
      insert_char(palabraIteracion, colaPalabras, palabraActual, dicc, SI);
      cola_desencolar(colaPalabras);
    }
    cola_destruir(colaPalabras);
    i = i->sig;
  }
}

/*
 * FunciónDestruir para la estructura PalabraMal.
 */
void palabramal_destruir(void* dato) {
  PalabraMal* destruido = dato;
  int max = destruido->sizesugerencias;
  for (int i = 0; i < max; i++) free(destruido->sugerencias[i]);
  free(destruido->sugerencias);
  free(destruido->palabra);
  free(destruido);
}

/*
 * Recibe el archivo de salida y una lista de estructuras PalabraMal.
 * Imprime la información en el archivo.
 */
void imprime_correciones(char* archivoSalida, SHead palabras) {
  assert(palabras != NULL);
  FILE* archivo = fopen(archivoSalida, "w");
  if (archivo == NULL) {
    wprintf(L"No se pudo crear el archivo para la escritura\n");
    return;
  }
  for (SList i = palabras->inicio; i != NULL; i = i->sig) {
    PalabraMal* dato = i->dato;
    fwprintf(archivo, L"Línea %d,", dato->linea);
    wchar_t* palabra = dato->palabra;
    fwprintf(archivo, L"\"%ls\" no está en el diccionario.\n", palabra);
    fwprintf(archivo, L"Quizá quiso decir:");
    for (int i = 0; i < dato->sizesugerencias; i++) {
      wchar_t* palabraSugerida = dato->sugerencias[i];
      fwprintf(archivo, L" %ls", palabraSugerida);
      if (i != dato->sizesugerencias - 1)
        fwprintf(archivo, L",");
      else
        fwprintf(archivo, L".");
    }
    fwprintf(archivo, L"\n");
  }
  fclose(archivo);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    wprintf(L"La cantidad de argumentos es incorrecta.\n");
    return 1;
  }
  setlocale(LC_ALL, "");
  TablaHash* diccionario = tablahash_crear(SIZEHASH, funcion_hasheo);
  leer_diccionario(diccionario);
  SHead incorrectas = palabras_incorrectas(argv[1], diccionario);
  crea_correciones(incorrectas, diccionario);
  imprime_correciones(argv[2], incorrectas);
  slist_destruir(incorrectas, palabramal_destruir);
  tablahash_destruir(diccionario);
  return 0;
}