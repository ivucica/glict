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
    printf("MessageBox generated.\n");


    this->AddObject(&pnlMessage);
    pnlMessage.SetHeight(90);
    pnlMessage.SetWidth(300);
    pnlMessage.SetPos(0,10);

    this->AddObject(&btnOk);
    btnOk.SetBGColor(1.0,0.0,0.0,1.0);
    btnOk.SetOnClick(_glictMessageBox_Closer);
    btnOk.SetCaption("Ok");
    btnOk.SetHeight(16);
    btnOk.SetWidth(64);
    btnOk.SetPos(150 - 32, 95 - 16);

    SetHeight(100);
    SetWidth(300);

    this->SetCaption("Message Box");
    this->SetMessage("Sample message box.");
    strcpy(objtype, "MessageBox");
}


glictMessageBox::~glictMessageBox() {
    this->RemoveObject(&btnOk);
}

void glictMessageBox::Paint() {
    printf("msgbox\n");
    //system("pause");
    printf("now painting window:\n");

    //static_cast<glictWindow*>(this))->Paint();
    glictWindow::Paint();
    printf("window painted\n");
    //system("pause");
}

void glictMessageBox::SetMessage(std::string msg) {
    this->mesg = msg;
    pnlMessage.SetCaption(msg);
}

void glictMessageBox::SetOnDismiss(void(*OnDism)(glictPos* relmousepos, glictContainer* callerclass)) {
    OnDismissFunction = OnDism;
}
