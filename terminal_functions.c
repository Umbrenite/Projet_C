#include "include.c"

void confirm_paths(char *log_file_path)
{
    char confirm;                                                                                                 //On initialise une valeur pour demander confirmation à l'utilisateur du chemin de référence
    printf("Veuillez confirmer que le chemin de votre fichier de log se trouve ici : %s (y/n)\n", log_file_path); //On demande à l'user de confirmer si le log.txt se trouve ici
    scanf("%c", &confirm);

    //DEBUT DE LA BOUCLE
    while (confirm != 'n' && confirm != 'y')
    {
        puts("Je n'ai pas compris votre requête. Écrivez 'y' si vous confirmez votre demande et 'n' pour la réfuter.");
        scanf("%c", &confirm);
    }

    if (confirm == 'n') // S'il ne confirme pas, on lui demande de rentrer le chemin manuellement
    {
        puts("Indiquez l'emplacement de votre script 'log.txt' (SENSIBLE À LA CASSE)");
        scanf("%s", log_file_path);
    }

    //BOUCLAGE DU RÉFÉRENCEMENT DU FICHIER
    while (access(log_file_path, F_OK) != 0 || strcmp(log_file_path + strlen(log_file_path) - 7, "log.txt"))
    //tant que l'accès au fichier est impossible ou que le chemin ne finit pas par log.txt, on boucle (Seule possibilité étant le fichier référencé au bon endroit)
    {
        printf("Le fichier référencé à l'emplacement %s est introuvable. Indiquez à nouveau son emplacement (SENSIBLE À LA CASSE)\n", log_file_path);
        scanf("%s", log_file_path);
    }
    //FIN DE LA BOUCLE
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int select_mod()
{
    int screen_mod = 0;
    //Printf du choix
    puts("[CHOIX DE L'AFFICHAGE DU SCRIPT]\nDe quelle maniere voulez vous afficher votre tracker ? \n1 = A l'ecrit \n2 = Graphiquement \n");

    //DÉBUT DE LA SELECTION
    while (screen_mod != '1' && screen_mod != '2')
    {                             //Tant que la variable screen_mod est différente de 1 ET de 2 (Les deux choix)
        screen_mod = 0;           //Reset de la variable screen mod à chaque itération pour éviter les erreurs d'input
        scanf("%s", &screen_mod); //On demande à l'utilisateur de rentrer le chiffre 1 ou 2
        switch (screen_mod)       //On effectue un switch sur tous les cas possibles (1,2 et autre chose)
        {
        case '1':                   //Si l'user écrit 1
            printf("\e[1;1H\e[2J"); //Ctrl+L sur terminal
            printf("Vous avez choisi de tracer votre objet depuis votre terminal.");
            break;
        case '2':                   //Si l'user écrit 2
            printf("\e[1;1H\e[2J"); //Ctrl+L sur terminal
            printf("Vous avez choisi de tracer votre objet graphiquement.");
            break;
        default:                    //Si l'user rentre autre chose (Lettre, chiffre, caractères spé<ciaux...)
            printf("\e[1;1H\e[2J"); //Ctrl+L sur terminal
            printf("Mauvais choix, recommencez");
            break;
        }
        if (screen_mod == '1' || screen_mod == '2')
        {
            printf("\n");
            return (char)screen_mod;
        }
    }
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

char* connect_into_mysql(char *register_file_path, char *log_file_path)
{
    FILE *fptr;
    char name[30];
    fptr = fopen(log_file_path, "w+"); // Ouverture du fichier référencé par le chemin indiqué en statut d'écriture avec remplacement
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
                puts("Votre fichier d'inscription n'existe pas, il sera donc créé.");
                rec = fopen(register_file_path, "w");
                puts("Indiquez où se trouve de votre base de données");
                scanf("%s", IP);
                puts("Indiquez le nom de l'utilisateur qui souhaite accéder à la base de données");
                scanf("%s", db_username);
                puts("Indiquez maintenant le mot de passe de votre utilisateur");
                scanf("%s", db_pwd);

                //Une fois les identifiants rentrés, on va lui demander si il veut enregistrer les infos afin de pouvoir s'auto-connecter quand on relance le code
                printf("Voulez-vous enregistrer vos informations de connexion pour ne plus avoir à les rentrer quand vous relancerez le code ? (y pour enregistrer, n pour passer)\n");
                scanf("%s", &ask_rec);
                switch (ask_rec) //On effectue un switch sur tous les cas possibles (y/n)
                {
                case 'y':                   //Si l'user écrit y, alors on enregistre les identifiants
                    printf("\e[1;1H\e[2J"); //Clear l'écran
                    puts("Vos identifiants seront enregistrés");
                    fprintf(rec, "%s\n", IP);
                    fprintf(rec, "%s\n", db_username);
                    fprintf(rec, "%s\n", db_pwd);
                    break;
                default:                    //Si l'user écrit n, alors on passe
                    printf("\e[1;1H\e[2J"); //Clear l'écran
                    puts("Vos identifiants ne seront pas enregistrés. Il faudra les retaper dans ce cas.");
                    break;
                }
            }
            else
            { //Inscription des identifiants rentrés dans la base de données
                printf("\n");
                puts("[DÉTÉCTION DES IDENTIFIANTS]");
                puts("[OK] Détection des identifiants réussie.");
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
                printf("Entrez maintenant le nom du produit que vous souhaitez tracer (Espacer les termes avec un underscore) \n"); //On indique le nom du produit recherché
                char data[100] = {0};
                char *replace; //Pointeur de remplacement de l'espace par un underscore
                scanf("%c", &data);
                fgets(data, 100, stdin);
                data[strcspn(data, "\n")] = '\0';

                //Remplacement des espaces par un underscore
                while (replace = strchr(data, ' '))
                {
                    *replace = '+';
                }
                static char url[100];
                int url_former = snprintf(url, 100, "https://www.amazon.fr/s?k=%s", data);
                return url;
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

    //FIN MYSQL
    fclose(fptr);
}

//----------------------------------------------------------------------------------------------------------------------------------------------

char html_curl(char * log_file_path, char * url)
{
    CURL *curl;
    CURLcode res;

    printf("%s",url);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* create an output file and prepare to write the response */
        FILE *output_file = fopen(log_file_path, "w");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %sn", 
                curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}