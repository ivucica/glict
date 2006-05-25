#include <GL/glut.h>
#include <stdio.h>
#include "window.h"
#include "globals.h"
#include "glut-helper.h"
glitWindow::glitWindow() {
    
    this->bgcolor.r = 0.75;
    this->bgcolor.g = 0.75;
    this->bgcolor.b = 0.75;
    this->bgcolor.a = 1.0;
    strcpy(this->objtype, "Window");
    
    this->parent = NULL;
    this->SetCaption("Untitled Window");
}    
glitWindow::~glitWindow() {
    
}
void glitWindow::Paint() {

//    printf("panel\n");
    
    
    
    glColor4f(
        (float)this->bgcolor.r,
        (float)this->bgcolor.g,
        (float)this->bgcolor.b,
        (float)this->bgcolor.a
    );
    glBegin(GL_QUADS);
    glVertex2f(this->x,this->y+12);
    glVertex2f(this->x+this->width,this->y+12);
    glVertex2f(this->x+this->width,this->y+this->height);
    glVertex2f(this->x,this->y+this->height);
    glEnd();
    
    
    
    this->CPaint();
    
    
    this->SetScissor();
	
	glColor4fv(glitGlobals.windowTitleBgColor);
    
    glBegin(GL_QUADS);
    glVertex2f(this->x,this->y);
    glVertex2f(this->x + this->width, this->y);
    glVertex2f(this->x + this->width, this->y+12);
    glVertex2f(this->x, this->y+12);
    glEnd();
//    glDisable(GL_SCISSOR_TEST);
    glColor4fv(glitGlobals.windowTitleColor);
    
    glPushMatrix();
    //glRotatef(180.0, 0.0, 0.0, 1.0);
    //glutxStrokeString(this->caption.c_str(),GLUT_STROKE_ROMAN, (this->x + this->width / 2 + glutxStrokeSize(this->caption.c_str(), GLUT_STROKE_ROMAN) / 2) * -1, this->y - 9);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    //glDisable(GL_SCISSOR_TEST);
    glutxStrokeString(this->caption.c_str(),GLUT_STROKE_ROMAN, (this->width / 2 - glutxStrokeSize(this->caption.c_str(), GLUT_STROKE_ROMAN) / 2) , this->y*-1 - 9);
    //glEnable(GL_SCISSOR_TEST);
    glPopMatrix();
}
void glitWindow::SetBGColor(float r, float g, float b, float a) {
    this->bgcolor.r = r;
    this->bgcolor.g = g;
    this->bgcolor.b = b;
    this->bgcolor.a = a;
}
void glitWindow::SetCaption(std::string caption) {
    this->caption = caption;
}    
