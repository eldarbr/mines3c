#include "Interface.h"


void ClearScreen(void) {
    RepositionCursor(1, 1);
    printf("\033[J");
}

void RepositionCursor(size_t x, size_t y) {
    if (x < 1 || y < 1) {
        Exception("ERR: illegal argument (Interface.RepositionCursor)", 1);
    }
    printf("\033[%zu;%zuH", x, y);
}


void Exception(const char message[], int q) {
    printf("%s", message);
    exit(q);
}


void DrawMineField(const struct MineField *const mf, const bool zCoord) {
    const size_t fieldSize = mf->fieldSize;

    for (size_t ix = 0; ix != fieldSize; ++ix) {
        for (size_t iy = 0; iy != fieldSize; ++iy) {
            const struct Coords tempCoords = {zCoord, ix, iy};
            const signed char tileContent = mfGetTileContent(mf, &tempCoords);
            bool tileMask = mfGetTileMask(mf, &tempCoords);

            if (!tileMask) {
                printf("%c ", (char)177);
            } else {
                if (tileContent > 0) {
                    printf("%d", tileContent);
                } else if (tileContent == 0) {
                    printf(" ");
                } else {
                    printf("x");
                }
            }
        }
        printf("\n");
    }
}


void PauseExec(void) {
    printf("Press ENTER to continue..");
    while ( getchar() != '\n' );
}


int GetUserChoice(void) {
    int userChoice;
    printf("> ");
    if (scanf_s("%d", &userChoice) == 0) {
        return -1;
    }
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
