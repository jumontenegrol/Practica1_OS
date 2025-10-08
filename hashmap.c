#include "hashmap.h"
#include <string.h>

// --- Hash function djb2 ---
unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// --- Normaliza una clave a longitud fija (rellenando con 'a') ---
void normalize_key(char *dest, const char *src) {
    size_t len = strlen(src);
    if (len >= KEY_SIZE){
        // Si la clave es demasiado larga, truncarla
        strncpy(dest, src, KEY_SIZE);
    }
    else {
        strcpy(dest, src);
        for (size_t i = len; i < KEY_SIZE; i++)
            dest[i] = 'a';
    }
    dest[KEY_SIZE] = '\0';
}