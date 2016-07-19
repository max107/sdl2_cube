//
// Created by max on 17.07.16.
//

#ifndef SDL2_CUBE_CHUNK_H
#define SDL2_CUBE_CHUNK_H

#include <vector>

#include "Cube.h"
#include "FastNoise.h"

class Chunk {

public:
    Chunk(FastNoise *noise);

    void render();

private:
	std::vector<Cube *> _cubes;

    static const int CHUNK_SIZE = 1;
    static const int CHUNK_DEPTH = 1;

    float data[CHUNK_SIZE][CHUNK_SIZE][CHUNK_DEPTH];
};


#endif //SDL2_CUBE_CHUNK_H
