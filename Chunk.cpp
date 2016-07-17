//
// Created by max on 17.07.16.
//

#include "Chunk.h"

void Chunk::render() {
    // with glDrawElements() with interleave vertex array
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                draw_cube(x, y, data[x][y][z]);
            }
        }
    }
}

Chunk::Chunk(int seed) : _seed(seed) {
    FastNoise myNoise;
    myNoise.SetSeed(seed);
    myNoise.SetNoiseType(FastNoise::Simplex);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                data[x][y][z] = 20 * myNoise.GetSimplex(x, y, .8f);
            }
        }
    }
}

glm::vec3 Chunk::toGlobalCoordinates(int x, int y, int z) {
    int blockSize = 2;
    return glm::vec3(x * blockSize, y * blockSize, z * blockSize);
}

void Chunk::draw_cube(int x, int y, int z) {
    glm::vec3 pos = toGlobalCoordinates(x, y, z);

    // enable and specify pointers to vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices + 3);
    glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices + 6);
    glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices);

    glPushMatrix();
    // move to bottom-left
    // glTranslatef(-2, -2, 0);
    glTranslatef(pos.x, pos.y, pos.z);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void Chunk::setSeed(int seed) {
    _seed = seed;
}
