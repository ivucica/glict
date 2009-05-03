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
#include "scrollbar.h"
#include "globals.h"
#include "fonts.h"
glictScrollbar::glictScrollbar() {

	this->containeroffsetx = 0;
	this->containeroffsety = 0;


	this->bgcolor.r = 0.75;
	this->bgcolor.g = 0.75;
	this->bgcolor.b = 0.75;
	this->bgcolor.a = 1.0;
	strcpy(this->objtype, "Scrollbar");

	this->parent = NULL;

	this->focusable = true;

	this->SetHeight(100);
	this->SetWidth(10);

	this->SetPos(0,0);

	this->highlightdn = false;
	this->highlightup = false;

	this->min = 0;
	this->max = 100;
	this->value = 0;
	this->step = 1;

	this->draggingchip = false;
}
glictScrollbar::~glictScrollbar() {
	//this->glictWindow::RemoveObject(&panel);
}
void glictScrollbar::Paint() {
	if (!GetVisible())
        return;

    if (this->GetWidth() <= this->GetHeight()) {
        PaintVertical();
    } else {
        PaintHorizontal();
    }


}

void glictScrollbar::PaintVertical() {
    glictColor col;
    glictSkinner* activeSkin = NULL;
	// upper "button" //
	glictRect upperbuttonRect = {
	    this->x+glictGlobals.translation.x,
	    this->x+this->width+glictGlobals.translation.x,
        this->y+glictGlobals.translation.y,
        this->y+this->width+glictGlobals.translation.y
	};

    activeSkin = NULL;
    if (!highlightup)
        if (!glictGlobals.scrollbarUpperSkin)
            col = bgcolor;
        else
            activeSkin = glictGlobals.scrollbarUpperSkin;
    else
        if (!glictGlobals.scrollbarUpperHighlightSkin)
            if (!glictGlobals.buttonHighlightSkin)
                HighlightColor(&bgcolor, &col);
		    else
		        activeSkin = glictGlobals.buttonHighlightSkin;
        else
            activeSkin = glictGlobals.scrollbarUpperHighlightSkin;

    if (!activeSkin)
        glictGlobals.PaintRect(upperbuttonRect,col);
    else
        PaintSkinned(upperbuttonRect, activeSkin);

	// lower "button" //
	glictRect lowerbuttonRect = {
	    this->x+glictGlobals.translation.x,
	    this->x+this->width+glictGlobals.translation.x,
        this->y+this->height-this->width+glictGlobals.translation.y,
        this->y+this->height+glictGlobals.translation.y
	};

    activeSkin = NULL;

	if (!highlightdn)
		if (!glictGlobals.scrollbarLowerSkin)
            col = bgcolor;
        else
            activeSkin = glictGlobals.scrollbarLowerSkin;
	else
		if (!glictGlobals.scrollbarLowerHighlightSkin)
		    if (!glictGlobals.buttonHighlightSkin)
		        HighlightColor(&bgcolor, &col);
		    else
		        activeSkin = glictGlobals.buttonHighlightSkin;
        else
            activeSkin = glictGlobals.scrollbarLowerHighlightSkin;

	if (!activeSkin)
        glictGlobals.PaintRect(lowerbuttonRect , col);
	else
	    PaintSkinned(lowerbuttonRect, activeSkin);


	// back panel
	glictRect backpanelRect = {
	    this->x+glictGlobals.translation.x,
	    this->x+this->width+glictGlobals.translation.x,
        this->y+this->width+glictGlobals.translation.y,
        this->y+this->height-this->width+glictGlobals.translation.y
	};
	if (!glictGlobals.scrollbarPanelSkin)
        glictGlobals.PaintRect(backpanelRect, MultiplyColorRGB(bgcolor, 0.7));
	else
	    PaintSkinned(backpanelRect, glictGlobals.scrollbarPanelSkin);

	// scroller chip
	glictRect scrollerchipRect = GetScrollerChip();

	if ((float)(this->max - this->min) != 0) // protection against division by zero
	{
        if (!glictGlobals.scrollbarDragSkin) {
            glictGlobals.PaintRect(scrollerchipRect,MultiplyColorRGB(bgcolor, 0.6));
        }
        else
            PaintSkinned(scrollerchipRect, glictGlobals.scrollbarDragSkin);
	}

	this->CPaint();

	// this is here so that scissoring resumes properly
	this->SetScissor();


	glictGlobals.SetColor(1., 1., 1., 1.);


    if (!glictGlobals.scrollbarUpperSkin && !highlightup ||
        !glictGlobals.scrollbarUpperHighlightSkin && highlightup) {
        glictFontRender("^","system",
            this->x + (this->width / 2 - glictFontSize("^", "system") / 2 +glictGlobals.translation.x) ,
            this->y - 9 + width / 2 + 9/2 +glictGlobals.translation.y);
    }
    if (!glictGlobals.scrollbarLowerSkin && !highlightdn ||
        !glictGlobals.scrollbarLowerHighlightSkin && highlightdn) {
        glictFontRender("V","system",
            this->x + (this->width / 2 - glictFontSize("V", "system") / 2 +glictGlobals.translation.x) ,
            this->y + this->height - width / 2 - 9 / 2 +glictGlobals.translation.y);
    }

}



