#include "World.h"

World::World(int seed) : _seed(seed) {
    FastNoise *noise = new FastNoise;
    noise->SetSeed(seed);
    noise->SetNoiseType(FastNoise::Simplex);
    _chunks.push_back(new Chunk(noise));
}

void World::render() {
    for(int i = 0; i < _chunks.size(); i++) {
        _chunks[i]->render();
    }
}
