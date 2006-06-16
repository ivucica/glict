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

#include <stdlib.h> // to make system() work
glictMessageBox::glictMessageBox() {
    printf("MessageBox generated.\n");
    this->AddObject(&btnOk);
    btnOk.SetBGColor(1.0,0.0,0.0,1.0);
    strcpy(objtype, "MessageBox");
}
glictMessageBox::~glictMessageBox() {
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
