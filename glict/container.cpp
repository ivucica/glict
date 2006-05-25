#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "container.h"
#include "types.h"
#include "globals.h"

#ifndef min
#define min(a,b) (a<b ? a : b)
#define max(a,b) (a>b ? a : b)
#endif
glitContainer::glitContainer() {
    this->height = 32;
    this->width = 32;
    this->x = 0;
    this->y = 0;
    strcpy(this->objtype,"Container");

    this->parent = NULL;    
    this->SetRect(this->x, this->y, this->x + this->width, this->y + this->height);
    this->SetClip(this->left, this->top, this->right, this->bottom);
    

    this->guid = rand();
}
glitContainer::~glitContainer() {
    
}
void glitContainer::AddObject(glitContainer* obj) {
    obj->parent = this;
    this->objects.insert(this->objects.end(), obj);
}
void glitContainer::RemoveObject(glitContainer* object) {
    vector<glitContainer*>::iterator it;
    for (it=objects.begin(); it!=objects.end(); it++) {
        if ((*it)==object) {
            
            delete *it;
            objects.erase(it);
        }    
    }    
}
void glitContainer::SetHeight(int h) {
    this->SetRect(this->left, this->top, this->right, this->bottom + h - this->height);
    this->height = h;

}
void glitContainer::SetWidth(int w) {

    this->SetRect(this->left, this->top, this->right + w - this->width, this->bottom);
    this->width = w;

}
void glitContainer::SetPos(int x, int y) {

    //printf("Postavka pozicije %s (%s) na %d %d\n", objtype, (parent ? parent->objtype : "NULL"), x, y);
    //printf("This->x %d This->y %d\n", this->x, this->y);
    this->SetRect(this->left + x - this->x  , this->top + y - this->y, this->right + x - this->x, this->bottom + y - this->y);
    
    
    this->x = x;
    this->y = y;
    
    
    glitSize size;
    for (vector<glitContainer*>::iterator it=objects.begin(); it!=objects.end(); it++) {
        (*it)->GetPos(&x, &y);
        (*it)->GetSize(&size);
        //printf("Postavljam dijete %s od %s (size: %d %d)\n", (*it)->objtype, objtype, size.w, size.h);
        (*it)->SetRect(this->left + x, this->top + y, this->left + x + size.w, this->top + y + size.h);
        //(*it)->SetClip(max(this->left + x, this->clipleft), max(this->top + y, this->cliptop), min(this->left + x + size.w, this->clipright), min(this->top + y + size.h, this->clipbottom));
    }
}
void glitContainer::SetPos(glitPos pos) {
    this->SetPos(pos.x,pos.y);
}
void glitContainer::GetPos(int* x, int* y) {
    *x = this->x;
    *y = this->y;
    
}
void glitContainer::GetPos(glitPos* pos) {
    pos->x = this->x;
    pos->y = this->y;
}
/* TODO (Khaos#2#): GetWidth(w);
                    GetHeight(h) */
