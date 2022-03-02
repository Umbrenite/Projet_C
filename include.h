#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <wchar.h>
#include <locale.h>

char glade_file[PATH_MAX] = {0};

// constante créée par préprocesseur pour la partie graphique
#define GLADE_CONFIRMATION "../build/confirmation.glade" 
#define GLADE_ENTER "../build/enter.glade"
#define GLADE_ARTICLES "../build/articles.glade"

//Initialisation du chemin du fichier de log
char log_file_path[PATH_MAX] = "../texts/log.txt";

//Initialisation du chemin du fichier de register (Enregistrement des identifiants de connexion à la BDD)
char register_file_path[PATH_MAX] = "../texts/register.txt";

// INITIALISATION DES VARIABLES DE DATABASE
char IP[30], db_username[30], db_pwd[30], db_name[30];
MYSQL *mysql = NULL;

// Initialisation de la variable de modification de l'url
char url[101];

// Initialisation des variables pour l'affichage des articles en mode graphique
char file_reader[50];    // Lignes du fichier log
char title[10][50];      // Titre de l'article
char price[10][150];      // Prix de l'article
char stars[10][150];      // Avis de l'article (nombre d'étoiles) 
char num_rates[10][150];  // Nombre d'avis de l'article
char query[150];          // Pour l'insertion des données
int price_converted;      // Prix converti
int before_coma;          // Chiffres avant la virgule pour le prix
int after_coma;           // Chiffres après la virgule pour le prix
int count_search = 1;     // Compteur de recherches (10)
int i = 0;                // Boucle d'itérations
char *replace;            // Remplacement temporaire des valeurs

void removeLineFeed(char* str) {
    str[strcspn(str, "\n")] = '\0';
}

#include "terminal_functions.c"
#include "windows_create.c"