/* 
 * File:   main.c
 * Author: kostas_1989
 *
 * Created on March 31, 2012, 7:57 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <openssl/sha.h>
#include <string.h>
#include <dirent.h>


#include "filehasher.h"
#include "files.h"
#include "database.h"

char *server = "localhost";
int port = 8889;
char *user = "root";
char *password = "root"; /* set me first */
char *database = "FileHasher";

MYSQL *conn;


int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: %s [PATH]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Start by connecting to the database
    database_init(&conn, server, port, user, password, database);
    
    
    
    return EXIT_SUCCESS;
}





