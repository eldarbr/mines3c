#ifndef CLI_PROD
#define CLI_PROD


#include <stdio.h>
#include <stdlib.h>


void ClearScreen(void);

void RepositionCursor(size_t, size_t);

void Exception(const char[], int);


#endif
