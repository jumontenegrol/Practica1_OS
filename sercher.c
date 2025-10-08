#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

int main() {
    FILE *header_file = fopen("header.dat", "rb");
    if (!header_file) {
        perror("Error al abrir header.dat");
        return 1;
    }

    FILE *index_file = fopen("index.dat", "rb");
    if (!index_file) {
        perror("Error al abrir index.dat");
        fclose(header_file);
        return 1;
    }

    FILE *csv = fopen("final_animedataset.csv", "r");
    if (!csv) {
        perror("Error al abrir final_animedataset.csv");
        fclose(header_file);
        fclose(index_file);
        return 1;
    }

    // --- Cargar header.dat completo a memoria ---
    long header[TABLE_SIZE];
    fread(header, sizeof(long), TABLE_SIZE, header_file);
    fclose(header_file);

    // --- Pedir clave al usuario ---
    char input_key[] = "AleXxD";
    char key[KEY_SIZE + 1];
    normalize_key(key, input_key);

    // --- Calcular bucket ---
    unsigned long hash = djb2_hash(key);
    unsigned long bucket = hash % TABLE_SIZE;

    // --- Recuperar primer offset desde header ---
    long current_offset = header[bucket];

    if (current_offset == -1) {
        printf("No se encontraron registros para la clave '%s'.\n", key);
        fclose(index_file);
        fclose(csv);
        return 0;
    }

    printf("Buscando registros para '%s'...\n", key);

    // --- Recorrer lista enlazada en index.dat ---
    Node node;
    int found = 0;

    while (current_offset != -1) {
        fseek(index_file, current_offset, SEEK_SET);
        fread(&node, sizeof(Node), 1, index_file);

        // Leer registro en CSV usando dataset_offset
        char line[LINE_BUFFER];
        fseek(csv, node.dataset_offset, SEEK_SET);
        if (fgets(line, sizeof(line), csv))
            printf("→ %s", line);

        found++;
        current_offset = node.next_node_offset;
    }

    if (found == 0)
        printf("No se encontraron registros para '%s'.\n", key);
    else
        printf("\nTotal de registros encontrados: %d\n", found);

    fclose(index_file);
    fclose(csv);
    return 0;
}