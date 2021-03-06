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

// This program is NOT meant to serve as an example,
// it is sort-of a test suite instead, a mish-mash of unrelated
// widgets to facilitate testing. Go to examples/ folder for
// some examples.

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
//#include <wingdi.h>
//#include <GL/gl.h>

#include <GL/glut.h>
#include <stdio.h>
#include <GLICT/container.h>
#include <GLICT/button.h>
#include <GLICT/panel.h>
#include <GLICT/window.h>
#include <GLICT/messagebox.h>
#include <GLICT/textbox.h>
#include <GLICT/list.h>
#include <GLICT/globals.h>
#include <GLICT/fonts.h>
#include <GLICT/skinner.h>
//#include <GLICT/types.h>



#include "glut-helper.h"
#include "texload.h"
glictWindow panela2;
glictPanel panela4;
glictMessageBox msgbox;
glictTextbox textbox;
glictList l;
bool textured=true;


unsigned int windowhandle;
glictContainer desktop, desktop2;
glictWindow window2;
int ww, wh;
glictPos mousepos;
glictSkinner skinner;
void display();


void key(unsigned char key, int x, int y) {
    desktop.CastEvent(GLICT_KEYPRESS, &key, 0);
    glutPostRedisplay();
}

void mouse(int button, int shift, int mousex, int mousey) {
    glictPos pos;
    pos.x = mousex;
    pos.y = mousey;
    desktop.TransformScreenCoords(&pos);
    if (shift==GLUT_DOWN) desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0);
    if (shift==GLUT_UP) desktop.CastEvent(GLICT_MOUSEUP, &pos, 0);

    //char zz[256];
    //sprintf(zz, "%d %d", pos.x, pos.y);
    //glutSetWindowTitle(zz);
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
    ww = x;
    wh = y;
//    desktop.SetPos(0,0);

    //desktop.ReportDebug();
    display();
}
#include <math.h>
float kut = 0.;
void display() {
    glDisable(GL_STENCIL_TEST);
    //glDisable(GL_SCISSOR_TEST);
    //printf("PAINT STARTUP\nPAINT STARTUP\nPAINT STARTUP\n-------------------\n");
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);
    //glEnable(GL_SCISSOR_TEST);
    glPushMatrix();


    //glRotatef(10., 0., 0., 1.);
    desktop.RememberTransformations();
    desktop.Paint();
    glPopMatrix();

    /*glPushMatrix();
    //glTranslatef( sin(kut++ * 3.14 / 180.)*90., 150., 0.);
    //glScalef(2.00,2.00,2.00);
    //glRotatef(5.0,0.0,0.0,1.0);
    glTranslatef(100, 50, 0);

    desktop.RememberTransformations();
    desktop.SetScissor();
    desktop.Paint();

    glPopMatrix();
*/
    glDisable(GL_STENCIL_TEST);
    glBegin(GL_QUADS);
    glColor4f(0.5,0.5,0.5,1.0);
    glVertex2f(mousepos.x, mousepos.y);
    glVertex2f(mousepos.x-10, mousepos.y);
    glVertex2f(mousepos.x-10, mousepos.y-10);
    glVertex2f(mousepos.x, mousepos.y-10);
    glEnd();
    glEnable(GL_STENCIL_TEST);

    /*glEnable(GL_ALPHA_TEST);
    {
        glictSize s = {100, 100};
        skinner.Paint(&s);
    }
    glDisable(GL_ALPHA_TEST);*/

    glutReportErrors();
    //printf("FINISHED PAINT.\n");
    glutSwapBuffers();
    //desktop.ReportDebug();
}
bool buttonstate = true;
void onpanel2paint(glictRect* real, glictRect* clipped, glictContainer* callerclass) {


    glPushAttrib(GL_VIEWPORT);
    glViewport(clipped->left, glictGlobals.h - clipped->bottom, clipped->right - clipped->left, clipped->bottom - clipped->top);
    //printf("%g %g %g %g\n", clipped->left, glictGlobals.h - clipped->bottom, clipped->right - clipped->left, clipped->bottom - clipped->top);
    //glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    //glLoadIdentity();
    //gluOrtho2D(0, 200, 0, 200);


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(100, 200, 0);
    glBegin(GL_TRIANGLES);
    glColor3f(1.,0.,0.);
    glVertex2f(0, 0);
    glColor3f(0.,1.,0.);
    glVertex2f(100,0);
    glColor3f(0.,0.,1.);
    glVertex2f(100,100);
    glEnd();

    glColor3f(1,1,1);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    //glMatrixMode(GL_PROJECTION);
    //glPopMatrix();

    glPopAttrib();
}

