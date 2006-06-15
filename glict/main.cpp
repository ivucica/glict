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

#include <windows.h>
//#include <wingdi.h>
//#include <GL/gl.h>

#include <GL/glut.h>
#include <stdio.h>
#include "container.h"
#include "button.h"
#include "panel.h"
#include "window.h"
#include "globals.h"
#include "types.h"

glictWindow panela2;
glictPanel panela4;

unsigned int windowhandle;
glictContainer desktop;
int ww, wh;
glictPos mousepos;
void display();

void mouse(int button, int shift, int mousex, int mousey) {
    glictPos pos;
    pos.x = mousex; pos.y = mousey;
    desktop.TransformScreenCoords(&pos);
    if (shift==GLUT_DOWN) desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0);
    if (shift==GLUT_UP) desktop.CastEvent(GLICT_MOUSEUP, &pos, 0);
    glutPostRedisplay();
}
void passivemouse(int mousex, int mousey) {
    mousepos.x = mousex;
    mousepos.y = mousey;
    desktop.TransformScreenCoords(&mousepos);
    display();
}
void reshape(int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,x,y);
    gluOrtho2D(0,x,0,y);
    //glRotatef(180.0, 0.0, 0.0, 1.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glTranslatef(0,-y,0.0);
    //glTranslatef(-x,-y,0.0);
    glMatrixMode(GL_MODELVIEW);
    //printf("Postavljam sirinu desktopa\n");
    desktop.SetWidth((int)(x*0.8));
    //printf("Postavljam visinu desktopa\n");
    //desktop.SetHeight(y/2);
    desktop.SetHeight(y);
    //printf("Postavljam poziciju desktopa\n");
    desktop.SetPos(0, 0);

    panela2.SetHeight(y/2);


    glictGlobals.h = y;
    glictGlobals.w = x;
    ww = x; wh = y;
//    desktop.SetPos(0,0);

    desktop.ReportDebug();
    display();
}
#include <math.h>
float kut = 0.;
void display() {
    glDisable(GL_STENCIL_TEST);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);


    glPushMatrix();
    desktop.RememberTransformations();
    desktop.SetScissor();
    desktop.Paint();
    glPopMatrix();

    glPushMatrix();
    //glTranslatef( sin(kut++ * 3.14 / 180.)*90., 150., 0.);
    //glScalef(2.00,2.00,2.00);
    glTranslatef(100, 0, 0);
    glRotatef(25.0,0.0,0.0,1.0);

    desktop.RememberTransformations();
    desktop.SetScissor();
    desktop.Paint();

    glPopMatrix();

    glDisable(GL_STENCIL_TEST);
    glBegin(GL_QUADS);
    glColor4f(0.5,0.5,0.5,1.0);
    glVertex2f(mousepos.x, mousepos.y);
    glVertex2f(mousepos.x-10, mousepos.y);
    glVertex2f(mousepos.x-10, mousepos.y-10);
    glVertex2f(mousepos.x, mousepos.y-10);
    glEnd();
    glEnable(GL_STENCIL_TEST);

    glutSwapBuffers();
    //desktop.ReportDebug();
}
bool buttonstate = true;
void onpanel5click(glictPos *a, glictContainer* callclass) {
    buttonstate = !buttonstate;

    (dynamic_cast<glictButton*>(callclass))->SetCaption(buttonstate ? "Button" : "Clicked\nButton");
    panela2.SetCaption(buttonstate ? "Sample Window" : "Name Changed"); // window
    if (buttonstate)
        panela2.SetPos(10,15);
    else
        panela2.SetPos(120,90);
}
void glinit() {
    glictPanel* panela = new glictPanel;
    //panela2 = new glictWindow;
    glictPanel* panela3 = new glictPanel;
    glictButton* panela5 = new glictButton;

    desktop.AddObject((panela));
    panela->SetBGColor(0.5,0.5,1.0,1.0);
    panela->AddObject((&panela2));
	panela->SetWidth(600);
	panela->SetHeight(256);

	panela5->SetBGColor(1,0,0,1);
	panela5->SetPos(0,90);
	panela->AddObject(panela5);

    panela2.SetBGColor(0.2,1.0,0.2,1.0);
    panela2.SetPos(10,15);
    panela2.SetHeight(50);
    panela2.SetWidth(400);

    panela2.SetCaption("Sample Window");
    /*panela3->SetPos(12,12);
    panela2->AddObject(panela3);*/

    panela2.AddObject(&panela4);
    panela4.SetHeight(128);

    panela5->SetOnClick(onpanel5click);
    panela5->SetWidth(64);
}
int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize (640, 480);
    glutInitWindowPosition (0, 0);

    windowhandle = glutCreateWindow ("GLICT Demo");

    glinit();

    glutSetWindow(windowhandle);
    glutShowWindow();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);


    glutIdleFunc (display);

    glutPassiveMotionFunc(passivemouse);

    glutMainLoop();
	return 0;
}
