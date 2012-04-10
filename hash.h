/* 
 * File:   hash.h
 * Author: kostas_1989
 *
 * Created on April 10, 2012, 11:07 PM
 */

#ifndef HASH_H
#define	HASH_H

void hex(char * dst, unsigned char * src, int size);
void hash(char * file, int file_id, int size, MYSQL * conn);

#endif	/* HASHH_ */

