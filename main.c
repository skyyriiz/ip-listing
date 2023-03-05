#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>


void loadEnv(){
    FILE * fp = fopen(".env", "r");
    char buffer[256];
    while(fgets(buffer, sizeof(buffer), fp)){
        char *key = strtok(buffer, "=");
        char *value = strtok(NULL, "=");
        value[strcspn(value, "\n")] = 0;
        setenv(key, value, 0);
    }

}

MYSQL *getDatabaseConnection(){
    MYSQL *conn;

    if ((conn = mysql_init(NULL)) == NULL) {
        fprintf(stderr, "Initialisation failed");
        exit(1);
    }
    return conn;
}

void connectToDatabase(MYSQL *connection){
    if(mysql_real_connect(connection, getenv("DB_HOST"), getenv("DB_USER"), getenv("DB_PASSWORD"), getenv("DB_NAME"), 0, NULL, 0) == NULL) {
        fprintf(stderr, "Connection failed");
        exit(1);
    }
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void displayIpAddresses(int format, const char* mask, MYSQL *connection){
    int part;
    int y = 0;

    if(strcmp(mask, "0") != 0){
        printf("Mask filtering");
    }else{
        if (mysql_query(connection, "SELECT * FROM ips")) {
            fprintf(stderr, "Select failed");
            exit(1);
        }
        MYSQL_RES *result = mysql_store_result(connection);

        if (result == NULL) {
            fprintf(stderr, "Select failed");
            exit(1);
        }

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;

        system("clear");
        printf("Voici votre liste d'adresse ip:\n");
        
        switch(format){
            case 1:
                while ((row = mysql_fetch_row(result))) {
                    for(int i = 0; i < num_fields; i++) {
                        printf("%s ", row[i] ? row[i] : "NULL");

                        if (strlen(row[i]) > 2) {
                            char **parts = str_split(row[i], '.');
                            char finals;
                            char *stock[200];
                            char temp;
                            if (parts)
                            {
                                int i;
                                for (i = 0; *(parts + i); i++)
                                {
                                    part = atoi(*(parts + i));
                                    printf("%d", part);

                                

                                    stock[y] = *(parts + i);
                                    y++;
                                }
                                
                                free(parts);
                            }
                            
                            
                        }   
                                    
                    }
                    printf("\n");
                }
                mysql_free_result(result);
                break;
            case 2:
                while ((row = mysql_fetch_row(result))) {
                    for(int i = 0; i < num_fields; i++) {
                        printf("%s ", row[i] ? row[i] : "NULL");

                        if (strlen(row[i]) > 2) {
                            char **parts = str_split(row[i], '.');
                            char finals;
                            if (parts)
                            {
                                int i;
                                for (i = 0; *(parts + i); i++)
                                {
                                    int part = atoi(*(parts + i));
                                    printf("%X", part);
                                }
                                free(parts);
                            }
                        }
                    }
                    printf("\n");
                }
                mysql_free_result(result);
                break;
            case 3:
                printf("Vous avez choisi d'afficher les adresses IP en décimal");
                break;
            case 4:
                printf("Vous avez choisi de quitter");
                break;
            default:
                printf("Choix invalide");
                break;
        }

        while ((row = mysql_fetch_row(result))) {
            for(int i = 0; i < num_fields; i++) {
                printf("%s ", row[i] ? row[i] : "NULL");
            }
            printf("\n");
        }

        mysql_free_result(result);
    }
}

void insertIpToDatabase(char ip_address[], MYSQL *connection){
    char buf[512] = {};
    char query_string[] = {
            "INSERT INTO ips(ip) VALUES(%s)"
    };
    sprintf(buf, query_string, ip_address);
    if (mysql_query(connection, buf)) {
        fprintf(stderr, "Insertion failed");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int choix;
    bool loop = true;

    loadEnv();

    MYSQL *conn = getDatabaseConnection();
    connectToDatabase(conn);

    while(loop != false){
        system("clear");
        printf("Bienvenue dans le répertoire d\'adresses IP\n");
        printf("Faites votre choix:\n");
        printf("1. Ajouter une adresse IP\n");
        printf("2. Supprimer une adresse IP\n");
        printf("3. Afficher les adresses IP\n");
        printf("4. Quitter\n");

        fflush(stdin);
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                system("clear");
                printf("Veuillez saisir une adresse ip:");
                char ip[64];
                scanf("%s", ip);
                ip[strcspn(ip, "\n")] = 0;
                insertIpToDatabase(ip, conn);

                printf("Insertion réussie\n");
                break;
            case 2:
                system("clear");
                printf("Vous avez choisi de supprimer une adresse IP\n");
                break;
            case 3:
                //printf("Vous avez choisi d\'afficher les adresses IP\n");
                system("clear");
                printf("Sous quel format voulez-vous les afficher ?\n");
                printf("Faites votre choix:\n");
                printf("1. Sous forme binaire\n");
                printf("2. Sous forme hexadecimale\n");
                printf("3. Sous forme décimale\n");
                printf("4. Quitter\n");


                int choix1;
                fflush(stdin);
                scanf("%d", &choix1);

                system("clear");
                printf("Insérez un masque pour filtrer (ou 0 si aucun):");

                fflush(stdin);
                char choix2[15];
                scanf("%s", choix2);

                displayIpAddresses(choix1, choix2, conn);

                sleep(10);
                //WIP scanf pour savoir quand quitter le menu d'affichage
                break;
            case 4:
                system("clear");
                printf("Vous avez choisi de quitter\n");
                loop = false;
                break;
            default:
                system("clear");
                printf("Choix invalide\n");
                break;
        }
    }


}
