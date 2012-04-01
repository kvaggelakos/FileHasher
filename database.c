#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

#include "database.h"


/**
 * Initializes and connects to the database specified
 * @param conn The mysql handle
 * @param address The address to the mysql-server
 * @param user Username to the mysql-server
 * @param password Password to the mysql-server
 * @param database Database name to use
 * @param port The port to the mysql-server
 */
void database_init(MYSQL ** conn, char * address, int port, char * user, char * password,
        char * database) {
    // Init conn
    *conn = mysql_init(NULL);
    
    // Try to connect
    if (!mysql_real_connect(*conn, address, user, password, database, port, NULL, 0)) {
        fprintf(stderr, "There was an error connecting to the database: %s\n", mysql_error(*conn));
        exit(1);
    }
}

/**
 * Closes the database connection
 * @param conn The connection to close
 */
void database_close(MYSQL * conn) {
    mysql_close(conn);
}


/**
 * Adds an entry to the file table
 * @param conn The connection to the database
 * @param path The path to the file to store
 * @return -1 if there was an error with the connection or query. Otherwise a positive 
 * integer containing the id of the newly inserted object.
 */
int add_file(MYSQL * conn, char * path) {
    
    if (conn == NULL) {
        return -1;
    }
    
    char query[100];
    sprintf(query, "INSERT INTO %s (path) VALUES ('%s')", TABLE_FILES, path);
    printf("Query: %s\n", query);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "There was an error adding the file: %s to the database.\nError: %s\n",
                path, mysql_error(conn));
        return -1;
    }
    
    // Return the id
    return mysql_insert_id(conn);
}


/**
 * Adds a hash to the database
 * @param conn The mysql connection to use
 * @param size The size of the blocks
 * @param hash The generated hash
 * @param file The file id to which the hash belongs
 * @param block The block id within the file
 * @return -1 if there was an error with the connection or query. Otherwise a positive 
 * integer containing the id of the newly inserted object.
 */
int add_hash(MYSQL * conn, int size, char * hash, int file, int block) {
    
    if (conn == NULL) {
        return -1;
    }
    
    char query[300];
    sprintf(query, "INSERT INTO %s (size, hash, file, block) VALUES ('%i', '%s', '%i', '%i')",
            TABLE_HASHES, size, hash, file, block);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "There was an error adding the hash for file: %i\nError:%s\n",
                file, mysql_error(conn));
        return -1;
    }
    
    // Return the id
    return mysql_insert_id(conn);
}
