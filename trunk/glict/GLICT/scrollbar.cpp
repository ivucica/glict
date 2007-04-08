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


}
glictScrollbar::~glictScrollbar() {
	//this->glictWindow::RemoveObject(&panel);
}
void glictScrollbar::Paint() {
	if (!GetVisible()) return;

	// upper "button" //
	if (!highlightup) {
		glColor4f(
			(float)this->bgcolor.r,
			(float)this->bgcolor.g,
			(float)this->bgcolor.b,
			(float)this->bgcolor.a
		);
	} else {
		glColor4f(
			this->bgcolor.r < .5 ? (float)this->bgcolor.r * 1.5 : (float)this->bgcolor.r / 1.5,
			this->bgcolor.g < .5 ? (float)this->bgcolor.g * 1.5 : (float)this->bgcolor.g / 1.5,
			this->bgcolor.b < .5 ? (float)this->bgcolor.b * 1.5 : (float)this->bgcolor.b / 1.5,
			this->bgcolor.a < .5 ? (float)this->bgcolor.a * 1.5 : (float)this->bgcolor.a / 1.5
			);
	}
	glBegin(GL_QUADS);
	glVertex2f(this->x,this->y);
	glVertex2f(this->x,this->y+this->width);
	glVertex2f(this->x+this->width,this->y+this->width);
	glVertex2f(this->x+this->width,this->y);
	glEnd();


	// lower "button" //
	if (!highlightdn) {
		glColor4f(
			(float)this->bgcolor.r,
			(float)this->bgcolor.g,
			(float)this->bgcolor.b,
			(float)this->bgcolor.a
		);
	} else {
		glColor4f(
			this->bgcolor.r < .5 ? (float)this->bgcolor.r * 1.5 : (float)this->bgcolor.r / 1.5,
			this->bgcolor.g < .5 ? (float)this->bgcolor.g * 1.5 : (float)this->bgcolor.g / 1.5,
			this->bgcolor.b < .5 ? (float)this->bgcolor.b * 1.5 : (float)this->bgcolor.b / 1.5,
			this->bgcolor.a < .5 ? (float)this->bgcolor.a * 1.5 : (float)this->bgcolor.a / 1.5
			);
	}
	glBegin(GL_QUADS);
	glVertex2f(this->x,this->y+this->height);
	glVertex2f(this->x,this->y+this->height-this->width);
	glVertex2f(this->x+this->width,this->y+this->height-this->width);
	glVertex2f(this->x+this->width,this->y+this->height);
	glEnd();

	// back panel
	glColor4f(
		(float)this->bgcolor.r*0.7,
		(float)this->bgcolor.g*0.7,
		(float)this->bgcolor.b*0.7,
		(float)this->bgcolor.a
	);
	glBegin(GL_QUADS);
	glVertex2f(this->x,this->y+this->width);
	glVertex2f(this->x,this->y+this->height-this->width);
	glVertex2f(this->x+this->width,this->y+this->height-this->width);
	glVertex2f(this->x+this->width,this->y+this->width);
	glEnd();

	// scroller chip
	glColor4f(
		(float)this->bgcolor.r * 0.8,
		(float)this->bgcolor.g * 0.8,
		(float)this->bgcolor.b * 0.8,
		(float)this->bgcolor.a
	);
	glBegin(GL_QUADS);
	glVertex2f(
		this->x,
		this->y + // normal beginning coord of the object
		this->width + // increased by height of top button
		((float)(this->value-this->min) / (float)(this->max - this->min)) // at this percent
		* (float)(this->height - this->width*2 - this->width) // which should be a height, reduced by top and bottom button's height, but also by scroller's height
	);
	glVertex2f(this->x + this->width, this->y + this->width + ((float)(this->value-this->min) / (float)(this->max - this->min)) * (float)(this->height - this->width*2 - this->width));
	glVertex2f(this->x + this->width, this->y + this->width + ((float)(this->value-this->min) / (float)(this->max - this->min)) * (float)(this->height - this->width*2 - this->width) + this->width);
	glVertex2f(this->x, this->y + this->width + ((float)(this->value-this->min) / (float)(this->max - this->min)) * (float)(this->height - this->width*2 - this->width) + this->width);
	glEnd();

	this->CPaint();

	// this is here so that scissoring resumes properly
	this->SetScissor();


	glColor4f(1., 1., 1., 1.);


	glRotatef(180.0, 1.0, 0.0, 0.0);
	glictFontRender("^","system",
		this->x + (this->width / 2 - glictFontSize("^", "system") / 2) ,
		this->y*-1. - 9 - width / 2 + 9/2);

	glictFontRender("V","system",
		this->x + (this->width / 2 - glictFontSize("V", "system") / 2) ,
		-this->y - this->height + width / 2 - 9 / 2);

    glRotatef(180.0, -1.0, 0.0, 0.0);
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
	if (evt == GLICT_MOUSECLICK || evt == GLICT_MOUSEDOWN || evt == GLICT_MOUSEUP) {
		if (((glictPos*)wparam)->x > this->clipleft &&
			((glictPos*)wparam)->x < this->clipright &&
			((glictPos*)wparam)->y > this->cliptop &&
			((glictPos*)wparam)->y < this->clipbottom) {




			if (evt == GLICT_MOUSECLICK) {
				this->Focus(this);
				if (((glictPos*)wparam)->y - this->top < this->width) { // mousedown within upper button?
					if (this->value > this->min) this->value -= this->step;
					if (this->value < this->min) this->value = this->min;
				}
				if (((glictPos*)wparam)->y - this->top > this->height - this->width) { // mousedown within upper button?
					if (this->value < this->max) this->value += this->step;
					if (this->value > this->max) this->value = this->max;
				}
			}
			if (evt == GLICT_MOUSEDOWN) {
				if (((glictPos*)wparam)->y - this->top < this->width) { // mousedown within upper button?
					this->highlightup = true;
				}
				if (((glictPos*)wparam)->y - this->top > this->height - this->width) { // mousedown within upper button?
					this->highlightdn = true;
				}
			}
			if (evt == GLICT_MOUSEUP) {
				this->highlightdn = false;
				this->highlightup = false;
			}

		}
		return DefaultCastEvent(evt, wparam, lparam, returnvalue);
	}

	return false;
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



