#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

int main(int argc, char *argv[]) {
    MYSQL *conn;
    int choix;
    bool loop = true;

    if ((conn = mysql_init(NULL)) == NULL) {
        fprintf(stderr, "Initialisation failed");
        exit(1);
    }

    if(mysql_real_connect(conn, "localhost", "mike", "lolmdr123", "ip", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Connection failed");
        exit(1);
    }

    while(loop != false){
        printf("Bienvenu dans le répertoire d\'adresses IP\n");
        printf("Faites votre choix:\n");
        printf("1. Ajouter une adresse IP\n");
        printf("2. Supprimer une adresse IP\n");
        printf("3. Afficher les adresses IP\n");
        printf("4. Quitter\n");

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
                printf("Vous avez choisi d\'afficher les adresses IP\n");

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
