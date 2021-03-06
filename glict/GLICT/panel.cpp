/*
	GLICT - Graphics Library Interface Creation Toolkit
	Copyright (C) 2006-2008 Ivan Vucica

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
#include <stdio.h>
#include <string.h>
#include "panel.h"
#include "globals.h"
#include "fonts.h"

#include <algorithm>

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

    virtualsize.x = 0;
    virtualsize.y = 0;

    textoffx = 0;
    textoffy = 0;

    this->AddObject(&sbVertical);

	sbVertical.SetVisible(false);
	//sbHorizontal.SetVisible(false);// FIXME horizontal scrollbar widget must be done in order to be implemented here

	skin = NULL;
	draggable = false;
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

    /* DEBUGGING CODE REMOVEME
    if (glictGlobals.clipperCallback)
        glictGlobals.clipperCallback(clipleft,clipright,cliptop,clipbottom);
    glictGlobals.PaintRectLines(left,right,top,bottom);
    glictGlobals.PaintRectLines(this->x+glictGlobals.translation.x, this->x+this->width+glictGlobals.translation.x,
								   this->y+glictGlobals.translation.y, this->y+this->height+glictGlobals.translation.y);
    */


    SetScissor();


    if (IsVScrollbarVisible()) {
        sbVertical.SetWidth(GetVScrollbarInternalWidth()); //GetVScrollbarWidth() returns left size and right size, too; which is not good.
        sbVertical.SetHeight(GetHeight());//- (virtualsize.w > width ? 10 : 0));
        sbVertical.SetPos(width - GetVScrollbarWidth(), sbVertical.GetValue());
        sbVertical.SetVisible(true);

        sbVertical.SetMin(0);
        sbVertical.SetMax((int)(virtualsize.h - height));
		if (sbVertical.GetValue() > virtualsize.h - height) sbVertical.SetValue((int)(virtualsize.h - height));
    } else {
        sbVertical.SetVisible(false);
    }

    this->virtualpos.x = 0;
    this->virtualpos.y = sbVertical.GetValue();

    if ( this->virtualsize.h != this->height )
        SetPos(x,y);

	if (this->bgactive) {
	    if (!skin) {
            glictGlobals.PaintRect(this->x+glictGlobals.translation.x, this->x+this->width+glictGlobals.translation.x,
								   this->y+glictGlobals.translation.y, this->y+this->height+glictGlobals.translation.y, bgcolor);

	    } else {
	        glictSize s;
	        s.h = height, s.w = width;
            glictGlobals.Translatef(x,y,0);
	        skin->Paint(&s);
	        glictGlobals.Translatef(-x,-y,0);

	    }
	}

    glictColor oldcol = glictFontColor(fontname.c_str());
    if (captioncolor.r == captioncolor.g && captioncolor.g == captioncolor.b && captioncolor.b == captioncolor.a == 1.)
        glictFontColor(fontname.c_str(), glictGlobals.panelTextColor);
    else
        glictFontColor(fontname.c_str(), captioncolor);

	glictFontRender(this->caption.c_str(), fontname.c_str(), x+glictGlobals.translation.x + textoffx , y + glictGlobals.translation.y + textoffy);
	glictFontColor(fontname.c_str(), oldcol);

	if (this->OnPaint) {
		glictRect r, c;

		r.top = this->top+containeroffsety;
		r.bottom = this->bottom;
		r.left = this->left+containeroffsetx;
		r.right = this->right;

		c.top = std::max(this->cliptop, this->top+containeroffsety);
		c.bottom = this->clipbottom;
		c.left = std::max(this->clipleft, this->left+containeroffsetx);
		c.right = this->clipright;
		this->OnPaint(&r, &c, this);
	}




    this->CPaint();


    if (IsVScrollbarVisible()) {
        sbVertical.SetPos(width - GetVScrollbarWidth(), 0);
    }



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
		case GLICT_KEYPRESS:
			switch (*((char*)wparam)) {
				case 9:
					if (next)
						next->Focus(NULL);
					else if (parent && parent->GetNext())
						parent->GetNext()->Focus(NULL);
					break;
				case 13:
				case 32:
				{
					glictPos p = {this->clipleft, this->cliptop};
					if (OnClick) {
						OnClick(&p, this);
						return true;
					}
				}
				default:
					//printf("Panel getting key %d\n", *((char*)wparam));
					break;

			}
			break;
		case GLICT_MOUSEUP:
		case GLICT_MOUSEDOWN:
		case GLICT_MOUSECLICK: {

            glictPos p; // scrollbar related begin
            p.x = ((glictPos*)wparam)->x;
            p.y = ((glictPos*)wparam)->y - sbVertical.GetValue(); // scrollbar related end

			if (((glictPos*)wparam)->x > this->clipleft &&
				((glictPos*)wparam)->x < this->clipright &&
				((glictPos*)wparam)->y > this->cliptop &&
				((glictPos*)wparam)->y < this->clipbottom) {
                //printf("EVENT WITHIN PANEL %s (%s)...!\n", objtype, parent ? parent->objtype : "NULL");

                sbVertical.SetPos(sbVertical.GetX(), sbVertical.GetY() + sbVertical.GetValue());
                if (sbVertical.CastEvent(evt, wparam, lparam, returnvalue)) { // scrollbar related begin
                    sbVertical.SetPos(sbVertical.GetX(), sbVertical.GetY() - sbVertical.GetValue());

                    return true;
                } // scrollbar related end
                sbVertical.SetPos(sbVertical.GetX(), sbVertical.GetY() - sbVertical.GetValue());


                if (evt == GLICT_MOUSEDOWN && this->OnMouseDown) {
                        if (this->OnMouseDown) {

                            glictPos relpos;
                            relpos.x = ((glictPos*)wparam)->x - this->left - this->containeroffsetx + this->virtualpos.x;
                            relpos.y = ((glictPos*)wparam)->y - this->top - this->containeroffsety + this->virtualpos.y;
                            this->OnMouseDown(&relpos, this);
                            return true;
                        }
                }

				// if a child caught click, we dont handle it otherwise
				return DefaultCastEvent(evt, wparam, lparam, returnvalue);

				//return DefaultCastEvent(evt, wparam, lparam, returnvalue); // replace &p with wparam
				// otherwise we could handle it mroe but we'll simply tell we didnt proces it

			} else {
			    //printf("PANEL DID NOT FIND THIS THING. X, Y: %d %d Clip: %d %d %d %d\n", ((glictPos*)wparam)->x, ((glictPos*)wparam)->y, clipleft, clipright, cliptop, clipbottom);
			    return DefaultCastEvent(evt, wparam, lparam, returnvalue);
			}
			//printf("It occured outside the panel, ignored.\n");
			break;
		}
	}

	return DefaultCastEvent(evt, wparam, lparam, returnvalue);
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
  * Scrolls to the virtual area's bottom.
  */
