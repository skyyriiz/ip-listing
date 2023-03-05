#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <unistd.h>


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

void displayIpAddresses(int format, const char* mask, MYSQL *connection){
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
    char buf[64] = {};
    char query_string[] = {
            "INSERT INTO ips(ip) VALUES('%s')"
    };
    sprintf(buf, query_string, ip_address);
    if (mysql_query(connection, buf)) {
        fprintf(stderr, "Insertion failed");
        exit(1);
    }
}

void editIpFromDatabase(int id_address, char ip_address[], MYSQL *connection){
    char buf[128] = {};
    char query_string[] = {
            "UPDATE ips SET ip='%1$s' WHERE id=%2$d"
    };
    sprintf(buf, query_string, ip_address, id_address);
    if (mysql_query(connection, buf)) {
        fprintf(stderr, "Modification failed");
        exit(1);
    }
}

void menuEditIpFromDatabase(int temp_int, char *temp_string, MYSQL *connection){
    system("clear");
    printf("Veuillez saisir l'ID de l'adresse que vous souhaitez modifier:");

    fflush(stdin);
    scanf("%d", &temp_int);

    int id_database_address = temp_int;

    printf("\nVeuillez saisir la modification que vous souhaitez apporter:");

    fflush(stdin);
    scanf("%s", temp_string);

    editIpFromDatabase(id_database_address, temp_string, connection);
}

void deleteIpFromDatabase(int id_address, MYSQL *connection){
    char buf[64] = {};
    char query_string[] = {
            "DELETE FROM ips WHERE id=%d"
    };
    sprintf(buf, query_string, id_address);
    if (mysql_query(connection, buf)) {
        fprintf(stderr, "Delete failed");
        exit(1);
    }
}

void menuDeleteIpFromDatabase(int temp_id, MYSQL *connection){
    system("clear");
    printf("Veuillez saisir l'ID de l'adresse IP que vous souhaitez supprimer:");
    fflush(stdin);
    scanf("%d", &temp_id);
    deleteIpFromDatabase(temp_id, connection);
    system("clear");
    printf("L'adresse a bien été supprimée, retour au menu principal...");
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
        printf("4. Modifier une adresses IP\n");
        printf("5. Quitter\n");
        printf("\n\n>");

        fflush(stdin);
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                system("clear");
                printf("Veuillez saisir une adresse ip:");
                char ip[32];
                scanf("%s", ip);
                ip[strcspn(ip, "\n")] = 0;
                insertIpToDatabase(ip, conn);

                printf("Insertion réussie\n");
                break;

            case 2:
                menuDeleteIpFromDatabase(choix, conn);
                break;

            case 3:
                system("clear");
                printf("Sous quel format voulez-vous les afficher ?\n");
                printf("Faites votre choix:\n");
                printf("1. Sous forme binaire\n");
                printf("2. Sous forme hexadecimale\n");
                printf("3. Sous forme décimale\n");
                printf("4. Quitter\n");
                printf("\n\n>");



                fflush(stdin);
                scanf("%d", &choix);

                system("clear");
                printf("Insérez un masque pour filtrer (ou 0 si aucun):");

                fflush(stdin);
                char choix2[15];
                scanf("%s", choix2);
                choix2[strcspn(choix2, "\n")] = 0;
                displayIpAddresses(choix, choix2, conn);

                fflush(stdin);
                printf("\n\n\nQue voulez vous faire ?\n");
                printf("0. Revenir au menu principal\n");
                printf("1. Modifier une adresse ip\n");
                printf("2. Supprimer une adresse ip");
                printf("\n\n>");
                fflush(stdin);
                scanf("%d", &choix);
                switch (choix) {
                    case 0:
                        break;
                    case 1:
                        menuEditIpFromDatabase(choix, choix2, conn);
                        break;
                    case 2:
                        menuDeleteIpFromDatabase(choix, conn);
                        break;

                    default:
                        system("clear");
                        printf("Choix invalide\n");
                        break;
                }
                break;


            case 4:
                menuEditIpFromDatabase(choix, choix2, conn);
                break;

            case 5:
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
