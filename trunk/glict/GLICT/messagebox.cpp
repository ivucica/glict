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
#include <string.h>
#include "messagebox.h"
#include "globals.h"

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

    textoffsetx = textoffsety = 0;

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

void glictMessageBox::SetHeight(float h) {
	glictWindow::SetHeight(h);

	btnOk.SetPos(width/2 - 32, height - 21);
	if (this->GetEnabled())
		pnlMessage.SetHeight(h-10 - (glictGlobals.windowBodySkin ? glictGlobals.windowBodySkin->GetBottomSize().h : 0) - textoffsety);
	else
		pnlMessage.SetHeight(h - (glictGlobals.windowBodySkin ? glictGlobals.windowBodySkin->GetBottomSize().h : 0) - textoffsety);
	pnlMessage.SetPos(textoffsetx,textoffsety);//containeroffsety);
    pnlMessage.SetHeight(h);


}
void glictMessageBox::SetWidth(float w) {
	glictWindow::SetWidth(w);

	btnOk.SetPos(width/2 - 32, height - 21);
	if (this->GetEnabled())
		pnlMessage.SetWidth(w-10 - (glictGlobals.windowBodySkin ? glictGlobals.windowBodySkin->GetRightSize().w : 0) - textoffsetx);
	else
		pnlMessage.SetWidth(w - (glictGlobals.windowBodySkin ? glictGlobals.windowBodySkin->GetRightSize().w : 0) - textoffsetx);
	pnlMessage.SetPos(textoffsetx,textoffsety);//containeroffsety);
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
void glictMessageBox::Focus(glictContainer* c) {
	if (!c)
		btnOk.Focus(NULL);
	else
		glictContainer::Focus(c);
}

void glictMessageBox::SetTextOffset(int x, int y){
    textoffsetx = x;
    textoffsety = y;
    SetHeight(GetHeight());
    SetWidth(GetWidth());
}
