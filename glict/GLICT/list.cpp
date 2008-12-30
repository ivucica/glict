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
#include <string.h>
glictList::glictList() {
    forcedheight = 14;
    totalheight = 0;
    defocusabilize_element = true;

    strcpy(this->objtype, "List");

}
glictList::~glictList() {
}

void glictList::AddObject(glictContainer* object) {
    object->SetPos(0, totalheight);
    if(forcedheight)
        object->SetHeight(forcedheight);
    totalheight += object->GetHeight()+object->GetTopSize()+object->GetBottomSize();

    glictContainer::AddObject(object);
    listlist.push_back(object);
    if (defocusabilize_element)
        object->SetFocusable(false);
    SetVirtualSize(width, totalheight);
    SetWidth(width);

}
void glictList::RemoveObject(glictContainer *object) {

    float currentheight = 0;
    for (std::list<glictContainer*>::iterator it = listlist.begin() ; it != listlist.end() ; it++) {
        if ((*it)!=object) {
            bool proceed = true;
            for (std::vector<glictContainer*>::iterator it2 = delayedremove.begin(); it2 != delayedremove.end(); it2++) {
                if ((*it2) == (*it)) {
                    //printf("Hit into an already removed object\n");
                    proceed = false;
                    break;
                }
            }
            if (!proceed)  // the object that needs to be shifted has been removed already. skip.
                continue;

            (*it)->SetPos(0, currentheight);
            currentheight += (*it)->GetHeight();
        } else {
            std::list<glictContainer*>::iterator oldit=it;
            if (listlist.begin() != it) {
                it--;
                listlist.erase(oldit); // since this is std::list we can safely resume
            } else {
                listlist.erase(it);
                if (listlist.size())
                    it = listlist.begin();
                else
                    break;
            }
        }
    }
    totalheight = currentheight;
    glictContainer::RemoveObject(object);
    SetWidth(width);
}

void glictList::SetWidth(float width) {
    glictPanel::SetWidth(width);
    RebuildList();
}
void glictList::SetHeight(float height) {
    glictPanel::SetHeight(height);
    RebuildList();
}

void glictList::RebuildList(){

    int currentheight=0;
    for (std::list<glictContainer*>::iterator it = listlist.begin() ; it != listlist.end() ; it++) {
        (*it)->SetPos(0, currentheight);
        if (forcedheight)
            (*it)->SetHeight(forcedheight);
        (*it)->SetWidth(width - GetCurrentVScrollbarWidth() - (*it)->GetLeftSize() - (*it)->GetRightSize());
        currentheight += (*it)->GetHeight()+(*it)->GetTopSize()+(*it)->GetBottomSize();
    }

    totalheight = currentheight;
    //SetVirtualSize(width, totalheight);
}
void glictList::SetForcedHeight(int _fh){
    this->forcedheight = _fh;
}
