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
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                draw_cube(x, y, data[x][y][z]);
            }
        }
    }
}

GLuint Chunk::loadTexture(std::string path) {
    // Load an OpenGL texture.
    // We could directly use a sf::Texture as an OpenGL texture (with its Bind() member function),
    // but here we want more control on it (generate mipmaps, ...) so we create a new one from an image
    GLuint texture = 0;

    // You should probably use CSurface::OnLoad ... ;)
    //-- and make sure the Surface pointer is good!
    SDL_Surface *Surface = IMG_Load(path.c_str());

    int imageMode = GL_RGB;
    if (Surface->format->BytesPerPixel == 4) {
        imageMode = GL_RGBA;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, imageMode, Surface->w, Surface->h, 0, imageMode, GL_UNSIGNED_BYTE,
                 Surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    return texture;
}

Chunk::Chunk(FastNoise *noise) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                data[x][y][z] = 20 * noise->GetSimplex(x, y, .8f);
            }
        }
    }

    texture = loadTexture("../texture.jpg");
}

glm::vec3 Chunk::toGlobalCoordinates(int x, int y, int z) {
    return glm::vec3(x * BLOCK_SIZE, y * BLOCK_SIZE, z * BLOCK_SIZE);
}

void Chunk::draw_cube(int x, int y, int z) {
    glm::vec3 pos = toGlobalCoordinates(x, y, z);

    // enable and specify pointers to vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // Color
    // glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices + 3);
    // TODO fix textures
    glTexCoordPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices);

    // Color
    // glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices + 6);
    glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices);

    glPushMatrix();
    // move to bottom-left
    glTranslatef(pos.x, pos.y, pos.z);



    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    // Color
    // glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
