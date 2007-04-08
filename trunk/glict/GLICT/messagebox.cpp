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
#include "messagebox.h"
#include <GL/glut.h>

void _glictMessageBox_Closer(glictPos* relmousepos, glictContainer* caller) {
	void(*UponClick)(glictPos* relmousepos, glictContainer* callerclass);
	glictContainer* parent=caller->GetParent(); // this gets the messagebox
	glictContainer* parent2=parent->GetParent(); // this gets the parent of messagebox
	UponClick = dynamic_cast<glictMessageBox*>(parent)->OnDismissFunction;
	parent2->RemoveObject(parent);
	if (UponClick) UponClick(relmousepos, parent);

}

glictMessageBox::glictMessageBox() {
	//printf("MessageBox generated.\n");



	this->AddObject(&pnlMessage);
	pnlMessage.SetHeight(90);
	pnlMessage.SetWidth(300);
	pnlMessage.SetPos(0,0);//containeroffsety);
	pnlMessage.SetBGColor(0.75,0.75,0.75,1.);
    pnlMessage.SetBGActiveness(false);

	this->AddObject(&btnOk);
	btnOk.SetBGColor(0.6,0.6,0.6,1.0);
	btnOk.SetOnClick(_glictMessageBox_Closer);
	btnOk.SetCaption("Ok");
	btnOk.SetHeight(16);
	btnOk.SetWidth(64);
	btnOk.SetPos(300/2 - 32, 100 - 21);
	btnOk.Focus(NULL);

	SetWidth(300);
	SetHeight(100);

	this->SetCaption("Message Box");
	this->SetMessage("Sample message box.");
	strcpy(objtype, "MessageBox");

	this->focusable = true;

	this->OnDismissFunction = NULL;

	FixContainerOffsets();
}


glictMessageBox::~glictMessageBox() {
	this->RemoveObject(&btnOk);
}

void glictMessageBox::Paint() {
	if (!GetVisible()) return;

	glictWindow::Paint();
}

void glictMessageBox::SetMessage(std::string msg) {
	this->mesg = msg;
	pnlMessage.SetCaption(msg);
}

void glictMessageBox::SetOnDismiss(void(*OnDism)(glictPos* relmousepos, glictContainer* callerclass)) {
	OnDismissFunction = OnDism;
}

void glictMessageBox::SetHeight(int h) {
	glictWindow::SetHeight(h);

	btnOk.SetPos(width/2 - 32, height - 21);
	if (this->GetEnabled())
		pnlMessage.SetHeight(h-10 - containeroffsety);
	else
		pnlMessage.SetHeight(h - containeroffsety);
	pnlMessage.SetPos(0,0);//containeroffsety);

}
void glictMessageBox::SetWidth(int w) {
	glictWindow::SetWidth(w);

	btnOk.SetPos(width/2 - 32, height - 21);
	pnlMessage.SetWidth(w);

}
void glictMessageBox::SetEnabled(bool enabled) {
	glictContainer::SetEnabled(enabled);
	btnOk.SetVisible(enabled);
	this->SetHeight(this->height);
}
void glictMessageBox::SetBGColor(float r, float g, float b, float a) {
	glictWindow::SetBGColor(r,g,b,a);
	pnlMessage.SetBGColor(r,g,b,a);
}
