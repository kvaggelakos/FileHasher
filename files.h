/* 
 * File:   files.h
 * Author: kostas_1989
 *
 * Created on March 31, 2012, 10:30 PM
 */

#ifndef FILES_H
#define	FILES_H

#define MAX_DIRECTORY_SIZE 200
#define MAX_PATH_SIZE 150

int getfiles(char * path, struct dirent ** files);
int getdirs(char * path, struct dirent ** dirs);

#endif	/* FILES_H */

