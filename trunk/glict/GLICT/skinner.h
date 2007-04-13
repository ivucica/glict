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

#ifndef __GLICT_SKINNER_H
#define __GLICT_SKINNER_H
#include <windows.h>
#include <GL/gl.h>
#include <GLICT/types.h>

/// A rectangular area with texturable areas.
///
/// This class is NOT a widget, this is meant to be used as part of other
/// widgets to provide skinnable interface.

class glictSkinner {
    public:
        glictSkinner();
        ~glictSkinner();

        void Paint(glictSize *size); ///< Paints a skinned rectangle of specified total size

        // first, the corner parts. their both height and width get specified.
        // look at how beautifully "aligned" the texxt is :)
        void SetTL(GLuint textureid, glictSize *size);    ///< Sets the top left portion's texture ID and size. You need to set up the texture yourself.
        void SetTR(GLuint textureid, glictSize *size);    ///< Sets the top right portion's texture ID and size. You need to set up the texture yourself.
        void SetBL(GLuint textureid, glictSize *size);    ///< Sets the bottom left portion's texture ID and size. You need to set up the texture yourself.
        void SetBR(GLuint textureid, glictSize *size);    ///< Sets the bottom right portion's texture ID and size. You need to set up the texture yourself.

        void SetTop(GLuint textureid, glictSize *size);   ///< Sets the top middle portion's texture ID and size. This one will be tiled. You need to set up the texture yourself.
        void SetLeft(GLuint textureid, glictSize *size);  ///< Sets the left middle portion's texture ID and size. This one will be tiled. You need to set up the texture yourself.
        void SetRight(GLuint textureid, glictSize *size); ///< Sets the right middle portion's texture ID and size. This one will be tiled.You need to set up the texture yourself.
        void SetBottom(GLuint textureid, glictSize *size);///< Sets the bottom middle portion's texture ID and size. This one will be tiled.You need to set up the texture yourself.

        void SetCenter(GLuint textureid, glictSize *size);///< Sets the central portion's texture ID and size. This one will be tiled.You need to set up the texture yourself.

        glictSize *GetTopSize();
        glictSize *GetLeftSize();
        glictSize *GetRightSize();
        glictSize *GetBottomSize();
    private:
        GLuint topleft; glictSize toplefts;
        GLuint topright; glictSize toprights;
        GLuint bottomleft; glictSize bottomlefts;
        GLuint bottomright; glictSize bottomrights;

        GLuint top; glictSize tops;
        GLuint left; glictSize lefts;
        GLuint right; glictSize rights;
        GLuint bottom; glictSize bottoms;

        GLuint center; glictSize centers;

        glictSize size;
};

#endif
