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





/**
 * \file container.h
 * \brief Container class header
 * \sa glictContainer
 */



#ifndef _GLICT_CONTAINER_H
#define _GLICT_CONTAINER_H
#include <vector>
#include <string>
#include "types.h"
using namespace std;



/**
 * \brief Container class, base for all other widgets
 *
 * This is the Container class, which is actually a base class for all
 * other widgets. Althought this one does not render itself, it actually
 * contains most of reusable code for the widgets to inherit.
 * An example of these inherited functionalities are children management,
 * children rendering, proper clipping, mouse coordinate transformation, et al.
 */
class glictContainer  {
    public:
        glictContainer(); ///< Constructor for the class.
        glictContainer(long guid); ///< Constructor for the class, specifying guid. (Guid is unused at the moment)
        ~glictContainer(); ///< Destructor for the class.

        // the following elements are replaced, not necessarily inherited in child  classes
        virtual void Paint(); ///< Renders the element. Should contain call to CPaint(). (Does not actually render anything in this class; intended for redefinition in classes that inherit this one.)
        virtual bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue); ///< Casts an event to the class, so it can process it. Example is mouseclick or mousedown.

        // the following elements are always inherited from container
        void CPaint(); ///< Renders all children.
        void AddObject(glictContainer* object); ///< Adds an object as a child.
        void RemoveObject(glictContainer* object); ///< Removes an object that's a child.
        void SetHeight(int h); ///< Sets object's height.
        void SetWidth(int w); ///< Sets object's width.
        void SetPos(int x, int y); ///< Sets object's position using classical access using two integers.
        void SetPos(glictPos pos); ///< Sets object's position using predefined type. Useful when exchanging data between library's functions.
        void GetPos(int* x, int* y); ///< Gets object's position and writes it into integers x and y.
        void GetPos(glictPos* pos); ///< Gets object's position and writes it into predefined type. Useful when exchanging data between library's functions.
        void GetSize(glictSize* size); ///< Gets object's size (height and width) and writes it into predefined type. TODO: integer version
        void SetScissor(); ///< This one adjusts the clipping window through which something can be seen, and which is set by SetClip

        void RememberTransformations(); ///< When calling parent's paint, call it's 'remember transformations' too, so clicking detection is done properly. if clicking unused, or no transformations done, then not important
        void ResetTransformations(); ///< Resets transformations to default transf matrix (identity matrix)
        void TransformScreenCoords(glictPos *pos); ///< Transforms screen coordinates into plane coordinates

        bool CastEvent(glictEvents evt, void* wparam, long lparam); ///< Casts an event omitting the returnvalue. (deprecated, I'm lazy and don't want to rewrite code so I abuse namespace)
        bool DefaultCastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue); ///< Casts an event into default event processor, omitting the widget's code.

        void SetOnClick(void(*OnClickFunction)(glictPos* relmousepos, glictContainer* callerclass)); ///< Sets a function to execute upon click.
        void SetCaption(std::string caption); ///< Sets the caption of the control, if supported.

        void ReportDebug(); ///< Reports debug information to stdout.


        int height, width; ///< Current height and width of the widget. FIX: Make private.
        int x, y; ///< Current position of the widget. FIX: Make private
        int left, right, top, bottom; ///< Current boundaries of the widget, calculated from height, width, x and y. FIX: Make private.
        int clipleft, clipright, cliptop, clipbottom; ///< Current clipping boundaries of the widget. Somewhat depends on the parent's clipping. FIX: Make private.


    private:
        // these should be called only internally
        void SetRect(int left, int top, int right, int bottom); ///< Internal function. Sets the boundaries of the widget.
        void SetClip(int left, int top, int right, int bottom); ///< Internal function. Sets the clipping boundaries of the widget.



    protected: // remark: protected allows access to friends, private doesnt
        vector <glictContainer*> objects; ///< Contains all the children objects.
        /*int height, width;
        int x, y;
        int left, right, top, bottom;
        int clipleft, clipright, cliptop, clipbottom;*/
        char objtype[50]; ///< Short descriptive string containing name of the object. (Each class actually rewrites this one upon intialization in constructor.)
        glictContainer* parent; ///< Pointer to class' parents.
        unsigned int guid; ///< Unique identifier; unused so far
        float ModelviewMatrix[16]; ///< Modelview matrix, as remembered last time RememberTransformations() was called.

        void(*OnClick)(glictPos* relmousepos, glictContainer* callerclass); ///< Pointer to function specified as OnClick function.


        std::string caption; ///< Caption written on the control, if control supports it.

};
#endif
