#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <mysql.h>

#include "hash.h"
#include "database.h"

void hex(char * dst, unsigned char * src, int size) {
    int i;
    char *tmp = dst;
    
    for (i = 0; i < size; i++) {
        sprintf(tmp, "%02x", src[i]);
        tmp += 2;
    }
}

void hash(char * file, int file_id, int size, MYSQL * conn) {
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