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






#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "container.h"
#include "types.h"
#include "globals.h"


#include <stdlib.h>

#ifndef min
#define min(a,b) (a<b ? a : b)
#define max(a,b) (a>b ? a : b)
#endif
glictContainer::glictContainer() {
    this->height = 32;
    this->width = 32;
    this->x = 0;
    this->y = 0;
    strcpy(this->objtype,"Container");

    this->parent = NULL;    
    this->SetRect(this->x, this->y, this->x + this->width, this->y + this->height);
    this->SetClip(this->left, this->top, this->right, this->bottom);
    
    this->OnClick = NULL;

    this->guid = rand();
    
    this->ResetTransformations();
    
}
glictContainer::~glictContainer() {
    
}
void glictContainer::ResetTransformations() {
    glPushMatrix();
    glLoadIdentity();
    this->RememberTransformations();
    glPopMatrix();
}    
void glictContainer::AddObject(glictContainer* obj) {
    obj->parent = this;
    this->objects.insert(this->objects.end(), obj);
}
void glictContainer::RemoveObject(glictContainer* object) {
    vector<glictContainer*>::iterator it;
    for (it=objects.begin(); it!=objects.end(); it++) {
        if ((*it)==object) {
            
            delete *it;
            objects.erase(it);
        }    
    }    
}
void glictContainer::SetHeight(int h) {
    this->SetRect(this->left, this->top, this->right, this->bottom + h - this->height);
    this->height = h;

}
void glictContainer::SetWidth(int w) {

    this->SetRect(this->left, this->top, this->right + w - this->width, this->bottom);
    this->width = w;

}
void glictContainer::SetPos(int x, int y) {

    //printf("Postavka pozicije %s (%s) na %d %d\n", objtype, (parent ? parent->objtype : "NULL"), x, y);
    //printf("This->x %d This->y %d\n", this->x, this->y);
    this->SetRect(this->left + x - this->x  , this->top + y - this->y, this->right + x - this->x, this->bottom + y - this->y);
    
    
    this->x = x;
    this->y = y;
    
    
    glictSize size;
    for (vector<glictContainer*>::iterator it=objects.begin(); it!=objects.end(); it++) {
        (*it)->GetPos(&x, &y);
        (*it)->GetSize(&size);
        //printf("Postavljam dijete %s od %s (size: %d %d)\n", (*it)->objtype, objtype, size.w, size.h);
        (*it)->SetRect(this->left + x, this->top + y, this->left + x + size.w, this->top + y + size.h);
        //(*it)->SetClip(max(this->left + x, this->clipleft), max(this->top + y, this->cliptop), min(this->left + x + size.w, this->clipright), min(this->top + y + size.h, this->clipbottom));
    }
}
void glictContainer::SetPos(glictPos pos) {
    this->SetPos(pos.x,pos.y);
}
void glictContainer::GetPos(int* x, int* y) {
    *x = this->x;
    *y = this->y;
    
}
void glictContainer::GetPos(glictPos* pos) {
    pos->x = this->x;
    pos->y = this->y;
}
/* TODO (Khaos#2#): GetWidth(w);
                    GetHeight(h) */
