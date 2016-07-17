//
// Created by max on 17.07.16.
//

#include "World.h"

World::World(int seed) : _seed(seed) {
    // TODO add chunk position
    _chunks.push_back(new Chunk(_seed));
}

void World::render() {
    for(int i = 0; i < _chunks.size(); i++) {
        _chunks[i]->render();
    }
}

void World::setSeed(int seed) {
    _seed = seed;
}



