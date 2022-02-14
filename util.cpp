#include <stdio.h>
#include "util.h"
#include <stdlib.h>

void errif(bool condition, const char* message){
    if(condition){
        perror(message);
        exit(EXIT_FAILURE);
    }
}