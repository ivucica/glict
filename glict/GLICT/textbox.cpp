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
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include "textbox.h"
#include "globals.h"
#include "fonts.h"
glictTextbox::glictTextbox() {
    this->bgcolor.r = .7;
    this->bgcolor.g = .7;
    this->bgcolor.b = .7;
    this->bgcolor.a = 1.0;
    strcpy(this->objtype, "Textbox");

    this->parent = NULL;
    //printf("Textbox generated.\n");

    this->focusable = true;

    this->SetHeight(14);
    this->SetWidth(100);

	this->passprotectchar = 0;
}
glictTextbox::~glictTextbox() {

}
void glictTextbox::Paint() {
    if (!GetVisible()) return;
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

	std::string oldcaption;
	char* asterisked=NULL;
	if (passprotectchar) {
		asterisked = (char*)malloc(caption.size() + 1);
		for (int i = 0; i < caption.size() ; i++) {
			asterisked[i] = passprotectchar;
		}
		asterisked[caption.size()] = 0;
	}
    if (glictGlobals.topFocused==this) {
		oldcaption = caption;
		if (asterisked)
			this->SetCaption((std::string)(asterisked) + "_");
		else
			this->SetCaption(caption + "_");
    }

	
    glColor4f(1., 1., 1., 1.);
    glPushMatrix();
    glTranslatef(this->x, this->y+10.,0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glColor4f(1.,1.,1.,1.);
	if (asterisked && glictGlobals.topFocused != this) 
		glictFontRender(asterisked, "system", 10, 0, 0);
	else
		glictFontRender(this->caption.c_str(), "system", 10, 0, 0);
		
    glPopMatrix();

    if (glictGlobals.topFocused==this) {
        caption = oldcaption;
    }

    this->CPaint();



}
void glictTextbox::SetBGColor(float r, float g, float b, float a) {
    this->bgcolor.r = r;
    this->bgcolor.g = g;
    this->bgcolor.b = b;
    this->bgcolor.a = a;
}
/// This is the copypasteable castevent usable in other widgets
bool glictTextbox::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
    if (!GetVisible() || !GetEnabled()) return false;
    switch (evt) {
        case GLICT_MOUSEUP:
        case GLICT_MOUSEDOWN:
        case GLICT_MOUSECLICK:
            if (((glictPos*)wparam)->x > this->clipleft &&
                ((glictPos*)wparam)->x < this->clipright &&
                ((glictPos*)wparam)->y > this->cliptop &&
                ((glictPos*)wparam)->y < this->clipbottom) {


                //if (evt==GLICT_MOUSECLICK) MessageBox(0,caption.c_str(),NULL,0);

                // if a child caught click, we dont handle it otherwise
                return DefaultCastEvent(evt, wparam, lparam, returnvalue);
                // otherwise we could handle it mroe but we'll simply tell we didnt proces it

            }
            //It occured outside the textbox, ignored.
            break;
        case GLICT_KEYPRESS:
            //printf("Textbox got %c key.\n", *((char*)wparam));
            switch (*((char*)wparam)) {
                default:
                    this->SetCaption(caption + *((char*)wparam));
                    break;
                case 8:
                    if (caption.size()) {
                        char* Title = (char*)malloc(caption.size()+1);
                        strcpy(Title, caption.c_str());
                        Title[strlen(Title)-1]=0;
                        caption = Title;
                        free(Title);

                    }
                    break;
            }
            break;
    }

    return false;
}
/**
  * \param asterisk The character that'll be used. If passed a 0, then regular characters will be used.
  */
void glictTextbox::SetPassProtectCharacter(char asterisk) {
	passprotectchar = asterisk;
}