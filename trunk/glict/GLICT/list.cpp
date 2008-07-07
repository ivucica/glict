/*
	GLICT - Graphics Library Interface Creation Toolkit
	Copyright (C) 2006-2008 Ivan Vucica

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
#include <GLICT/list.h>
#include <GLICT/globals.h>
glictList::glictList() {

}
glictList::~glictList() {
}

void glictList::AddObject(glictContainer* object) {
    object->SetPos(0, objects.size()*14 - 14 );
    object->SetHeight(14);

    glictContainer::AddObject(object);
	object->SetFocusable(false);
    SetVirtualSize(width, objects.size() * 14 - 14);
    SetWidth(width);

}

void glictList::RemoveObject(glictContainer *object) {

    float currentheight = 0;
    for (std::vector<glictContainer*>::iterator it = objects.begin() ; it != objects.end() ; it++) {
        if ((*it)!=object) {
            (*it)->SetPos(0, currentheight);
            currentheight += 14;
        }
    }
    glictContainer::RemoveObject(object);
    SetHeight(currentheight);
}

void glictList::SetWidth(float width) {
    for (std::vector<glictContainer*>::iterator it = objects.begin() ; it != objects.end() ; it++) {
        (*it)->SetWidth(width-(virtualsize.h > height ? GetScrollbarWidth() : 0));
    }
    glictContainer::SetWidth(width);
}
