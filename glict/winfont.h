// NOT A PART OF GLICT.
// This code is not licensed under GNU GPL.

#ifndef __WINFONT_H
#define __WINFONT_H


#define WINFONT_BOLD 1
#define WINFONT_ITALIC 2
#define WINFONT_UNDERLINE 4
#define WINFONT_STRIKEOUT 8

#include <windows.h>
#include <GL/gl.h>



typedef struct {
	GLuint base;
	GLYPHMETRICSFLOAT gmf[256];
} WinFontStruct;

WinFontStruct *WinFontCreate(char* fontname, char style, char size);
void WinFontDelete(void* font);
float WinFontSize(const char* txt, const void* fontvoid);
void WinFontDraw(const char* txt, const void* fontvoid, float x, float y);

#endif
