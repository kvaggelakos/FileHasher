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
    
    if (size < 20) {
        printf("There is no use in using sizes lesser than 20!\nExiting...\n");
        exit(1);
    }
    
    // Start by connecting to the database
    database_init(&conn, server, port, user, password, database);
    
    // Traverse through the specified path
    tree(argv[1]);
    
    // Close the database connection
    database_close(conn);
    
    return EXIT_SUCCESS;
}

/**
 * Traverses 
 * @param path
 */
void tree(char * path) {
    
    struct dirent * info[MAX_DIRECTORY_SIZE];
    char newpath[MAX_PATH_SIZE];
    int found, i, file_id;
    
    found = getdirs(path, info);
    for (i = 0; i < found; i++) {
        sprintf(newpath, "%s/%s", path, info[i]->d_name);
        printf("[INFO] Path: %s\n", newpath);
        tree(newpath);
    }
    
    found = getfiles(path, info);
    for (i = 0; i < found; i++) {
        sprintf(newpath, "%s/%s", path, info[i]->d_name);
        
        // Add file to the database
        file_id = add_file(conn, newpath);
        
        // Check if it was added correctly
        if (file_id > 0) {
            // Debug print
            printf("[INFO] Adding file: %s to database with id: %i\n", newpath, file_id);
            
            // Add hashes for that file
            hash(newpath, file_id);
        } else {
            fprintf(stderr, "[ERROR] Couldn't add file: %s to database\n", newpath);
        }
    }
}


void hash(char * file, int file_id) {
    FILE *fp;
    unsigned char block[size];
    unsigned char md[SHA_DIGEST_LENGTH];
    int i=0;
    
    // Open file with read-only
    fp = fopen(file, "r");
    
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] The file: %s, didn't exist or couldn't be read\n", file);
    }

    while (fread(block, size, 1, fp) > 0) {
        // Hash the block
        SHA1(block, sizeof(block), md);
        
        if (add_hash(conn, size, md, file_id, i) > 0) {
            printf("[INFO] Added block nr: %i to database for file: %s\n", i, file);
        } else {
            fprintf(stderr, "Couldn't add block nr: %i to database for file: %s\n", i, file);
        }
    }
    
    // Close the file
    fclose(fp);
}

