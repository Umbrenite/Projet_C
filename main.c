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

    if (screen_mod == '2') {
        GtkApplication *app;
        int status;

        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
        status = g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);

        return status;
    }*/
    FILE *fptr;  
    int id;  
    char name[30];  
    float salary;  
    fptr = fopen("/home/arthur/Projet/BDD.txt", "w+");/*  open for writing */  
    if (fptr == NULL)  
    {  
        printf("File does not exists \n");  
        return;  
    }  
    printf("Enter the id\n");  
    scanf("%d", &id);  
    fprintf(fptr, "Id= %d\n", id);  
    printf("Enter the name \n");  
    scanf("%s", name);  
    fprintf(fptr, "Name= %s\n", name);  
    printf("Enter the salary\n");  
    scanf("%f", &salary);  
    fprintf(fptr, "Salary= %.2f\n", salary);  
    fclose(fptr);  
}
