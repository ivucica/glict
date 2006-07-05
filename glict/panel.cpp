/*
    GLICT - Graphics Library Interface Creation Toolkit
    Copyright (C) 2006 OBJECT Networks

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <GL/glut.h>
#include <stdio.h>
#include "panel.h"
#include "globals.h"
#include "glut-helper.h"
glictPanel::glictPanel() {
    this->bgcolor.r = .7;
    this->bgcolor.g = .7;
    this->bgcolor.b = .7;
    this->bgcolor.a = 1.0;
    strcpy(this->objtype, "Panel");

    this->parent = NULL;
    //printf("Panel generated.\n");

    this->focusable = false;
}
glictPanel::~glictPanel() {

}
void glictPanel::Paint() {

    glColor4f(
        (float)this->bgcolor.r,
        (float)this->bgcolor.g,
        (float)this->bgcolor.b,
        (float)this->bgcolor.a
    );
    glBegin(GL_QUADS);
    glVertex2f(this->x,this->y);
    glVertex2f(this->x+this->width,this->y);
    glVertex2f(this->x+this->width,this->y+this->height);
    glVertex2f(this->x,this->y+this->height);
    glEnd();


    glColor4f(1., 1., 1., 1.);
    glPushMatrix();
    glTranslatef(this->x, this->y+10.,0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glColor4f(1.,1.,1.,1.);
    glutxStrokeString(this->caption.c_str(), GLUT_STROKE_MONO_ROMAN, 0, 0);
    glPopMatrix();

    this->CPaint();
}
void glictPanel::SetBGColor(float r, float g, float b, float a) {
    this->bgcolor.r = r;
    this->bgcolor.g = g;
    this->bgcolor.b = b;
    this->bgcolor.a = a;
}
/// This is the copypasteable castevent usable in other widgets
bool glictPanel::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
    switch (evt) {
        case GLICT_MOUSEUP:
        case GLICT_MOUSEDOWN:
        case GLICT_MOUSECLICK:
            if (((glictPos*)wparam)->x > this->clipleft &&
                ((glictPos*)wparam)->x < this->clipright &&
                ((glictPos*)wparam)->y > this->cliptop &&
                ((glictPos*)wparam)->y < this->clipbottom) {

                // if a child caught click, we dont handle it otherwise
                return DefaultCastEvent(evt, wparam, lparam, returnvalue);
                // otherwise we could handle it mroe but we'll simply tell we didnt proces it

            }
            //printf("It occured outside the panel, ignored.\n");
            break;
    }

    return false;
}
