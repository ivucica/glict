/*
	GLICT2 - Graphics Library Interface Creation Toolkit 2
	Copyright (c) 2008 Ivan Vucica & Robbert de Vries

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
 * \file container.h
 * \brief Container class header
 * \sa glictContainer
 */

#ifndef __GLICT2_CONTAINER_H
#define __GLICT2_CONTAINER_H

#include <GLICT/eventhandler.h>

class glictContainer : public glictEventHandler 
{
	public:
		glictContainer();  // constructor.
		~glictContainer(); // destructor.
};

#endif
