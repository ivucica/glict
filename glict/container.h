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

#ifndef _GLICT_CONTAINER_H
#define _GLICT_CONTAINER_H
#include <vector>
#include "types.h"
using namespace std;
class glictContainer  {
    public:
        glictContainer();
        glictContainer(long guid);
        ~glictContainer();
        
        // the following elements are replaced, not necessarily inherited in child  classes
        virtual void Paint();
        virtual bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);
        
        // the following elements are always inherited from container
        void CPaint();
        void AddObject(glictContainer* object);
        void RemoveObject(glictContainer* object);
        void SetHeight(int h);
        void SetWidth(int w);
        void SetPos(int x, int y);
        void SetPos(glictPos pos);
        void GetPos(int* x, int* y);
        void GetPos(glictPos* pos);
        void GetSize(glictSize* size);
        void SetScissor(); // this one adjusts the clipping window through which something can be seen, and which is set by SetClip
        
        void RememberTransformations(); // when calling parent's paint, call it's 'remember transformations' too, so clicking detection is done properly. if clicking unused, or no transformations done, then not important
        void ResetTransformations(); // resets transformations to default transf matrix (identity matrix)
        void TransformScreenCoords(glictPos *pos); // transforms screen coordinates into plane coordinates
        
        bool CastEvent(glictEvents evt, void* wparam, long lparam);
        bool DefaultCastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);
        
        void SetOnClick(void(*OnClickFunction)(glictPos* relmousepos, glictContainer* callerclass));
        
        void ReportDebug();


		int height, width;
        int x, y;
        int left, right, top, bottom;
        int clipleft, clipright, cliptop, clipbottom;
        
    private:
        // these should be called only internally
        void SetRect(int left, int top, int right, int bottom);
        void SetClip(int left, int top, int right, int bottom);

    protected:
        vector <glictContainer*> objects;
        /*int height, width;
        int x, y;
        int left, right, top, bottom;
        int clipleft, clipright, cliptop, clipbottom;*/
        char objtype[50];
        glictContainer* parent; 
        unsigned int guid;
        float ModelviewMatrix[16];
        
        void(*OnClick)(glictPos* relmousepos, glictContainer* callerclass);
};
#endif
