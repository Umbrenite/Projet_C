#include "include.h"

int main(int argc, char **argv)
{

    //setlocale(LC_ALL, "");
    //int *osef = L"Éé$&";
    //wprintf(L"%ls\n", osef);

// DÉBUT CONFIGURATION DU SCRIPT
    puts("AVANT LE LANCEMENT DU SCRIPT, VEUILLEZ VÉRIFIER QUE LES FICHIERS SOIENT RÉFÉRENCÉS AUX BONS EMPLACEMENTS \n");
      
    if (log_file_path)
    {
        confirm_paths(log_file_path); // Permet de demander à l'user de vérifier ses chemins de fichiers.
    }
    else
    {
        puts("Une erreur a été détectée dans votre fichier de log. Le script s'arrête ici.");
        return 0;
    }

    if (access(log_file_path, F_OK) == 0 && !strcmp(log_file_path + strlen(log_file_path) - 7, "log.txt"))
    {
        printf("\e[1;1H\e[2J"); // clear l'écran
        FILE *clear = fopen(log_file_path, "w"); // on clear le fichier de log.txt avant toute intervention
        if (clear == NULL)
        {
            puts("Erreur de traitement");
            exit(1);
        }
        fclose(clear);
        printf("Fichier de log trouvé\n \n");
    }
//FIN CONFIGURATION DU SCRIPT



//DEBUT SELECTION DU MODE D'ÉCRITURE
    char mod = select_mod();
//FIN SELECTION DU MODE D'ÉCRITURE



//DÉBUT DU CHOIX ÉCRIT
    if (mod == '1')
    {
        char *url = url_former();
        write_curl(log_file_path, url);

        if (strlen(url) > 0) {

            MYSQL *mysql = NULL;

            FILE *rec;
            rec = fopen(register_file_path, "r");
            puts("[DÉTÉCTION DES IDENTIFIANTS]");
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
            } else {
                auto_connect(IP, db_username, db_pwd, rec);
                printf("Ip : %s\n",IP);
                printf("Username : %s\n",db_username);
                printf("pass : %s\n",db_pwd);
            }
        connect_into_mysql(register_file_path, log_file_path, mysql, IP, db_username, db_pwd, db_name);
        }
        else 
        {
        puts("Une erreur a été détectée dans l'URL de l'API. Relancez-le programme.");
        return EXIT_FAILURE;
        }
    }
//FIN DU CHOIX ÉCRIT



// DÉBUT CHOIX GRAPHIQUE
    if (mod == '2')
    {
        gtk_init(&argc, &argv); // initialise gtk + tout paramètres qu'il reconnait/utilise dans processus d'initialisation
        page_confirmation();
    }
// FIN CHOIX GRAPHIQUE


}
