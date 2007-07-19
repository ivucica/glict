/*
	GLICT - Graphics Library Interface Creation Toolkit
	Copyright (C) 2006-2007 OBJECT Networks

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
 * \file button.cpp
 * \brief Button class code
 * \sa glictButton
 */


#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include "button.h"
#include "globals.h"
#include "fonts.h"

/**
  * It fills up the class with default infos.
  */
glictButton::glictButton() {
	//printf("init panele\n");
	this->bgcolor.r = 0.0;
	this->bgcolor.g = 0.0;
	this->bgcolor.b = 0.0;
	this->bgcolor.a = 0.0;

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
	if (!GetVisible() || !GetEnabled()) return false;

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
					//printf("Caught button click!\n",0,0);
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
			} else {
			    //printf("BUTTON DID NOT FIND THIS THING. X, Y: %d %d Clip: %d %d %d %d\n", ((glictPos*)wparam)->x, ((glictPos*)wparam)->y, clipleft, clipright, cliptop, clipbottom);
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
	if (!GetVisible()) return;


	if (glictGlobals.debugCallback) {
		glictGlobals.debugCallback(strlen("glictButton::Paint()"), "glictButton::Paint()");
	}

	this->SetScissor();

	if (!((glictGlobals.buttonSkin && !highlighted) || (glictGlobals.buttonHighlightSkin && highlighted))) {
		glictColor col;
        if (!highlighted) {
            col = bgcolor;
        } else {
            col.r = this->bgcolor.r < .5 ? (float)this->bgcolor.r * 1.5 : (float)this->bgcolor.r / 1.5;
			col.g = this->bgcolor.g < .5 ? (float)this->bgcolor.g * 1.5 : (float)this->bgcolor.g / 1.5;
			col.b = this->bgcolor.b < .5 ? (float)this->bgcolor.b * 1.5 : (float)this->bgcolor.b / 1.5;
			col.a = this->bgcolor.a;
        }
        glictGlobals.PaintRect(this->x+glictGlobals.translation.x, this->x+this->width+glictGlobals.translation.x,
								this->y+glictGlobals.translation.y, this->y+this->height+glictGlobals.translation.y,
								col);

	} else {
        glictSize s = {this->width, this->height};

        glictGlobals.Translatef(this->x, this->y, 0);
        if (!highlighted)
            glictGlobals.buttonSkin->Paint(&s);
        else
            glictGlobals.buttonHighlightSkin->Paint(&s);
        glictGlobals.Translatef(-this->x, -this->y, 0);
	}



    if (!highlighted) {
    	glictGlobals.SetColor(
            glictGlobals.buttonTextColor.r,
            glictGlobals.buttonTextColor.g,
            glictGlobals.buttonTextColor.b,
            glictGlobals.buttonTextColor.a
        );
    } else {
        glictGlobals.SetColor(
            glictGlobals.buttonHighlightTextColor.r,
            glictGlobals.buttonHighlightTextColor.g,
            glictGlobals.buttonHighlightTextColor.b,
            glictGlobals.buttonHighlightTextColor.a

            //0.95, 0.95, 0.95, 1.
        );
    }



	//glPushMatrix();
	if (highlighted) glictGlobals.Translatef(2.,2.,0.);



	glictFontRender(
		this->caption.c_str(),
		"system",
		this->x + this->width / 2. - glictFontSize(this->caption.c_str(), "system") / 2. +glictGlobals.translation.x,
		(this->y + this->height / 2. - 5. + 5.*((float)glictFontNumberOfLines(this->caption.c_str())-1.))*+1. +glictGlobals.translation.y
		);


    if (highlighted) glictGlobals.Translatef(-2.,-2.,0.);
	//glPopMatrix();

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
