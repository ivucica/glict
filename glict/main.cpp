#include <windows.h>
//#include <wingdi.h>
//#include <GL/gl.h>

#include <GL/glut.h>
#include <stdio.h>
#include "container.h"
#include "panel.h"
#include "window.h"
#include "globals.h"
#include "types.h"

glitWindow panela2;
glitPanel panela4;

unsigned int windowhandle;
glitContainer desktop;
int ww, wh;

void display();

void mouse(int button, int shift, int mousex, int mousey) {
    glitPos pos;
    pos.x = mousex; pos.y = mousey;
    desktop.CastEvent(GLIT_MOUSECLICK, &pos, 0);


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
    desktop.SetWidth(x/2);
    //printf("Postavljam visinu desktopa\n");
    //desktop.SetHeight(y/2);
    desktop.SetHeight(y);
    //printf("Postavljam poziciju desktopa\n");
    desktop.SetPos(0, 10);
    
    panela2.SetHeight(y/2);
    panela2.SetPos(10,15);
    
    glitGlobals.h = y;
    glitGlobals.w = x;
    ww = x; wh = y;
//    desktop.SetPos(0,0);

    desktop.ReportDebug();
    display();
}    
void display() {
    glDisable(GL_SCISSOR_TEST);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);


//    glPushMatrix();
//    glRotatef(15.0,0.0,0.0,1.0);
    
    desktop.SetScissor();
    desktop.Paint();
    
//        glPopMatrix();

    glutSwapBuffers();
    //desktop.ReportDebug();
}   
void glinit() {
    glitPanel* panela = new glitPanel;
    //panela2 = new glitWindow;
    glitPanel* panela3 = new glitPanel;
    desktop.AddObject((panela));
    panela->SetBGColor(0.5,0.5,1.0,1.0);
    panela->AddObject((&panela2));
	panela->SetWidth(256);
	panela->SetHeight(256);
    
    panela2.SetBGColor(0.2,1.0,0.2,1.0);
    panela2.SetPos(3,0);
    panela2.SetHeight(50);
    panela2.SetWidth(400);
    
    panela2.SetCaption("Sample Window");
    /*panela3->SetPos(12,12);
    panela2->AddObject(panela3);*/

    panela2.AddObject(&panela4);
    panela4.SetHeight(128);
}
int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize (640, 480);
    glutInitWindowPosition (0, 0);
    
    windowhandle = glutCreateWindow ("GLIT Demo");

    glinit();
       
    glutSetWindow(windowhandle);
    glutShowWindow();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();  
	return 0;
}
