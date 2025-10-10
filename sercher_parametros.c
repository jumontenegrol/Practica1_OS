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
    char criterio_1[] = "";
    char criterio_2[] = "Light novel";
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
        fseek(index_file, current_offset, SEEK_SET); // ubica el puntero interno del archivo al valor que se le pase
        fread(&node, sizeof(Node), 1, index_file); // (variable donde guardar, cantidad de bytes a leer, cantidad de variables (1 nodo), archivo)

        // Leer registro en CSV usando dataset_offset
        char line[LINE_BUFFER]; // definir un string tan grande para que quepa (2048)
        fseek(csv, node.dataset_offset, SEEK_SET);
        if (fgets(line, sizeof(line), csv)) {
            char line_copy[LINE_BUFFER];
            strcpy(line_copy, line);
            char *line_key = strtok(line, ","); // Primer campo
            strtok(NULL, ","); // Segundo campo
            strtok(NULL, ","); // Tercer campo
            strtok(NULL, ","); // Cuarto campo
            strtok(NULL, ","); // Quinto campo 
            char *line_criterio1 = strtok(NULL, ","); // Sexto campo (Nombre anime)
            strtok(NULL,","); //Septimo campo
            char *line_criterio2 = strtok(NULL, ","); // Octavo campo (Manga - Novel - Light Novel)
            if (strcmp(line_key, input_key) == 0) { // si la clave del input coincide con la del archivo
                if ((criterio_1[0] == '\0' && criterio_2[0] == '\0')) {
                    printf("→ %s", line_copy);
                    found++;
                } else if (criterio_1[0] != '\0' && criterio_2[0] == '\0') {
                    if (strcmp(line_criterio1, criterio_1) == 0) {
                        printf("→ %s", line_copy);
                        found++;
                    }
                } else if (criterio_1[0] == '\0' && criterio_2[0] != '\0') {
                    if (strcmp(line_criterio2, criterio_2) == 0) {
                        printf("→ %s", line_copy);
                        found++;
                    }
                } else {
                    if (strcmp(line_criterio1, criterio_1) == 0 && strcmp(line_criterio2, criterio_2) == 0) {
                        printf("→ %s", line_copy);
                        found++;
                    }
                }
            }
        }
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