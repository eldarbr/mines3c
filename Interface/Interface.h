#ifndef CLI_PROD
#define CLI_PROD


#include <stdio.h>
#include <stdlib.h>
#include "../MineField/MineField.h"


void ClearScreen(void);

void RepositionCursor(size_t, size_t);

void Exception(const char[], int);

void DrawMineField(const struct MineField *const, const bool zCoord);


#endif
