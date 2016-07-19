#include "Cube.h"
#include "utils.h"

Cube::Cube(int x, int y, int z) {
	_x = x;
	_y = y;
	_z = z;

    texture = loadTexture("../texture.jpg");
}

void Cube::draw(CubeSide direction) {
	switch(direction) {
        case TOP:

            break;
        case BOTTOM:

            break;
		case LEFT:

			break;
		case RIGHT:

			break;
		case FRONT:

			break;
		case BACK:

			break;
		default:
			// render nothing
			break;
    }
}

void Cube::draw_cube() {
    glm::vec3 pos = toGlobalCoordinates(_x, _y, _z);

    // enable and specify pointers to vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // Color
    // glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices + 3);
    // TODO fix textures
    // glTexCoordPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices + 6);

    static GLfloat textures[] = {
        0, 0, 1, 0, 0, 1, 1, 1,
        0, 0, 0, 1, 1, 0, 1, 1,

        0, 0, 1, 0, 0, 1, 1, 1,
        0, 0, 0, 1, 1, 0, 1, 1,

        0, 0, 1, 0, 0, 1, 1, 1,
        0, 0, 0, 1, 1, 0, 1, 1
    };
    glTexCoordPointer(2, GL_FLOAT, 0, textures);

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

glm::vec3 Cube::toGlobalCoordinates(int x, int y, int z) {
    return glm::vec3(x * BLOCK_SIZE, y * BLOCK_SIZE, z * BLOCK_SIZE);
}
