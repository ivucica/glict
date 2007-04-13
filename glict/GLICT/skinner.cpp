
#include <windows.h>
#include <GL/gl.h>
#include "skinner.h"

/// \todo   Check if structs can be assigned directly (not the specific elements
///         one by one...)

#ifndef min
    #define min(a, b) ((a) < (b) ? (a) : (b))
#endif
glictSkinner::glictSkinner() {
    topleft = 0;
    topright = 0;
    bottomleft = 0;
    bottomright = 0;

    top = 0;
    left = 0;
    right = 0;
    bottom = 0;

    center = 0;
}

glictSkinner::~glictSkinner() {
}

void glictSkinner::Paint(glictSize *size) {
    int x1, y1, x2, y2;

    glEnable(GL_TEXTURE_2D);
    glColor4f(1., 1., 1., 1.);
    // first the corners...

    if (topleft) {
        x1 = y1 = 0;
        x2 = toplefts.w+1; y2 = toplefts.h+1;
        glBindTexture(GL_TEXTURE_2D, topleft);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(x1, y1);
        glTexCoord2f(0, 1);
        glVertex2f(x1, y2);
        glTexCoord2f(1, 1);
        glVertex2f(x2, y2);
        glTexCoord2f(1, 0);
        glVertex2f(x2, y1);
        glEnd();
    }

    if (topright) {
        x1 = size->w - toprights.w -1; y1 = 0;
        x2 = size->w; y2 = toprights.h + 1;
        glBindTexture(GL_TEXTURE_2D, topright);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(x1, y1);
        glTexCoord2f(0, 1);
        glVertex2f(x1, y2);
        glTexCoord2f(1, 1);
        glVertex2f(x2, y2);
        glTexCoord2f(1, 0);
        glVertex2f(x2, y1);
        glEnd();
    }

    if (bottomleft) {
        x1 = 0; y1 = size->h - bottomlefts.h - 1;
        x2 = bottomlefts.w + 1; y2 = size->h;
        glBindTexture(GL_TEXTURE_2D, bottomleft);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(x1, y1);
        glTexCoord2f(0, 1);
        glVertex2f(x1, y2);
        glTexCoord2f(1, 1);
        glVertex2f(x2, y2);
        glTexCoord2f(1, 0);
        glVertex2f(x2, y1);
        glEnd();
    }

    if (bottomright) {
        x1 = size->w - bottomrights.w - 1; y1 = size->h - bottomrights.h - 1;
        x2 = size->w; y2 = size->h;
        glBindTexture(GL_TEXTURE_2D, bottomright);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(x1, y1);
        glTexCoord2f(0, 1);
        glVertex2f(x1, y2);
        glTexCoord2f(1, 1);
        glVertex2f(x2, y2);
        glTexCoord2f(1, 0);
        glVertex2f(x2, y1);
        glEnd();
    }



    if (top) {
        for (int pos = topleft ? toplefts.w : 0; pos < size->w - (topright ? toprights.w : 0); pos+=tops.w) {
            x1 = pos; y1 = 0;
            x2 = min(size->w - (topright ? toprights.w : 0) , pos + tops.w) + 1; y2 = tops.h + 1;
            glBindTexture(GL_TEXTURE_2D, top);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(x1, y1);
            glTexCoord2f(0, 1);
            glVertex2f(x1, y2);
            glTexCoord2f(1, 1);
            glVertex2f(x2, y2);
            glTexCoord2f(1, 0);
            glVertex2f(x2, y1);
            glEnd();
        }
    }

    if (bottom) {
        for (int pos = bottomleft ? bottomlefts.w : 0; pos < size->w - (bottomright ? bottomrights.w : 0); pos+=bottoms.w) {
            x1 = pos; y1 = size->h - bottoms.h - 1;
            x2 = min(size->w - (bottomleft ? bottomlefts.w : 0), pos + bottoms.w) + 1; y2 = size->h;
            glBindTexture(GL_TEXTURE_2D, bottom);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(x1, y1);
            glTexCoord2f(0, 1);
            glVertex2f(x1, y2);
            glTexCoord2f(1, 1);
            glVertex2f(x2, y2);
            glTexCoord2f(1, 0);
            glVertex2f(x2, y1);
            glEnd();
        }
    }



    if (left) {
        for (int pos = topleft ? toplefts.h : 0; pos < size->h - (bottomleft ? bottomlefts.w : 0); pos+=lefts.h) {
            x1 = 0; y1 = pos;
            x2 = lefts.w; y2 = min(size->h - (bottomleft ? bottomlefts.h : 0), pos + lefts.h);
            glBindTexture(GL_TEXTURE_2D, left);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(x1, y1);
            glTexCoord2f(0, 1);
            glVertex2f(x1, y2);
            glTexCoord2f(1, 1);
            glVertex2f(x2, y2);
            glTexCoord2f(1, 0);
            glVertex2f(x2, y1);
            glEnd();
        }
    }


    if (right) {
        for (int pos = topright ? toprights.h : 0; pos < size->h - (bottomright ? bottomrights.w : 0); pos+=rights.h) {
            x1 = size->w - rights.w; y1 = pos;
            x2 = size->w; y2 = min(size->h - (bottomright ? bottomrights.w : 0), pos + rights.h);
            glBindTexture(GL_TEXTURE_2D, right);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(x1, y1);
            glTexCoord2f(0, 1);
            glVertex2f(x1, y2);
            glTexCoord2f(1, 1);
            glVertex2f(x2, y2);
            glTexCoord2f(1, 0);
            glVertex2f(x2, y1);
            glEnd();
        }
    }


    if (center) {
        for (int pos = topleft ? toplefts.w : 0; pos < size->w - (topright ? toprights.w : 0); pos+=tops.w) {
            for (int pos2 = topleft ? toplefts.h : 0; pos2 < size->h - (bottomleft ? bottomlefts.w : 0); pos2+=lefts.h) {
                y1 = pos2;
                y2 = min(size->h - (bottomright ? bottomrights.w : 0), pos2 + centers.h);

                x1 = pos;
                x2 = min(size->w - (topright ? toprights.w : 0), pos + centers.w);


                glBindTexture(GL_TEXTURE_2D, center);
                glBegin(GL_QUADS);
                glTexCoord2f(0, 0);
                glVertex2f(x1, y1);
                glTexCoord2f(0, 1);
                glVertex2f(x1, y2);
                glTexCoord2f(1, 1);
                glVertex2f(x2, y2);
                glTexCoord2f(1, 0);
                glVertex2f(x2, y1);
                glEnd();
            }
        }
    }



    glDisable(GL_TEXTURE_2D);

}

