#ifndef CLI_PROD
#define CLI_PROD


#include <stdio.h>
#include <stdlib.h>
#include "../MineField/MineField.h"


void PauseExec(void);

void ClearScreen(void);

void ClearPartOfScreen(void);

void RepositionCursor(size_t, size_t);

void Exception(const char[], int);

void DrawMineField(const struct MineField *const mf, const bool zCoord);


int GetUserChoice(void);





void Greetings(void);

void AskForDifficulty(void);



#endif
