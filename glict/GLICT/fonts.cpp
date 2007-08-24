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
 * \file fonts.cpp
 * \brief Font management
 *
 * This file manages font output. Fonts are managed inside a font list, and
 * for each font a render function, a size function and optionally a 'font'
 * parameter that would be passed to the render and size functions. Two
 * versions of size and render functions are supported, one that specifies the
 * size and the other that does not do such a thing. Only one shall be active.
 * When the funt is used, correct function is determined, and font size may be
 * adjusted by code in this file in case the font renderer does not support
 * it. If a bitmap font is being rendered then this file cannot do much to
 * resize it.
 */


#ifdef WIN32
	// GL.h doesnt like it that we dont include windows.h in MSVC
	#include <windows.h>
#endif

#include <vector>
#include <GL/gl.h>
#include "fonts.h"

_GLICTFONTVECTOR glictFonts;

/** \brief Creates the font for further use
  * \param name Specifies the font name that'll be used when referencing to
  * this font
  *
  * Using this function, font space is created and reserved for future use.
  * To make this font space make sense, you need to assign the render and size
  * functions. Specify the render function using SetRenderFunc(), and specify
  * the size function using SetSizeFunc(). They are overloaded and there are
  * several flavours of each, so you have some flexibility in designing your
  * font system.
  *
  * \sa glictFont::SetRenderFunc(), glictFont::SetSizeFunc()
  */

glictFont* glictCreateFont(const char* name) {

	if (glictFindFont(name)) return NULL;


	glictFont* fnt = new glictFont;
	if (!fnt) return NULL;


	glictFonts.insert(glictFonts.end(), fnt);
	fnt->SetName(name);
	return fnt;
}

glictFont* glictFindFont(const char* name) {

	_GLICTFONTVECTORITERATOR it;
	for (it = glictFonts.begin(); it<glictFonts.end() && strcmp((*it)->GetName().c_str(), name); it++);
	if (it!=glictFonts.end()) return *it;
	return NULL;
}

bool glictDeleteFont(const char* name) {
	_GLICTFONTVECTORITERATOR it;

	for (it = glictFonts.begin(); it!=glictFonts.end() && strcmp((*it)->GetName().c_str(), name); it++);
	if (it!=glictFonts.end()) {
		delete *it;
		glictFonts.erase(it);
		return true;

	}
	return false;
}

/** \brief Renders the font using default size
  * \param text Specifies the text being drawn
  * \param fontname Specifies name of font used for rendering
  * \param x X coordinate of destination
  * \param y Y coordinate of destination
  *
  * Renders the text specified using the default font size. This function
  * finds the font specified by fontname parameter, and then calls its
  * render function.
  *
  * If needed to call many times, you should consider finding the class of the
  * font you're trying to render using glictFindFont() and remember its pointer,
  * then call it directly, especially if you load tons of fonts.
  *
  * Default size is set to 10. Font resizer believes the font is rendered
  * at size 1.
  *
  * There is an overloaded variant of this function that allows specifying font
  * size as third parameter.
  *
  * \sa glictFont::SetRenderFunc
  */

bool glictFontRender(const char* text, const char* fontname, float x, float y) {
	return glictFontRender(text, fontname, 10, x, y);
}


/** \brief Renders the font using specified size
  * \param text Specifies the text being drawn
  * \param fontname Specifies name of font used for rendering
  * \param fontsize Specifies size of font used for rendering
  * \param x X coordinate of destination
  * \param y Y coordinate of destination
  *
  * Renders the text specified using the default font size. This function
  * finds the font specified by fontname parameter, and then calls its
  * render function.
  *
  * If needed to call many times, you should consider finding the class of the
  * font you're trying to render using glictFindFont() and remember its pointer,
  * then call it directly, especially if you load tons of fonts.
  *
  * Default size is set to 10. Font resizer believes the font is rendered
  * at size 1.
  *
  * There is an overloaded variant of this function that renders using default
  * font size.
  *
  *
  */

#ifdef NO_GL
#define glMatrixMode(x)
#define glScalef(x,y,z)