void glictSkinner::SetTL(GLuint textureid, glictSize *size ) {
    this->topleft = textureid;
    if (size) this->toplefts = *size;
}
void glictSkinner::SetTR(GLuint textureid, glictSize *size ) {
    this->topright = textureid;
    if (size) this->toprights = *size;
}
void glictSkinner::SetBL(GLuint textureid, glictSize *size ) {
    this->bottomleft = textureid;
    if (size) this->bottomlefts = *size;
}
void glictSkinner::SetBR(GLuint textureid, glictSize *size ) {
    this->bottomright = textureid;
    if (size) this->bottomrights = *size;
}
void glictSkinner::SetTop(GLuint textureid, glictSize *size ) {
    this->top = textureid;
    if (size) this->tops = *size;
}
void glictSkinner::SetBottom(GLuint textureid, glictSize *size ) {
    this->bottom = textureid;
    if (size) this->bottoms = *size;
}
void glictSkinner::SetLeft(GLuint textureid, glictSize *size ) {
    this->left = textureid;
    if (size) this->lefts = *size;
}
void glictSkinner::SetRight(GLuint textureid, glictSize *size ) {
    this->right = textureid;
    if (size) this->rights = *size;
}

void glictSkinner::SetCenter(GLuint textureid, glictSize *size ) {
    this->center = textureid;
    if (size) this->centers = *size;
}

glictSize *glictSkinner::GetLeftSize() {
    return &(this->lefts);
}
glictSize *glictSkinner::GetTopSize() {
    return &(this->tops);
}
glictSize *glictSkinner::GetRightSize() {
    return &(this->rights);
}
glictSize *glictSkinner::GetBottomSize() {
    return &(this->bottoms);
}
