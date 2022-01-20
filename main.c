#include "windows_create.c"

int main(int argc, char **argv)
{
    //DÉBUT DU SCRIPT
    //Printf du choix
    int screen_mod = 0;
    printf("De quelle maniere voulez vous afficher votre tracker ? \n");
    printf("1 = A l'ecrit \n");
    printf("2 = Graphiquement \n");

    //DÉBUT DE LA SELECTION
    while (screen_mod != '1' && screen_mod != '2')
    {                             //Tant que la variable screen_mod est différente de 1 ET de 2 (Les deux choix)
        screen_mod = 0;           //Reset de la variable screen mod à chaque itération pour éviter les erreurs d'input
        scanf("%s", &screen_mod); //On demande à l'utilisateur de rentrer le chiffre 1 ou 2
        switch (screen_mod)       //On effectue un switch sur tous les cas possibles (1,2 et autre chose)
        {
        case '1': //Si l'user écrit 1
            printf("Vous avez choisi de tracer votre objet depuis votre terminal.");
            break;
        case '2': //Si l'user écrit 2
            printf("Vous avez choisi de tracer votre objet graphiquement.");
            break;
        default: //Si l'user rentre autre chose (Lettre, chiffre, caractères spé<ciaux...)
            printf("Mauvais choix, recommencez");
            break;
        }
        if (screen_mod == '1' || screen_mod == '2')
        {
            printf("\n");
            break;
        }
    }
    //FIN DE LA SELECTION

    //DÉBUT DU CHOIX ÉCRIT
    if (screen_mod == '1')
    {
        FILE *fptr;
        char name[30];
        fptr = fopen("/home/arthur/Projet_C/texts/log.txt", "w"); // Ouverture du fichier référencé par le chemin indiqué en statut d'écriture avec remplacement
        if (fptr == NULL)                                   //Si le fichier référencé n'existe pas
        {
            printf("Le fichier référencé n'existe pas. Créez-le puis relancez le code. \n");
            return 0;
        }

        //MYSQL
        int ret = EXIT_SUCCESS;
        MYSQL *mysql = NULL;

        //INITIALISATION DES VARIABLES DE DATABASE
        char IP[30];
        char db_username[30];
        char db_pwd[30];
        char db_name[30];
        char ask_rec;

        /* Initialisation bibliotheque mysql */
        if (mysql_library_init(0, NULL, NULL) == 0)
        {
            /* Initialisation du pointeur MYSQL */
            mysql = mysql_init(NULL);
            if (mysql != NULL) //Si le pointeur est bien initialisé
            {
                fprintf(stdout, "[OK] la fonction mysql_init est bien prise en compte\n");

                FILE *rec;
                rec = fopen("/home/arthur/Projet_C/texts/Register.txt", "r");
                if (rec == NULL) //Si le fichier référencé n'existe pas
                {
                    printf("Votre fichier d'inscription n'existe pas, il sera donc créé. \n");
                    rec = fopen("/home/arthur/Projet_C/texts/Register.txt", "w");
                    printf("Indiquez où se trouve de votre base de données \n");
                    scanf("%s", IP);
                    printf("Indiquez le nom de l'utilisateur qui souhaite accéder à la base de données \n");
                    scanf("%s", db_username);
                    printf("Indiquez maintenant le mot de passe de votre utilisateur \n");
                    scanf("%s", db_pwd);

                    printf("Voulez-vous enregistrer vos informations de connexion pour ne plus avoir à les rentrer quand vous relancerez le code ? (y pour enregistrer, n pour passer)\n");
                    scanf("%s", &ask_rec);
                    switch (ask_rec) //On effectue un switch sur tous les cas possibles (1,2 et autre chose)
                    {
                    case 'y': //Si l'user écrit 1
                        printf("Vos identifiants seront enregistrés\n");
                        fprintf(rec, "%s\n", IP);
                        fprintf(rec, "%s\n", db_username);
                        fprintf(rec, "%s\n", db_pwd);
                        break;
                    default: //Si l'user rentre autre chose (Lettre, chiffre, caractères spéciaux...)
                        printf("Vos identifiants ne seront pas enregistrés. Il faudra les retaper dans ce cas.\n");
                        break;
                    }
                }
                else
                {
                    printf("[OK] Détection des identifiants réussie.\n");
                    fscanf(rec, "%s", IP);
                    fscanf(rec, "%s", db_username);
                    fscanf(rec, "%s", db_pwd);
                }

                /* Connexion au serveur mysql */
                if (mysql_real_connect(mysql, IP, db_username, db_pwd, "", 0, NULL, 0) != NULL)
                //if (mysql_real_connect(mysql, "localhost", "arthur", "arthur", "", 0, NULL, 0) != NULL)
                //Syntaxe : mysql_real_connect(mysql, IP,username,password,db); -> se connecte à la DB
                {
                    fprintf(stdout, "[OK] la fonction mysql_real_connect est bien prise en compte\n");

                    printf("Entrez le nom du produit que vous souhaitez tracer\n"); //On indique le nom du produit recherché
                    scanf("%s", name);
                    fprintf(fptr, "Lien demandé : https://www.amazon.fr/s?k=%s\n", name);
                    //l'URL que va suivre le traceur sera placé dans le fichier référencé plus haut. Remplacé à chaque itération.

                    //DEBUT DES REQUÊTES MYSQL
                    //FIN DES REQUÊTES MYSQL
                }
                else
                {
                    fprintf(stderr, "[ERR] une erreur empêche la fonction mysql_real_connect de fonctionner : '%s'\n", mysql_error(mysql));
                    ret = EXIT_FAILURE;
                }

                /* Fermeture de la connexion / Liberation memoire mysql */
                mysql_close(mysql);
                mysql = NULL;
            }
            else
            {
                perror("mysql_init");
                ret = EXIT_FAILURE;
            }

            /* Fermeture de la bibliotheque */
            mysql_library_end();
        }
        else
        {
            fprintf(stderr, "[ERR] une erreur dans la librairie mysql_library_init a été détéctée\n");
            ret = EXIT_FAILURE;
        }

        return ret;

        //MYSQL

        fclose(fptr);
    }
    //FIN DU CHOIX ÉCRIT

    //DÉBUT DU CHOIX GRAPHIQUE

    //Mise en place de la fenêtre paramétrée dans "windows_create.c"
    if (screen_mod == '2')
    {
        GtkApplication *app;
        int status;

        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
        status = g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);

        return status;
    }
    //FIN DU CHOIX GRAPHIQUE
}
//FIN DU SCRIPT
