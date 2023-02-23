#pragma once
#include <GL/gl.h>

class Renderer
{
private:

public:

    static void drawRectangleOutline(float x, float y, float width, float height, float r, float g, float b, float lineWidth) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glLineWidth(lineWidth);
        glColor3f(r, g, b);

        glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + width, y);
            glVertex2f(x + width, y + height);
            glVertex2f(x, y + height);
        glEnd();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

};