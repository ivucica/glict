/*
	GLICT - Graphics Library Interface Creation Toolkit
	Copyright (C) 2006-2008 Ivan Vucica

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
#include <stdio.h>
#include <string.h>
#include "button.h"
#include "globals.h"
#include "fonts.h"

#include <algorithm>

/**
  * It fills up the class with default infos.
  */
glictButton::glictButton() {
	//printf("init panele\n");
	this->bgcolor.r = 0.0;
	this->bgcolor.g = 0.0;
	this->bgcolor.b = 0.0;
	this->bgcolor.a = 1.0;

	this->fgcolor.r = 1.0;
	this->fgcolor.g = 1.0;
	this->fgcolor.b = 1.0;
	this->fgcolor.a = 1.0;


	strcpy(this->objtype, "Button");

	this->highlighted = false;
	this->hold = false;
	this->parent = NULL;

	this->focusable = true;

	this->caption = "Button";

	this->skin = NULL;
	this->hskin = NULL;
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
		case GLICT_KEYPRESS:
			switch (*((char*)wparam)) {
				case 9:
					if (next) {
						next->Focus(NULL);
						return true;
					}
					else if (parent && parent->GetNext()) {
						parent->GetNext()->Focus(NULL);
						return true;
					}
					break;
				case 13:
				case 32:
				{
					glictPos p = {this->clipleft, this->cliptop};
					if (OnClick) {
						OnClick(&p, this);
						return true;
					}
				}

			}
			return false;
			break;
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

    glictSkinner*  skin_final = glictGlobals.buttonSkin;
    glictSkinner* hskin_final = glictGlobals.buttonHighlightSkin;

    if (this->skin) skin_final = this->skin;
    if (this->hskin) hskin_final = this->hskin;



	if (glictGlobals.debugCallback) {
		glictGlobals.debugCallback(strlen("glictButton::Paint()"), "glictButton::Paint()");
	}

	/* DEBUGGING CODE REMOVEME
    if (glictGlobals.clipperCallback)
        glictGlobals.clipperCallback(clipleft,clipright,cliptop,clipbottom);
    glictGlobals.PaintRectLines(left,right,top,bottom);
            glictGlobals.PaintRectLines(this->x+glictGlobals.translation.x, this->x+this->width+glictGlobals.translation.x,
								this->y+glictGlobals.translation.y, this->y+this->height+glictGlobals.translation.y);
    */



	this->SetScissor();

	if (!((skin_final && (!highlighted && !hold)) || (hskin_final && (highlighted || hold)))) {
		// if skin is not set, then let's draw a plain rectangle
		glictColor col;
        if (!highlighted || !hold) {
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
		// if skin is set, then draw appropriate skin
        glictSize s = {this->width, this->height};

        glictGlobals.Translatef(this->x, this->y, 0);
        if (!highlighted && !hold)
            skin_final->Paint(&s);
        else
            hskin_final->Paint(&s);
        glictGlobals.Translatef(-this->x, -this->y, 0);

	}

	// restore color
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
        );
    }

	if (glictGlobals.drawFocus)
		if (this == glictGlobals.topFocused) {
			// if focused, draw the "focused" line
			glictColor col;

			col.r = this->bgcolor.r < .5 ? (float)this->bgcolor.r * 1.6 : (float)this->bgcolor.r / 1.6;
			col.g = this->bgcolor.g < .5 ? (float)this->bgcolor.g * 1.6 : (float)this->bgcolor.g / 1.6;
			col.b = this->bgcolor.b < .5 ? (float)this->bgcolor.b * 1.6 : (float)this->bgcolor.b / 1.6;
			col.a = this->bgcolor.a;

			glictGlobals.PaintRectLines(this->x+glictGlobals.translation.x + 3, this->x+this->width+glictGlobals.translation.x - 3,
									this->y+glictGlobals.translation.y + 3, this->y+this->height+glictGlobals.translation.y - 3,
									col);

		}

	//glPushMatrix();
	if (highlighted) glictGlobals.Translatef(1.,1.,0.);

    glictColor oldcol = glictFontColor(fontname.c_str());
    glictFontColor(fontname.c_str(), captioncolor);
	glictFontRender(
		this->caption.c_str(),
		fontname.c_str(),
		fontsize,
		this->x + this->width / 2. - glictFontSize(this->caption.c_str(), fontname.c_str(), fontsize) / 2. +glictGlobals.translation.x,
		(this->y + this->height / 2. - (fontsize/2.)*((float)glictFontNumberOfLines(this->caption.c_str())))*+1. +glictGlobals.translation.y
		);
    glictFontColor(fontname.c_str(),oldcol);

    if (highlighted) glictGlobals.Translatef(-1.,-1.,0.);
	//glPopMatrix();


	if (this->OnPaint) {
		glictRect r, c;

		r.top = this->top+containeroffsety;
		r.bottom = this->bottom;
		r.left = this->left+containeroffsetx;
		r.right = this->right;

		c.top = std::max(this->cliptop, this->top+containeroffsety);
		c.bottom = this->clipbottom;
		c.left = std::max(this->clipleft, this->left+containeroffsetx);
		c.right = this->clipright;
		this->OnPaint(&r, &c, this);
	}


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

/**
  * \param holdvalue Should it be rendered as highlighed no matter what
  *
  * If holdvalue is set to true, then we'll be rendering the button as
  * if the mouse is being held on it, no matter what.
  */

void glictButton::SetHold (bool holdvalue) {
	this->hold = holdvalue;
}
