#ifndef BLOCK_H
#define BLOCK_H

#include "raylib.h"

struct Block {
    Rectangle rec;
    float vel;
    bool active;
};

#endif