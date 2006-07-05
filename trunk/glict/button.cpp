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

/**
 * \file container.cpp
 * \brief Button class code
 * \sa glictButton
 */


#include <GL/glut.h>
#include <stdio.h>
#include "button.h"
#include "globals.h"
#include "glut-helper.h"

/**
  * It fills up the class with default infos.
  */
glictButton::glictButton() {
    //printf("init panele\n");
    this->bgcolor.r = 1.0;
    this->bgcolor.g = 1.0;
    this->bgcolor.b = 1.0;
    this->bgcolor.a = 1.0;

    this->fgcolor.r = 1.0;
    this->fgcolor.g = 1.0;
    this->fgcolor.b = 1.0;
    this->fgcolor.a = 1.0;



    strcpy(this->objtype, "Button");

    this->highlighted = false;
    this->parent = NULL;

    this->focusable = true;

    this->caption = "Button";
}

/**
  * Currently does nothing.
  */
glictButton::~glictButton() {

}

/**
  * \param evt Event to process. Processed are GLICT_MOUSEUP, GLICT_MOUSEDOWN
  *            and GLICT_MOUSECLICK.
  * \param *wparam Pointer parameter depending on event processed
  * \param lparam Numeric parameter depending on event processed.
  * \param returnvalue Not filled up with any currently processed event.
  * \return Boolean value, true if event processed
  *
  * If evt is mouse-related, then this function understands *wparam as a
  * pointer to glictPos. If these coordinates are within clipping boundaries,
  * then different actions are taken depending on the precise event. Even if
  * they are not, in case the event is a mouse release we dehighlight the
  * button, yet we still trick the parents that the event was not processed.
  *
  * \sa glictContainer::CastEvent()
  */
bool glictButton::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
    //printf("Event of type %s passing through %s (%s)\n", EvtTypeDescriptor(evt), objtype, parent ? parent->objtype : "NULL");
    switch (evt) {
        case GLICT_MOUSEUP:
        case GLICT_MOUSEDOWN:
        case GLICT_MOUSECLICK:
            if (((glictPos*)wparam)->x >= this->clipleft &&
                ((glictPos*)wparam)->x <= this->clipright &&
                ((glictPos*)wparam)->y >= this->cliptop &&
                ((glictPos*)wparam)->y <= this->clipbottom) {
                //printf("Within button\n");
                if (evt == GLICT_MOUSECLICK) {
                    //this->Focus(NULL);
                    printf("Caught button click!\n",0,0);
                }

                if (evt == GLICT_MOUSEUP) { // the trick is that button doesnt need to be dereleased inside window to be dereleased! however it also doesnt do default click behaviour
                    //printf("Dehighlighting button\n");
                    highlighted = false;
                }
                if (evt == GLICT_MOUSEDOWN) {
                    //printf("Highlighting button\n",0,0);
                    highlighted = true;
                }

                if (DefaultCastEvent(evt, wparam, lparam, returnvalue)) { // if a child caught click, we dont handle it otherwise
                    //printf("Default even was cast\n");
                    return true; // we simply return
                }
                // otherwise we could handle it mroe ...
                //return false;
                return true;
            }


            if (evt == GLICT_MOUSEUP) { // the trick is that button doesnt need to be dereleased inside window to be dereleased! however it also doesnt do default click behaviour
                //printf("Dehighlighting button while outside its borders\n");
                highlighted = false;
                return false;
            }
            break;
    }



    return false;
}

/**
  * Paints a button. Button is a square thing rendered within boundaries
  * specified by SetRect(). The square's color is specified with SetBGColor().
  * Upon rendering the square, white text specified with SetCaption() is placed
  * on center of the widget.
  */
void glictButton::Paint() {
    if (!highlighted) {
        glColor4f(
            (float)this->bgcolor.r,
            (float)this->bgcolor.g,
            (float)this->bgcolor.b,
            (float)this->bgcolor.a
        );
    }
    else {
        glColor4f(
            1.0, 1.0, 1.0, 1.0
        );
//        printf("RENDERING HIGHLIGHTED BUTTON\n");
    }
    glBegin(GL_QUADS);
    glVertex2f(this->x,this->y);
    glVertex2f(this->x+this->width,this->y);
    glVertex2f(this->x+this->width,this->y+this->height);
    glVertex2f(this->x,this->y+this->height);
    glEnd();

    glColor4f(0., 0., 0., 1.);

    glPushMatrix();
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glutxStrokeString(
        this->caption.c_str(),
        GLUT_STROKE_MONO_ROMAN,
        this->x + this->width / 2. - glutxStrokeSize(this->caption.c_str(), GLUT_STROKE_MONO_ROMAN) / 2.,
        (this->y + this->height / 2. + 5. - 5.*((float)glutxNumberOfLines(this->caption.c_str())-1.))*-1.
        );
    glPopMatrix();
    this->CPaint();
}
/**
  * \param r Red element of the color.
  * \param g Green element of the color.
  * \param b Blue element of the color.
  * \param a Alpha element of the color.
  *
  * Sets the background color of the button that'll be used in
  * Paint().
  */
void glictButton::SetBGColor(float r, float g, float b, float a) {
    this->bgcolor.r = r;
    this->bgcolor.g = g;
    this->bgcolor.b = b;
    this->bgcolor.a = a;
}

/**
  * \param r Red element of the color.
  * \param g Green element of the color.
  * \param b Blue element of the color.
  * \param a Alpha element of the color.
  *
  * Sets the foreground color of the button that'll be used in
  * Paint(). Namely, the text color.
  */
void glictButton::SetFGColor(float r, float g, float b, float a) {
    this->fgcolor.r = r;
    this->fgcolor.g = g;
    this->fgcolor.b = b;
    this->fgcolor.a = a;
}
