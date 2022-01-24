#include "windows_create.c"

//DÉBUT DU SCRIPT
int main(int argc, char **argv)
{
//CONFIGURATION DU SCRIPT
    printf("AVANT LE LANCEMENT DU SCRIPT, VEUILLEZ VÉRIFIER QUE LES FICHIERS SOIENT RÉFÉRENCÉS AUX BONS EMPLACEMENTS \n \n");

    //Initialisation du chemin du fichier de log
    char log_file_path[PATH_MAX];
    getcwd(log_file_path, sizeof(log_file_path)); //On définit le chemin de l'user avec getcwd qui définit où on se trouve 

   //Initialisation du chemin du fichier de register (Enregistrement des identifiants de connexion à la BDD)
    char register_file_path[PATH_MAX];
    getcwd(register_file_path, sizeof(register_file_path)); //On définit le chemin de l'user avec getcwd qui définit où on se trouve 


    strcat(log_file_path, "/texts/log.txt");          //On concatène la chaîne de caractères "/texts/log.txt", cette partie étant statique
    strcat(register_file_path, "/texts/register.txt"); //On concatène la chaîne de caractères "/texts/register.txt", cette partie étant statique
    if (log_file_path)
    {                                                                                                                                      //Si la variable HOME existe alors
        char confirm;                                                                                                                      //On initialise une valeur pour demander confirmation à l'utilisateur du chemin de référence
        printf("Veuillez confirmer que le chemin de votre fichier de log se trouve ici : %s (y/n)\n", log_file_path);                      //On demande à l'user de confirmer si le log.txt se trouve ici
        scanf("%c", &confirm);

        while (confirm != 'n' && confirm != 'y') {
            printf("Je n'ai pas compris votre requête. Écrivez 'y' si vous confirmez votre demande et 'n' pour la réfuter.");
            scanf("%c", &confirm);
        }

        if (confirm == 'n') // S'il ne confirme pas, on lui demande de rentrer le chemin manuellement
        {
            printf("Indiquez l'emplacement de votre script 'log.txt' (SENSIBLE À LA CASSE) \n");
            scanf("%s", log_file_path);
        }

        //BOUCLAGE DU RÉFÉRENCEMENT DU FICHIER
        while (access(log_file_path, F_OK) != 0 || strcmp(log_file_path + strlen(log_file_path) - 7, "log.txt"))
        //tant que l'accès au fichier est impossible ou que le chemin ne finit pas par log.txt, on boucle (Seule possibilité étant le fichier référencé au bon endroit)
        {
            printf("Le fichier référencé 'log.txt' n'existe pas. Indiquez son emplacement (SENSIBLE À LA CASSE)\n");
            scanf("%s", log_file_path);
        }
        //FIN DE LA BOUCLE
    }
    else
    {
        printf("Un problème à la variable HOME a été détecté.");
        return 0;
    }

    if (access(log_file_path, F_OK) == 0 && !strcmp(log_file_path + strlen(log_file_path) - 7, "log.txt"))
    {
        printf("\e[1;1H\e[2J"); //Clear l'écran
        printf("Fichier de log trouvé\n \n");
    }
//FIN DE LA CONFIGURATION DU SCRIPT

//Printf du choix
    int screen_mod = 0;
    printf("[CHOIX DE L'AFFICHAGE DU SCRIPT]\n");
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
            printf("\e[1;1H\e[2J"); //Clear l'écran
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
        fptr = fopen(log_file_path, "w+"); // Ouverture du fichier référencé par le chemin indiqué en statut d'écriture avec remplacement
        if (fptr == NULL)                                          //Si le fichier référencé n'existe pas
        {
            printf("Le fichier référencé n'existe pas. Créez-le puis relancez le code. \n");
            return 0;
        }

    //DÉBUT MYSQL
        int ret = EXIT_SUCCESS;
        MYSQL *mysql = NULL;

        //INITIALISATION DES VARIABLES DE DATABASE
        char IP[30];
        char db_username[30];
        char db_pwd[30];
        char db_name[30];
        char ask_rec;

        // Initialisation bibliotheque mysql
        if (mysql_library_init(0, NULL, NULL) == 0)
        {
            // Initialisation du pointeur MYSQL
            mysql = mysql_init(NULL);
            if (mysql != NULL) //Si le pointeur est bien initialisé
            {
                fprintf(stdout, "[OK] la fonction mysql_init est bien prise en compte\n");

                FILE *rec;
                rec = fopen(register_file_path, "r");
                if (rec == NULL) //Si le fichier référencé n'existe pas, alors on demande à l'utilisateur de rentrer ses informations
                {
                    printf("Votre fichier d'inscription n'existe pas, il sera donc créé. \n");
                    rec = fopen(register_file_path, "w");
                    printf("Indiquez où se trouve de votre base de données \n");
                    scanf("%s", IP);
                    printf("Indiquez le nom de l'utilisateur qui souhaite accéder à la base de données \n");
                    scanf("%s", db_username);
                    printf("Indiquez maintenant le mot de passe de votre utilisateur \n");
                    scanf("%s", db_pwd);

                    //Une fois les identifiants rentrés, on va lui demander si il veut enregistrer les infos afin de pouvoir s'auto-connecter quand on relance le code
                    printf("Voulez-vous enregistrer vos informations de connexion pour ne plus avoir à les rentrer quand vous relancerez le code ? (y pour enregistrer, n pour passer)\n");
                    scanf("%s", &ask_rec);
                    switch (ask_rec) //On effectue un switch sur tous les cas possibles (y/n)
                    {
                    case 'y': //Si l'user écrit y, alors on enregistre les identifiants
                        printf("Vos identifiants seront enregistrés\n");
                        fprintf(rec, "%s\n", IP);
                        fprintf(rec, "%s\n", db_username);
                        fprintf(rec, "%s\n", db_pwd);
                        break;
                    default: //Si l'user écrit n, alors on passe
                        printf("Vos identifiants ne seront pas enregistrés. Il faudra les retaper dans ce cas.\n");
                        break;
                    }
                }
                else
                { //Inscription des identifiants rentrés dans la base de données
                    printf("\e[1;1H\e[2J"); //Clear l'écran
                    printf("\n");
                    printf("[DÉTÉCTION DES IDENTIFIANTS]\n");
                    printf("[OK] Détection des identifiants réussie.\n");
                    fscanf(rec, "%s", IP);          //Récupération de l'IP
                    fscanf(rec, "%s", db_username); //Récupération du username référencé dans le fichier de *rec
                    fscanf(rec, "%s", db_pwd);      //Récupération du password référencé dans le fichier de *rec
                }

                // Connexion au serveur mysql
                printf("\n");
                printf("[MISE EN RELATION AVEC LA BASE DE DONNÉES]\n");
                if (mysql_real_connect(mysql, IP, db_username, db_pwd, "", 0, NULL, 0) != NULL)
                //Syntaxe : mysql_real_connect(mysql, IP,username,password,db); -> tentative de connection à la DB
                {
                    fprintf(stdout, "[OK] Les identifiants récupérés depuis le fichier Register.txt ont été reconnus par la base MySQL\n \n");

                    printf("[RECHERCHE D'UN PRODUIT]\n");
                    printf("Entrez maintenant le nom du produit que vous souhaitez tracer\n"); //On indique le nom du produit recherché
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

                // Fermeture de la connexion / Liberation memoire mysql
                mysql_close(mysql);
                mysql = NULL;
            }
            else
            {
                perror("mysql_init");
                ret = EXIT_FAILURE;
            }

            // Fermeture de la bibliotheque
            mysql_library_end();
        }
        else
        {
            fprintf(stderr, "[ERR] une erreur dans la librairie mysql_library_init a été détéctée\n");
            ret = EXIT_FAILURE;
        }

        return ret;

    //FIN MYSQL

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
