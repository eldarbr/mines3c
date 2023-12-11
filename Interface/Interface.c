#include "Interface.h"


void ClearScreen(void) {
    RepositionCursor(1, 1);
    ClearPartOfScreen();
}


void ClearPartOfScreen(void) {
    printf("\033[J");
}


/*
*   Cursor goes to the desired x,y coords (1-based)
*   if x equals 0, only moves the cursosr to y in the same line
*/
void RepositionCursor(size_t x, size_t y) {
    if (x < 0 || y < 0) {
        Exception("ERR: illegal argument (Interface.RepositionCursor)", 1);
    }
    if (x == 0) {
        printf("\033[;%zuH", y);
    }
    printf("\033[%zu;%zuH", x, y);
}


/*
*   Print the text and exit from the program
*/
void Exception(const char message[], int q) {
    printf("%s", message);
    exit(q);
}


/*
*   Output the z-state and draw the field, coordinate system
*   Respects the masking of the field and the user marks
*   Clears the screen then outputs starting from the first line
*/
void DrawMineField(const struct MineField *const mf, const bool zCoord) {
    ClearScreen();

    const size_t fieldSize = mf->fieldSize;

    if (zCoord) {
        printf("+");
    } else {
        printf("-");
    }
    printf(",%lu,%lu\n", mf->numberCorrectlyMarkedMines, mf->numberOpenTiles);

    for (size_t ix = 0; ix != fieldSize; ++ix) {
        for (size_t iy = 0; iy != fieldSize; ++iy) {
            const struct Coords tempCoords = {zCoord, ix, iy};
            const signed char tileContent = mfGetTileContent(mf, &tempCoords);
            bool tileMask = mfGetTileMask(mf, &tempCoords);
            bool markedTile = mfGetTileMarking(mf, &tempCoords);

            if (markedTile) {
                printf("P");
            } else if (!tileMask) {
                printf("\u2592"); // filler character
            } else {
                if (tileContent > 0) {
                    printf("%d", tileContent); // impact number
                } else if (tileContent == 0) {
                    printf(" "); // empty tile
                } else {
                    printf("x");   // a mine
                }
            }
            printf(" "); // column separator
        }
        printf(" %lu\n", ix+1); // x-coordinate - a column after the field line
    }
    // y-coordinates  -  a line after the field
    for (size_t iy = 0; iy != fieldSize; ++iy) {
        printf("%lu", iy+1);
        if (iy+1 < 10) {
            printf(" ");
        }
    }
    printf("\n");
}


void PauseExec(void) {
    printf("Press ENTER to continue..");
    while ( getchar() != '\n' );
}


int GetUserChoiceDiff(void) {
    int userChoice;
    printf("> ");
    if (scanf("%d", &userChoice) == 0) {
        return -1;
    }
    // empty the buffer of not expected user input chars
    while ( getchar() != '\n' );
    return userChoice;
}


void Greetings(void) {
    printf("mines3c\n\n");
    PauseExec();
}


void AskForDifficulty(void) {
    printf("Please choose game difficulty:\n");
    printf("1. 8*8\n");
    printf("2. 10*10\n");
}


int GetUserCommand(int startLine) {
    RepositionCursor(startLine, 1);
    ClearPartOfScreen();
    int userChoice;
    char command;
    printf("> ");
    scanf("%c%d", &command, &userChoice);
    while ( getchar() != '\n' );
    if (command == 'r') {
        return 0;
    } else if (command == 'm') {
        return -userChoice;
    } else if (command == 'o') {
        if (userChoice == 0) {
            return GetUserCommand(startLine);
        }
        return userChoice;
    } else {
        return GetUserCommand(startLine);
    }
}
