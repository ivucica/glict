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
 
// Glut-helper.h
// contains glut helper

// objnet glut helper
// (c) 2005 object networks
//
// contains some helper code for glut
// e.g. entire string printing

#include <GL/glut.h>
#include <string.h>

void DEBUGPRINT(char* txt, ...);

char* glutxStrokeStringExpert(const char* txt, void* font) {
    glPushMatrix();
    char *p;
    for (p = (char*)txt; *p && *p!='\n'; p++) {
        glutStrokeCharacter(font,*p);
    }    
    glPopMatrix();
    return p;
}
void glutxStrokeString(const char* txt, void* font, float x, float y) {
    char* fromwhere=(char*)txt;
    do {
        if (fromwhere!=txt) fromwhere++;
        glPushMatrix();
        glTranslatef(x,y,0);
        if (font==GLUT_STROKE_ROMAN)
            glScalef(0.1,0.075,0.075);
        else
            glScalef(0.075,0.075,0.075);
        fromwhere = glutxStrokeStringExpert(fromwhere,font);
        glPopMatrix();
        y -= 10;
    } while (*fromwhere);
}    

void glutxBitmapString(char* txt, void* font,int x,int y) {
    int len, i;



    glRasterPos2f(x, y);
    len = (int) strlen(txt);
    for (i = 0; i < len; i++) {
      glutBitmapCharacter(font, txt[i]);
    }
    
}
float glutxBitmapSize(char* txt, void* font) {
    int size=0, len=strlen(txt);
    for (int i=0;i<len;i++) {
        size+=glutBitmapWidth(font,txt[i]);
    }    
    return (float)size;
}


float glutxStrokeSize(const char* txt, void* font) {
    int size=0, len=strlen(txt);
    int maxsize=0;
    for (int i=0;i<len;i++) {
        size+=glutStrokeWidth(font,txt[i]);
        if (txt[i]=='\n') {
            if (size>maxsize) maxsize=size;
            size=0;
        }
    }
    if (size>maxsize) maxsize=size;
    size = maxsize;
    
    // scale it down appropriately (like we've done before)
    if (font != GLUT_STROKE_ROMAN)
        return (float)size*0.075;
    else 
        return (float)size*0.1;//0.12
}
int glutxNumberOfLines(char* txt) {
    int count=1; // at least 1 line
    //DEBUGPRINT("NUMBEROFLINES: ");
    for (unsigned int i=0;i<strlen(txt);i++) {
        if (txt[i]=='\n') {
            count++;
        } //else DEBUGPRINT("%d ",txt[i]);
    }
    
    return count;
}
