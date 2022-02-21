void create_window(const char* glade_file){
        GtkWidget *window;
        GtkWidget *fixed1;
        GtkWidget *button1;
        GtkWidget *label1;
        GtkBuilder *builder; // pointeur vers ce qui est utilisé en relation avec le chargement du fichier XML

            // etablissement du contact avec le code xml utilisé pour ajuster les paramètres des widgets

            builder = gtk_builder_new_from_file (glade_file); // builder est un pointeur

            window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

            g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // sert à libere les ressources pour éviter que le programme ne continu à fonctionner alors que la fenetre est ferme

            gtk_builder_connect_signals(builder, NULL); // construit un tableau qui identifie tout les signaux et fonctions qui vont être appelé

            fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
            button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
            label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));

            gtk_window_set_title(GTK_WINDOW(window), "Projet en C - EVRARD/BLANDIN");
            gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

            gtk_widget_show(window); // rend la fenetre visible

            gtk_main(); // surveille les signaux et regarde les événements

        void    on_button1_clicked (GtkButton *n) {
            gtk_label_set_text (GTK_LABEL("label1"), (const gchar*)"Hello World"); 
        };
            
}