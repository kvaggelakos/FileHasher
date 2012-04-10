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

    if (argc != 3) {
        printf("Usage: %s [PATH] [SIZE]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (!opendir(argv[1])) {
        printf("The path specified doesn't exist!\n");
        exit(1);
    }
    
    // Set the size
    size = atoi(argv[2]);
    
    if (size < 20) {
        printf("There is no use in using sizes lesser than 20!\nExiting...\n");
        exit(1);
    }
     
    // Start by connecting to the database
    database_init(&conn, server, port, user, password, database);
    
    // Tree through the specified path
    tree(argv[1]);
    
    // Close the database connection
    database_close(conn);
    
    return EXIT_SUCCESS;
}


void tree(char * path) {
    
    struct dirent info[MAX_DIRECTORY_SIZE];
    char newpath[MAX_PATH_SIZE];
    int found, i, file_id;
    struct stat st;
    
    found = list_dirs(path, info);
    for (i = 0; i < found; i++) {
        remove_trailing_slash(path);
        sprintf(newpath, "%s/%s", path, info[i].d_name);
        printf("[INFO] Path: %s\n", newpath);
        tree(newpath);
    }
        
    found = list_files(path, info);
    for (i = 0; i < found; i++) {
        remove_trailing_slash(path);
        sprintf(newpath, "%s/%s", path, info[i].d_name);
        printf("[INFO] File: %s\n", newpath);

        // Check that the file is big enough to care about (filesize > size)
        stat(newpath, &st);
        if (st.st_size < size) {
            continue;
        }
        
        // Add file to the database
        file_id = add_file(conn, newpath);
        
        // Check if it was added correctly
        if (file_id) {
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
    char md_hex[SHA_DIGEST_LENGTH * 2]; // x2 since each has a XX representation
    char *md_hex_ptr;
    int read, i, j;
    
    // Open file with read-only
    fp = fopen(file, "r");
    
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] The file: %s, didn't exist or couldn't be read\n", file);
    }

    i = 0;
    while ((read = fread(block, size, 1, fp)) > 0) {
        // Reset the pointer
        md_hex_ptr = md_hex;
        
        // Hash the block
        SHA1(block, size, md);
        
        // Replace each char with its hexadecimal representation
        for (j = 0; j < SHA_DIGEST_LENGTH; j++) {
            sprintf(md_hex_ptr, "%02x", md[j]);
            md_hex_ptr += 2;
        }
        
        if (add_hash(conn, size, md_hex, file_id, i++)) {
#ifdef DEBUG
            printf("[INFO] Added block nr: %i to database for file: %s\n", i - 1, file);
#endif
        } else {
            fprintf(stderr, "Couldn't add block nr: %i to database for file: %s\n", i - 1, file);
        }
    }
    
    // Close the file
    fclose(fp);
}

void remove_trailing_slash(char * buf) {
    size_t size = strlen(buf) - 1;
    if (buf[size] == '/') {
        buf[size] = '\0';
    }
}