/* TODO (Khaos#2#): GetSize(w,h); */
void glictContainer::GetSize(glictSize* size) {
    size->w = this->width;
    size->h = this->height;
}
void glictContainer::SetRect(int left, int top, int right, int bottom) {
    //printf("%s s parentom %s postaje %d %d %d %d\n", this->objtype, (parent ? parent->objtype : "NULL"), left, right, top, bottom);
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
    
    if (!this->parent) {
		//printf("Sada, %s s parentom %s sebe postavlja na clip %d %d %d %d\n", this->objtype, (parent ? parent->objtype : "NULL"), left, right, top, bottom);
        this->SetClip(left,top,right,bottom);
    } else {
		//printf("Sada, %s s parentom %s sebe postavlja na clip %d %d %d %d (otac r %d moj r %d)\n", this->objtype, (parent ? parent->objtype : "NULL"), max(parent->left, left), min(parent->right, right), max(parent->top, top), min(parent->bottom, bottom), parent->right, right);
        this->SetClip(max(parent->clipleft, left), max(parent->cliptop, top), min(parent->clipright, right), min(parent->clipbottom, bottom));
    }
}
void glictContainer::SetClip(int left, int top, int right, int bottom) {

    this->clipleft = left;
    this->clipright = right;
    this->cliptop = top;
    this->clipbottom = bottom;
    
    //printf("%s s parentom %s clippa kao %d %d %d %d\n", this->objtype, (parent ? parent->objtype : "NULL"), clipleft, clipright, cliptop, clipbottom);
    
    glictSize size;
	int x, y;
    if (objects.size()) for (vector<glictContainer*>::iterator it=objects.begin(); it!=objects.end(); it++) {
        (*it)->GetPos(&x, &y);
        (*it)->GetSize(&size);
        
		
        (*it)->SetRect(this->left + x, this->top + y, this->left + x + size.w, this->top + y + size.h);
        (*it)->SetClip(max(this->left + (*it)->x, this->clipleft), max(this->top + (*it)->y, this->cliptop), min(this->left + (*it)->x + size.w, this->clipright), min(this->top + (*it)->y + size.h, this->clipbottom));
    }

    
}
void glictContainer::SetScissor() {
    // disabled scissor test because it doesnt like rescale, rotate etc.
    /*if (glictGlobals.renderMode==GLICT_RENDERING) {
        printf("Scissor testing %s (%s)\n", this->objtype, (this->parent ? this->parent->objtype : "NULL"));
        glScissor(
            this->clipleft,
            (int)glictGlobals.h - this->clipbottom,
            this->clipright - this->clipleft, 
            this->clipbottom - this->cliptop
        );
        
    } else {*/
        //glLoadName(this->guid);
        printf("Stencil testing %s (%s); %d %d %d %d\n", this->objtype, (this->parent ? this->parent->objtype : "NULL"), clipleft, clipright, cliptop, clipbottom);
        glClear(GL_STENCIL_BUFFER_BIT);
        //glDisable(GL_SCISSOR_TEST);
        //glEnable(GL_STENCIL_TEST); // was Disable
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
        glColorMask(0,0,0,0);
        glDepthMask(0);
            
        //glColor3b(rand(), rand(), rand());
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();
        glLoadMatrixf(ModelviewMatrix);
        glBegin(GL_QUADS);
            glVertex2f(this->clipleft, this->clipbottom);
            glVertex2f(this->clipright, this->clipbottom);
            glVertex2f(this->clipright, this->cliptop);
            glVertex2f(this->clipleft, this->cliptop);
        glEnd();
    	glPopMatrix();
        
        //glEnable(GL_STENCIL_TEST);
        glColorMask(1,1,1,1);
        glDepthMask(1);
        
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        
        
    //}    
}    
void glictContainer::Paint() {

    this->SetScissor();
    
    this->CPaint();
    
}
void glictContainer::CPaint() {

    printf("Rendering %s (child of %s)\n", objtype, parent ? parent->objtype : "NULL");

    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glDisable(GL_SCISSOR_TEST);
/*    glBegin(GL_LINES);
        glColor3f(1.0,1.0,1.0);
        glVertex2f(this->clipleft,this->cliptop);
        glVertex2f(this->clipleft,this->clipbottom);
        
        glVertex2f(this->clipleft,this->clipbottom);
        glVertex2f(this->clipright,this->clipbottom);
        
        glVertex2f(this->clipright,this->clipbottom);
        glVertex2f(this->clipright,this->cliptop);
        
        glVertex2f(this->clipright,this->cliptop);
        glVertex2f(this->clipleft,this->cliptop);
    glEnd();*/
//	glEnable(GL_SCISSOR_TEST);
	glPopMatrix();
    
    glPushMatrix();
    glTranslatef(this->x, this->y,0.0);
    vector<glictContainer*>::iterator it;
    for (it=objects.begin(); it!=objects.end(); it++) {
        (*it)->SetScissor();
        (*it)->Paint();
    }
    glPopMatrix();
    

}    

