#ifndef MINEFIELD_PROD
#define MINEFIELD_PROD


#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


struct Coords {
    /*
    *   Coordinates in the game are:
    *   z (flag) - first or second layer
    *   x (unsigned, [0;]) - vertical, 0-top
    *   y (unsigned, [0;]) - horizontal, 0-left
    */

    bool z;
    size_t x;
    size_t y;
};


struct MineField {
    signed char ***fieldContent;
    bool ***fieldMask;  // false = tile is closed (default)
    size_t fieldSize;
};
struct MineField* mfConstructor(const size_t size, const size_t numMines);
void mfDestructor(struct MineField *const);


const signed char*** mfGetContent(const struct MineField *const);
const bool*** mfGetMask(const struct MineField *const);
void mfOpenTile(struct MineField *const, const struct Coords *const);
bool mfIsThisAMine(const struct MineField *const, const struct Coords *const);


struct Coords GenerateMine(const size_t);


#endif
