/* 
 * File:   files.h
 * Author: kostas_1989
 *
 * Created on March 31, 2012, 10:30 PM
 */

#ifndef FILES_H
#define	FILES_H

#define MAX_DIRECTORY_SIZE 500
#define MAX_PATH_SIZE 1000

int list_files(char * path, struct dirent * files);
int list_dirs(char * path, struct dirent * dirs);
void tree(char * path, int size, MYSQL * conn);

#endif	/* FILES_H */

