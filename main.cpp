#ifdef __APPLE__

#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>
#include <sstream>
#include <iomanip>

using std::stringstream;
using std::cout;
using std::endl;
using std::ends;

// global variables
void *font = GLUT_BITMAP_8_BY_13;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 300;
const float CAMERA_DISTANCE = 10.0f;
const int TEXT_WIDTH = 8;
const int TEXT_HEIGHT = 13;

float cameraAngleX = 0.f;
float cameraAngleY = 0.f;
float cameraDistance = 10.0f;

static bool quitting = false;
static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

// index array of vertex array for glDrawElements() & glDrawRangeElement()
GLubyte indices[] = {0, 1, 2, 2, 3, 0,      // front
                     4, 5, 6, 6, 7, 4,      // right
                     8, 9, 10, 10, 11, 8,      // top
                     12, 13, 14, 14, 15, 12,      // left
                     16, 17, 18, 18, 19, 16,      // bottom
                     20, 21, 22, 22, 23, 20};    // back

// interleaved vertex array for glDrawElements() & glDrawRangeElements() ======
// All vertex attributes (position, normal, color) are packed together as a
// struct or set, for example, ((V,N,C), (V,N,C), (V,N,C),...).
// It is called an array of struct, and provides better memory locality.
GLfloat vertices3[] = {1, 1, 1, 0, 0, 1, 1, 1, 1,              // v0 (front)
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
                       1, 1, -1, 0, 0, -1, 0, 1, 1};            // v5

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
void draw5() {
    // enable and specify pointers to vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices3 + 3);
    glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices3 + 6);
    glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices3);

    glPushMatrix();
    glTranslatef(-2, -2, 0);                // move to bottom-left

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights() {
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 20, 1}; // positional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}

///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL() {
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}

///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective() {
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei) SCREEN_WIDTH, (GLsizei) SCREEN_HEIGHT);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float) (SCREEN_WIDTH) / SCREEN_HEIGHT, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void drawString(const char *str, int x, int y, float color[4], void *font) {
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);

    glColor4fv(color);          // set text color
    glRasterPos2i(x, y);        // place text position

    // loop all characters in the string
    while (*str) {
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}


///////////////////////////////////////////////////////////////////////////////
// draw a string in 3D space
///////////////////////////////////////////////////////////////////////////////
void drawString3D(const char *str, float pos[3], float color[4], void *font) {
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);

    glColor4fv(color);          // set text color
    glRasterPos3fv(pos);        // place text position

    // loop all characters in the string
    while (*str) {
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}

int maxVertices;
int maxIndices;

///////////////////////////////////////////////////////////////////////////////
// display info messages
///////////////////////////////////////////////////////////////////////////////
void showInfo() {
    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
    glPushMatrix();                     // save current projection matrix
    glLoadIdentity();                   // reset projection matrix
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT); // set to orthogonal projection

    float color[4] = {1, 1, 1, 1};

    stringstream ss;
    ss << std::fixed << std::setprecision(3);

    ss << "Max Elements Vertices: " << maxVertices << ends;
    drawString(ss.str().c_str(), 1, SCREEN_HEIGHT - TEXT_HEIGHT, color, font);
    ss.str("");

    ss << "Max Elements Indices: " << maxIndices << ends;
    drawString(ss.str().c_str(), 1, SCREEN_HEIGHT - (2 * TEXT_HEIGHT), color, font);
    ss.str("");

    drawString("Press F1 to change draw mode", 1, SCREEN_HEIGHT - (3 * TEXT_HEIGHT), color, font);

    // unset floating format
    ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}

void render() {
    toPerspective();

    SDL_GL_MakeCurrent(window, gl_context);

    // default example
    /*
    glClearColor(.5f, .5f, .5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    */

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // tramsform camera
    glTranslatef(0, 0, -cameraDistance);
    glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    glRotatef(cameraAngleY, 0, 1, 0);   // heading
    glRotatef(0.0f, 1, 0, 0);   // pitch
    glRotatef(0.0f, 0, 1, 0);   // heading

    // with glDrawElements() with interleave vertex array
    draw5();

    // print 2D text
    float pos[4] = {-4.0f, 3.5f, 0, 1};
    float color[4] = {1, 1, 1, 1};
    pos[0] = -5.0f;
    pos[1] = -4.0f;
    drawString3D("glDrawElements()", pos, color, font);

    showInfo();     // print max range of glDrawRangeElements

    glPopMatrix();

    glutSwapBuffers();

    SDL_GL_SwapWindow(window);
}

int SDLCALL

watch(void *userdata, SDL_Event *event) {
    if (event->type == SDL_APP_WILLENTERBACKGROUND) {
        quitting = true;
    }
    return 1;
}

int drawMode = 0;

void update() {
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_ESCAPE]) {

    }

    int xrel, yrel;
    SDL_GetRelativeMouseState(&xrel, &yrel);
}

void changeDrawMode() {
    drawMode = ++drawMode % 3;
    if (drawMode == 0)        // fill mode
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
    else if (drawMode == 1)  // wireframe mode
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }
    else                    // point mode
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_OPENGL);
    gl_context = SDL_GL_CreateContext(window);
    SDL_AddEventWatch(watch, NULL);

//    SDL_SetRelativeMouseMode(SDL_TRUE);
//    SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    initGL();

    // check max of elements vertices and elements indices that your video card supports
    // Use these values to determine the range of glDrawRangeElements()
    // The constants are defined in glext.h
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxVertices);
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &maxIndices);

    int x, y;
    bool first = true;

    while (!quitting) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitting = true;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitting = true;
                }

                if (event.key.keysym.sym == SDLK_F1) {
                    changeDrawMode();
                }
            }
        }

        x = event.button.x;
        y = event.button.y;

        int xrel, yrel;
        SDL_GetRelativeMouseState(&xrel, &yrel);

        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            cameraAngleY += xrel;
            cameraAngleX += yrel;
        }
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            cameraDistance -= yrel * 0.2f;
        }

        update();
        render();
        SDL_Delay(2);
    }

    SDL_DelEventWatch(watch, NULL);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
}