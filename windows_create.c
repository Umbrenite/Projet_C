#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

void button_clicked(GtkWidget *widget,gpointer data)
{
    g_print("Button Cliqué\n");
}

static void
activate(GtkApplication *app,
         gpointer user_data)
{
    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *button = gtk_button_new_with_label("Cliquez sur ce bouton (Affiche dans le terminal)");
    g_signal_connect(button,"clicked",G_CALLBACK(button_clicked),NULL);

    gtk_window_set_title(GTK_WINDOW(window), "Projet en C - EVRARD/BLANDIN");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    gtk_container_add(GTK_CONTAINER(window),button);
    gtk_widget_show_all(window);
}