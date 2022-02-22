//FONCTION DE CONFIRMATION DU CHEMIN DE log.txt
void confirm_paths(char *log_file_path)
{
    char confirm;                                                                                                 // On initialise une valeur pour demander confirmation à l'utilisateur du chemin de référence
    printf("Veuillez confirmer que le chemin de votre fichier de log se trouve ici : %s (y/n)\n", log_file_path); // On demande à l'user de confirmer si le log.txt se trouve ici

    // DEBUT DE LA BOUCLE
    while (scanf("%c", &confirm), confirm != 'n' && confirm != 'y')
    {
        puts("Je n'ai pas compris votre requête. Écrivez 'y' si vous confirmez votre demande et 'n' pour la réfuter.");
    }

    if (confirm == 'n') // S'il ne confirme pas, on lui demande de rentrer le chemin manuellement
    {
        puts("Indiquez l'emplacement de votre script 'log.txt' (SENSIBLE À LA CASSE)");
        scanf("%s", log_file_path);
    }

    // BOUCLAGE DU RÉFÉRENCEMENT DU FICHIER
    while (access(log_file_path, F_OK) != 0 || strcmp(log_file_path + strlen(log_file_path) - 7, "log.txt"))
    // tant que l'accès au fichier est impossible ou que le chemin ne finit pas par log.txt, on boucle (Seule possibilité étant le fichier référencé au bon endroit)
    {
        printf("Le fichier référencé à l'emplacement %s est introuvable. Indiquez à nouveau son emplacement (SENSIBLE À LA CASSE)\n", log_file_path);
        scanf("%s", log_file_path);
    }
    // FIN DE LA BOUCLE
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//FONCTION DE SELECTION DE MODE D'AFFICHAGE (GRAPHIQUE/TERMINAL)
int select_mod()
{
    int screen_mod = 0;
    // Printf du choix
    puts("[CHOIX DE L'AFFICHAGE DU SCRIPT]\nDe quelle maniere voulez vous afficher votre tracker ? \n1 = A l'ecrit \n2 = Graphiquement \n");

    // DÉBUT DE LA SELECTION
    while (screen_mod != '1' && screen_mod != '2')
    {                             // Tant que la variable screen_mod est différente de 1 ET de 2 (Les deux choix)
        screen_mod = 0;           // Reset de la variable screen mod à chaque itération pour éviter les erreurs d'input
        scanf("%s", &screen_mod); // On demande à l'utilisateur de rentrer le chiffre 1 ou 2
        switch (screen_mod)       // On effectue un switch sur tous les cas possibles (1,2 et autre chose)
        {
        case '1':                   // Si l'user écrit 1
            printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal
            puts("Vous avez choisi de tracer votre objet depuis votre terminal.\n");
            break;
        case '2':                   // Si l'user écrit 2
            printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal
            puts("Vous avez choisi de tracer votre objet graphiquement.\n");
            break;
        default: // Si l'user rentre autre chose (Lettre, chiffre, caractères spé<ciaux...)
            puts("Mauvais choix, recommencez");
            break;
        }
        if (screen_mod == '1' || screen_mod == '2')
        {
            puts("");
            return (char)screen_mod;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//FONCTION QUI VA PERMETTRE DE FORMER L'URL QUE LE SCRIPT VA UTILISER
char *url_former()
{
    puts("[RECHERCHE D'UN PRODUIT]\n");
    puts("Entrez maintenant le nom du produit que vous souhaitez tracer \n"); // On indique le nom du produit recherché
    char data[100] = {0};
    char *replace; // Pointeur de remplacement de l'espace par un underscore
    scanf("%c", &data);
    fgets(data, 100, stdin);
    removeLineFeed(data);

    // Remplacement des espaces par un signe "+"
    while (replace = strchr(data, ' '))
    {
        *replace = '+';
    }
    static char url[100];
    int url_former = snprintf(url, 100, "https://api.zinc.io/v1/search?query=%s&retailer=amazon", data);
    return url;
    // l'URL que va suivre le traceur sera référencée par l'url_former
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//FONCTION D'ÉCRITURE DU RÉSULTAT DE L'API DANS log.txt
void write_curl(char *log_file_path, char *url)
{
    CURL *curl;
    CURLcode res;
    char *token = "25A58CFA811030FC6EAB3252:";

    /* Initialisation de curl */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Initialisation de la libcurl */
    curl = curl_easy_init();
    if (curl)
    {

        /* Le but de curl ici va être de récupérer le contenu de la 1ère page d'amazon en XML grâce à une API. */

        curl_easy_setopt(curl, CURLOPT_URL, url);                             // On utilise l'URL générée afin de pouvoir tracker nos produits
        curl_easy_setopt(curl, CURLOPT_USERPWD, "25A58CFA811030FC6EAB3252:"); // On utilise le token d'authentifiacation de l'API afin de pouvoir l'utiliser

        //Écriture sur le fichier log.txt
        FILE *output_file = fopen(log_file_path, "w");          // On ouvre une fonction de file afin d'écrire dans le fichier log.txt
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file); // On écrit le contenu récupéré dans le fichier log.txt

        /* Test de requête (CURLE_OK en variable binaire qui vérifie si la requête s'est bien passée) */
        res = curl_easy_perform(curl);
        /* Si res est différent de CURLE_OK, alors il y'a une erreur */
        if (res != CURLE_OK)
            fprintf(stderr, "la fonction curl_easy_perform() a rencontré un soucis: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
        puts("Notre API nous a permis de trouver une page de résultat. Nous allons traiter ces données.");
        // sleep(3);
    }
    curl_global_cleanup();
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//FONCTION QUI DEMANDE À L'UTILISATEUR QUEL ARTICLE IL VEUT SELECTIONNER
int ask_article()
{
    int ask_article = 0;
    puts("Indiquez le numéro du produit que vous souhaitez traquer");
    while (scanf("%d", &ask_article), ask_article < 1 || ask_article > 10)
    {
        puts("Mauvais nombre");
    }
    return ask_article;
}
//------------------------------------------------------------------------------------------------------------------
//FONCTION DE SAUVEGARDE DES IDENTIFIANTS SI ILS ONT ÉTÉ RENTRÉS MANUELLEMENT PAR L'UTILISATEUR
void* register_identifiers(char *register_file_path, char *IP, char* db_username, char* db_pwd, FILE* rec)
{
    char ask_rec;

        // Une fois les identifiants rentrés, on va lui demander si il veut enregistrer les infos afin de pouvoir s'auto-connecter quand on relance le code
        printf("Voulez-vous enregistrer vos informations de connexion pour ne plus avoir à les rentrer quand vous relancerez le code ? (y pour enregistrer, n pour passer)\n");
        scanf("%s", &ask_rec);
        switch (ask_rec) // On effectue un switch sur tous les cas possibles (y/n)
        {
        case 'y':                   // Si l'user écrit y, alors on enregistre les identifiants
            printf("\e[1;1H\e[2J"); // Clear l'écran
            puts("Vos identifiants seront enregistrés");
            rec = fopen(register_file_path, "w");
            fprintf(rec, "%s\n", IP);
            fprintf(rec, "%s\n", db_username);
            fprintf(rec, "%s\n", db_pwd);
            fclose(rec);
            break;
        default:                    // Si l'user écrit n, alors on passe
            printf("\e[1;1H\e[2J"); // Clear l'écran
            puts("Vos identifiants ne seront pas enregistrés. Il faudra les retaper dans ce cas.");
            break;
        }
}
//------------------------------------------------------------------------------------------------------------------
//FONCTION D'AUTO CONNEXION À LA BASE DE DONNÉES SI LE FICHIER Register.txt EXISTE
void * auto_connect(char *IP, char* db_username, char* db_pwd, FILE* rec) {
    { // Inscription des identifiants rentrés dans la base de données
        printf("\n");
        puts("[OK] Détection des identifiants réussie.");
        fscanf(rec, "%s", IP);          // Récupération de l'IP
        fscanf(rec, "%s", db_username); // Récupération du username référencé dans le fichier de *rec
        fscanf(rec, "%s", db_pwd);      // Récupération du password référencé dans le fichier de *rec
        // sleep(2);
    }
}

//------------------------------------------------------------------------------------------------------------------
// DANS LE CAS OU LA REQUÊTE S'EST MAL PASSÉE, ON RENVOIT UNE ERREUR
void finish_with_error(MYSQL *mysql)
{
    fprintf(stderr, "Une erreur a été détectée durant le processus de commandes SQL : %s\n", mysql_error(mysql));
    mysql_close(mysql);
    exit(1);
}

void connect_into_mysql(char *register_file_path, char *log_file_path, MYSQL *mysql, char *IP, char *db_username, char *db_pwd, char *db_name)
{
    char name[30];
    // DÉBUT MYSQL

    // Initialisation bibliotheque mysql
    if (mysql_library_init(0, NULL, NULL) == 0)
    {
        // Initialisation du pointeur MYSQL
        mysql = mysql_init(NULL);
        if (mysql != NULL) // Si le pointeur est bien initialisé
        {
            printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal
            fprintf(stdout, "[OK] la fonction mysql_init est bien prise en compte\n");
            // sleep(2);
                // Connexion au serveur mysql
                printf("\n");
            printf("[MISE EN RELATION AVEC LA BASE DE DONNÉES]\n");
            if (mysql_real_connect(mysql, IP, db_username, db_pwd, "", 0, NULL, 0) != NULL)
            // Syntaxe : mysql_real_connect(mysql, IP,username,password,db); -> tentative de connection à la DB
            {
                fprintf(stdout, "[OK] Les identifiants récupérés depuis le fichier Register.txt ont été reconnus par la base MySQL\n \n");
                // sleep(2);
                // DEBUT DES REQUÊTES MYSQL
                // Création de la table "c_project"
                if (mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS c_project"))
                {
                    finish_with_error(mysql);
                }
                puts("[REQUÊTES SUR LA BASE DE DONNÉES]");
                puts("Base de données créée\n");
                // sleep(1);

                // On entre dans la base de données que l'on vient de créer
                if (mysql_query(mysql, "USE c_project"))
                {
                    finish_with_error(mysql);
                }
                puts("Nous utiliserons la base de données créée afin d'y insérer les données trouvées.");

                if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS ARTICLES (id_article INT NOT NULL AUTO_INCREMENT,title VARCHAR(100) NOT NULL,price VARCHAR(10),stars VARCHAR(10),nb_eval VARCHAR(10),PRIMARY KEY(id_article));"))
                // Commande pour créer la table produit si elle n'existe pas avec les colonnes ID, titre, prix, étoiles, nb_eval
                {
                    finish_with_error(mysql);
                }

                // sleep(3);

                puts("D'après les résultats de notre recherche, nous avons trouvé ces résultats :");
                // On affiche les 10 premiers résultats de la liste trouvée (titre, prix, avis, nombre d'avis)
                FILE *read_file = fopen(log_file_path, "r");
                if (read_file == NULL)
                {
                    fprintf(stderr, "Le fichier n'a pas pu être lu.");
                    return;
                }

                char file_reader[150];
                char title[150];     // Titre de l'objet
                char price[150];     // Prix
                char stars[150];     // Avis (nombre d'étoiles)
                char num_rates[150]; // Nombre d'avis
                char query[150];     // Sert à insérer des données

                char title_array[10][150];     // Sauvegarde tableau Titre de l'objet
                char price_array[10][150];     // Sauvegarde tableau Prix
                char stars_array[10][150];     // Sauvegarde tableau Sauvegarde tableau Avis (nombre d'étoiles)
                char num_rates_array[10][150]; // Sauvegarde tableau Nombre d'avis

                int price_converted;
                int count_search = 1;
                int i = 0;
                char *replace;

                while (fgets(file_reader, sizeof(file_reader), read_file) && count_search < 11)
                {

                    if (strstr(file_reader, "product_id") != NULL)
                    {
                        printf("Article N°%d", count_search);
                        puts("\n--------------------------------------");
                    }

                    if (strstr(file_reader, "title") != NULL)
                    {
                        memcpy(title, file_reader + 15, sizeof(file_reader));
                        while (replace = strchr(title, ','))
                        {
                            *replace = ' ';
                        }
                        printf("Titre de l'article : %s", title);
                        for (int j = 0; j < strlen(title); j++)
                        {
                            title_array[i][j] = title[j];
                        }
                    }

                    if (strstr(file_reader, "price") != NULL)
                    {
                        memcpy(price, file_reader + 15, sizeof(file_reader));
                        while (replace = strchr(price, ','))
                        {
                            *replace = ' ';
                        }
                        price_converted = atoi(price); // Convertit la chaîne de caractères de la variable *price* en integer
                        printf("Prix : %d,%d€\n", price_converted / 100, price_converted % 100);
                    }

                    if (strstr(file_reader, "num_reviews") != NULL)
                    {
                        memcpy(num_rates, file_reader + 21, sizeof(file_reader));
                        while (replace = strchr(num_rates, ','))
                        {
                            *replace = ' ';
                        }
                        printf("Nombre d'évaluations : %s", num_rates);
                        for (int j = 0; j < strlen(num_rates); j++)
                        {
                            num_rates_array[i][j] = num_rates[j];
                        }
                    }

                    if (strstr(file_reader, "stars") != NULL)
                    {
                        memcpy(stars, file_reader + 15, sizeof(file_reader));
                        while (replace = strchr(stars, ','))
                        {
                            *replace = ' ';
                        }
                        printf("Nombre d'étoiles : %s\n", stars);
                        for (int j = 0; j < strlen(stars); j++)
                        {
                            stars_array[i][j] = stars[j];
                        }
                        count_search++;
                        i++;
                    }

                    if (feof(read_file))
                    {
                        puts("\nLe dernier article trouvé par notre API a été atteint.\n");
                    }
                }

                int id_article = ask_article();
                printf("Vous avez demandé l'article N°%d", id_article);

                // COMMANDE POUR INSÉRER LES DONNEES DANS LES COLONNES CORRESPONDANTES DE LA TABLE c_project
                sprintf(query, "INSERT INTO ARTICLES (title, price, stars, nb_eval) VALUES ('%s', '%s', '%s', %s);", title[id_article], price[id_article], stars[id_article], num_rates[id_article]);
                mysql_query(mysql, query);

                if (mysql_query(mysql, "SELECT * FROM ARTICLES"))
                {
                    finish_with_error(mysql);
                }

                // FIN DES REQUÊTES MYSQL
            }
            else
            {
                fprintf(stderr, "[ERR] une erreur empêche la fonction mysql_real_connect de fonctionner : '%s'\n", mysql_error(mysql));
                exit(EXIT_FAILURE);
            }

            // Fermeture de la connexion / Liberation memoire mysql
            mysql_close(mysql);
            mysql = NULL;
        }
        else
        {
            perror("mysql_init");
            exit(EXIT_FAILURE);
        }

        // Fermeture de la bibliotheque
        mysql_library_end();
    }
    else
    {
        fprintf(stderr, "[ERR] une erreur dans la librairie mysql_library_init a été détéctée  : '%s'\n", mysql_error(mysql));
        exit(EXIT_FAILURE);
    }

    // FIN MYSQL
}

//----------------------------------------------------------------------------------------------------------------------------------------------

