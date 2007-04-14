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
#include "panel.h"
#include "globals.h"
#include "fonts.h"
glictPanel::glictPanel() {
	this->bgcolor.r = 0.75;
	this->bgcolor.g = 0.75;
	this->bgcolor.b = 0.75;
	this->bgcolor.a = 1.0;

	strcpy(this->objtype, "Panel");

	this->parent = NULL;
	//printf("Panel generated.\n");

	this->focusable = false;

	this->bgactive = true;

	sbVertical.SetVisible(false);
	//sbHorizontal.SetVisible(false);// FIXME horizontal scrollbar widget must be done in order to be implemented here
}
glictPanel::~glictPanel() {

}
void glictPanel::Paint() {
	if (!GetVisible()) return;


/*
    if (virtualsize.w > width) {
        sbHorizontal.SetHeight(10);
        sbHorizontal.SetWidth(width - (virtualsize.h > height ? 10 : 0));
        sbHorizontal.SetPos(0, height - 10);
        sbHorizontal.SetVisible(true);


        sbHorizontal.SetMin(0);
        sbHorizontal.SetMax(virtualsize.w);
    }*/
    if (virtualsize.h > height) {

        this->AddObject(&sbVertical);

        sbVertical.SetWidth(10);
        sbVertical.SetHeight(height - (virtualsize.w > width ? 10 : 0));
        sbVertical.SetPos(width - 10, 0);
        sbVertical.SetVisible(true);

        sbVertical.SetMin(0);
        sbVertical.SetMax(virtualsize.h);

        this->RemoveObject(&sbVertical);
    }



	if (this->bgactive) {
        glColor4f(
            (float)this->bgcolor.r,
            (float)this->bgcolor.g,
            (float)this->bgcolor.b,
            (float)this->bgcolor.a
        );
        glBegin(GL_QUADS);
        glVertex2f(this->x,this->y);
        glVertex2f(this->x,this->y+this->height);
        glVertex2f(this->x+this->width,this->y+this->height);
        glVertex2f(this->x+this->width,this->y);
        glEnd();
	}

	glColor4f(1., 1., 1., 1.);

    glPushMatrix(); // must remain here because of glictFontRender


		glTranslatef(this->x, this->y,0);
		glRotatef(180.0, 1.0, 0.0, 0.0);

		glColor4f(glictGlobals.panelTextColor.r , glictGlobals.panelTextColor.g, glictGlobals.panelTextColor.b, glictGlobals.panelTextColor.a);
		glictFontRender(this->caption.c_str(), "system", 0, -10);
		glColor4f(1., 1., 1., 1.);


		glRotatef(180.0, -1.0, 0.0, 0.0);
		glTranslatef(-this->x, -this->y,0);


	glPopMatrix();

	glPushMatrix();
		if (this->OnPaint) {
			glictRect r, c;

			r.top = this->top+containeroffsety;
			r.bottom = this->bottom;
			r.left = this->left+containeroffsetx;
			r.right = this->right;

			c.top = max(this->cliptop, this->top+containeroffsety);
			c.bottom = this->clipbottom;
			c.left = max(this->clipleft, this->left+containeroffsetx);
			c.right = this->clipright;
			this->OnPaint(&r, &c, this);
		}
	glPopMatrix();


    glTranslatef(-sbHorizontal.GetValue(), -sbVertical.GetValue(), 0);
    this->CPaint();
    glTranslatef(sbHorizontal.GetValue(), sbVertical.GetValue(), 0);

    glTranslatef(x,y,0);
    sbVertical.SetScissor();
    sbVertical.Paint();
    glTranslatef(-x,-y,0);

}
void glictPanel::SetBGColor(float r, float g, float b, float a) {
	this->bgcolor.r = r;
	this->bgcolor.g = g;
	this->bgcolor.b = b;
	this->bgcolor.a = a;
}
/// This is the copypasteable castevent usable in other widgets; just remove scrollbar-related code
bool glictPanel::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
	if (!GetVisible() || !GetEnabled()) return false;
	switch (evt) {



		case GLICT_MOUSEUP:
		case GLICT_MOUSEDOWN:
		case GLICT_MOUSECLICK: {

            glictPos p; // scrollbar related begin
            p.x = ((glictPos*)wparam)->x;
            p.y = ((glictPos*)wparam)->y + sbVertical.GetValue(); // scrollbar related end

			if (((glictPos*)wparam)->x > this->clipleft &&
				((glictPos*)wparam)->x < this->clipright &&
				((glictPos*)wparam)->y > this->cliptop &&
				((glictPos*)wparam)->y < this->clipbottom) {
                //printf("EVENT WITHIN PANEL %s (%s)...!\n", objtype, parent ? parent->objtype : "NULL");

                if (sbVertical.CastEvent(evt, wparam, lparam, returnvalue)) { // scrollbar related begin
                    printf("oi\n");
                    return true;
                } // scrollbar related end



				// if a child caught click, we dont handle it otherwise
				return DefaultCastEvent(evt, &p, lparam, returnvalue); // replace &p with wparam
				// otherwise we could handle it mroe but we'll simply tell we didnt proces it

			} else {
			    //printf("PANEL DID NOT FIND THIS THING. X, Y: %d %d Clip: %d %d %d %d\n", ((glictPos*)wparam)->x, ((glictPos*)wparam)->y, clipleft, clipright, cliptop, clipbottom);
			}
			//printf("It occured outside the panel, ignored.\n");
			break;
		}
	}

	return false;
}

/**
  * \param bg Sets visibility
  *
  * This function is used to set whether or not background is visible, which
  * is very useful in case the window below it is skinned and panel is used
  * only to group or to show text.
  */

void glictPanel::SetBGActiveness(bool bg) {
    bgactive = bg;
}

/**
  * \param w Virtual width
  * \param h Virtual height
  *
  * Sets the extended, virtual width and height of the panel, the total
  * area which can be accessed using scrollbars that become visible unless
  * the virtual width and height are set to zero or smaller than real width
  * and height.
  *
  * If virtual width and height are set to smaller than real width and height,
  * they are simply ignored.
  */
void glictPanel::SetVirtualSize(int w, int h) {
    virtualsize.w = w;
    virtualsize.h = h;

    sbVertical.SetStep(10);
    sbHorizontal.SetStep(10);
    sbVertical.SetValue(0);
    sbHorizontal.SetValue(0);

}
