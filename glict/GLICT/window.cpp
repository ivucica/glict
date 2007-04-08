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

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <GLICT/window.h>
#include <GLICT/globals.h>
#include <GLICT/fonts.h>
glictWindow::glictWindow() {

	this->containeroffsetx = 0;
	this->containeroffsety = 12;


	this->bgcolor.r = 0.75;
	this->bgcolor.g = 0.75;
	this->bgcolor.b = 0.75;
	this->bgcolor.a = 1.0;
	strcpy(this->objtype, "Window");

	this->parent = NULL;
	this->SetCaption("Untitled Window");

	this->focusable = true;

	this->mousedown = false;

	this->relmouse.x = 0;
	this->relmouse.y = 0;

	this->SetHeight(100);
	this->SetWidth(100);

	this->SetPos(0,0);

	/*this->glictContainer::AddObject(&panel);
	panel.SetPos(0, 14);
	panel.SetHeight(100);
	panel.SetWidth(100);
	panel.SetBGColor(1,0,0,1);*/

	//printf("Window generated.\n");

}
glictWindow::~glictWindow() {
	//this->glictWindow::RemoveObject(&panel);
}

/**
  * Paints the widget called window, simulating looks of windows on other
  * platforms (to a very plain and flat extent), and then calls CPaint()
  * which will paint all the subwidgets.
  *
  * \sa glictContainer::Paint()
  * \sa glictContainer::CPaint()
  * \todo Windows really need borders!
  */
void glictWindow::Paint() {
	if (!GetVisible()) return;
	//printf("window\n");

    if (!glictGlobals.windowBodySkin) { // if not skinned
        glColor4f(
            (float)this->bgcolor.r,
            (float)this->bgcolor.g,
            (float)this->bgcolor.b,
            (float)this->bgcolor.a
        );
        glBegin(GL_QUADS);
        glVertex2f(this->x,this->y+12);
        glVertex2f(this->x,this->y+this->height+12);
        glVertex2f(this->x+this->width,this->y+this->height+12);
        glVertex2f(this->x+this->width,this->y+12);
        glEnd();
    } else if (!glictGlobals.windowTitleSkin) { // if there's no title skin, but there's body skin
        glictSize s = {this->width + containeroffsetx*2, this->height + containeroffsety*2 };

        glTranslatef(this->x, this->y, 0);
        glictGlobals.windowBodySkin->Paint(&s);
        glTranslatef(-this->x, -this->y, 0);
    } else { // if skinned both title and body
        glictSize s = {this->width, this->height};

        glTranslatef(this->x + this->containeroffsetx , this->y + this->containeroffsety, 0);
        glictGlobals.windowBodySkin->Paint(&s);
        glTranslatef(-(this->x + this->containeroffsetx), -(this->y + this->containeroffsety), 0);
    }


    if (this->OnPaint) {
        glictRect r, c;

        r.top = this->top;
        r.bottom = this->bottom;
        r.left = this->left;
        r.right = this->right;

        c.top = max(this->cliptop, this->top + containeroffsety);
        c.bottom = min(this->clipbottom, this->bottom - containeroffsety);
        c.left = max(this->clipleft, this->left + containeroffsetx);
        c.right = min(this->clipright, this->right - containeroffsety);;

        this->OnPaint(&r, &c, this);

    }


	this->CPaint();
	//panel.Paint();


	// this is here so that scissoring resumes properly
	this->SetScissor();

    if (!glictGlobals.windowBodySkin && !glictGlobals.windowTitleSkin) {
        glColor4fv(glictGlobals.windowTitleBgColor);
        glBegin(GL_QUADS);
        glVertex2f(this->x,this->y);
        glVertex2f(this->x, this->y+12);
        glVertex2f(this->x + this->width, this->y+12);
        glVertex2f(this->x + this->width, this->y);
        glEnd();
    }

	glColor4fv(glictGlobals.windowTitleColor);

/*
    {
        int er;
        if ((er =glGetError())!=GL_NO_ERROR) printf("1EROR!!!\n");
    }
*/

	glRotatef(180.0, 1.0, 0.0, 0.0);
	glictFontRender(this->caption.c_str(),"system", this->x + (this->width / 2 - glictFontSize(this->caption.c_str(), "system") / 2) + (glictGlobals.windowBodySkin ? glictGlobals.windowBodySkin->GetLeftSize()->w : 0) , this->y*-1. - 9.);
	glRotatef(180.0, -1.0, 0.0, 0.0);


    /*{
        int er;
        if ((er =glGetError())!=GL_NO_ERROR) printf("2EROR!!!\n");
    }*/
}

