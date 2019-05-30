#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "tablahash.h"

void reemplazachar(wchar_t* palabra, int pos, int valorcambio) {
  char cambio;
  switch (valorcambio) {
    case 1:
    case 3:
      cambio = 'a';
      break;
    case 5:
    case 7:
      cambio = 'e';
      break;
    case 9:
    case 11:
      cambio = 'i';
      break;
    case 13:
    case 15:
    case 17:
    case 19:
      cambio = 'o';
      break;
    case 21:
    case 23:
    case 25:
    case 27:
      cambio = 'u';
      break;
    case 29:
    case 31:
      cambio = 'n';
      break;
  }
  palabra[pos] = cambio;
  pos++;
  int largo = strlen(palabra);
  for (; pos < largo; pos++) palabra[pos] = palabra[pos + 1];
}

void eliminarchar(wchar_t* palabra, int pos) {
  int nuevolargo = strlen(palabra) - 1;
  for (int i = pos; i < nuevolargo; i++) palabra[i] = palabra[i + 1];
}

int rangoascii(char c) { return ((c > 64 && c < 91) || (c > 96 && c < 123)); }

void simplificador(wchar_t* palabra) {
  char base[33] = "áÁéÉíÍóÓöÖúÚüÜñÑ";
  int i = 0;
  char it = palabra[i];
  while (it != '\0') {
    if (it == base[0]) {  // Se ejecuta si el caracter representa la primer
                          // parte de un caracter doble
      char its = palabra[i + 1];
      int valorcambio = 0;
      for (int j = 1; j < 32 && !valorcambio; j = j + 2)
        if (its == base[j]) valorcambio = j;
      reemplazachar(palabra, i, valorcambio);
    } else if (!rangoascii(it)) {
      eliminarchar(palabra, i);
    } else
      palabra[i] = tolower(palabra[i]);
    i++;
    it = palabra[i];
  }
}

int main() {
  char palabra[25];
  scanf("%s", palabra);
  printf("%d\n", palabra[0] == palabra[2]);
  // printf("%ldlargo\n", strlen(palabra));
  simplificador(palabra);
  // reemplazachar(palabra, 1, 1);
  printf("%s\n", palabra);
  return 1;
}