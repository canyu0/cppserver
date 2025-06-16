#include "util.h"
#include <cstdio>
#include <cstdlib>
void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}
void errif(int condition, const char *errmsg){
    if(condition<0){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}
int err1 = 0; // Initialize the global error variable