GtkWidget *window_conf;
GtkWidget *window_ent;
GtkWidget *window_art;
GtkWidget *entry_ent;
GtkWidget *entry_art;

void give_articles(char *register_file_path, char *log_file_path, MYSQL *mysql, char *IP, char *db_username, char *db_pwd, char *db_name) // Récupération des infos articles
{
    char name[30];
    if (mysql_library_init(0, NULL, NULL) == 0) // Initialisation bibliothèque MySQL
    {
        mysql = mysql_init(NULL); // Initialisation pointeur MySQL
        if (mysql != NULL)
        {
            printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal

            // Connexion au serveur mysql
            if (mysql_real_connect(mysql, IP, db_username, db_pwd, "", 0, NULL, 0) != NULL)
            {
                fprintf(stdout, "[OK] Les identifiants récupérés depuis le fichier Register.txt ont été reconnus par la base MySQL\n \n");

                // DEBUT DES REQUÊTES MYSQL
                if (mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS c_project")) // Création de la base de données
                {
                    finish_with_error(mysql);
                }

                if (mysql_query(mysql, "USE c_project")) // On rentre dans la base de données
                {
                    finish_with_error(mysql);
                }

                if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS ARTICLES (id_article INT NOT NULL AUTO_INCREMENT,title TEXT NOT NULL,price VARCHAR(10),stars VARCHAR(10),nb_eval VARCHAR(10),PRIMARY KEY(id_article));"))
                // Création de la table
                {
                    finish_with_error(mysql);
                }

                if (mysql_query(mysql, "SELECT * FROM ARTICLES")) // Sélection de tout les articles déjà enregistrés dans la bdd
                {
                    finish_with_error(mysql);
                }

                MYSQL_RES *result = mysql_store_result(mysql); // Stockage des articles sélectionnés

                if (result == NULL) // Affiche l'anomalie
                {
                    finish_with_error(mysql);
                }

                int num_fields = mysql_num_fields(result); // Retourne le nb de colonnes

                MYSQL_ROW row; // Initialise la variable en tant que ligne
                
                puts("La dernière fois que vous aviez lancé le code, vous aviez tracké ceci :\n");  // Faire une interface graphique pour ça
                while ((row = mysql_fetch_row(result)))
                {
                    printf("ID dans la table:%s \n", row[0] ? row[0] : "NULL");
                    printf("Titre :%s\n", row[1] ? row[1] : "NULL");
                    printf("Prix :%s \n", row[2] ? row[2] : "NULL");
                    printf("Nombre d'évaluations :%s", row[3] ? row[3] : "NULL");
                    printf("Nombre d'étoiles :%s \n", row[4] ? row[4] : "NULL");
                    printf("\n");
                }

                mysql_free_result(result); // libère le contenu result
                // FIN DES REQUÊTES MYSQL


                FILE *read_file = fopen(log_file_path, "r"); // Ouverture du fichier en mode lecture s'il existe
                if (read_file == NULL)
                {
                    fprintf(stderr, "Le fichier n'existe pas.");
                    return;
                }


                // DÉBUT PARTIE articles.glade
                while (fgets(file_reader, sizeof(file_reader), read_file) && count_search < 10)
                {

                    // RECUPERATION DU TITRE
                    if (strstr(file_reader, "title") != NULL) // if chaine "title" exist
                    {
                        memcpy(title[i], file_reader + 15, sizeof(file_reader)); // OPTIMISATION DE LA VARIABLE POUR N'AVOIR QUE CE QUI NOUS INTERESSE

                        while (replace = strchr(title[i], '"')) // remplacement de caractère
                        {
                            *replace = ' ';
                        }
                    }

                    // RECUPERATION DU PRIX
                    if (strstr(file_reader, "price") != NULL) // if chaine "price" exist
                    {
                        memcpy(price[i], file_reader + 15, sizeof(file_reader)); // OPTIMISATION DE LA VARIABLE POUR N'AVOIR QUE CE QUI NOUS INTERESSE
                        // BOUCLE DE REMPLACEMENT DE CARACTERES
                        while (replace = strchr(price[i], ','))
                        {
                            *replace = ' ';
                        }
                        price_converted = atoi(price[i]); // Convertit la chaîne de caractères de la variable *price* en integer
                        // On sépare les nombres en deux variables afin de pouvoir les réutiliser derrière
                        before_coma = price_converted / 100;
                        after_coma = price_converted % 100;
                        sprintf(price[i], "%d,%d", before_coma, after_coma); // On push à la ligne i du tableau price le prix avec une virgule
                    }

                    // RECUPERATION DU NOMBRE D'ÉVALUATIONS (+ Processus de développement vers un tableau)
                    if (strstr(file_reader, "num_reviews") != NULL) // SI IL TROUVE LA CHAINE "num_reviews" ALORS IL EXECUTE LE TRAITEMENT
                    {
                        memcpy(num_rates[i], file_reader + 21, sizeof(file_reader)); // OPTIMISATION DE LA VARIABLE POUR N'AVOIR QUE CE QUI NOUS INTERESSE
                        // BOUCLE DE REMPLACEMENT DE CARACTERES
                        while (replace = strchr(num_rates[i], ','))
                        {
                            *replace = ' ';
                        }
                    }

                    // RECUPERATION DU NOMBRE D'ÉTOILES (+ Processus de développement vers un tableau)
                    if (strstr(file_reader, "stars") != NULL) // SI IL TROUVE LA CHAINE "stars" ALORS IL EXECUTE LE TRAITEMENT
                    {
                        memcpy(stars[i], file_reader + 15, sizeof(file_reader)); // OPTIMISATION DE LA VARIABLE POUR N'AVOIR QUE CE QUI NOUS INTERESSE
                        // BOUCLE DE REMPLACEMENT DE CARACTERES
                        while (replace = strchr(stars[i], ','))
                        {
                            *replace = ' ';
                        }
                        count_search++;
                        i++;
                    }
                    
                }
                // FIN PARTIE articles.glade

            }
            else
            {
                fprintf(stderr, "[ERR] Une erreur empêche la connexion à la base de données : '%s'\n", mysql_error(mysql));
                exit(EXIT_FAILURE);
            }

            mysql_close(mysql); // Fermeture de la connexion
            mysql = NULL; // Réinitialise à 0
        }
        else
        {
            perror("mysql_init"); // Erreur d'exécution de mysql_init -> Si MySQL == NULL
            exit(EXIT_FAILURE);
        }
        mysql_library_end(); // Nettoie et libère les ressources utilisées par la bibliothèque pour éviter les fuites de mémoire
    }
    else
    {
        fprintf(stderr, "[ERR] Erreur dans l'initialisation de la librairie mysql_library_init : '%s'\n", mysql_error(mysql));
        exit(EXIT_FAILURE);
    }
}

void url_replace(char data[56]) { // Remplacement les espaces par un signe "+"
    char *replace;
    while (replace = strchr(data, ' '))
    {
        *replace = '+';
    }
    int url_former = snprintf(url, 100, "https://api.zinc.io/v1/search?query=%s&retailer=amazon", data);
}

int database_identifiers() { // Vérification et récupération des idientifiants de connexion à la bdd

    if (strlen(url) > 0) {

        FILE *rec;
        rec = fopen(register_file_path, "r");

        if (rec == NULL) // Si le fichier référencé n'existe pas, alors on demande à l'utilisateur de rentrer ses informations
        {
            puts("Votre fichier d'inscription n'existe pas. Vous devrez entrer vos identifiants manuellement.\n");
            puts("Indiquez où se trouve de votre base de données");
            scanf("%s", IP);
            puts("Indiquez le nom de l'utilisateur qui souhaite accéder à la base de données");
            scanf("%s", db_username);
            puts("Indiquez maintenant le mot de passe de votre utilisateur");
            scanf("%s", db_pwd);

            printf("Ip : %s\n",IP);
            printf("Username : %s\n",db_username);
            printf("pass : %s\n",db_pwd);
            register_identifiers(register_file_path, IP, db_username, db_pwd, rec);
            puts("");
            return 1;
        }
        else {
            auto_connect(IP, db_username, db_pwd, rec);
            printf("Ip : %s\n",IP);
            printf("Username : %s\n",db_username);
            printf("pass : %s\n",db_pwd);
            return 1;
        }
    }
    else {
        puts("Une erreur a été détectée dans l'URL. Veuillez relancer le programme.");
        return 0;
    }

}

void page_confirmation(){  // Affichage de la page confirmation.glade

            strcpy(glade_file, GLADE_CONFIRMATION);

            GtkWidget *fixed1;
            GtkWidget *buttony;
            GtkWidget *buttonn;
            GtkWidget *labelt;
            GtkWidget *labelc;
            GtkBuilder *builder_conf; // pointeur vers ce qui est utilisé en relation avec le chargement du fichier XML

            builder_conf = gtk_builder_new_from_file (glade_file); // builder = pointeur

            window_conf = GTK_WIDGET(gtk_builder_get_object(builder_conf, "window_conf"));

            g_signal_connect(window_conf, "destroy", G_CALLBACK(gtk_main_quit), NULL); // arrête le programme pour éviter son fonctionnement en fond

            gtk_builder_connect_signals(builder_conf, NULL); // construit un tableau qui identifie tout les signaux et fonctions qui vont être appelés

            fixed1 = GTK_WIDGET(gtk_builder_get_object(builder_conf, "fixed1"));
            buttony = GTK_WIDGET(gtk_builder_get_object(builder_conf, "buttony"));
            buttonn = GTK_WIDGET(gtk_builder_get_object(builder_conf, "buttonn"));
            labelt = GTK_WIDGET(gtk_builder_get_object(builder_conf, "labelt"));
            labelc = GTK_WIDGET(gtk_builder_get_object(builder_conf, "labelc"));

            // gtk_label_set_text (GTK_LABEL("label1"), (const gchar*)"Hello World");
            char temp[50] = "mettre la variable du chemin de fichier log ici"; // faut modifier ici
            gtk_label_set_text(GTK_LABEL(labelc), temp);

            gtk_window_set_title(GTK_WINDOW(window_conf), "Projet en C - EVRARD/BLANDIN");
            gtk_window_set_default_size(GTK_WINDOW(window_conf), 600, 250);

            gtk_widget_show(window_conf);

            gtk_main(); // surveille les signaux et regarde les événements
}

void page_enter(){ // Affichage de la page enter.glade

    strcpy(glade_file, GLADE_ENTER);

    GtkWidget *fixed_ent;
    GtkWidget *buttonok_ent;
    GtkWidget *buttonrm_ent;
    GtkWidget *labelt_ent;
    GtkBuilder *builder_ent; // pointeur vers ce qui est utilisé en relation avec le chargement du fichier XML

    builder_ent = gtk_builder_new_from_file (glade_file); // builder = pointeur

    window_ent = GTK_WIDGET(gtk_builder_get_object(builder_ent, "window_ent"));

    g_signal_connect(window_ent, "destroy", G_CALLBACK(gtk_main_quit), NULL); // arrête le programme pour éviter son fonctionnement en fond

    gtk_builder_connect_signals(builder_ent, NULL); // construit un tableau qui identifie tout les signaux et fonctions qui vont être appelés

    fixed_ent = GTK_WIDGET(gtk_builder_get_object(builder_ent, "fixed_ent"));
    buttonok_ent = GTK_WIDGET(gtk_builder_get_object(builder_ent, "buttonok_ent"));
    buttonrm_ent = GTK_WIDGET(gtk_builder_get_object(builder_ent, "buttonstop_ent"));
    labelt_ent = GTK_WIDGET(gtk_builder_get_object(builder_ent, "labelt_ent"));
    entry_ent = GTK_WIDGET(gtk_builder_get_object(builder_ent, "entry_ent"));

    gtk_window_set_title(GTK_WINDOW(window_ent), "Projet en C - EVRARD/BLANDIN");
    gtk_window_set_default_size(GTK_WINDOW(window_ent), 600, 250);

    gtk_widget_show(window_ent);

    gtk_main(); // surveille les signaux et regarde les événements
}

void page_articles(){ // Affichage de la page articles.glade

    strcpy(glade_file, GLADE_ARTICLES);

    GtkWidget *fixed_art;
    GtkWidget *labelt_art[9];
    GtkWidget *labelti_art[9];
    GtkWidget *labelp_art[9];
    GtkWidget *labeln_art[9];
    GtkWidget *labelnbe_art[9];
    GtkWidget *separatorh[3];
    GtkWidget *separatorv[2];
    GtkWidget *labeltnp_art;
    GtkWidget *buttonok_art;
    GtkWidget *buttonrm_art;
    GtkBuilder *builder_art; // pointeur vers ce qui est utilisé en relation avec le chargement du fichier XML

    builder_art = gtk_builder_new_from_file (glade_file); // builder = pointeur

    window_art = GTK_WIDGET(gtk_builder_get_object(builder_art, "window_art"));

    g_signal_connect(window_art, "destroy", G_CALLBACK(gtk_main_quit), NULL); // arrête le programme pour éviter son fonctionnement en fond

    gtk_builder_connect_signals(builder_art, NULL); // construit un tableau qui identifie tout les signaux et fonctions qui vont être appelés

    fixed_art = GTK_WIDGET(gtk_builder_get_object(builder_art, "fixed_art"));


    char tmp[15];

    for (i = 0; i < 9; i++) // labelt_art -> Nbs articles
    {
        sprintf(tmp, "labelt_art%d", i+1);
        labelt_art[i] = GTK_WIDGET(gtk_builder_get_object(builder_art, tmp)); // labelt_art[0] -> labelt_art1
    }

    for (i = 0; i < 9; i++) // labelti_art -> Titres
    {
        sprintf(tmp, "labelti_art%d", i+1);
        labelti_art[i] = GTK_WIDGET(gtk_builder_get_object(builder_art, tmp)); // labelti_art[0] -> labelti_art1
    }

    for (i = 0; i < 9; i++) // labelp_art -> Prix
    {
        sprintf(tmp, "labelp_art%d", i+1);
        labelp_art[i] = GTK_WIDGET(gtk_builder_get_object(builder_art, tmp)); // labelp_art[0] -> labelp_art1
    }

    for (i = 0; i < 9; i++) // labeln_art -> Notes
    {
        sprintf(tmp, "labeln_art%d", i+1);
        labeln_art[i] = GTK_WIDGET(gtk_builder_get_object(builder_art, tmp)); // labeln_art[0] -> labeln_art1
    }

    for (i = 0; i < 9; i++) // labelnbe_art -> Nbs évals
    {
        sprintf(tmp, "labelnbe_art%d", i+1);
        labelnbe_art[i] = GTK_WIDGET(gtk_builder_get_object(builder_art, tmp)); // labelnbe_art[0] -> labelnbe_art1
    }

    for (i = 0; i < 3; i++) // separatorh
    {
        sprintf(tmp, "separatorh%d", i+1);
        separatorh[i] = GTK_WIDGET(gtk_builder_get_object(builder_art, tmp)); // separatorh[0] -> separatorh1
    }

    for (i = 0; i < 2; i++) // separatorv
    {
        sprintf(tmp, "separatorv%d", i+1);
        separatorv[i] = GTK_WIDGET(gtk_builder_get_object(builder_art, tmp)); // separatorv[0] -> separatorv1
    }
    

    labeltnp_art = GTK_WIDGET(gtk_builder_get_object(builder_art, "labeltnp_art"));
    buttonok_art = GTK_WIDGET(gtk_builder_get_object(builder_art, "buttonok_art"));
    buttonrm_art = GTK_WIDGET(gtk_builder_get_object(builder_art, "buttonrm_art"));
    entry_art = GTK_WIDGET(gtk_builder_get_object(builder_art, "entry_art"));


    char msg[50];

    for (i = 0; i < 9; i++) // labelt_art -> Nbs articles
    {   
        sprintf(msg, "Article N°%d", i+1);
        gtk_label_set_text(GTK_LABEL(labelt_art[i]), msg);
    }

    for (i = 0; i < 9; i++) // labelti_art -> Titres
    {   
        gtk_label_set_text(GTK_LABEL(labelti_art[i]), title[i]);
    }

    for (i = 0; i < 9; i++) // labelp_art -> Prix
    {   
        sprintf(msg, "Prix : %s$", price[i]);
        gtk_label_set_text(GTK_LABEL(labelp_art[i]), msg);
    }

    for (i = 0; i < 9; i++) // labeln_art -> Notes
    {   
        sprintf(msg, "Note : %s", stars[i]);
        gtk_label_set_text(GTK_LABEL(labeln_art[i]), msg);
    }

    for (i = 0; i < 9; i++) // labelnbe_art -> Nbs évals
    {   
        sprintf(msg, "Nb éval : %s", num_rates[i]);
        gtk_label_set_text(GTK_LABEL(labelnbe_art[i]), msg);
    }


    gtk_window_set_title(GTK_WINDOW(window_art), "Projet en C - EVRARD/BLANDIN");
    gtk_window_set_default_size(GTK_WINDOW(window_art), 600, 250);

    gtk_widget_show(window_art);

    gtk_main(); // surveille les signaux et regarde les événements
}

void on_buttonn_clicked(){ // bouton Non de la page confirmation.glade
    gtk_widget_destroy(window_conf);
};

void on_buttony_clicked(){ // bouton Oui de la page confirmation.glade
    gtk_widget_destroy(window_conf);
    page_enter();
};

void on_buttonrm_ent_clicked() { // bouton Remove de la page enter.glade
    gtk_entry_set_text(GTK_ENTRY(entry_ent), "");
}

void on_buttonok_ent_clicked() { // bouton OK de la page enter.glade

    char data[56];
    strcpy(data, gtk_entry_get_text(GTK_ENTRY(entry_ent))); // Récup le contenu de l'input

    if (strlen(data) > 0)
    {
        gtk_widget_destroy(window_ent); // détruit la fenêtre enter.glade
        removeLineFeed(data); // suppr les sauts de ligne
        url_replace(data); // remplace les espaces par +
        write_curl(log_file_path, url); // récupération des articles dans log.txt via curl et l'url

        int identifiers_status = database_identifiers(); // détection des identiants
        if (identifiers_status == 1) // identifiants existent
        { 
            give_articles(register_file_path, log_file_path, mysql, IP, db_username, db_pwd, db_name);
            page_articles();
        }
    }
}

void on_buttonrm_art_clicked() { // bouton Remove de la page articles.glade
    gtk_entry_set_text(GTK_ENTRY(entry_art), "");
}

void on_buttonok_art_clicked() {  // bouton OK de la page articles.glade

    char num_art[1];
    strcpy(num_art, gtk_entry_get_text(GTK_ENTRY(entry_art))); // Récup le contenu de l'input
    unsigned int id_art = atoi(num_art);

    if (0 < num_art < 10)
    {
        if (mysql_library_init(0, NULL, NULL) == 0) // Initialisation bibliothèque MySQL
        {
            mysql = mysql_init(NULL); // Initialisation pointeur MySQL
            if (mysql != NULL)
            {
                if (query == NULL){
                    puts(mysql_error(mysql));
                }
                if (mysql_real_connect(mysql, IP, db_username, db_pwd, "", 0, NULL, 0) != NULL)
                {
                    fprintf(stdout, "[OK] Les identifiants récupérés depuis le fichier Register.txt ont été reconnus par la base MySQL\n \n");
                            // Pour insérer en bdd
                    sprintf(query, "INSERT INTO ARTICLES (title, price, stars, nb_eval) VALUES ('%s', '%s', '%s', '%s');", title[id_art - 1], price[id_art - 1], stars[id_art - 1], num_rates[id_art - 1]);
                    printf("Requête générée  : %s",query);

                    if (mysql_query(mysql, "USE c_project")) // On rentre dans la base de données
                    {
                        finish_with_error(mysql);
                    }
                    mysql_query(mysql,query);
                }
            }
        }

        gtk_widget_destroy(window_art); // détruit la fenêtre articles.glade
    }
}