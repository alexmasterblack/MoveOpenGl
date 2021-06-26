#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "TextureLoading.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

int width = 1000;
int height = 900;

//первый шар
GLdouble x = -2, y = 0, z = 0;

//второй шар
GLdouble x_two = 2, y_two = 0, z_two = 0;

bool selectedMode = true;
int selectedId = 0;

GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];

void Draw(TextureLoading earthTex, TextureLoading moonTex) {
    //
    glClearColor(0.160f, 0.725f, 0.780f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //

    if (selectedMode) {
        glDisable(GL_TEXTURE_2D);
    }
    else {
        glEnable(GL_TEXTURE_2D);
    }

    GLUquadricObj* obj;
    obj = gluNewQuadric();

    //
    glPushMatrix();
    if (selectedMode) {
        glColor3ub(100, 0, 0);
    }
    else if (selectedId == 100) {
        glColor3f(1, 1, 1);
    }
    glTranslated(x, y, z);
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricTexture(obj, GL_TRUE);
    earthTex.Binding();
    gluSphere(obj, 0.2, 20, 18);
    glPopMatrix();
    //

    //
    glPushMatrix();
    if (selectedMode) {
        glColor3ub(200, 0, 0);
    }
    else if (selectedId == 200) {
        glColor3f(1, 1, 1);
    }
    glTranslated(x_two, y_two, z_two);
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricTexture(obj, GL_TRUE);
    moonTex.Binding();
    gluSphere(obj, 0.4, 20, 18);
    glPopMatrix();
    //

    //
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, width / height, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    //
}

void SelectObject(GLdouble x, GLdouble y, TextureLoading earthTex, TextureLoading moonTex) {
    selectedMode = true;
    Draw(earthTex, moonTex);
    selectedMode = false;
    GLubyte rgb[3];
    glReadPixels(x, (float)viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);
    selectedId = rgb[0];
    std::cout << selectedId << std::endl;
}

using namespace std;
int main() {
    //
    sf::ContextSettings  settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 4;
    settings.minorVersion = 3;
    settings.antialiasingLevel = 2;

    sf::Window window(sf::VideoMode(width, height, 32), "Solar",
        sf::Style::Titlebar | sf::Style::Close, settings);

    glewExperimental = GL_TRUE;
    glEnable(GL_DEPTH_TEST);

    if (GLEW_OK != glewInit()) {
        cout << "Error:: glew not init =(" << endl;
        return -1;
    }
    //

    //
    TextureLoading earthTex("texture/earth.jpg");
    TextureLoading moonTex("texture/moon.jpg");
    //

    bool isGo = true;
    while (isGo) {
        Draw(earthTex, moonTex);
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type)
            {
            case sf::Event::Closed:
                isGo = false;
                break;
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto touch = event.mouseButton;
                    SelectObject(touch.x, touch.y, earthTex, moonTex);
                }
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    selectedId = 0;
                }
                break;
            }
            case sf::Event::MouseMoved:
            {
                auto shift = event.mouseMove;
                GLdouble posX = 0, posY = 0, posZ = 0;
                GLfloat winX, winY, winZ;
                winX = (float)shift.x;
                winY = (float)viewport[3] - (float)shift.y;
                if (selectedId == 100) {
                    gluUnProject(winX, winY, 0.84f, modelview, projection, viewport, &posX, &posY, &posZ);
                    x = posX;
                    y = posY;
                    z = posZ;
                }
                if (selectedId == 200) {
                    gluUnProject(winX, winY, 0.84f, modelview, projection, viewport, &posX, &posY, &posZ);
                    x_two = posX;
                    y_two = posY;
                    z_two = posZ;
                }
                break;
            }
            default:
                break;
            }
        }
        window.display();
    }
    window.close();
    return 0;
}