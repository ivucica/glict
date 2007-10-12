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

#include <stdlib.h>
#include <time.h>
#include "globals.h"


glictGlobalsClass glictGlobals;

glictGlobalsClass::glictGlobalsClass() {
	windowTitleBgColor.r = 0.0; windowTitleBgColor.g = 0.0; windowTitleBgColor.b = 1.0; windowTitleBgColor.a = 1.0;
	windowTitleColor.r = 1.0; windowTitleColor.g = 1.0; windowTitleColor.b = 1.0; windowTitleColor.a = 1.0;
    windowTitleSkin = NULL;
    windowBodySkin = NULL;
    buttonSkin = NULL;
    buttonHighlightSkin = NULL;
    textboxSkin = NULL;
    buttonTextColor.r = 1.; buttonTextColor.g = 1.; buttonTextColor.b = 1.; buttonTextColor.a = 1.;
    buttonHighlightTextColor.r = 1.; buttonHighlightTextColor.g = 1.; buttonHighlightTextColor.b = 1.; buttonHighlightTextColor.a = 1.;
    panelTextColor.r = 1.; panelTextColor.g = 1.; panelTextColor.b = 1.; panelTextColor.a = 1.;
    textboxTextColor.r = 1.; textboxTextColor.g = 1.; textboxTextColor.b = 1.; textboxTextColor.a = 1.;

	drawPartialOut = true;

	lastMousePos.x = 0; lastMousePos.y = 0;
	srand(time(NULL));

	topFocused = NULL;

	clippingMode = GLICT_NOCLIP;

	this->debugCallback = NULL;
	this->paintrectCallback = NULL;
	this->paintrectlinesCallback = NULL;
	#ifndef NO_GL
	this->enableGlTranslate = true; // if we want SDL or any other rendering engine, we gotta turn this to false
	#else
	this->enableGlTranslate = false;
	#endif
	translation.x = 0; translation.y = 0;
}
glictGlobalsClass::~glictGlobalsClass() {
}

void glictGlobalsClass::Translatef(float x, float y, float z) {
    if (enableGlTranslate) {
    	#ifndef NO_GL
        glTranslatef(x,y,z);
        #endif
    } else {
        translation.x += x;
        translation.y += y;
    }
}
void glictGlobalsClass::PaintRect(float left, float right, float top, float bottom ) {
	glictColor tmp (-1,-1,-1,-1);
	PaintRect(left, right, top, bottom, tmp);
}
void glictGlobalsClass::PaintRect(float left, float right, float top, float bottom, glictColor &col) {
	if (paintrectCallback) {
		paintrectCallback (left, right, top, bottom, col)  ;
	} else {
		#ifndef NO_GL
		if (col.a >= 0) glColor4f(col.r, col.g, col.b, col.a);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(left, top);
		glTexCoord2f(0, 1);
		glVertex2f(left, bottom);
		glTexCoord2f(1, 1);
		glVertex2f(right, bottom);
		glTexCoord2f(1, 0);
		glVertex2f(right, top);
		glEnd();
		#endif
	}
}

void glictGlobalsClass::PaintRectLines(float left, float right, float top, float bottom ) {
	glictColor tmp (-1,-1,-1,-1);
	PaintRectLines(left, right, top, bottom, tmp);
}
void glictGlobalsClass::PaintRectLines(float left, float right, float top, float bottom, glictColor &col) {
	if (paintrectCallback) {
		paintrectCallback (left, right, top, bottom, col)  ;
	} else {
		#ifndef NO_GL
		if (col.a >= 0) glColor4f(col.r, col.g, col.b, col.a);
		glBegin(GL_LINE_LOOP);
		glTexCoord2f(0, 0);
		glVertex2f(left, top);
		glTexCoord2f(0, 1);
		glVertex2f(left, bottom);
		glTexCoord2f(1, 1);
		glVertex2f(right, bottom);
		glTexCoord2f(1, 0);
		glVertex2f(right, top);
		glEnd();
		#endif
	}
}

void glictGlobalsClass::SetColor(float r, float g, float b, float a) {
	#ifndef NO_GL
	glColor4f(r,g,b,a);
	#endif
}
