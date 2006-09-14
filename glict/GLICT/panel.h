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

#ifndef __GLICT_PANEL_H
#define __GLICT_PANEL_H

#include <GLICT/container.h>
#include <GLICT/types.h>
class glictPanel : public glictContainer {
    public:
        glictPanel();
        ~glictPanel();

        void SetBGColor(float r, float g, float b, float a);

        // redefined functions
        void Paint();
        bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);
    private:
        glictColor bgcolor;
};
#endif
