#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>



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

void TypeIP(MYSQL *connection){
    char buf[512] = {};
    int Database_ID_Addr; 
    printf("Select l'ID que vous voulez voir \n");
    scanf("%d", &Database_ID_Addr);
    char GetIP[] = {
            "Select ip from ips where id = %d"
    };
    sprintf(buf, GetIP, Database_ID_Addr);
    if (mysql_query(connection, buf)) {
        fprintf(stderr, "Insertion failed");
        exit(1);
    }
    int First_Octet = atoi (strtok ( buf, "." ));
    int Second_Octet = atoi (strtok ( NULL, "." ));
    printf("%d", First_Octet);
    printf("%d", Second_Octet);
    if(First_Octet ==10){
	printf("this IP is private");
    }
    if(First_Octet ==172){
	if(Second_Octet >= 0 && Second_Octet <= 31){
    			printf("this IP is private");
    		}else{
    			printf("this IP is public");
    		}
    }
    if(First_Octet ==168){
	if(Second_Octet >= 0 && Second_Octet <= 31){
    		printf("this IP is private");
    	}else{
    		printf("this IP is public");
    	}
    }
}

int main(int argc, char *argv[]) {
    int choix;
    bool loop = 1;

    loadEnv();

    MYSQL *conn = getDatabaseConnection();
    connectToDatabase(conn);

    while(loop != 0){
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



                fflush(stdin);
                scanf("%d", &choix);

                system("clear");
                printf("Insérez un masque pour filtrer (ou 0 si aucun):");

                fflush(stdin);
                char choix2[15];
                scanf("%s", choix2);

                displayIpAddresses(choix, choix2, conn);
                
                //WIP scanf pour savoir quand quitter le menu d'affichage
                break;
            case 4:
                system("clear");
                printf("Vous avez choisi de quitter\n");
                loop = 1;
                break;
            default:
                system("clear");
                printf("Choix invalide\n");
                break;
        }
    }


}
