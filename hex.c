#include <stdio.h>
#include <stdlib.h>

#include "hex.h"

void hex(char * dst, unsigned char * src, int size) {
    int i;
    char *tmp = dst;
    
    for (i = 0; i < size; i++) {
        sprintf(tmp, "%02x", src[i]);
        tmp += 2;
    }
}
