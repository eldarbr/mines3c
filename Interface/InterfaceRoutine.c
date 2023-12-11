#include "InterfaceRoutine.h"


void InterfaceRoutine(void) {
    ClearScreen();

    Greetings();

    // ask for difficulty
    int difficulty = 0;
    do {
        RepositionCursor(3,1); // prepare the screen
        ClearPartOfScreen();
        AskForDifficulty();
        difficulty = GetUserChoiceDiff();
    } while (difficulty < 1 || difficulty > 2);

    // setup size and number of mines according to the
    // chosen difficulty
    size_t size, mines;
    switch (difficulty) {
        case 1:
            size = 8;
            mines = 15;
            break;
        case 2:
            size = 10;
            mines = 25;
            break;
        default:
            Exception("difficulty not in range", 22);
            return;
    }

    ClearScreen();

    struct MineField* mf = mfConstructor(size, mines);

    // flag that stores the state of display
    bool zState = false;

    while (1) {
        DrawMineField(mf, zState);

        // 3+size is the number of line where to put the
        // field for the user to answer
        int next = GetUserCommand(3+size);

        if (next == 0) {    // command to invert the display
            zState = !zState;
            continue;
        } else {
            // get the coordinates of the command
            // they're common for the two next commands
            // the only difference is the sign
            int absNext;
            if (next <0) absNext = -next;
            else absNext = next;

            int xC, yC;
            if (absNext >= 100) {
                xC = absNext / 100;
                yC = absNext % 100;
            } else {
                xC = absNext / 10;
                yC = absNext % 10;
            }
            xC--;  // user coordinates are 1-based
            yC--;  // user coordinates are 1-based

            // assert that the coords are in range
            if (xC >= (int)size || yC >= (int)size || xC < 0 || yC < 0) {
                continue; // ignore the command if the input is incorrect
            }

            struct Coords choiceTile = {zState, xC, yC};
            if (next > 0) {
                // if no tiles were previously opened, make a flood open
                // = it's the first open
                if (mf->numberOpenTiles == 0) {
                    mfFloodOpenQueue(mf, &choiceTile, mf->numberTotalTiles/2);
                }
                // if after the flood no tiles are still open
                // open by force
                // basically means that the first opened tile contains a mine
                if (mf->numberOpenTiles == 0) {
                    mfOpenTile(mf, &choiceTile);
                }
            } else {
                // mark the tile
                mfSwitchMarkTile(mf, &choiceTile);
            }
        }

    }

    mfDestructor(mf);
}
