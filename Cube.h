#ifndef SDL2_CUBE_CUBE
#define SDL2_CUBE_CUBE

#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <vector>

enum CubeSide {
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	FRONT,
	BACK
};


class Cube
{
	public:
		Cube(int x, int y, int z);

		void draw(CubeSide direction);

		///////////////////////////////////////////////////////////////////////////////
	    // draw cube at bottom-left corner with glDrawElements and interleave array
	    // All the vertex data (position, normal, colour) can be placed together into a
	    // single array, and be interleaved like (VNCVNC...). The interleave vertex data
	    // provides better memory locality.
	    // Since we are using a single interleaved vertex array to store vertex
	    // positions, normals and colours, we need to specify "stride" and "pointer"
	    // parameters properly for glVertexPointer, glNormalPointer and glColorPointer.
	    // Each vertex has 9 elements of floats (3 position + 3 normal + 3 color), so,
    	// the stride param should be 36 (= 9 * 4 bytes).
	    ///////////////////////////////////////////////////////////////////////////////
		void draw_cube();


protected:
    // index array of vertex array for glDrawElements() & glDrawRangeElement()
    GLubyte indices[36] = {
            0, 1, 2, 2, 3, 0,      // front
            4, 5, 6, 6, 7, 4,      // right
            8, 9, 10, 10, 11, 8,      // top
            12, 13, 14, 14, 15, 12,      // left
            16, 17, 18, 18, 19, 16,      // bottom
            20, 21, 22, 22, 23, 20    // back
    };

    // interleaved vertex array for glDrawElements() & glDrawRangeElements() ======
    // All vertex attributes (position, normal, color) are packed together as a
    // struct or set, for example, ((V,N,C), (V,N,C), (V,N,C),...).
    // It is called an array of struct, and provides better memory locality.
    GLfloat vertices[216] = {
            1, 1, 1, 0, 0, 1, 1, 1, 1,              // v0 (front)
            -1, 1, 1, 0, 0, 1, 1, 1, 0,              // v1
            -1, -1, 1, 0, 0, 1, 1, 0, 0,              // v2
            1, -1, 1, 0, 0, 1, 1, 0, 1,              // v3

            1, 1, 1, 1, 0, 0, 1, 1, 1,              // v0 (right)
            1, -1, 1, 1, 0, 0, 1, 0, 1,              // v3
            1, -1, -1, 1, 0, 0, 0, 0, 1,              // v4
            1, 1, -1, 1, 0, 0, 0, 1, 1,              // v5

            1, 1, 1, 0, 1, 0, 1, 1, 1,              // v0 (top)
            1, 1, -1, 0, 1, 0, 0, 1, 1,              // v5
            -1, 1, -1, 0, 1, 0, 0, 1, 0,              // v6
            -1, 1, 1, 0, 1, 0, 1, 1, 0,              // v1

            -1, 1, 1, -1, 0, 0, 1, 1, 0,              // v1 (left)
            -1, 1, -1, -1, 0, 0, 0, 1, 0,              // v6
            -1, -1, -1, -1, 0, 0, 0, 0, 0,              // v7
            -1, -1, 1, -1, 0, 0, 1, 0, 0,              // v2

            -1, -1, -1, 0, -1, 0, 0, 0, 0,              // v7 (bottom)
            1, -1, -1, 0, -1, 0, 0, 0, 1,              // v4
            1, -1, 1, 0, -1, 0, 1, 0, 1,              // v3
            -1, -1, 1, 0, -1, 0, 1, 0, 0,              // v2

            1, -1, -1, 0, 0, -1, 0, 0, 1,              // v4 (back)
            -1, -1, -1, 0, 0, -1, 0, 0, 0,              // v7
            -1, 1, -1, 0, 0, -1, 0, 1, 0,              // v6
            1, 1, -1, 0, 0, -1, 0, 1, 1             // v5
    };

	private:
	    GLuint texture = 0;
		int _x, _y, _z;
	    static const int BLOCK_SIZE = 2;

    	glm::vec3 toGlobalCoordinates(int x, int y, int z);
};

#endif