#endif
bool glictFontRender(const char* text, const char* fontname, float fontsize, float x, float y) {

	glictFont* fnt;
    glMatrixMode(GL_MODELVIEW);

	fnt = glictFindFont(fontname);
	if (!fnt) {
		printf("*** glictFontRender: Font %s not loaded\n", fontname);
		return false;
	}
	if (strcmp(fnt->GetName().c_str(), fontname)) printf("*** glictFontRender: Font name different from what we searched for\n");


	if (fnt->RenderBoolSize) {
		return fnt->RenderBoolSize(text, fnt->fontparam, fontsize, x, y); //
	}
	if (fnt->RenderBoolNoSize) {
		//glPushMatrix();

		glScalef(fontsize, fontsize, fontsize);

		bool r = fnt->RenderBoolNoSize(text, fnt->fontparam, x/fontsize, y/fontsize);
		glScalef(1./fontsize, 1./fontsize, 1./fontsize);
		//glPopMatrix();
		return r;
	}
	if (fnt->RenderVoidSize) {
		fnt->RenderVoidSize(text, fnt->fontparam, fontsize, x, y); //
		return true;
	}
	if (fnt->RenderVoidNoSize) {
		//glPushMatrix();
		glScalef(fontsize, fontsize, fontsize);
		fnt->RenderVoidNoSize(text, fnt->fontparam, x/fontsize, y/fontsize);
		glScalef(1./fontsize, 1./fontsize, 1./fontsize);
		//glPopMatrix();
		return true;
	}

	printf("*** glictFontRender: Font %s not renderable\n", fontname);
	return false;
}


float glictFontSize(const char* text, const char* font) {
	return glictFontSize(text, font, 10);
}

float glictFontSize(const char* text, const char* font, float size) {

	glictFont* fnt = glictFindFont(font);
	if (!fnt) printf("*** glictFontSize: Font %s not found\n", font);
	if (!fnt) return false;
	if (fnt->SizeSize) {
		return fnt->SizeSize(text, fnt->fontparam, size); //

	}
	if (fnt->SizeNoSize) {
		return fnt->SizeNoSize(text, fnt->fontparam)*size;
	}
	return 0;
}

int glictFontNumberOfLines(const char* txt) {
	int count=1; // at least 1 line
	for (unsigned int i=0;i<strlen(txt);i++) {
		if (txt[i]=='\n') {
			count++;
		}
	}

	return count;
}

/////////////////////////////NOW THE CLASS THINGIES///////////////////


glictFont::glictFont() {
	// nope
}

glictFont::~glictFont() {
	// nope
}

void glictFont::SetName(const char* name) {
	this->name = name;
}
std::string glictFont::GetName() {
	return this->name;
}

/// Sets the render function that will be used to draw font. (Version with size, with success return)
void glictFont::SetRenderFunc(_glictFontRenderFuncBoolSize funcpointer) {
	this->RenderBoolNoSize = NULL;
	this->RenderBoolSize = funcpointer;
	this->RenderVoidNoSize = NULL;
	this->RenderVoidSize = NULL;
}

/// Sets the render function that will be used to draw font. (Version without size, with success return)
void glictFont::SetRenderFunc(_glictFontRenderFuncBoolNoSize funcpointer) {
	this->RenderBoolNoSize = funcpointer;
	this->RenderBoolSize = NULL;
	this->RenderVoidNoSize = NULL;
	this->RenderVoidSize = NULL;
}

/// Sets the render function that will be used to draw font. (Version with size, without success return)
void glictFont::SetRenderFunc(_glictFontRenderFuncVoidSize funcpointer) {
	this->RenderBoolNoSize = NULL;
	this->RenderBoolSize = NULL;
	this->RenderVoidNoSize = NULL;
	this->RenderVoidSize = funcpointer;
}

/// Sets the render function that will be used to draw font. (Version without size, without success return)
void glictFont::SetRenderFunc(_glictFontRenderFuncVoidNoSize funcpointer) {
	this->RenderBoolNoSize = NULL;
	this->RenderBoolSize = NULL;
	this->RenderVoidNoSize = funcpointer;
	this->RenderVoidSize = NULL;
}

/// Sets the size function that will be used to get text width. (Version with size)
void glictFont::SetSizeFunc(_glictFontSizeFuncSize funcpointer) {
	this->SizeNoSize = NULL;
	this->SizeSize = funcpointer;
}

/// Sets the size function that will be used to get text width. (Version without size)
void glictFont::SetSizeFunc(_glictFontSizeFuncNoSize funcpointer) {
	this->SizeNoSize = funcpointer;
	this->SizeSize = NULL;
}

void glictFont::SetFontParam(void* fp) {
	this->fontparam = fp;
}
