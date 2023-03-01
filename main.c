#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

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

void displayIpAddresses(char format, const char* mask){
    MYSQL *conn = getDatabaseConnection();
    if(strcmp(mask, "0") != 0){
        printf("Mask filtering");
    }else{
        if (mysql_query(conn, "SELECT * FROM ips")) {
            fprintf(stderr, "Select failed");
            exit(1);
        }
        MYSQL_RES *result = mysql_store_result(conn);

        if (result == NULL) {
            fprintf(stderr, "Select failed");
            exit(1);
        }

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;

        while ((row = mysql_fetch_row(result))) {
            for(int i = 0; i < num_fields; i++) {
                printf("%s ", row[i] ? row[i] : "NULL");
            }
            printf("\n");
        }

        mysql_free_result(result);
    }
}

int main(int argc, char *argv[]) {
    int choix;
    bool loop = true;

    MYSQL *conn = getDatabaseConnection();
    connectToDatabase(conn);

    while(loop != false){
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
                printf("Ajouter une adresse IP\n");
                char ip[20];
                //scanf("%s", &ip);

                if (mysql_query(conn, ip)) {
                    fprintf(stderr, "Insertion failed");
                    exit(1);
                }

                printf("Insertion réussie\n");
                break;
            case 2:
                printf("Vous avez choisi de supprimer une adresse IP\n");
                break;
            case 3:
                //printf("Vous avez choisi d\'afficher les adresses IP\n");
                printf("Sous quel format voulez-vous les afficher ?\n");
                printf("Faites votre choix:\n");
                printf("1. Sous forme binaire\n");
                printf("2. Sous forme hexadecimale\n");
                printf("3. Sous forme décimale\n");
                printf("4. Quitter\n");

                char* choix2;

                fflush(stdin);
                scanf("%d", &choix);

                printf("Insérez un masque pour filtrer (ou 0 si aucun):");

                fflush(stdin);
                scanf("%s", &choix2);

                displayIpAddresses(choix, choix2);

                break;
            case 4:
                printf("Vous avez choisi de quitter\n");
                loop = false;
                break;
            default:
                printf("Choix invalide\n");
                break;
        }
    }


}