void omd(glictPos *a, glictContainer* callclass) {
    //printf("OMD\n");
}
void omu(glictPos *a, glictContainer* callclass) {
    //printf("OMU\n");
}
void onnbclick(glictPos *a, glictContainer *callclass) {
    l.RemoveObject(callclass);
    l.DelayedRemove();
    delete callclass;
}
void onlb2click(glictPos *a, glictContainer* callclass) {
    glictButton* nb = new glictButton;
    nb->SetOnClick(onnbclick);
    l.AddObject(nb);
}
void onpanel5click(glictPos *a, glictContainer* callclass) {
    buttonstate = !buttonstate;

    (dynamic_cast<glictButton*>(callclass))->SetCaption(buttonstate ? "Button" : "Clicked\n(indeed)\nvery much");
    (dynamic_cast<glictButton*>(callclass))->SetWidth(100);
    (dynamic_cast<glictButton*>(callclass))->SetFont("system", buttonstate ? 10 : 5);

    panela2.SetCaption(buttonstate ? "Sample Window" : textbox.GetCaption()); // window

    if (buttonstate)
        panela2.SetPos(0,0);
    else
        panela2.SetPos(120,90);
}


void paintrectcallback(float left, float right, float top, float bottom, glictColor &col) {
	if (col.a >= 0) glColor4f(col.r-0.5, col.g, col.b*0, col.a);
	/*glBegin(GL_LINES);

		glVertex2f(left, top-1);
		glVertex2f(left, bottom);

		glVertex2f(left, bottom);
		glVertex2f(right-1, bottom);

		glVertex2f(right-1, top-1);
		glVertex2f(right-1, bottom);


		glVertex2f(right-1, top-2);
		glVertex2f(left, top-2);
	glEnd();*/


	glBegin(GL_QUADS);
	glVertex2f(left, top);
	glVertex2f(left, bottom);
	glVertex2f(right, bottom);
	glVertex2f(right, top);
	glEnd();
}


void prlc(float left, float right, float top, float bottom, const glictColor &col) {

	glColor4f(col.r, col.g, col.b, col.a);

	glBegin(GL_LINE_LOOP);
	glVertex2f(left, top);
	glVertex2f(left, bottom);
	glVertex2f(right, bottom);
	glVertex2f(right, top);
	glEnd();
}


		glictWindow login;
		glictPanel pnlLogin;
		glictPanel pnlLoginProtocol, pnlLoginServer, pnlLoginUsername, pnlLoginPassword;
		glictTextbox txtLoginProtocol, txtLoginServer, txtLoginUsername, txtLoginPassword;
		glictButton btnLoginLogin, btnLoginCancel;