void glictWindow::SetBGColor(float r, float g, float b, float a) {
	this->bgcolor.r = r;
	this->bgcolor.g = g;
	this->bgcolor.b = b;
	this->bgcolor.a = a;
	//this->panel.SetBGColor(r,g,b,a);
}
/**
  * \param evt Specifies the event being cast
  * \param wparam Specifies a pointer to the event-defined parameter
  * \param lparam Specifies a long value, an event-defined parameter
  *
  * Reacts to events in a way a window should, meaning, it detects a click
  * and a drag and possibly moves the window if they occured in specific
  * locations. Also makes use of Container::DefaultCastEvent() for default
  * parsing of events, or letting children widgets know of stuff that
  * happened.
  *
  * \bug Moving of this window by dragging has problems because of the
  *      containeroffsets if a window is a child of another window.
  *      Possibly a general problem with glictContainer::SetPos().
  *      (See also, bugnote there)
  */
bool glictWindow::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
	//printf("Event of type %s passing through %s (%s)\n", EvtTypeDescriptor(evt), objtype, parent ? parent->objtype : "NULL");
	if (!GetVisible()) return false;
	int oldx = this->x, oldy = this->y;

	if (evt == GLICT_MOUSECLICK || evt == GLICT_MOUSEDOWN || evt == GLICT_MOUSEUP) {
		if (((glictPos*)wparam)->x > this->clipleft &&
			((glictPos*)wparam)->x < this->clipright &&
			((glictPos*)wparam)->y > this->cliptop &&
			((glictPos*)wparam)->y < this->clipbottom) {

            //printf("Within %s\n", this->GetCaption().c_str());


			if (((glictPos*)wparam)->y <= this->cliptop + containeroffsety) {
				if (evt == GLICT_MOUSEDOWN) {

					this->Focus(NULL);
					this->mousedown = true;
					this->relmouse.x = ((glictPos*)wparam)->x-this->x ;
					this->relmouse.y = ((glictPos*)wparam)->y-this->y ;
					// dont to defaultcastevent as it might call a child that's below our titlebar
                    //printf("DRAG!\n");
					return true;
				}
			}


			if (evt == GLICT_MOUSEDOWN) {
				// we dont want any child to catch this one so return true

				if (GetEnabled())
					DefaultCastEvent(evt,wparam,lparam,returnvalue);

				return true;
			}

			if (evt == GLICT_MOUSEUP && this->mousedown) {
				this->SetPos(
					((glictPos*)wparam)->x - this->relmouse.x,
					((glictPos*)wparam)->y - this->relmouse.y
				);
				this->mousedown = false;

				return DefaultCastEvent(evt,wparam,lparam,returnvalue);
			}

			if (GetEnabled())
				return DefaultCastEvent(evt, wparam, lparam, returnvalue);
			else
				return false;
		} else
		// if mouse drag is done outside object...
		if (evt == GLICT_MOUSEUP && this->mousedown) {
			this->SetPos(
				((glictPos*)wparam)->x - this->relmouse.x,
				((glictPos*)wparam)->y - this->relmouse.y
			);
			this->mousedown = false;
            return true;
		}


		return DefaultCastEvent(evt, wparam, lparam, returnvalue);
	}

	return false;
}

void glictWindow::FixContainerOffsets() {
    if (!glictGlobals.windowBodySkin && !glictGlobals.windowTitleSkin) {// revert to defaults
        this->containeroffsetx = 0;
        this->containeroffsety = 12;
    } else {
        if (!glictGlobals.windowTitleSkin) { // all is placed inside body
            this->containeroffsetx = glictGlobals.windowBodySkin->GetLeftSize()->w;
            this->containeroffsety = glictGlobals.windowBodySkin->GetTopSize()->h;


        } else { // there are separate body and titlebar skin
            this->containeroffsetx = glictGlobals.windowBodySkin->GetLeftSize()->w; // FIXME should not work this way
            this->containeroffsety = glictGlobals.windowBodySkin->GetTopSize()->h;
        }
    }
    printf("%s container offsets %d %d\n", objtype,  containeroffsetx, containeroffsety);
}