void glictScrollbar::PaintHorizontal() {
    glictColor col;
    glictSkinner* activeSkin = NULL;
	// left "button" //
	glictRect leftbuttonRect = {
	    this->x+glictGlobals.translation.x,
	    this->x+this->height+glictGlobals.translation.x,
        this->y+glictGlobals.translation.y,
        this->y+this->height+glictGlobals.translation.y
	};

    activeSkin = NULL;
    if (!highlightup)
        if (!glictGlobals.scrollbarLeftSkin)
            col = bgcolor;
        else
            activeSkin = glictGlobals.scrollbarLeftSkin;
    else
        if (!glictGlobals.scrollbarLeftHighlightSkin)
            if (!glictGlobals.buttonHighlightSkin)
                HighlightColor(&bgcolor, &col);
		    else
		        activeSkin = glictGlobals.buttonHighlightSkin;
        else
            activeSkin = glictGlobals.scrollbarLeftHighlightSkin;

    if (!activeSkin)
        glictGlobals.PaintRect(leftbuttonRect,col);
    else
        PaintSkinned(leftbuttonRect, activeSkin);

	// right "button" //
	glictRect rightbuttonRect = {
	    this->x+this->width-this->height+glictGlobals.translation.x,
	    this->x+this->width+glictGlobals.translation.x,
        this->y+glictGlobals.translation.y,
        this->y+this->height+glictGlobals.translation.y
	};

    activeSkin = NULL;

	if (!highlightdn)
		if (!glictGlobals.scrollbarRightSkin)
            col = bgcolor;
        else
            activeSkin = glictGlobals.scrollbarRightSkin;
	else
		if (!glictGlobals.scrollbarRightHighlightSkin)
		    if (!glictGlobals.buttonHighlightSkin)
		        HighlightColor(&bgcolor, &col);
		    else
		        activeSkin = glictGlobals.buttonHighlightSkin;
        else
            activeSkin = glictGlobals.scrollbarRightHighlightSkin;

	if (!activeSkin)
        glictGlobals.PaintRect(rightbuttonRect , col);
	else
	    PaintSkinned(rightbuttonRect, activeSkin);


	// back panel
	glictRect backpanelRect = {
	    this->x+this->height+glictGlobals.translation.x,
	    this->x+this->width-this->height+glictGlobals.translation.x,
        this->y+glictGlobals.translation.y,
        this->y+this->height+glictGlobals.translation.y
	};
	if (!glictGlobals.scrollbarPanelSkin)
        glictGlobals.PaintRect(backpanelRect, MultiplyColorRGB(bgcolor, 0.7));
	else
	    PaintSkinned(backpanelRect, glictGlobals.scrollbarPanelSkin);

	// scroller chip
	glictRect scrollerchipRect = GetScrollerChip();

    if ((float)(this->max - this->min) != 0) // protection against division by zero
	{
        if (!glictGlobals.scrollbarDragSkin) {
            glictGlobals.PaintRect(scrollerchipRect,MultiplyColorRGB(bgcolor, 0.6));
        }
        else
            PaintSkinned(scrollerchipRect, glictGlobals.scrollbarDragSkin);
	}
	this->CPaint();

	// this is here so that scissoring resumes properly
	this->SetScissor();


	glictGlobals.SetColor(1., 1., 1., 1.);


    if (!glictGlobals.scrollbarLeftSkin && !highlightup ||
        !glictGlobals.scrollbarLeftHighlightSkin && highlightup) {
        glictFontRender("<","system",
            this->x - 9 + height / 2 + glictFontSize("^", "system") / 2 +glictGlobals.translation.x,
            this->y + (this->height / 2 - 9 / 2 +glictGlobals.translation.y));

    }
    if (!glictGlobals.scrollbarRightSkin && !highlightdn ||
        !glictGlobals.scrollbarRightHighlightSkin && highlightdn) {
        glictFontRender(">","system",
            this->x + this->width - height / 2 - glictFontSize(">", "system") / 2 +glictGlobals.translation.x ,
            this->y + (this->height / 2 - 9 / 2 +glictGlobals.translation.y));
    }

}

