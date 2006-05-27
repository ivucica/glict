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

#ifndef _GLICT_TYPES_H
#define _GLICT_TYPES_H
typedef struct {
    union {
        int x,w;
    };
    union {
        int y,h;
    };
} glictPos; 
typedef glictPos glictSize;
typedef struct {
    float r, g, b, a;
} glictColor;
enum glictEvents {
    GLICT_MOUSECLICK = 0,
    GLICT_MOUSEDOWN = 1,
    GLICT_MOUSEUP = 2
};
#endif
