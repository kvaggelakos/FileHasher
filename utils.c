#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void remove_trailing_slash(char * buf) {
    size_t size = strlen(buf) - 1;
    if (buf[size] == '/') {
        buf[size] = '\0';
    }
}