void glictScrollbar::SetBGColor(float r, float g, float b, float a) {
	this->bgcolor.r = r;
	this->bgcolor.g = g;
	this->bgcolor.b = b;
	this->bgcolor.a = a;
	//this->panel.SetBGColor(r,g,b,a);
}

bool glictScrollbar::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
	//printf("Event of type %s passing through %s (%s)\n", EvtTypeDescriptor(evt), objtype, parent ? parent->objtype : "NULL");
	if (!GetVisible()) return false;
	int oldx = this->x, oldy = this->y;
	if (evt == GLICT_MOUSECLICK || evt == GLICT_MOUSEDOWN || evt == GLICT_MOUSEUP || evt == GLICT_MOUSEMOVE) {
		if (((glictPos*)wparam)->x > this->clipleft &&
			((glictPos*)wparam)->x < this->clipright &&
			((glictPos*)wparam)->y > this->cliptop &&
			((glictPos*)wparam)->y < this->clipbottom) {

            const float w = GetWidth();
            const float h = GetHeight();

            glictRect scrollerChip = GetScrollerChip();


			if (evt == GLICT_MOUSECLICK) {
				this->Focus(this);
				if (h > w) { // vertical
                    if (((glictPos*)wparam)->y - this->top < this->width) { // mousedown within upper button?
                        if (this->value > this->min) this->value -= this->step;
                        if (this->value < this->min) this->value = this->min;
                    }
                    else if (((glictPos*)wparam)->y - this->top > this->height - this->width) { // mousedown within upper button?
                        if (this->value < this->max) this->value += this->step;
                        if (this->value > this->max) this->value = this->max;
                    }
				} else { // horizontal
				    if (((glictPos*)wparam)->x - this->left < this->height) { // mousedown within upper button?
                        if (this->value > this->min) this->value -= this->step;
                        if (this->value < this->min) this->value = this->min;
                    }
                    else if (((glictPos*)wparam)->x - this->left > this->width - this->height) { // mousedown within upper button?
                        if (this->value < this->max) this->value += this->step;
                        if (this->value > this->max) this->value = this->max;
                    }
				}
			}
			if (evt == GLICT_MOUSEDOWN) {
			    if (h > w) { // vertical
                    if (((glictPos*)wparam)->y - this->top < this->width) { // mousedown within upper button?
                        this->highlightup = true;
                    }
                    else if (((glictPos*)wparam)->y - this->top > this->height - this->width) { // mousedown within upper button?
                        this->highlightdn = true;
                    }
                    else if((((glictPos*)wparam)->y > scrollerChip.top) && (((glictPos*)wparam)->y > scrollerChip.bottom)) { // mousedown within chip?
						draggingchip = true;
					}
			    } else { // horizontal
			        if (((glictPos*)wparam)->x - this->left < this->height) { // mousedown within upper button?
                        this->highlightup = true;
                    }
                    else if (((glictPos*)wparam)->x - this->left > this->width - this->height) { // mousedown within upper button?
                        this->highlightdn = true;
                    }
                    else if((((glictPos*)wparam)->x > scrollerChip.right) && (((glictPos*)wparam)->x > scrollerChip.left)) { // mousedown within chip?
						draggingchip = true;
					}
			    }
			}
			if (evt == GLICT_MOUSEUP) {
				this->highlightdn = false;
				this->highlightup = false;

				if(draggingchip) { // chip dragging
                    UpdateScrollchipDragging(((glictPos*)wparam)->x,((glictPos*)wparam)->y);
					draggingchip = false;
				}
			}
			if (evt == GLICT_MOUSEMOVE) {
				if(draggingchip) { // chip dragging
                    UpdateScrollchipDragging(((glictPos*)wparam)->x,((glictPos*)wparam)->y);
				}
			}


		}
		return DefaultCastEvent(evt, wparam, lparam, returnvalue);
	}

	return false;
}

