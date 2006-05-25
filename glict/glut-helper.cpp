/**********************************
 *           THE OUTCAST          *
 *       Codename: Tibia3d        *
 *                                *
 * copyright 2005 OBJECT Networks *
 *********************************/
 
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
