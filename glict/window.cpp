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
#include "window.h"
#include "globals.h"
#include "glut-helper.h"
glictWindow::glictWindow() {

    this->bgcolor.r = 0.75;
    this->bgcolor.g = 0.75;
    this->bgcolor.b = 0.75;
    this->bgcolor.a = 1.0;
    strcpy(this->objtype, "Window");

    this->parent = NULL;
    this->SetCaption("Untitled Window");

    printf("Window generated.\n");
}
glictWindow::~glictWindow() {

}
void glictWindow::Paint() {

    printf("window\n");


    glColor4f(
        (float)this->bgcolor.r,
        (float)this->bgcolor.g,
        (float)this->bgcolor.b,
        (float)this->bgcolor.a
    );
    glBegin(GL_QUADS);
    glVertex2f(this->x,this->y+12);
    glVertex2f(this->x+this->width,this->y+12);
    glVertex2f(this->x+this->width,this->y+this->height);
    glVertex2f(this->x,this->y+this->height);
    glEnd();



    this->CPaint();


    this->SetScissor();

	glColor4fv(glictGlobals.windowTitleBgColor);

    glBegin(GL_QUADS);
    glVertex2f(this->x,this->y);
    glVertex2f(this->x + this->width, this->y);
    glVertex2f(this->x + this->width, this->y+12);
    glVertex2f(this->x, this->y+12);
    glEnd();
//    glDisable(GL_SCISSOR_TEST);
    glColor4fv(glictGlobals.windowTitleColor);

    glPushMatrix();
    //glRotatef(180.0, 0.0, 0.0, 1.0);
    //glutxStrokeString(this->caption.c_str(),GLUT_STROKE_ROMAN, (this->x + this->width / 2 + glutxStrokeSize(this->caption.c_str(), GLUT_STROKE_ROMAN) / 2) * -1, this->y - 9);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    //glDisable(GL_SCISSOR_TEST);
    glutxStrokeString(this->caption.c_str(),GLUT_STROKE_ROMAN, this->x + (this->width / 2 - glutxStrokeSize(this->caption.c_str(), GLUT_STROKE_ROMAN) / 2) , this->y*-1. - 9.);
    //glEnable(GL_SCISSOR_TEST);
    glPopMatrix();
}
void glictWindow::SetBGColor(float r, float g, float b, float a) {
    this->bgcolor.r = r;
    this->bgcolor.g = g;
    this->bgcolor.b = b;
    this->bgcolor.a = a;
}

bool glictWindow::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
    if (evt == GLICT_MOUSECLICK || evt == GLICT_MOUSEDOWN || evt == GLICT_MOUSEUP) {
        if (((glictPos*)wparam)->x > this->clipleft &&
            ((glictPos*)wparam)->x < this->clipright &&
            ((glictPos*)wparam)->y > this->cliptop &&
            ((glictPos*)wparam)->y < this->clipbottom) {

            if (((glictPos*)wparam)->y <= this->cliptop+10) {
                if (evt == GLICT_MOUSEDOWN) {
                    this->mousedown = true;
                    this->relmouse.x = ((glictPos*)wparam)->x-this->x;
                    this->relmouse.y = ((glictPos*)wparam)->y-this->y;
                    return true;
                }
            }
            if (evt == GLICT_MOUSEUP && this->mousedown) {
                this->SetPos(
                    ((glictPos*)wparam)->x - this->relmouse.x,
                    ((glictPos*)wparam)->y - this->relmouse.y
                );
                this->mousedown = false;
                return true;
            }

            return DefaultCastEvent(evt, wparam, lparam, returnvalue);
        } else
        // if mouse drag is done outside object...
        if (evt == GLICT_MOUSEUP && this->mousedown) {
            this->SetPos(
                ((glictPos*)wparam)->x - this->relmouse.x,
                ((glictPos*)wparam)->y - this->relmouse.y
            );
            this->mousedown = false;
            return true;
        }
        return DefaultCastEvent(evt, wparam, lparam, returnvalue);
    }

    return false;
}
