#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdint.h>

#define TABLE_SIZE 100000   // tamaño del header
#define KEY_SIZE 10          // longitud fija de la clave
#define LINE_BUFFER 2048     // tamaño del buffer para leer líneas

// Estructura de un nodo en el archivo index.dat
typedef struct {
    long dataset_offset;     // offset del registro en el CSV
    long next_node_offset;   // offset del siguiente nodo en index.dat
} Node;

// ---- Funciones auxiliares ----
unsigned long djb2_hash(const char *str);
void normalize_key(char *dest, const char *src);

#endif // HASHMAP_H