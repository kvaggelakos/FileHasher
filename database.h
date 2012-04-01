/* 
 * File:   database.h
 * Author: kostas_1989
 *
 * Created on April 1, 2012, 12:14 AM
 */

#ifndef DATABASE_H
#define	DATABASE_H

#define TABLE_FILES "files"
#define TABLE_HASHES "hashes"

void database_init(MYSQL ** conn, char * address, int port, char * user, char * password,
        char * database);
void database_close(MYSQL * conn);
int add_file(MYSQL * conn, char * filename);
int add_hash(MYSQL * conn, int size, char * hash, int file, int block);


#endif	/* DATABASE_H */

