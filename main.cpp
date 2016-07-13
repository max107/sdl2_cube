#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "opengl_utils.h"
#include "cube.h"
#include "utils.h"

using std::stringstream;
using std::cout;
using std::endl;
using std::ends;

// global variables
//void *font = GLUT_BITMAP_8_BY_13;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 300;
const float CAMERA_DISTANCE = 10.0f;
const int TEXT_WIDTH = 8;
const int TEXT_HEIGHT = 13;

float cameraAngleX = 0.f;
float cameraAngleY = 0.f;
float cameraDistance = 10.0f;

static bool quitting = false;
static TTF_Font *font;
static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

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

    glClearColor(.5f, .5f, .5f, .5f);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}

///////////////////////////////////////////////////////////////////////////////
// set projection matrix as orthogonal
///////////////////////////////////////////////////////////////////////////////
void toOrtho() {
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei) SCREEN_WIDTH, (GLsizei) SCREEN_HEIGHT);

    // set orthographic viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    GLfloat ratio = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;
    glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 1500.f); // 1500.f

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
        // glutBitmapCharacter(font, *str);
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
        // glutBitmapCharacter(font, *str);
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
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1); // set to orthogonal projection

    float color[4] = {1, 1, 1, 1};

//    glColor3f(255.0f, 255.0f, 255.0f);
//    glTranslatef(10, 10, 0.0f);
    draw_text(font);

    /*
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
    */

    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}

void render() {
    // Clear the window

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
    draw_cube(-2, -2, 0);
    draw_cube(0, 0, 0);

    // print 2D text
    /*
    float pos[4] = {-4.0f, 3.5f, 0, 1};
    float color[4] = {1, 1, 1, 1};
    pos[0] = -5.0f;
    pos[1] = -4.0f;
    drawString3D("glDrawElements()", pos, color, font);
    */

    showInfo();     // print max range of glDrawRangeElements

    glPopMatrix();

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

    if (TTF_Init() == -1) {
        std::cout << "Init TTF  failt : " << SDL_GetError() << std::endl;
        return -1;
    }

    //this opens a font style and sets a size
    font = TTF_OpenFont("Ubuntu-R.ttf", 16);
    // Error check
    if (font == nullptr) {
        std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
        return false;
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
