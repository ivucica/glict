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

#ifndef __GLICT_SCROLLBAR_H
#define __GLICT_SCROLLBAR_H
#include <string>
#include <GLICT/container.h>
#include <GLICT/types.h>
class glictScrollbar : public glictContainer {
	public:
		glictScrollbar();
		~glictScrollbar();

		void SetBGColor(float r, float g, float b, float a);
		void SetValue(int val);
		int GetValue();
		void SetStep (unsigned int newstep); ///<How much is increased/decreased by each step
		unsigned int GetStep();
		void SetMin (int newmin);
		int GetMin();
		void SetMax (int newmax);
		int GetMax();
		// redefined functions
		virtual void Paint();
		bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);
	private:
		glictColor bgcolor;
		
		// upper and lower "button" highlighted?
		bool highlightup;
		bool highlightdn;

		int min, max, value;
		unsigned int step;
		friend class glictContainer;
};
#endif
