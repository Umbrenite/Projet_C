// FONCTION DE CONFIRMATION DU CHEMIN DE log.txt
void confirm_paths(char *log_file_path)
{
    char confirm;                                                                                              // On initialise une valeur pour demander confirmation à l'utilisateur du chemin de référence
    sleep(1);
    printf("Veuillez confirmer que le chemin de votre fichier de log se trouve ici : %s \n\n", log_file_path); // On demande à l'user de confirmer si le log.txt se trouve ici
    sleep(2);

    // DEBUT DE LA BOUCLE
    do {
        puts("Écrivez 'y' pour confirmer ou 'n' pour refuser.");
        scanf(" %c", &confirm);
        if (confirm == 'n') break;
        if (confirm == 'y') break;
    } while (true);

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


// FONCTION DE SELECTION DE MODE D'AFFICHAGE (GRAPHIQUE/TERMINAL)
int select_mod()
{
    int screen_mod = 0;
    // Printf du choix
    puts("[CHOIX DE L'AFFICHAGE DU SCRIPT]\nDe quelle manière souhaitez-vous afficher votre tracker ?\n\n1 = Écrit \n2 = Graphique\n");

    // DÉBUT DE LA SELECTION
    while (screen_mod != '1' && screen_mod != '2') // Tant que la variable screen_mod est différente de 1 ET de 2 (Les deux choix)
    {
        screen_mod = 0;           // Reset variable à chaque itération pour éviter les erreurs d'input
        scanf("%s", &screen_mod); // rentrer 1 ou 2
        switch (screen_mod)       // On effectue un switch sur tous les cas possibles (1,2 et autre chose)
        {
        case '1':                   // Si l'user écrit 1
            printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal (clear)
            sleep(1);
            puts("\nVous avez choisi de tracker votre objet depuis votre terminal.\n");
            sleep(1);
            puts("[VEUILLEZ PATIENTER]");
            sleep(3);
            printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal (clear)
            break;
        case '2':                   // Si l'user écrit 2
            printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal (clear)
            sleep(2);
            puts("Vous avez choisi de tracker votre objet graphiquement.\n");
            sleep(3);
            break;
        default: // Si l'user rentre autre chose (Lettre, chiffre, caractères spéciaux...)
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


// FONCTION QUI VA PERMETTRE DE FORMER L'URL QUE LE SCRIPT VA UTILISER
char *url_former()
{
    puts("[RECHERCHE D'UN PRODUIT]\n");
    sleep(1);
    puts("Entrez maintenant le nom du produit que vous souhaitez tracker \n"); // On indique le nom du produit recherché
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
    // l'URL que va suivre le trackeur sera référencée par l'url_former
}


// FONCTION D'ÉCRITURE DU RÉSULTAT DE L'API DANS log.txt
void write_curl(char *log_file_path, char *url)
{
    CURL *curl;
    CURLcode res;
    char *token = "25A58CFA811030FC6EAB3252:";

    /* Initialisation de la libcurl */
    curl = curl_easy_init();
    if (curl != NULL)
    {
        /* Le but de curl ici va être de récupérer le contenu de la 1ère page d'amazon en XML grâce à une API. */

        curl_easy_setopt(curl, CURLOPT_URL, url);                             // On utilise l'URL générée afin de pouvoir tracker nos produits
        curl_easy_setopt(curl, CURLOPT_USERPWD, "25A58CFA811030FC6EAB3252:"); // On utilise le token d'authentifiacation de l'API afin de pouvoir l'utiliser

        //Écriture sur le fichier log.txt
        FILE *output_file = fopen(log_file_path, "w");          // On ouvre une fonction de file afin d'écrire dans le fichier log.txt
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file); // On écrit le contenu récupéré dans le fichier log.txt

        /* Test de requête (CURLE_OK en variable binaire qui vérifie si la requête s'est bien passée) */
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "la fonction curl_easy_perform() a rencontré un problème: %s\n",
            curl_easy_strerror(res));
            exit(1);
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}


// FONCTION QUI DEMANDE À L'UTILISATEUR QUEL ARTICLE IL VEUT SELECTIONNER
unsigned int ask_article()
{
    unsigned int ask_article = 0;
    puts("Indiquez le numéro du produit que vous souhaitez traquer");
    while (scanf("%u", &ask_article), ask_article < 1 || ask_article > 10)
    {
        puts("Mauvais nombre");
    }
    return ask_article;
}


// FONCTION DE SAUVEGARDE DES IDENTIFIANTS SI ILS ONT ÉTÉ RENTRÉS MANUELLEMENT PAR L'UTILISATEUR
void register_identifiers(char *register_file_path, char *IP, char *db_username, char *db_pwd, FILE *rec)
{
    char ask_rec[2];

    // Une fois les identifiants rentrés, on va lui demander si il veut enregistrer les infos afin de pouvoir s'auto-connecter quand on relance le code
    printf("Voulez-vous enregistrer vos informations de connexion pour ne plus avoir à les rentrer quand vous relancerez le code ? (y pour enregistrer, n pour passer)\n");
    scanf("%s", &ask_rec);
    switch (*ask_rec) // On effectue un switch sur tous les cas possibles (y/n)
    {
        case 'y':                   // Si l'user écrit y, alors on enregistre les identifiants
            printf("\e[1;1H\e[2J"); // Clear l'écran
            puts("Vos identifiants seront enregistrés");
            rec = fopen(register_file_path, "w");
            if (rec == NULL){
                printf("ça bug");
                exit(1);
            }
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


// FONCTION D'AUTO CONNEXION À LA BASE DE DONNÉES SI LE FICHIER Register.txt EXISTE
void *auto_connect(char *IP, char *db_username, char *db_pwd, FILE *rec)
{
    { // Inscription des identifiants rentrés dans la base de données
        printf("\n");
        puts("[OK] Détection des identifiants réussie.");
        fscanf(rec, "%s", IP);          // Récupération de l'IP
        fscanf(rec, "%s", db_username); // Récupération du username référencé dans le fichier de *rec
        fscanf(rec, "%s", db_pwd);      // Récupération du password référencé dans le fichier de *rec
    }
}


// DANS LE CAS OU LA REQUÊTE S'EST MAL PASSÉE, ON RENVOIT UNE ERREUR
void finish_with_error(MYSQL *mysql)
{
    fprintf(stderr, "Une erreur a été détectée durant le processus de commandes SQL : %s\n", mysql_error(mysql));
    mysql_close(mysql);
    exit(1);
}


//------------------------------------------------------------------
void connect_into_mysql(char *register_file_path, char *log_file_path, MYSQL *mysql, char *IP, char *db_username, char *db_pwd, char *db_name)
{

    // Initialisation bibliotheque mysql
    if (mysql_library_init(0, NULL, NULL) == 0)
    {
        mysql = mysql_init(NULL);
        if (mysql != NULL) // Si le pointeur est bien initialisé
        {
            printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal
            fprintf(stdout, "[OK] la fonction mysql_init est bien prise en compte\n");
            sleep(1);
            
            // Connexion au serveur mysql
            printf("\n");
            printf("[MISE EN RELATION AVEC LA BASE DE DONNÉES]\n");
            sleep(1);
            if (mysql_real_connect(mysql, IP, db_username, db_pwd, "", 0, NULL, 0) != NULL)
            // Syntaxe : mysql_real_connect(mysql, IP,username,password,db); -> tentative de connection à la DB
            {
                fprintf(stdout, "[OK] Les identifiants depuis le fichier Register.txt ont été reconnus par la base MySQL\n \n");
                sleep(1);

                // DEBUT DES REQUÊTES MYSQL

                // Création de la table "c_project"
                if (mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS c_project"))
                {
                    finish_with_error(mysql);
                }
                puts("[REQUÊTES SUR LA BASE DE DONNÉES]");
                sleep(2);
                printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal

                // On entre dans la base de données que l'on vient de créer
                if (mysql_query(mysql, "USE c_project"))
                {
                    finish_with_error(mysql);
                }

                if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS ARTICLES (id_article INT NOT NULL AUTO_INCREMENT,title TEXT NOT NULL,price VARCHAR(10),stars VARCHAR(10),nb_eval VARCHAR(10),PRIMARY KEY(id_article));"))
                // Commande pour créer la table produit si elle n'existe pas avec les colonnes ID, titre, prix, étoiles, nb_eval
                {
                    finish_with_error(mysql);
                }

                if (mysql_query(mysql, "SELECT * FROM ARTICLES"))
                {
                    finish_with_error(mysql);
                }

                MYSQL_RES *result = mysql_store_result(mysql);

                if (result == NULL)
                {
                    finish_with_error(mysql);
                }

                int num_fields = mysql_num_fields(result);

                MYSQL_ROW row;

                while ((row = mysql_fetch_row(result)))
                {
                    puts("La dernière fois que vous aviez lancé le code, vous aviez tracké ceci :\n");
                    sleep(1);
                    printf("ID dans la table:%s \n", row[0] ? row[0] : "NULL");
                    printf("Titre :%s\n", row[1] ? row[1] : "NULL");
                    printf("Prix :%s \n", row[2] ? row[2] : "NULL");
                    printf("Nombre d'évaluations :%s", row[3] ? row[3] : "NULL");
                    printf("Nombre d'étoiles :%s \n", row[4] ? row[4] : "NULL");
                    printf("\n");
                }

                mysql_free_result(result);
                //puts("Nous utiliserons la base de données créée afin d'y insérer les données trouvées.");

                puts("D'après votre demande, voici ce que l'API à trouvé pour vous !\n");
                sleep(3);
                // On affiche les 10 premiers résultats de la liste trouvée (titre, prix, avis, nombre d'avis)
                FILE *read_file = fopen(log_file_path, "r");
                if (read_file == NULL)
                {
                    fprintf(stderr, "Le fichier n'a pas pu être lu.");
                    return;
                }

                // DÉBUT DE L'AFFICHAGE EN LIGNE DE COMMADE
                while (fgets(file_reader, sizeof(file_reader), read_file) && count_search < 11)
                {

                    // RECUPERATION DE L'ID (+ Affichage de l'ID de l'article)
                    if (strstr(file_reader, "product_id") != NULL) // SI IL TROUVE LA CHAINE "product_id" ALORS IL EXECUTE LE TRAITEMENT
                    {
                        printf("Article N°%d", count_search);
                        puts("\n--------------------------------------");
                    }

                    // RECUPERATION DU TITRE (+ Processus de développement vers un tableau)
                    if (strstr(file_reader, "title") != NULL) // SI IL TROUVE LA CHAINE "title" ALORS IL EXECUTE LE TRAITEMENT
                    {
                        memcpy(title[i], file_reader + 15, sizeof(file_reader)); // OPTIMISATION DE LA VARIABLE POUR N'AVOIR QUE CE QUI NOUS INTERESSE
                        // BOUCLE DE REMPLACEMENT DE CARACTERES
                        while (replace = strchr(title[i], ','))
                        {
                            *replace = ' ';
                        }
                        printf("Titre de l'article : %s\n", title[i]);
                    }

                    // RECUPERATION DU PRIX (+ Processus de développement vers un tableau)
                    if (strstr(file_reader, "price") != NULL) // SI IL TROUVE LA CHAINE "price" ALORS IL EXECUTE LE TRAITEMENT
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
                        printf("Prix : %d,%d$\n", before_coma, after_coma);
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
                        printf("Nombre d'évaluations : %s", num_rates[i]);
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
                        printf("Nombre d'étoiles : %s\n", stars[i]);
                        count_search++;
                        i++;
                    }

                    // SI LA FIN DU FICHIER EST ATTEINTE, ON LE PRÉCISE DANS LE TERMINAL
                    if (feof(read_file))
                    {
                        puts("\nLe dernier article trouvé par notre API a été atteint.\n");
                    }
                }

                // ON DEMANDE À L'UTILISATEUR QUEL ARTICLE IL VEUT SELECTIONNER

                sleep(1);
                unsigned int id_article = ask_article();
                sleep(1);

                // ON AFFICHE LES DONNÉES POUR CONFIRMER CE QUE L'ON A SELECTIONNÉ
                printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal (clear)
                sleep(1);
                printf("Vous avez demandé l'article N°%u\n", id_article);
                sleep(1);
                puts("\nIl contient les informations suivantes :");
                sleep(1);
                printf("Titre : %s \n", title[id_article - 1]);
                sleep(1);
                printf("Prix : %s$ \n", price[id_article - 1]);
                sleep(1);
                printf("Nombre d'évaluations : %s", num_rates[id_article - 1]);
                sleep(1);
                printf("Nombre d'étoiles : %s \n", stars[id_article-1]);

                // COMMANDE POUR INSÉRER LES DONNEES DANS LES COLONNES CORRESPONDANTES DE LA TABLE c_project
                sprintf(query, "INSERT INTO ARTICLES (title, price, stars, nb_eval) VALUES ('%s', '%s', '%s', '%s');", title[id_article - 1], price[id_article - 1], stars[id_article - 1], num_rates[id_article - 1]);
                printf("Requête générée, veuillez vérifier dans votre base de données");
                puts("");
                puts("");

                puts("L'application s'arrêtera dans 5 secondes");
                sleep(5);
                printf("\e[1;1H\e[2J"); // Ctrl+L sur terminal (clear)
                
                sleep(1);
                //printf("Requête générée  : %s",query);
                if (query == NULL){
                    puts(mysql_error(mysql));
                    }
                mysql_query(mysql, query);
                exit(0);

                // FIN DES REQUÊTES MYSQL
            }
            else
            {
                fprintf(stderr, "[ERR] une erreur empêche la fonction mysql_real_connect de fonctionner : '%s'\n", mysql_error(mysql));
                exit(EXIT_FAILURE);
            }

            // Fermeture de la connexion / Liberation memoire mysql
            mysql_close(mysql);
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
}