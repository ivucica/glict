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


#ifndef __GLICT_MESSAGEBOX_H
#define __GLICT_MESSAGEBOX_H
#include <string>
#include <GLICT/window.h>
#include <GLICT/button.h>
#include <GLICT/panel.h>
class glictMessageBox : public glictWindow {
    public:
        glictMessageBox();
        ~glictMessageBox();

        void Paint();
        void SetMessage(std::string msg);
        void SetOnDismiss(void(*OnClickFunction)(glictPos* relmousepos, glictContainer* callerclass));
		void SetHeight(int height);
		void SetWidth(int width);
		void SetEnabled(bool enabled);
		void SetBGColor(float r, float g, float b, float a);
    private:
        glictButton btnOk;
        glictPanel pnlMessage;
        std::string mesg;
        void(*OnDismissFunction)(glictPos* relmousepos, glictContainer* callerclass);
    //    void KillYourself();

    friend void _glictMessageBox_Closer(glictPos* relmousepos, glictContainer* caller);
};
#endif
