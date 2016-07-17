//
// Created by max on 17.07.16.
//

#ifndef SDL2_CUBE_WORLD_H
#define SDL2_CUBE_WORLD_H

#include <vector>
#include "Chunk.h"

class World {
public:
    World(int seed);

    void setSeed(int seed);

    void render();

private:
    int _seed = 1;
    std::vector<Chunk*> _chunks;
};


#endif //SDL2_CUBE_WORLD_H
