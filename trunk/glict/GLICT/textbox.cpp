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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
	this->allowedchr = "";

	caption = "";
}
glictTextbox::~glictTextbox() {

}
void glictTextbox::Paint() {
	if (!GetVisible()) return;


    if (!glictGlobals.textboxSkin) {
		glictGlobals.PaintRect(this->x+glictGlobals.translation.x, this->x+this->width+glictGlobals.translation.x,
					this->y+glictGlobals.translation.y, this->y+this->height+glictGlobals.translation.y, bgcolor);


/*        glBegin(GL_QUADS);
        glVertex2f(this->x+glictGlobals.translation.x,this->y+glictGlobals.translation.y);
        glVertex2f(this->x+glictGlobals.translation.x,this->y+this->height+glictGlobals.translation.y);
        glVertex2f(this->x+this->width+glictGlobals.translation.x,this->y+this->height+glictGlobals.translation.y);
        glVertex2f(this->x+this->width+glictGlobals.translation.x,this->y+glictGlobals.translation.y);
        glEnd();*/
    } else {
        glictSize s = {this->width, this->height};

        glictGlobals.Translatef(this->x, this->y, 0);
        glictGlobals.textboxSkin->Paint(&s);
        glictGlobals.Translatef(-this->x, -this->y, 0);


    }


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

    glictColor oldcol = glictFontColor(fontname.c_str());
    if (captioncolor.r == captioncolor.g == captioncolor.b == captioncolor.a == 1.)
        glictFontColor(fontname.c_str(), glictGlobals.textboxTextColor);
    else
        glictFontColor(fontname.c_str(), captioncolor);
	if (asterisked && glictGlobals.topFocused != this)
		glictFontRender(asterisked, fontname.c_str(), fontsize, x+(glictGlobals.textboxSkin ? glictGlobals.textboxSkin->GetLeftSize().w : 0) +glictGlobals.translation.x , y+(glictGlobals.textboxSkin ? glictGlobals.textboxSkin->GetTopSize().h : 0) +glictGlobals.translation.y);
	else
		glictFontRender(this->caption.c_str(), fontname.c_str(), fontsize, x+(glictGlobals.textboxSkin ? glictGlobals.textboxSkin->GetLeftSize().w : 0) +glictGlobals.translation.x, y+(glictGlobals.textboxSkin ? glictGlobals.textboxSkin->GetTopSize().h : 0) + glictGlobals.translation.y);
	glictFontColor(fontname.c_str(), oldcol);


	if (glictGlobals.topFocused==this) {
		caption = oldcaption;
	}
	if (asterisked) free(asterisked);
	this->CPaint();

}
void glictTextbox::SetBGColor(float r, float g, float b, float a) {
	this->bgcolor.r = r;
	this->bgcolor.g = g;
	this->bgcolor.b = b;
	this->bgcolor.a = a;
}
/** \brief Receives click event (to get focus) and keys to manipulate its caption
  *
  * \todo Document me!
  */
// Copypastable to other widgets.
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
			switch (*((char*)wparam)) {
				default:
					if (allowedchr.size() == 0 || (allowedchr.find(*((char*)wparam))<allowedchr.size())) { // FIXME (ivucica#1#) one of the conditions concerning if character is contained in allowedchr is redundant, its needed to see what exactly happens if char is not in there and optimize
						this->SetCaption(caption + *((char*)wparam));
					}
					break;
				case 9:
					if (next)
						next->Focus(NULL);
					else if (parent && parent->GetNext())
						parent->GetNext()->Focus(NULL);
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
  * \brief Sets the character that'll be used for password protection.
  * \param asterisk The character that'll be used. If passed a 0, then
  *                 regular characters will be used.
  *
  * Sets the character that'll be used in place of regular characters that are
  * actually stored within the textbox.
  */
void glictTextbox::SetPassProtectCharacter(char asterisk) {
	passprotectchar = asterisk;
}

/**
  * \brief Defines all characters that are allowed to be typed in this textbox
  * \param s String containing all permitted characters.
  *
  * This string is treated as an array of characters which are permitted
  * to be used. If a character does not appear in this list, it will have
  * no effect on the textbox when punched in. If the string is empty,
  * then all characters are permitted.
  */
void glictTextbox::SetAllowedChars (std::string s) {
	allowedchr = s;
}
