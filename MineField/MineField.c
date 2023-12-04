#include "MineField.h"


struct MineField* mfConstructor(const size_t inpSize, const size_t inpNumMines) {
    
    // new seed each time you want to play
    srand(time(NULL));

    struct MineField *mineField = malloc(sizeof(struct MineField));

    mineField->fieldSize = inpSize;

    // allocate and init field content and mask 
    mineField->fieldContent = malloc(sizeof(signed char) * 2 * inpSize * inpSize);
    mineField->fieldMask = malloc(sizeof(bool) * 2 * inpSize * inpSize);
    for (size_t i = 0; i != 2; ++i) {
        mineField->fieldContent[i] = malloc(sizeof(signed char) * inpSize * inpSize);
        mineField->fieldMask[i] = malloc(sizeof(bool) * inpSize * inpSize);
        for (size_t j = 0; j != inpSize; ++j) {
            mineField->fieldContent[i][j] = malloc(sizeof(signed char) * inpSize);
            mineField->fieldMask[i][j] = malloc(sizeof(bool) * inpSize);
            for (size_t k = 0; k != inpSize; ++k) {
                mineField->fieldContent[i][j][k] = 0;
                mineField->fieldMask[i][j][k] = false;
            }
        }
    }

    // generate and save mines
    for (size_t i = 0; i != inpNumMines; ++i) {
        struct Coords newMine = GenerateMine(inpSize);
        if (mfIsThisAMine(mineField, &newMine)) {
            --i;
            continue;
        }
        // tiles with mines get value of -6
        // as the max impact to a tile is +5
        mineField->fieldContent[newMine.z][newMine.x][newMine.y] = -6;

        if (newMine.z)
        mineField->fieldContent[newMine.z-1][newMine.x][newMine.y]++;
        if (!newMine.z)
        mineField->fieldContent[newMine.z+1][newMine.x][newMine.y]++;
        if (newMine.x > 1)
        mineField->fieldContent[newMine.z][newMine.x-1][newMine.y]++;
        if (newMine.x + 1 < inpSize)
        mineField->fieldContent[newMine.z][newMine.x+1][newMine.y]++;
        if (newMine.y > 1)
        mineField->fieldContent[newMine.z][newMine.x][newMine.y-1]++;
        if (newMine.y + 1 < inpSize)
        mineField->fieldContent[newMine.z][newMine.x][newMine.y+1]++;
    }

    return mineField;
}


void mfDestructor(struct MineField *const mineField) {
    const size_t fieldSize = mineField->fieldSize;

    for (int i = 1; i != -1; --i) {
        for (long j = ((long)fieldSize)-1; j != -1; --j) {
            free(mineField->fieldContent[i][j]);
            free(mineField->fieldMask[i][j]);
        }
        free(mineField->fieldContent[i]);
        free(mineField->fieldMask[i]);
    }
    free(mineField->fieldContent);
    free(mineField->fieldMask);

    free(mineField);
}


const signed char ***mfGetContent(const struct MineField *const mineField) {
    return (const signed char ***)mineField->fieldContent;
}


const bool ***mfGetMask(const struct MineField *const mineField) {
    return (const bool ***)mineField->fieldMask;
}


void mfOpenTile(struct MineField *const mineField, const struct Coords *const tileCoords) {
    mineField->fieldMask[(size_t)tileCoords->z][tileCoords->x][tileCoords->y] = true;
}


bool mfIsThisAMine(const struct MineField *const MineField, const struct Coords *const tileCoords) {
    const bool *tileZ = &(tileCoords->z);
    const size_t *tileX = &(tileCoords->x);
    const size_t *tileY = &(tileCoords->y);

    if (MineField->fieldContent[*tileZ][*tileX][*tileY] < 0) {
        return true;
    } else {
        return false;
    }
}


struct Coords GenerateMine(const size_t fieldSize) {
    size_t x = rand() % fieldSize;
    size_t y = rand() % fieldSize;
    bool z = rand() % 2;
    struct Coords c = {z, x, y};
    return c;
}
