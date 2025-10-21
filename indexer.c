#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

int main()
{
  FILE *csv = fopen("final_animedataset.csv", "r");
  if (!csv)
  {
    perror("Error al abrir final_animedataset.csv");
    return 1;
  }

  FILE *index_file = fopen("index.dat", "wb");
  if (!index_file)
  {
    perror("Error al crear index.dat");
    fclose(csv);
    return 1;
  }

  // -definir el hashtable con 100000 de entradas inicializadas en -1
  long header[TABLE_SIZE];
  for (int i = 0; i < TABLE_SIZE; i++)
  {
    // -1 indica que el bucket está vacío
    header[i] = -1;
  }

  char line[LINE_BUFFER];
  char raw_key[256];
  char key[KEY_SIZE + 1];
  long dataset_offset;
  unsigned long line_count = 0;

  printf("Iniciando indexación...\n");

  // --- Lectura e indexación línea por línea ---
  while (1)
  {
    dataset_offset = ftell(csv);
    if (!fgets(line, sizeof(line), csv))
    {
      break;
    }

    // Obtener la primera columna como clave
    char *token = strtok(line, ",");
    if (!token)
    {
      continue;
    }

    strcpy(raw_key, token);
    normalize_key(key, raw_key);

    // Calcular bucket
    unsigned long hash = djb2_hash(key);
    unsigned long bucket = hash % TABLE_SIZE;

    // Crear nodo
    Node node;
    node.dataset_offset = dataset_offset;
    node.next_node_offset = header[bucket];

    // Escribir nodo en index.dat
    fwrite(&node, sizeof(Node), 1, index_file);

    // Calcular offset del nodo recién escrito
    long current_offset = ftell(index_file) - sizeof(Node);

    // Actualizar header en RAM
    header[bucket] = current_offset;

    line_count++;
    if (line_count % 100 == 0)
      printf("Procesadas %lu líneas...\n", line_count);
  }

  printf("Indexación completada. Total de registros: %lu\n", line_count);

  fclose(csv);
  fclose(index_file);

  // --- Guardar header.dat al final ---
  FILE *header_file = fopen("header.dat", "wb");
  if (!header_file)
  {
    perror("Error al crear header.dat");
    return 1;
  }

  fwrite(header, sizeof(long), TABLE_SIZE, header_file);
  fclose(header_file);

  printf("header.dat y index.dat generados correctamente.\n");
  return 0;
}