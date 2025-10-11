#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // write, mkfifo, close
#include <fcntl.h>      // open, O_RDONLY, O_WRONLY
#include <sys/types.h>  // ssize_t
#include <sys/stat.h>   // mkfifo
#include "hashmap.h"

//#define FIFO_TO_BACK "fifo_to_back"
//#define FIFO_TO_GUI "fifo_to_gui"
#define FIFO_IN  "/tmp/fifo_in"
#define FIFO_OUT "/tmp/fifo_out"

GtkWidget *entry_clave;
GtkWidget *entry_nombre;
GtkWidget *entry_id;
GtkWidget *textview_output;

void on_search_clicked(GtkWidget *widget, gpointer data) {
    const char *clave = gtk_entry_get_text(GTK_ENTRY(entry_clave));
    const char *nombre = gtk_entry_get_text(GTK_ENTRY(entry_nombre));
    const char *id = gtk_entry_get_text(GTK_ENTRY(entry_id));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview_output));

    // Validar que la clave no esté vacía
    if (!clave || strlen(clave) == 0) {
        gtk_text_buffer_set_text(buffer, "Ingrese al menos la clave.\n", -1);
        return;
    }

    // Si nombre o id están vacíos, reemplazarlos con "-"
    char nombre_val[128];
    char id_val[128];
    snprintf(nombre_val, sizeof(nombre_val), "%s", (nombre && strlen(nombre) > 0) ? nombre : "-");
    snprintf(id_val, sizeof(id_val), "%s", (id && strlen(id) > 0) ? id : "-");

    // Enviar datos al backend usando write
    int f_out = open(FIFO_IN, O_WRONLY);
    if (!f_out) {
        gtk_text_buffer_set_text(buffer, "Error al abrir FIFO de salida.\n", -1);
        return;
    }

    // Enviar con formato: clave nombre id
    char mensaje[512];
    snprintf(mensaje, sizeof(mensaje), "%s,%s,%s\n", clave, nombre_val, id_val);
    write(f_out, mensaje, strlen(mensaje));
    close(f_out);


    // Leer respuesta del backend usando fread
    int f_in = open(FIFO_OUT, O_RDONLY);
    if (!f_in) {
        gtk_text_buffer_set_text(buffer, "Error al abrir FIFO de entrada.\n", -1);
        return;
    }
    char buffer1[256];
    ssize_t n;

    gtk_text_buffer_set_text(buffer, "", -1);  // limpia antes

    while ((n = read(f_in, buffer1, sizeof(buffer1)-1)) > 0) {
        gtk_text_buffer_insert_at_cursor(buffer, buffer1, -1);

        // Forzar actualización visual inmediata
        while (gtk_events_pending()) gtk_main_iteration();
    }

    if (n == 0) {
        gtk_text_buffer_insert_at_cursor(buffer, "\n Fin del envío.\n", -1);
    }
    close(f_in);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Buscador de Registros");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Label de instrucciones
    GtkWidget *label = gtk_label_new("Ingrese la clave a buscar:");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    // Entradas de texto
    entry_clave = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_clave), "Clave...");
    gtk_box_pack_start(GTK_BOX(vbox), entry_clave, FALSE, FALSE, 0);

    entry_nombre = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_nombre), "Nombre...");
    gtk_box_pack_start(GTK_BOX(vbox), entry_nombre, FALSE, FALSE, 0);

    entry_id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_id), "ID...");
    gtk_box_pack_start(GTK_BOX(vbox), entry_id, FALSE, FALSE, 0);

    // Botón de búsqueda
    GtkWidget *button = gtk_button_new_with_label("Buscar registro");
    g_signal_connect(button, "clicked", G_CALLBACK(on_search_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    // Área de salida
    textview_output = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_output), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), textview_output, TRUE, TRUE, 0);

     // --- Cargar CSS ---
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}

