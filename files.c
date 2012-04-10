/* 
 * File:   files.c
 * Author: kostas_1989
 *
 * Created on March 31, 2012, 7:57 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "files.h"


/**
 * Get a list of files in the directory path
 * @param path The path to the directory where the files are stored
 * @param files An array of dirent pointers where the result will be stored
 * @return 0 if path didn't exist. Otherwise the number of files found.
 */
int list_files(char * path, struct dirent * files) {
    
    DIR *dir = opendir(path);
    int i = 0;
    struct dirent *tmp;
    
    // Check if path exists
    if (dir == NULL) {
        return 0;
    }
    
    while ((tmp = readdir(dir)) != NULL) {
        memcpy(files, tmp, sizeof(struct dirent));
        if (files->d_type != DT_DIR) {
            files++;
            i++;
        }
    }
    
    // Close the resources afterwards
    closedir(dir);
    
    // Return the amount of files in path
    return i;
} 


/**
 * Get a list of folders in the directory path
 * @param path The path to the directory where the folders are stored
 * @param dirs An array of dirent pointers where the result will be stored
 * @return 0 if the path didn't exist. Otherwise the number of folders found
 */
int list_dirs(char * path, struct dirent * dirs) {

    DIR *dir = opendir(path);
    int i = 0;
    struct dirent *tmp;
    
    // Check if path exists
    if (dir == NULL) {
        return 0;
    }

    while ((tmp = readdir(dir)) != NULL) {
        memcpy(dirs, tmp, sizeof(struct dirent));
        if (dirs->d_type == DT_DIR &&
                strcmp(dirs->d_name, ".") != 0 &&
                strcmp(dirs->d_name, "..") != 0) {
            dirs++;
            i++;
        }
    }
    
    // Close the resources afterwards
    closedir(dir);
    
    // Return the amount of files in path
    return i;
} 
