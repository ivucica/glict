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

#ifndef __GLICT_BUTTON_H
#define __GLICT_BUTTON_H

#include <string>

#include "window.h"
#include "types.h"
class glictButton : public glictContainer {
    public:
        glictButton();
        ~glictButton();

        void SetBGColor(float r, float g, float b, float a);
        void SetCaption(std::string caption);
        
        // redefined functions
        void Paint();
        bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);
    private:
        glictColor bgcolor;
        bool highlighted; // is mouse pressed
        std::string caption;
};
#endif

