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
 * \file fonts.h
 * \brief Font management
 *
 * Declares stuff used in fonts.cpp
 * \sa fonts.cpp
 */

#include <string>
#include <vector>
typedef bool(*_glictFontRenderFuncBoolNoSize)(const char* text, void* font, float x, float y) ;
typedef bool(*_glictFontRenderFuncBoolSize)(const char* text, void* font, float size, float x, float y) ;
typedef void(*_glictFontRenderFuncVoidNoSize)(const char* text, void* font, float x, float y) ;
typedef void(*_glictFontRenderFuncVoidSize)(const char* text, void* font, float size, float x, float y) ;


typedef bool(*_glictFontRenderFuncBoolNoSize)(const char* text, void* font, float x, float y) ;
typedef bool(*_glictFontRenderFuncBoolSize)(const char* text, void* font, float size, float x, float y) ;
typedef void(*_glictFontRenderFuncVoidNoSize)(const char* text, void* font, float x, float y) ;
typedef void(*_glictFontRenderFuncVoidSize)(const char* text, void* font, float size, float x, float y) ;


typedef float(*_glictFontSizeFuncSize)(const char* text, void* font, float size) ;
typedef float(*_glictFontSizeFuncNoSize)(const char* text, void* font) ;



class glictFont {
    public:
        glictFont();
        ~glictFont();

        void SetName(const char* newname);
        std::string GetName();
        void SetFontParam(void* font);

        void SetRenderFunc(_glictFontRenderFuncBoolNoSize funcpointer);
        void SetRenderFunc(_glictFontRenderFuncBoolSize funcpointer);
        void SetRenderFunc(_glictFontRenderFuncVoidNoSize funcpointer);
        void SetRenderFunc(_glictFontRenderFuncVoidSize funcpointer);

        void SetSizeFunc(_glictFontSizeFuncSize funcpointer);
        void SetSizeFunc(_glictFontSizeFuncNoSize funcpointer);


        void SetRenderFunc();
    private:
        std::string name;
        _glictFontRenderFuncBoolNoSize RenderBoolNoSize;
        _glictFontRenderFuncBoolSize RenderBoolSize;
        _glictFontRenderFuncVoidNoSize RenderVoidNoSize;
        _glictFontRenderFuncVoidSize RenderVoidSize;

        _glictFontSizeFuncSize SizeSize;
        _glictFontSizeFuncNoSize SizeNoSize;


        void* fontparam;

    friend bool glictFontRender(const char* text, const char* fontname, float x, float y);
    friend bool glictFontRender(const char* text, const char* fontname, float fontsize, float x, float y);

    friend float glictFontSize(const char* name, const char* font);
    friend float glictFontSize(const char* name, const char* font, float size);

};



typedef std::vector<glictFont*> _GLICTFONTVECTOR;
typedef _GLICTFONTVECTOR::iterator _GLICTFONTVECTORITERATOR;


glictFont* glictCreateFont(const char* name);
glictFont* glictFindFont(const char* name);
bool glictDeleteFont(const char* name);

float glictFontSize(const char* name, const char* font);
float glictFontSize(const char* name, const char* font, float size);

int glictFontNumberOfLines(const char* text);

bool glictFontRender(const char* text, const char* fontname, float x, float y);
bool glictFontRender(const char* text, const char* fontname, float fontsize, float x, float y);
