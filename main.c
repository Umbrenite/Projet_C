#include "windows_create.c"
#include "include.c"
#include "terminal_functions.c"

//DÉBUT DU SCRIPT
int main(int argc, char **argv)
{
//CONFIGURATION DU SCRIPT
    puts("AVANT LE LANCEMENT DU SCRIPT, VEUILLEZ VÉRIFIER QUE LES FICHIERS SOIENT RÉFÉRENCÉS AUX BONS EMPLACEMENTS \n");

    //Initialisation du chemin du fichier de log
    char log_file_path[PATH_MAX];
    getcwd(log_file_path, sizeof(log_file_path)); //On définit le chemin de l'user avec getcwd qui définit où on se trouve

    //Initialisation du chemin du fichier de register (Enregistrement des identifiants de connexion à la BDD)
    char register_file_path[PATH_MAX];
    getcwd(register_file_path, sizeof(register_file_path)); //On définit le chemin de l'user avec getcwd qui définit où on se trouve

    char glade_file[PATH_MAX];
    getcwd(glade_file, sizeof(glade_file)); //On définit le chemin de l'user avec getcwd qui définit où on se trouve

    strcat(log_file_path, "/texts/log.txt");           //On concatène la chaîne de caractères "/texts/log.txt", cette partie étant statique
    strcat(register_file_path, "/texts/register.txt"); //On concatène la chaîne de caractères "/texts/register.txt", cette partie étant statique
    strcat(glade_file, "/test1.glade");
    if (log_file_path)
    {                                                                                                                 //Si la variable HOME existe alors
        confirm_paths(log_file_path);
    }
    else
    {
        puts("Un problème dans votre fichier de log a été détecté.");
        return 0;
    }

    if (access(log_file_path, F_OK) == 0 && !strcmp(log_file_path + strlen(log_file_path) - 7, "log.txt"))
    {
        printf("\e[1;1H\e[2J"); //Clear l'écran
        printf("Fichier de log trouvé\n \n");
    }
//FIN DE LA CONFIGURATION DU SCRIPT

//SELECTION DU MODE D'ÉCRITURE
    char mod = select_mod();


//DÉBUT DU CHOIX ÉCRIT
    if (mod == '1')
    {
        
        char *url = connect_into_mysql(register_file_path, log_file_path);
        html_curl(log_file_path, url);
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
