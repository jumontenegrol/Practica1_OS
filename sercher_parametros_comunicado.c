#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // write, mkfifo, close
#include <fcntl.h>      // open, O_RDONLY, O_WRONLY
#include <sys/types.h>  // ssize_t
#include <sys/stat.h>   // mkfifo
#include "hashmap.h"

#define FIFO_IN  "/tmp/fifo_in"
#define FIFO_OUT "/tmp/fifo_out"
#define LINE_BUFFER 2048


void sercher(char *parametros, long *header, FILE *index_file, FILE *csv, int fd_out) {
    char *input_key, *criterio_1, *criterio_2;
    input_key = strtok(parametros, ",");
    criterio_1 = strtok(NULL, ",");
    criterio_2 = strtok(NULL, "\0");

    char key[KEY_SIZE + 1];
    normalize_key(key, input_key);

    // --- Calcular bucket ---
    unsigned long hash = djb2_hash(key);
    unsigned long bucket = hash % TABLE_SIZE;

    // --- Recuperar primer offset desde header ---
    long current_offset = header[bucket];

    if (current_offset == -1) {
        printf("No se encontraron registros para la clave '%s'.\n", key);
        dprintf(fd_out, "No se encontraron registros para la clave '%s'.\n", key);
        return;
    }

    printf("Buscando registros para '%s' que coincida con '%s','%s'...\n", input_key, criterio_1, criterio_2);
    dprintf(fd_out, "Buscando registros para '%s' que coincida con '%s','%s'...\n", input_key, criterio_1, criterio_2);

    // --- Recorrer lista enlazada en index.dat ---
    Node node;
    int found = 0;

    while (current_offset != -1) {
        fseek(index_file, current_offset, SEEK_SET);
        fread(&node, sizeof(Node), 1, index_file);

        char line[LINE_BUFFER];
        fseek(csv, node.dataset_offset, SEEK_SET);
        if (fgets(line, sizeof(line), csv)) {
            char line_copy[LINE_BUFFER];
            strcpy(line_copy, line);
            char *line_key = strtok(line, ",");
            strtok(NULL, ",");
            strtok(NULL, ",");
            strtok(NULL, ",");
            strtok(NULL, ",");
            char *line_criterio1 = strtok(NULL, ",");
            strtok(NULL,",");
            char *line_criterio2 = strtok(NULL, ",");
            if (strcmp(line_key, input_key) == 0) {
                if ((strcmp(criterio_1, "-") == 0 && strcmp(criterio_2, "-") == 0)) {
                    printf("%s", line_copy);
                    dprintf(fd_out, "%s", line_copy);
                    found++;
                } else if (strcmp(criterio_1, "-") != 0 && strcmp(criterio_2, "-") == 0) {
                    if (strcmp(line_criterio1, criterio_1) == 0) {
                        printf("%s", line_copy);
                        dprintf(fd_out, "%s", line_copy);
                        found++;
                    }
                } else if (strcmp(criterio_1, "-") == 0 && strcmp(criterio_2, "-") != 0) {
                    if (strcmp(line_criterio2, criterio_2) == 0) {
                        printf("%s", line_copy);
                        dprintf(fd_out, "%s", line_copy);
                        found++;
                    }
                } else {
                    if (strcmp(line_criterio1, criterio_1) == 0 && strcmp(line_criterio2, criterio_2) == 0) {
                        printf("%s", line_copy);
                        dprintf(fd_out, "%s", line_copy);
                        found++;
                    }
                }
            }
        }
        current_offset = node.next_node_offset;
    }

    if (found == 0){
        printf("No se encontraron registros para '%s'.\n", key);
        dprintf(fd_out, "No se encontraron registros para '%s'.\n", key);
        return;
    } else {
        printf("\nTotal de registros encontrados: %d\n", found);
        dprintf(fd_out, "\nTotal de registros encontrados: %d\n", found);
        return;
    }
}

int main() {
    //Cargar archivos necesarios
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

    //char parametros[] = "AleXxD,-,-";
    //sercher(parametros, header, index_file, csv);

    //Definicion y apertura de tuberias
    mkfifo(FIFO_IN, 0666);
    mkfifo(FIFO_OUT, 0666);

    int fd_in, fd_out;
    char buffer[256];

    while(1){
        fd_in = open(FIFO_IN, O_RDONLY);
        read(fd_in, buffer, sizeof(buffer));
        close(fd_in);

        // Procesar el comando recibido
        printf("Comando recibido: %s\n", buffer);
        dprintf(fd_out, "Comando recibido: %s\n", buffer);
        fd_out = open(FIFO_OUT, O_WRONLY);
        sercher(buffer, header, index_file, csv, fd_out);
        close(fd_out);
    }

    fclose(index_file);
    fclose(csv);
    return 0;
}