void glictPanel::VirtualScrollBottom() {
	if (IsVScrollbarVisible()) {
		sbVertical.SetMax((int)(virtualsize.h - height));
		sbVertical.SetValue(sbVertical.GetMax());
	} else {
		sbVertical.SetValue(0);
	}
}

/**
  * Enhances glictContainer::SetVirtualSize() by setting the scrollbar
  * properties. Needed because glictContainer does not contain scrollbars.
  */

void glictPanel::SetVirtualSize(float w, float h) {

    bool newheightbigger = (h <= height);

    glictContainer::SetVirtualSize(w,h);


    sbVertical.SetStep(10);
    sbHorizontal.SetStep(10);

    if (newheightbigger) {
        VirtualScrollBottom();
    }

}


/**
  * \param skin Pointer to a glictSkinner object containing he skin rectangle
  *
  * Sets this panel's skin.
  */
  #include <stdlib.h>
void glictPanel::SetSkin(glictSkinner* skin) {

    this->skin = skin;
}


int glictPanel::GetVScrollbarWidth() const {
    if (glictGlobals.scrollbarUpperSkin) {
        return  int(glictGlobals.scrollbarUpperSkin->GetLeftSize().w +
                    glictGlobals.scrollbarUpperSkin->GetCenterSize().w +
                    glictGlobals.scrollbarUpperSkin->GetRightSize().w);
    }
    return 10;
}
int glictPanel::GetVScrollbarInternalWidth() const {
    if (glictGlobals.scrollbarUpperSkin) {
        return  int(glictGlobals.scrollbarUpperSkin->GetCenterSize().w);
    }
    return 10;
}
void glictPanel::SetHeight(float height) {
    glictContainer::SetHeight(height);
    sbVertical.SetHeight(GetHeight());

    if (height < GetHeight())
        if (IsVScrollbarVisible())
            sbVertical.SetValue(virtualsize.h-height);
        else
            sbVertical.SetValue(0);
}
