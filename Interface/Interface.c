#include "Interface.h"


void ClearScreen(void) {
    RepositionCursor(1, 1);
    ClearPartOfScreen();
}


void ClearPartOfScreen(void) {
    printf("\033[J");
}


void RepositionCursor(size_t x, size_t y) {
    if (x < 0 || y < 0) {
        Exception("ERR: illegal argument (Interface.RepositionCursor)", 1);
    }
    if (x == 0) {
        printf("\033[;%zuH", y);
    }
    printf("\033[%zu;%zuH", x, y);
}


void Exception(const char message[], int q) {
    printf("%s", message);
    exit(q);
}


void DrawMineField(const struct MineField *const mf, const bool zCoord) {
    ClearScreen();

    const size_t fieldSize = mf->fieldSize;

    if (zCoord) {
        printf("+\n");
    } else {
        printf("-\n");
    }

    for (size_t ix = 0; ix != fieldSize; ++ix) {
        for (size_t iy = 0; iy != fieldSize; ++iy) {
            const struct Coords tempCoords = {zCoord, ix, iy};
            const signed char tileContent = mfGetTileContent(mf, &tempCoords);
            bool tileMask = mfGetTileMask(mf, &tempCoords);
            bool markedTile = mfGetTileMarking(mf, &tempCoords);

            if (markedTile) {
                printf("P ");
            } else if (!tileMask) {
                printf("\u2592 ");
            } else {
                if (tileContent > 0) {
                    printf("%d ", tileContent);
                } else if (tileContent == 0) {
                    printf("  ");
                } else {
                    printf("x ");
                }
            }
        }
        printf(" %lu\n", ix+1);
    }
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
