/* 
 * File:   main.c
 * Author: kostas_1989
 *
 * Created on March 31, 2012, 7:57 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


#include "filehasher.h"
#include "files.h"
#include "database.h"

char *server = "localhost";
int port = 8889;
char *user = "root";
char *password = "root"; /* set me first */
char *database = "FileHasher";

MYSQL *conn;

int size;


int main(int argc, char* argv[]) {

    if (argc != 4) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    
    if (!opendir(argv[2])) {
        printf("The path specified doesn't exist!\n");
        exit(1);
    }
    
    // Set the size
    size = atoi(argv[3]);
    
    if (size < 20) {
        printf("There is no use in using sizes lesser than 20!\nExiting...\n");
        exit(1);
    }

    if (!strcmp(argv[1], "database")) {
        // Start by connecting to the database
        database_init(&conn, server, port, user, password, database);

        // Tree through the specified path
        tree(argv[2], size, conn);

        // Close the database connection
        database_close(conn);
    } else if (!strcmp(argv[1], "terminal")) {
        //Todo: implement this
        printf("Not implemented yet, exiting...\n");
        exit(0);
    } else {
        usage(argv[0]);
    }
    
    
    return EXIT_SUCCESS;
}

void usage(char * argv0) {
    printf("Usage: %s [MODE] [PATH] [SIZE]\n", argv0);
}

