//
// Created by max on 17.07.16.
//

#include "Chunk.h"

void Chunk::render() {
    /*
    if (_has_list) {
        glCallList(cubeList);
    } else {
        cubeList = glGenLists(1);
        glNewList(cubeList, GL_COMPILE);
        draw_cube(x, y, data[x][y][z]);
        glEndList();
        _has_list = true;
    }
    */

    // with glDrawElements() with interleave vertex array
	for (auto & cube : _cubes) {
    	cube->draw_cube();
	}
	/*
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                draw_cube(x, y, data[x][y][z]);
			}
        }
    }
	*/
}

Chunk::Chunk(FastNoise *noise) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                data[x][y][z] = 20 * noise->GetSimplex(x, y, .8f);

				_cubes.push_back(new Cube(x, y, data[x][y][z]));
            }
        }
    }
}




