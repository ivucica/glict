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
	windowTitleBgColor[0] = 0.0; windowTitleBgColor[1] = 0.0; windowTitleBgColor[2] = 1.0; windowTitleBgColor[3] = 1.0;
	windowTitleColor[0] = 1.0; windowTitleColor[1] = 1.0; windowTitleColor[2] = 1.0; windowTitleColor[3] = 1.0;

	//renderMode = GLICT_RENDERING; defunct

	lastMousePos.x = 0; lastMousePos.y = 0;
	srand(time(NULL));

	topFocused = NULL;


	clippingMode = GLICT_SCISSORTEST;

}
glictGlobalsClass::~glictGlobalsClass() {
}