void glictScrollbar::UpdateScrollchipDragging(int mousex, int mousey)
{
    const float w = GetWidth();
    const float h = GetHeight();

    if (h > w) { // vertical
        this->value =
            // we calculate internal position by taking position at which we're currently positioning
            // the mouse. then we subtract the relative position of mouse when we started dragging.
            // this is because we're interested in chip's position, not in mouse's position.
            // then, we subtract top coordinate from the chip's position. this way we get the coordinates
            // in the coordinate system of the scrollbar itself. however that's not all. we don't really
            // need coordinates in the system of scrollbar: we need coordinates in the system of
            // draggable area. that means we're now offsetting everything by height of top button.
            // for vertical scrollbar this is (or should be!) GetWidth().
            (mousey /* FIXME - relative y from top of chip upon start of dragging */ - this->top - this->GetWidth())
            // now we need to scale from GetHeight() - (topchip + bottomchip + scrollchip) to max-min
            * ((this->max - this->min) / (float)(GetHeight() - GetWidth()*3))
             ;
        this->value -= this->value % this->step; // fix to the proper step
        if (this->value > this->max)
            this->value = this->max;
        if (this->value < this->min)
            this->value = this->min;

    }
    else { // horizontal
        // FIXME implement according to above
    }

}

void glictScrollbar::SetValue(int val) {
	this->value = val;
}
int glictScrollbar::GetValue() {
	return this->value;
}
void glictScrollbar::SetStep (unsigned int newstep) {
	this->step = newstep;
}
unsigned int glictScrollbar::GetStep() {
	return this->step;
}
void glictScrollbar::SetMin (int newmin) {
	this->min = newmin;
}
int glictScrollbar::GetMin() {
	return min;
}
void glictScrollbar::SetMax (int newmax) {
	this->max = newmax;
}
int glictScrollbar::GetMax() {
	return this->max;
}



void glictScrollbar::HighlightColor(const glictColor *col, glictColor *destcol) const {
    destcol->r = col->r < .5 ? (float)col->r * 1.5 : (float)col->r / 1.5;
    destcol->g = col->g < .5 ? (float)col->g * 1.5 : (float)col->g / 1.5;
    destcol->b = col->b < .5 ? (float)col->b * 1.5 : (float)col->b / 1.5;
    destcol->a = col->a < .5 ? (float)col->a * 1.5 : (float)col->a / 1.5;
}
void glictScrollbar::PaintSkinned(const glictRect &r, glictSkinner* skin) const {
    glictSize s = {r.right - r.left, r.bottom - r.top};
    float gtx = glictGlobals.translation.x;
    float gty = glictGlobals.translation.y;

    glictGlobals.Translatef(r.left-gtx, r.top-gty, 0);
    skin->Paint(&s);
    glictGlobals.Translatef(-r.left+gtx, -r.top+gty, 0);
}

glictColor glictScrollbar::MultiplyColorRGB(const glictColor &col, float mul) const {
    glictColor c = col;
    c.r *= mul;
    c.g *= mul;
    c.b *= mul;
    return c;
}

glictRect glictScrollbar::GetScrollerChip()
{
	if(GetHeight() > GetWidth()) {
		// scroller chip
		glictRect scrollerchipRect = {
            this->x+glictGlobals.translation.x,
            this->x + this->width +glictGlobals.translation.x,

            this->y +glictGlobals.translation.y + // normal beginning coord of the object
            this->width + // increased by height of top button
            ((float)(this->value-this->min) / (float)(this->max - this->min)) // at this percent
            * (float)(this->height - this->width*2 - this->width), // which should be a height, reduced by top and bottom button's height, but also by scroller's height


            this->y +glictGlobals.translation.y + // normal beginning coord of the object
            this->width + // increased by height of top button
            ((float)(this->value-this->min) / (float)(this->max - this->min)) // at this percent
            * (float)(this->height - this->width*2 - this->width) // which should be a height, reduced by top and bottom button's height, but also by scroller's height
            + this->width // this is bottom, add some more
		};
		return scrollerchipRect;
	}
	else {
		glictRect scrollerchipRect = {
            this->x +glictGlobals.translation.x + // normal beginning coord of the object
            this->height + // increased by width of top button
            ((float)(this->value-this->min) / (float)(this->max - this->min)) // at this percent
            * (float)(this->width - this->height*2 - this->height), // which should be a width, reduced by left and right button's width, but also by scroller's width

            this->x +glictGlobals.translation.x + // normal beginning coord of the object
            this->height + // increased by height of left button
            ((float)(this->value-this->min) / (float)(this->max - this->min)) // at this percent
            * (float)(this->width - this->height*2 - this->height) // which should be a width, reduced by left and right button's width, but also by scroller's width
            + this->height, // this is right, add some more


            this->y+glictGlobals.translation.y,
            this->y + this->height +glictGlobals.translation.y

		};
		return scrollerchipRect;
	}
	glictRect scrollerchipRect = {0,0,0,0};
	return scrollerchipRect;
}
