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

int size;


int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Usage: %s [PATH] [SIZE]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Set the size
    size = atoi(argv[2]);
    
    // Start by connecting to the database
    database_init(&conn, server, port, user, password, database);
    
    // Traverse through the specified path
    traverse(argv[1]);
    
    
    return EXIT_SUCCESS;
}


void traverse(char * path) {
    
    struct dirent * info[MAX_DIRECTORY_SIZE];
    char newpath[MAX_PATH_SIZE];
    int found, i, file_id;
    
    found = getdirs(path, info);
    for (i = 0; i < found; i++) {
        sprintf(newpath, "%s/%s", path, info[i]->d_name);
        printf("[INFO] Path: %s\n", newpath);
        traverse(newpath);
    }
    
    found = getfiles(path, info);
    for (i = 0; i < found; i++) {
        sprintf(newpath, "%s/%s", path, info[i]->d_name);
        file_id = add_file(conn, newpath);
        if (file_id <= 0) {
            fprintf(stderr, "[ERROR] Couldn't add file: %s to database\n", newpath);
        } else {
            printf("[INFO] Adding file: %s to database with id: %i\n", newpath, file_id);
        }
    }
    
}