bool glictContainer::DefaultCastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
   switch (evt) {
       case GLICT_MOUSEUP:
       case GLICT_MOUSEDOWN:
       case GLICT_MOUSECLICK:
           {
           vector<glictContainer*>::iterator it;
           for (it=objects.begin(); it!=objects.end(); it++) {
               if ((*it)->CastEvent(evt, wparam, lparam, returnvalue))
                   return true;
           }
           if (evt == GLICT_MOUSEDOWN) {
               glictGlobals.lastMousePos.x = ((glictPos*)wparam)->x; // remembers x and y when pressing the mouse down
               glictGlobals.lastMousePos.y = ((glictPos*)wparam)->y;
           } else if (evt == GLICT_MOUSEUP) {
               if (abs (((glictPos*)wparam)->x - glictGlobals.lastMousePos.x) < 3 && // upon release verifies the location of mouse, and if nearby then it's a click - cast a click event
                   abs (((glictPos*)wparam)->y - glictGlobals.lastMousePos.y) < 3 ) { // if up to 2 pixels diff
                   return this->CastEvent(GLICT_MOUSECLICK, wparam, lparam, returnvalue);
               }    
           } else {
                glictPos relmousepos;
                relmousepos.x = ((glictPos*)wparam)->x - this->left;
                relmousepos.y = ((glictPos*)wparam)->y - this->top;
                if (this->OnClick) this->OnClick(&relmousepos, this);
           }    
           return false; // came here? defaultcastevent caught nothing
           break;
           }


       default:
           printf("Unhandled event\n");
           return false; // unprocessed, unknown event
   }    
}    
bool glictContainer::CastEvent(glictEvents evt, void* wparam, long lparam) {
   this->CastEvent(evt, wparam, lparam, NULL);
}
bool glictContainer::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
    if (evt == GLICT_MOUSEDOWN) printf("Mousedown event\n");
    if (evt == GLICT_MOUSEUP)   printf("Mouseup   event\n");
    switch (evt) {
        default:
            return DefaultCastEvent(evt, wparam, lparam, returnvalue); // use default processing for all events
        case GLICT_MOUSECLICK:
            return DefaultCastEvent(evt, wparam, lparam, returnvalue); // we never catch a mouse click in glictContainer... but some child might do so
            
            { // non-workign code below ... its supposed to use blending but it wont function :(
            
            GLint	oldviewport[4];
            glGetIntegerv(GL_VIEWPORT, oldviewport);            
            glViewport(0,0,50,50);
            
            int mousex = ((glictPos*)wparam)->x, mousey = ((glictPos*)wparam)->y;
            GLuint	buffer[512];
            GLint	pogotci;
            GLint	viewport[4];
            
            glGetIntegerv(GL_VIEWPORT, viewport);
            
            glSelectBuffer(512, buffer);
        
            //glRenderMode(GL_SELECT);
            glictGlobals.renderMode = GLICT_SELECTING;
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            
            glInitNames();
            glPushName(0);
        
            glMatrixMode(GL_PROJECTION);
        

        
            glPushMatrix();
            glLoadIdentity();
        
            // viewport set here
        
            gluPickMatrix((GLdouble) (mousex ) , (GLdouble) (/*viewport[3] - */(mousey - viewport[1]*2)), 1.0f, 1.0f, viewport);
        
            gluOrtho2D(0,glictGlobals.w ,0,glictGlobals.h);
        	glMatrixMode (GL_MODELVIEW);
        	
        	glEnable(GL_STENCIL_TEST);
        	glDisable(GL_SCISSOR_TEST);
            this->SetScissor();
            this->Paint();
            glEnable(GL_SCISSOR_TEST);
        	glDisable(GL_STENCIL_TEST);
            
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();

       
            /*glictGlobals.renderMode = GLICT_RENDERING;
            glutSwapBuffers();
            return;
            */
            
            glMatrixMode(GL_MODELVIEW);
            pogotci=glRenderMode(GL_RENDER);
        
            if (pogotci > 0)
            {
                int	najblizi = buffer[3];
                int dubina = buffer[1];
                        
                printf("%d hits\n", pogotci);
                for (int i = 1; i < pogotci; i++)
                {
                    printf("%d: %d %d %d %d\n", buffer[i*4], buffer[i*4+1], buffer[i*4+2], buffer[i*4+3]);
                    if (buffer[i*4+1] < GLuint(dubina))
                    {
                        najblizi = buffer[i*4+3];
                        dubina = buffer[i*4+1];
                        printf("%d at %d\n", najblizi, dubina);
                    }
                }
        
        /*        pointX = najblizi % 18;
                pointY = najblizi / 18 % 14;
                pointZ = najblizi / 18 / 14 % 14;
          		hitID = najblizi;*/
          		printf("Closest  %d\n", najblizi);
            } else {
        /*        pointX = -1;
                pointY = -1;
                pointZ = -1;
                hitID=-1;*/
                printf("Nothing selectable\n");
            }
            
            glViewport(oldviewport[0], oldviewport[1], oldviewport[2], oldviewport[3]);
            glutSwapBuffers();
            glictGlobals.renderMode = GLICT_RENDERING;
            }
            
            break;
	}
}

void glictContainer::SetOnClick(void(*f)(glictPos* relmousepos, glictContainer* callerclass)) {
    this->OnClick = f;
}    
void glictContainer::RememberTransformations() {
    
    vector<glictContainer*>::iterator it;   

    glGetFloatv(GL_MODELVIEW_MATRIX, ModelviewMatrix);    
    printf("Remembering %s's modelview matrix\n", objtype);
    
    for (it=objects.begin(); it!=objects.end(); it++) {
        (*it)->RememberTransformations();
    } 

} 
void glictContainer::ReportDebug() {
    printf("--%s--\n", objtype);
    printf("Height: %d\n", height);
    printf("Width: %d\n", width);
    printf("Pos: %d %d\n", x, y);
    printf("Rect TBLR: %d %d %d %d\n", top, bottom, left, right);
    printf("Clip TBLR: %d %d %d %d\n", cliptop, clipbottom, clipleft, clipright);
    printf("GUID: %d\n", guid);
    vector<glictContainer*>::iterator it;
    for (it=objects.begin(); it!=objects.end(); it++) {
        (*it)->ReportDebug();
    }
}    

