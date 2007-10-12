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





/* This file is now used only to support examples. Its functionality is now
   superseded by fonts.cpp which offers a wider variety of font support and
   extensibility. */


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

char* glutxStrokeStringExpert(const char* txt, const void* font) {

    glPushMatrix();
    char *p;
    for (p = (char*)txt; *p && *p!='\n' && *p!='\r'; p++) {
        glutStrokeCharacter((void*)font,*p);
    }
    glPopMatrix();
    return p;
}
#include <stdio.h>
void glutxStrokeString(const char* txt, const void* font, float x, float y) {


		glRotatef(180.0, 1.0, 0.0, 0.0);
		//glTranslatef(0, -5,0);

	printf("%s %g %g\n", txt, x, y);
	char* fromwhere=(char*)txt;
    do {
        if (fromwhere!=txt) fromwhere++;

		glTranslatef(x,-y-1,0);
        //if (font==GLUT_STROKE_ROMAN)
        //    glScalef(0.1,0.075,0.075);
        //else
        //    glScalef(0.075,0.075,0.075);
        glScalef(.0075, .0075, .0075);

        fromwhere = glutxStrokeStringExpert(fromwhere,font);

        glScalef(1./.0075, 1./.0075, 1./.0075);
		glTranslatef(-x,y+1,0);
        y += 1;
    } while (*fromwhere);

    //printf("Rendered %s\n", txt);

		//glTranslatef(0, 10,0);
		glRotatef(180.0, -1.0, 0.0, 0.0);

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


float glutxStrokeSize(const char* txt, const void* font) {


//    printf(" SIZE OF %s\n", txt);

    int size=0, len=strlen(txt);
    int maxsize=0;
    for (int i=0;i<len;i++) {
        size+=glutStrokeWidth((void*)font,txt[i]);
        if (txt[i]=='\n' || txt[i]=='\r') {
            if (size>maxsize) maxsize=size;
            if (i < len && (txt[i]=='\n' && txt[i+1]=='\r' || txt[i]=='\r' && txt[i+1]=='\n')) i++;
            size=0;
        }

    }
    if (size>maxsize) maxsize=size;
    size = maxsize;


    // scale it down appropriately (like we've done before)
    //if (font != GLUT_STROKE_ROMAN)
    //    return (float)size*0.075;
    //else
    //    return (float)size*0.1;//0.12
    return (float)size * .0075;

}
int glutxNumberOfLines(const char* txt) {
    int count=1; // at least 1 line
    //DEBUGPRINT("NUMBEROFLINES: ");
    for (unsigned int i=0;i<strlen(txt);i++) {
        if (txt[i]=='\n') {
            count++;
        } //else DEBUGPRINT("%d ",txt[i]);
    }

    return count;
}
