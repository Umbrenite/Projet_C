#include "include.h"

// test du commit

//DÉBUT DU SCRIPT
int main(int argc, char **argv)
{
//DEBUT CONFIGURATION DU SCRIPT
    puts("AVANT LE LANCEMENT DU SCRIPT, VEUILLEZ VÉRIFIER QUE LES FICHIERS SOIENT RÉFÉRENCÉS AUX BONS EMPLACEMENTS \n");

    //Initialisation du chemin du fichier de log
    char log_file_path[PATH_MAX] = {0};

    //Initialisation du chemin du fichier de register (Enregistrement des identifiants de connexion à la BDD)
    char register_file_path[PATH_MAX] = {0};

    //Initialisation du chemin du fichier Glade (Partie graphique)
    char glade_file[PATH_MAX] = {0};

    strcat(log_file_path, "../texts/log.txt");           //On concatène la chaîne de caractères "/texts/log.txt", cette partie étant statique
    strcat(register_file_path, "../texts/register.txt"); //On concatène la chaîne de caractères "/texts/register.txt", cette partie étant statique
    strcat(glade_file, "../test1.glade");                //On concatène la chaîne de caractères "/test1.glade", cette partie étant statique
    if (log_file_path)
    {                                                  //Si la variable HOME existe alors
        confirm_paths(log_file_path);                  //Fonction qui permet de demander à l'user de vérifier ses chemins de fichiers.
    }
    else
    {
        puts("Une erreur a été détectée dans votre fichier de log. Le script s'arrête ici.");
        return 0;
    }

    if (access(log_file_path, F_OK) == 0 && !strcmp(log_file_path + strlen(log_file_path) - 7, "log.txt"))
    {
        printf("\e[1;1H\e[2J"); //Clear l'écran
        FILE *clear = fopen(log_file_path, "w"); //On clear le fichier de log.txt avant toute intervention
        printf("Fichier de log trouvé\n \n");
    }
//FIN DE LA CONFIGURATION DU SCRIPT



//DEBUT SELECTION DU MODE D'ÉCRITURE
    char mod = select_mod();
//FIN SELECTION DU MODE D'ÉCRITURE



//DÉBUT DU CHOIX ÉCRIT
    if (mod == '1')
    {
        char *url = url_former();
        write_curl(log_file_path, url);

        if (url != "") {
        MYSQL *mysql = NULL;
        connect_into_mysql(register_file_path, log_file_path, mysql);
        }
    }
//FIN DU CHOIX ÉCRIT



//DÉBUT DU CHOIX GRAPHIQUE
    if (mod == '2')
    {   //Mise en place de la fenêtre paramétrée dans "windows_create.c"
        gtk_init(&argc, &argv); // initialise gtk et tous les paramètres qu'il reconnait/utilise dans le processus d'initialisation
        create_window(glade_file);
    }
//FIN DU CHOIX GRAPHIQUE
}
//FIN DU SCRIPT