void glictContainer::TransformScreenCoords(glictPos *pos) {


    int i,j,k;

    for (i=0;i<4;i++) {
        for (j=0;j<4;j++) 
            printf("%.02f ", ModelviewMatrix[i*4 + j]);
        printf("\n");
    }  


/*

    ModelviewMatrix[0] = 1;
    ModelviewMatrix[1] = 2;
    ModelviewMatrix[2] = 3;
    ModelviewMatrix[3] = 4;
    ModelviewMatrix[4] = 5;
    ModelviewMatrix[5] = 6;
    ModelviewMatrix[6] = 7;
    ModelviewMatrix[7] = 8;
    ModelviewMatrix[8] = 9;
    ModelviewMatrix[9] = 10;
    ModelviewMatrix[10] = 11;
    ModelviewMatrix[11] = 12;
    ModelviewMatrix[12] = 13;
    ModelviewMatrix[13] = 14;
    ModelviewMatrix[14] = 15;
    ModelviewMatrix[15] = 16;
*/
    // copy matrix for inversion; copy matrix in left part, and fill rest with identity matrix
    float m[32];
    m[0] = this->ModelviewMatrix[0];
    m[1] = this->ModelviewMatrix[1];
    m[2] = this->ModelviewMatrix[2];
    m[3] = this->ModelviewMatrix[3];
    m[4] = 1;
    m[5] = 0;
    m[6] = 0;
    m[7] = 0;
    
    m[8] = this->ModelviewMatrix[4];
    m[9] = this->ModelviewMatrix[5];
    m[10] = this->ModelviewMatrix[6];
    m[11] = this->ModelviewMatrix[7];
    m[12] = 0;
    m[13] = 1;
    m[14] = 0;
    m[15] = 0;
    
    m[16] = this->ModelviewMatrix[8];
    m[17] = this->ModelviewMatrix[9];
    m[18] = this->ModelviewMatrix[10];
    m[19] = this->ModelviewMatrix[11];
    m[20] = 0;
    m[21] = 0;
    m[22] = 1;
    m[23] = 0;
    
    m[24] = this->ModelviewMatrix[12];
    m[25] = this->ModelviewMatrix[13];
    m[26] = this->ModelviewMatrix[14];
    m[27] = this->ModelviewMatrix[15];
    m[28] = 0;
    m[29] = 0;
    m[30] = 0;
    m[31] = 1;
    
    
    
    int ti,tj;

    // invert matrix
    float tmprowel, tmprowel2; // temp row's element
    for (i=0;i<4;i++) { // go through each row
        for (j=0;j<8;j++) {
            m[i*8 + j] /= m[i * 8 + i]; // divide the row by [row][row], that's where we want a 1
        }    

        // go through all other rows except the one we're currently in
        for (j=0;j<4;j++) if (j!=i) {
            tmprowel2 = m[j*8+i];
            for (k=0;k<8;k++) {
                // multiply the row's elements by [row][row2]
                tmprowel = m[i*8 + k] * tmprowel2 * -1;
                // sum resulting row with row2
                m[j*8 + k] += tmprowel;
            } 
        }
    }    
    

    // right part is the resulting matrix
    float result[16];
    result[0] = m[4];
    result[1] = m[5];
    result[2] = m[6];
    result[3] = m[7];
    result[4] = m[12];
    result[5] = m[13];
    result[6] = m[14];
    result[7] = m[15];
    result[8] = m[20];
    result[9] = m[21];
    result[10] = m[22];
    result[11] = m[23];
    result[12] = m[28];
    result[13] = m[29];
    result[14] = m[30];
    result[15] = m[31];






    for (i=0;i<4;i++) {
        for (j=0;j<4;j++) 
            printf("%.02f ", result[i*4 + j]);
        printf("\n");
    }    


    
    // multiply matrix by coordinates
    pos->x = (int)((float)(pos->x) * result[0 * 4 + 0]
            + (float)(pos->y) * result[1 * 4 + 0]
            + (float)(0     ) * result[2 * 4 + 0]
            + (float)(1     ) * result[3 * 4 + 0]);
    pos->y = (int)((float)(pos->x) * result[0 * 4 + 1]
            + (float)(pos->y) * result[1 * 4 + 1]
            + (float)(1     ) * result[2 * 4 + 1]
            + (float)(0     ) * result[3 * 4 + 1]);
    
}
