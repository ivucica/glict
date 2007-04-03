#include <GL/gl.h>
#include "skinner.h"

/// \todo   Check if structs can be assigned directly (not the specific elements
///         one by one...)

glictSkinner::glictSkinner() {
}

glictSkinner::~glictSkinner() {
}

void glictSkinner::Paint() {
    int x1, y1, x2, y2;

    // first the corners...
    x1 = y1 = 0;
    x2 = toplefts.w; y2 = toplefts.h;
    glBindTexture(GL_TEXTURE_2D, topleft);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, x1);
    glEnd();

    x1 = size.w - toprights.w; y1 = 0;
    x2 = size.w; y2 = toprights.h;
    glBindTexture(GL_TEXTURE_2D, topright);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();

    x1 = 0; y1 = size.h - bottomlefts.h;
    x2 = bottomlefts.w; y2 = size.h;
    glBindTexture(GL_TEXTURE_2D, bottomleft);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();

    x1 = size.w - bottomrights.w; y1 = size.h - bottomrights.h;
    x2 = size.w; y2 = size.h;
    glBindTexture(GL_TEXTURE_2D, bottomright);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();

}

void glictSkinner::SetTL(GLuint textureid, glictSize size ) {
    this->topleft = textureid;
    this->toplefts.w = size.w;
    this->toplefts.h = size.h;
}
void glictSkinner::SetTR(GLuint textureid, glictSize size ) {
    this->topright = textureid;
    this->toprights.w = size.w;
    this->toprights.h = size.h;
}
void glictSkinner::SetBL(GLuint textureid, glictSize size ) {
    this->bottomleft = textureid;
    this->bottomlefts.w = size.w;
    this->bottomlefts.h = size.h;
}
void glictSkinner::SetBR(GLuint textureid, glictSize size ) {
    this->bottomright = textureid;
    this->bottomrights.w = size.w;
    this->bottomrights.h = size.h;
}

void glictSkinner::SetSize(glictSize size) {
    this->size.w = size.w;
    this->size.h = size.h;
}