/* TODO (Khaos#2#): GetSize(w,h); */
void glitContainer::GetSize(glitSize* size) {
    size->w = this->width;
    size->h = this->height;
}
void glitContainer::SetRect(int left, int top, int right, int bottom) {
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
void glitContainer::SetClip(int left, int top, int right, int bottom) {

    this->clipleft = left;
    this->clipright = right;
    this->cliptop = top;
    this->clipbottom = bottom;
    
    //printf("%s s parentom %s clippa kao %d %d %d %d\n", this->objtype, (parent ? parent->objtype : "NULL"), clipleft, clipright, cliptop, clipbottom);
    
    glitSize size;
	int x, y;
    if (objects.size()) for (vector<glitContainer*>::iterator it=objects.begin(); it!=objects.end(); it++) {
        (*it)->GetPos(&x, &y);
        (*it)->GetSize(&size);
        
		
        (*it)->SetRect(this->left + x, this->top + y, this->left + x + size.w, this->top + y + size.h);
        (*it)->SetClip(max(this->left + (*it)->x, this->clipleft), max(this->top + (*it)->y, this->cliptop), min(this->left + (*it)->x + size.w, this->clipright), min(this->top + (*it)->y + size.h, this->clipbottom));
    }

    
}
void glitContainer::SetScissor() {
    if (glitGlobals.renderMode==GLIT_RENDERING) {
        printf("Scissor testing %s (%s)\n", this->objtype, (this->parent ? this->parent->objtype : "NULL"));
        glScissor(
            this->clipleft,
            (int)glitGlobals.h - this->clipbottom,
            this->clipright - this->clipleft, 
            this->clipbottom - this->cliptop
        );
        
    } else {
        //glLoadName(this->guid);
        printf("Stencil testing %s (%s); %d %d %d %d\n", this->objtype, (this->parent ? this->parent->objtype : "NULL"), clipleft, clipright, cliptop, clipbottom);
        glClear(GL_STENCIL_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
        glColorMask(0,0,0,0);
            
            
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glBegin(GL_QUADS);
            glVertex2f(this->clipleft, this->clipbottom);
            glVertex2f(this->clipright, this->clipbottom);
            glVertex2f(this->clipright, this->cliptop);
            glVertex2f(this->clipleft, this->cliptop);
        glEnd();
    	glPopMatrix();
        
        glEnable(GL_STENCIL_TEST);
        glColorMask(1,1,1,1);
        
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        
        
    }    
}    
void glitContainer::Paint() {

    this->SetScissor();
    
    this->CPaint();
    
}
void glitContainer::CPaint() {

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
    vector<glitContainer*>::iterator it;
    for (it=objects.begin(); it!=objects.end(); it++) {
        (*it)->SetScissor();
        (*it)->Paint();
    }
    glPopMatrix();
    

}    


void glitContainer::CastEvent(glitEvents evt, void* wparam, long lparam ) {
    switch (evt) {
        case GLIT_MOUSECLICK:
            {
            int mousex = ((glitPos*)wparam)->x, mousey = ((glitPos*)wparam)->y;
            GLuint	buffer[512];
            GLint	pogotci;
            GLint	viewport[4];
        
            glGetIntegerv(GL_VIEWPORT, viewport);
            glSelectBuffer(512, buffer);
        
            glRenderMode(GL_SELECT);
            glitGlobals.renderMode = GLIT_SELECTING;
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            
            glInitNames();
            glPushName(0);
        
            glMatrixMode(GL_PROJECTION);
        

        
            glPushMatrix();
            glLoadIdentity();
        
            // viewport set here
        
            gluPickMatrix((GLdouble) (mousex ) , (GLdouble) (/*viewport[3] - */(mousey - viewport[1]*2)), 1.0f, 1.0f, viewport);
        
            gluOrtho2D(0,glitGlobals.w ,0,glitGlobals.h);
        	glMatrixMode (GL_MODELVIEW);
        	
        	glEnable(GL_STENCIL_TEST);
        	glDisable(GL_SCISSOR_TEST);
            this->SetScissor();
            this->Paint();
            glEnable(GL_SCISSOR_TEST);
        	glDisable(GL_STENCIL_TEST);
            
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();

       
            /*glitGlobals.renderMode = GLIT_RENDERING;
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
            
            glitGlobals.renderMode = GLIT_RENDERING;
            }
            
            break;
        default:
            printf("Unhandled event\n");
	}
}

void glitContainer::ReportDebug() {
    printf("--%s--\n", objtype);
    printf("Height: %d\n", height);
    printf("Width: %d\n", width);
    printf("Pos: %d %d\n", x, y);
    printf("Rect TBLR: %d %d %d %d\n", top, bottom, left, right);
    printf("Clip TBLR: %d %d %d %d\n", cliptop, clipbottom, clipleft, clipright);
    printf("GUID: %d\n", guid);
    vector<glitContainer*>::iterator it;
    for (it=objects.begin(); it!=objects.end(); it++) {
        (*it)->ReportDebug();
    }
}    
