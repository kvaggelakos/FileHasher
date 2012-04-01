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

    // TODO: Change argc check
    if (argc != 1) {
        printf("Usage: %s [PATH] [SIZE]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Start by connecting to the database
    //database_init(&conn, server, port, user, password, database);
    
    traverse("/tmp/hash");
    
    
    return EXIT_SUCCESS;
}


void traverse(char * path) {
    
    struct dirent * info[MAX_DIRECTORY_SIZE];
    char newpath[MAX_PATH_SIZE];
    int found, i;
    
    found = getdirs(path, info);
    for (i = 0; i < found; i++) {
        sprintf(newpath, "%s/%s", path, info[i]->d_name);
        printf("Path: %s\n", newpath);
        traverse(newpath);
    }
    
    found = getfiles(path, info);
    for (i = 0; i < found; i++) {
        sprintf(newpath, "%s/%s", path, info[i]->d_name);
        printf("File: %s\n", newpath);
    }
    
}


