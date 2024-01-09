#include "networking.h"
#include <stdio.h>

void debug(char * message) {
    if (DEBUG == 1) {
        printf("%s",message);
    }
}