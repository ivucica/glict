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



/* This file is now defunct. Its functionality is now superseded by fonts.h,
   except in this example. */



// Glut-helper.cpp
// contains glut helper

// objnet glut helper
// (c) 2005 object networks
//
// contains some helper code for glut
// e.g. entire string printing

#ifndef _glutx_
#define _glutx_
void glutxStrokeString(const char* txt, const void* font, float x, float y) ;
char* glutxStrokeStringExpert(const char* txt, const void* font);
void glutxBitmapString(char* txt, const void* font,int x, int y) ;
float glutxBitmapSize(char* txt, const void* font);
float glutxStrokeSize(const char* txt, const void* font);
int glutxNumberOfLines(const char* txt);
#endif
