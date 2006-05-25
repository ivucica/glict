#include <GL/glut.h>
#include <stdio.h>
#include "panel.h"
#include "globals.h"
glitPanel::glitPanel() {
    //printf("init panele\n");
    this->bgcolor.r = 1.0;
    this->bgcolor.g = 1.0;
    this->bgcolor.b = 1.0;
    this->bgcolor.a = 1.0;
    strcpy(this->objtype, "Panel");
    
    this->parent = NULL;
}    
glitPanel::~glitPanel() {
    
}
void glitPanel::Paint() {

//    printf("panel\n");
    
    glColor4f(
        (float)this->bgcolor.r,
        (float)this->bgcolor.g,
        (float)this->bgcolor.b,
        (float)this->bgcolor.a
    );
    glBegin(GL_QUADS);
    glVertex2f(this->x,this->y);
    glVertex2f(this->x+this->width,this->y);
    glVertex2f(this->x+this->width,this->y+this->height);
    glVertex2f(this->x,this->y+this->height);
    glEnd();
    
    this->CPaint();
}
void glitPanel::SetBGColor(float r, float g, float b, float a) {
    this->bgcolor.r = r;
    this->bgcolor.g = g;
    this->bgcolor.b = b;
    this->bgcolor.a = a;
}
