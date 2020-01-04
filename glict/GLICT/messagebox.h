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


#ifndef __GLICT_MESSAGEBOX_H
#define __GLICT_MESSAGEBOX_H
#include <string>
#include <GLICT/window.h>
#include <GLICT/button.h>
#include <GLICT/panel.h>
#include <GLICT/types.h>

/// Derived from window, this widget contains a panel with settable caption, and an OK button which closes the messagebox and possibly calls a settable function
class glictMessageBox : public glictWindow {
	public:
		glictMessageBox();
		~glictMessageBox();

		void Paint();
		void SetMessage(std::string msg); //< Sets the message that will be displayed on the messagebox. (Not the caption, the message itself!)
		void SetOnDismiss(void(*OnClickFunction)(glictPos* relmousepos, glictContainer* callerclass)); //< Sets function to be called upon dismiss of the msgbox. OBJECT MUST NOT DESTROY ITSELF OR REMOVE ITSELF FROM OBJECT LIST OF ITS PARENT FROM WITHIN.
		void SetHeight(float height);
		void SetWidth(float width);
		void SetEnabled(bool enabled);
		void SetBGColor(float r, float g, float b, float a);
		void Focus(glictContainer* c);
		void SetTextOffset(int x, int y);
		void SetButtonCaption(std::string caption);
		void SetButtonFont(std::string font, unsigned int size=10);
		void SetButtonAlignment(glictVerticalAlignment valign);
		void SetButtonMarginX(float marginX);
		void SetButtonMarginY(float marginY);
		void SetButtonWidth(float width);
		void SetButtonHeight(float height);
		void EnableSeparator(glictSkinner *skin, float height=2, float marginX=2, float marginY=2, float r=0, float g=0, float b=0, float a=1);

	private:
		glictButton btnOk;
		glictPanel pnlMessage;
		glictPanel pnlSeparator;
		glictVerticalAlignment buttonVAlign;
		int buttonMarginX, buttonMarginY;
		int separatorMarginX, separatorMarginY;
		std::string mesg;
		int textoffsetx, textoffsety;
		void(*OnDismissFunction)(glictPos* relmousepos, glictContainer* callerclass); //< Called upon dismiss of the msgbox. OBJECT MUST NOT DESTROY ITSELF OR REMOVE ITSELF FROM OBJECT LIST OF ITS PARENT FROM WITHIN.

		void RepositionButton();
		void RepositionSeparator();

	friend void _glictMessageBox_Closer(glictPos* relmousepos, glictContainer* caller);
};
#endif
