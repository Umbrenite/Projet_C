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

void removeLineFeed(char* str) {
    str[strcspn(str, "\n")] = '\0';
}

#include "terminal_functions.c"
#include "windows_create.c"