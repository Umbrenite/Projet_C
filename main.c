#include "windows_create.c"

int main(int argc, char **argv)
{
    /*int screen_mod = 0;
    printf("De quelle maniere voulez vous afficher votre tracker ? \n");
    printf("1 = A l'ecrit \n");
    printf("2 = Graphiquement \n");

    //DÉBUT DU CHOIX
    while (screen_mod != '1' && screen_mod != '2')
    {                             //Tant que la variable screen_mod est différente de 1 ET de 2
        screen_mod = 0;           //Reset de la variable screen mod à chaque itération
        scanf("%s", &screen_mod); //On demande à l'utilisateur de rentrer le chiffre 1 ou 2
        switch (screen_mod)       //On effectue un switch sur tous les cas possibles
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
    //FIN DU CHOIX

    //À L'ÉCRIT
    if (screen_mod == '1') {
        FILE *fptr;   
        char name[30];  
        fptr = fopen("/home/arthur/Projet_C/log.txt", "w");// open for writing
        if (fptr == NULL)  
        {  
            printf("Le fichier référencé n'existe pas. Créez-le puis relancez le code. \n");  
            return 0;  
        }      
        printf("Entrez le nom du produit que vous souhaitez tracer\n");  
        scanf("%s", name);  
        fprintf(fptr, "Lien demandé : https://www.amazon.fr/s?k=%s\n", name);  
        fclose(fptr);
    }
    //À L'ÉCRIT

    //GRAPHIQUEMENT
    if (screen_mod == '2') {
        GtkApplication *app;
        int status;

        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
        status = g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);

        return status;
    }
    //GRAPHIQUEMENT*/

    //MYSQL

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

char *server = "localhost";
char *user = "root";
char *password = "rootarthur";
char *database = "mysql";

conn = mysql_init(NULL);

/* Connect to database */
if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
{
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(0);
}

  return 0;

    //MYSQL

}
