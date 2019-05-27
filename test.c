#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "tablahash.h"

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
  printf("%s reemplaze\n", palabra);
}

void simplificador(char* palabra) {
  char base[33] = "áéíóúÁÉÍÓÚñÑöÖüÜ";
  int i = 0;
  char it = palabra[i];
  while (it != '\0') {
    if (it == base[0]) {
      printf("hola\n");
      char its = palabra[i + 1];
      int valorcambio = 0;
      for (int j = 1; j < 32 && !valorcambio; j = j + 2)
        if (its == base[j]) valorcambio = j;
      reemplazachar(palabra, i, valorcambio);
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