void glinit() {
    glictPanel* panela = new glictPanel;
    //panela2 = new glictWindow;
    glictPanel* panela3 = new glictPanel;
    glictButton* panela5 = new glictButton;
    glictButton* panela6 = new glictButton;

    desktop.AddObject((panela));
    panela->SetBGColor(0.5,0.5,1.0,1.0);
    panela->AddObject((&panela2));
    panela->SetWidth(600);
    panela->SetHeight(512);


    panela5->SetBGColor(1,0,0,1);
    panela5->SetPos(0,90);
    //panela5->SetCustomData((void*)"Hello");
    panela->AddObject(panela5);


    panela2.SetBGColor(0.2,1.0,0.2,1.0);
    panela2.SetPos(0,0);
    panela2.SetHeight(50);
    panela2.SetWidth(400);

    panela2.SetCaption("Sample Window");
    panela2.SetOnPaint(onpanel2paint);

    /*panela3->SetPos(12,12);
    panela2->AddObject(panela3);*/


    panela2.AddObject(&panela4);
    panela4.SetHeight(128);
    panela4.SetCaption("pnl");
    panela4.SetVirtualSize(500,500);
    panela4.SetPos(10,20);
    panela4.AddObject(panela6);

    panela6->SetBGColor(1,0,0,1);
    panela6->SetPos(4,4);
    panela6->SetPos(0,115);

    panela5->SetOnClick(onpanel5click);
    panela5->SetWidth(64);


    panela->AddObject(&msgbox);
    msgbox.SetMessage("Hello there. What's better,\nto live or to die? You should not\ntake care about such\nsilly stuff.");
    msgbox.SetCaption("Nice Messagebox");
    msgbox.SetPos(50,50);
    msgbox.SetHeight(120);

    panela2.AddObject(&textbox);
    textbox.SetPos(50,0);
    textbox.SetCaption("Text");
    textbox.SetHeight(64);
    textbox.SetWidth(250);
    textbox.SetAllowedChars("abc");

    desktop2.AddObject(&window2);
    desktop2.SetHeight(200);
    desktop2.SetWidth(200);
    window2.SetHeight(25);
    window2.SetWidth(25);
    window2.SetPos(100,100);





    glictButton* lb1 = new glictButton, *lb2 = new glictButton;
    panela->AddObject(&l);
    l.AddObject(lb1);
    l.AddObject(lb2);
    l.SetPos(100,0);
	l.SetWidth(100);
    l.SetHeight(70);
    lb1->SetCaption("oi");
    lb2->SetCaption("me");
    lb2->SetOnClick(onlb2click);


	panela->AddObject(&login);
	login.SetWidth(300);
	login.SetHeight(180);
	login.SetCaption("Log in");
	login.AddObject(&pnlLogin);
	login.SetPos(200, 100);
	pnlLogin.SetCaption("Please enter the username, password and \n"
	                    "server address of the server to connect to.\n"
	                    "\n"
	                    "We advise you not to connect to CipSoft's\n"
	                    "servers using this client, as this is a \n"
	                    "breach of Tibia Rules.");
	pnlLogin.SetWidth(300);
	pnlLogin.SetPos(0,0);
	pnlLogin.SetHeight(92);
	pnlLogin.SetBGActiveness(false);

    char tmp[256] = {0};

	login.AddObject(&pnlLoginProtocol);
	pnlLoginProtocol.SetCaption("Protocol:");
	pnlLoginProtocol.SetPos(0, 5*15);
	pnlLoginProtocol.SetHeight(14);
	pnlLoginProtocol.SetWidth(70);
	pnlLoginProtocol.SetBGActiveness(false);
	login.AddObject(&txtLoginProtocol);
	txtLoginProtocol.SetPos(100, 5*15);
	txtLoginProtocol.SetHeight(14);
	txtLoginProtocol.SetWidth(150);
	txtLoginProtocol.SetNext(&txtLoginServer);

    txtLoginProtocol.SetCaption(tmp);

	login.AddObject(&pnlLoginServer);
	pnlLoginServer.SetCaption("Server:");
	pnlLoginServer.SetPos(0, 6*15);
	pnlLoginServer.SetHeight(14);
	pnlLoginServer.SetWidth(70);
	pnlLoginServer.SetBGActiveness(false);
	login.AddObject(&txtLoginServer);
	txtLoginServer.SetPos(100, 6*15);
	txtLoginServer.SetHeight(14);
	txtLoginServer.SetWidth(150);
	txtLoginServer.SetNext(&txtLoginUsername);

	txtLoginServer.SetCaption( tmp );

	login.AddObject(&pnlLoginUsername);
	pnlLoginUsername.SetCaption("Username:");
	pnlLoginUsername.SetPos(0, 7*15);
	pnlLoginUsername.SetHeight(14);
	pnlLoginUsername.SetWidth(70);
	pnlLoginUsername.SetBGActiveness(false);
	login.AddObject(&txtLoginUsername);
	txtLoginUsername.SetPos(100, 7*15);
	txtLoginUsername.SetHeight(14);
	txtLoginUsername.SetWidth(150);
	txtLoginUsername.SetNext(&txtLoginPassword);

	txtLoginUsername.SetCaption(tmp);

	login.AddObject(&pnlLoginPassword);
	pnlLoginPassword.SetCaption("Password:");
	pnlLoginPassword.SetPos(0, 8*15);
	pnlLoginPassword.SetHeight(14);
	pnlLoginPassword.SetWidth(70);
	pnlLoginPassword.SetBGActiveness(false);
	login.AddObject(&txtLoginPassword);
	txtLoginPassword.SetPos(100, 8*15);
	txtLoginPassword.SetHeight(14);
	txtLoginPassword.SetWidth(150);
	txtLoginPassword.SetPassProtectCharacter('*');
	txtLoginPassword.SetNext(&btnLoginLogin);

	txtLoginPassword.SetCaption(tmp);

	login.AddObject(&btnLoginLogin);
	btnLoginLogin.SetPos(170, 14 + 9*15);
	btnLoginLogin.SetWidth(130);
	btnLoginLogin.SetCaption("Log in");
	btnLoginLogin.SetBGColor(.6,.6,.6,1.);
	btnLoginLogin.SetNext(&btnLoginCancel);

	login.AddObject(&btnLoginCancel);
	btnLoginCancel.SetPos(0, 14 + 9*15);
	btnLoginCancel.SetWidth(130);
	btnLoginCancel.SetCaption("Cancel");
	btnLoginCancel.SetBGColor(.6,.6,.6,1.);
	btnLoginCancel.SetNext(&txtLoginProtocol);




    if (txtLoginPassword.GetCaption() != "") {
        //printf("%s !!!!!!!!!!!\n", txtLoginPassword.GetCaption().c_str());
    }




    glictGlobals.clippingMode = GLICT_STENCILTEST;
    //glictGlobals.clippingMode = GLICT_SCISSORTEST;

	glictGlobals.drawFocus = true;
	glictGlobals.paintrectlinesCallback = prlc;

    if (textured) {
        glictSize elementsize = {20, 20};
        skinner.SetTL(BitmapLoad("topleft.bmp"), &elementsize);
        skinner.SetTR(BitmapLoad("topright.bmp"), &elementsize);
        skinner.SetBL(BitmapLoad("bottomleft.bmp"), &elementsize);
        skinner.SetBR(BitmapLoad("bottomright.bmp"), &elementsize);

        skinner.SetTop(BitmapLoad("top.bmp"), &elementsize);
        skinner.SetLeft(BitmapLoad("left.bmp"), &elementsize);
        skinner.SetRight(BitmapLoad("right.bmp"), &elementsize);
        skinner.SetBottom(BitmapLoad("bottom.bmp"), &elementsize);

        skinner.SetCenter(BitmapLoad("center.bmp"), &elementsize);


    }

    if (textured) {
        glictSkinner *skn = new glictSkinner, *skn2 = new glictSkinner;
        glictSize elementsize = {8, 8};
        skn->SetTL(BitmapLoad("topleft.bmp"), &elementsize);
        skn->SetTR(BitmapLoad("topright.bmp"), &elementsize);
        skn->SetBL(BitmapLoad("bottomleft.bmp"), &elementsize);
        skn->SetBR(BitmapLoad("bottomright.bmp"), &elementsize);

        skn->SetTop(BitmapLoad("top.bmp"), &elementsize);
        skn->SetLeft(BitmapLoad("left.bmp"), &elementsize);
        skn->SetRight(BitmapLoad("right.bmp"), &elementsize);
        skn->SetBottom(BitmapLoad("bottom.bmp"), &elementsize);

        skn->SetCenter(BitmapLoad("center.bmp"), &elementsize);


        panela4.SetOnMouseDown(omd);
        panela4.SetOnMouseUp(omu);



        skn2->SetTL(BitmapLoad("toplefth.bmp"), &elementsize);
        skn2->SetTR(BitmapLoad("toprighth.bmp"), &elementsize);
        skn2->SetBL(BitmapLoad("bottomlefth.bmp"), &elementsize);
        skn2->SetBR(BitmapLoad("bottomrighth.bmp"), &elementsize);

        skn2->SetTop(BitmapLoad("toph.bmp"), &elementsize);
        skn2->SetLeft(BitmapLoad("lefth.bmp"), &elementsize);
        skn2->SetRight(BitmapLoad("righth.bmp"), &elementsize);
        skn2->SetBottom(BitmapLoad("bottomh.bmp"), &elementsize);

        skn2->SetCenter(BitmapLoad("centerh.bmp"), &elementsize);


        glictGlobals.windowBodySkin = skn;

        glictGlobals.buttonSkin = skn;
        glictGlobals.buttonHighlightSkin = skn2;
        glictGlobals.buttonTextColor.r = 0;
        glictGlobals.buttonTextColor.g = 0;
        glictGlobals.buttonTextColor.b = 0;
        glictGlobals.buttonTextColor.a = 1;

        glictGlobals.windowTitleColor.r = 0;
        glictGlobals.windowTitleColor.g = 0;
        glictGlobals.windowTitleColor.b = 0;
        glictGlobals.windowTitleColor.a = 1;

        glictGlobals.panelTextColor.r = 0;
        glictGlobals.panelTextColor.g = 0;
        glictGlobals.panelTextColor.b = 0;
        glictGlobals.panelTextColor.a = 1;


    }

    //glictGlobals.enableGlTranslate = false;
    //glictGlobals.paintrectCallback = paintrectcallback;


    glictFont* sysfont = glictCreateFont("system");

    sysfont->SetFontParam(GLUT_STROKE_MONO_ROMAN);
    sysfont->SetRenderFunc(glutxStrokeString);
    sysfont->SetSizeFunc(glutxStrokeSize);


/*
		sysfont->SetFontParam(WinFontCreate("Arial", WINFONT_BOLD, 7));
		sysfont->SetRenderFunc(WinFontDraw);
		sysfont->SetSizeFunc(WinFontSize);
*/

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

    glutKeyboardFunc(key);
//    glutIdleFunc (display);



    //glutPassiveMotionFunc(passivemouse);

    glutMainLoop();
    return 0;
}
