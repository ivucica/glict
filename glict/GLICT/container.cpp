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



/**
 * \file container.cpp
 * \brief Container class code
 * \sa glictContainer
 */


#include <stdlib.h>
#ifdef WIN32
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include "container.h"
#include "types.h"
#include "globals.h"


#include <stdlib.h>

#ifndef min
#define min(a,b) (a<b ? a : b)
#define max(a,b) (a>b ? a : b)
#endif

/**
  * It fills up the class with default infos.
  */
glictContainer::glictContainer() {
	this->height = 32;
	this->width = 32;
	this->x = 0;
	this->y = 0;
	strcpy(this->objtype,"Container");

	this->parent = NULL;

	this->OnClick = NULL;
	this->OnPaint = NULL;

	this->guid = rand();

	this->ResetTransformations();

	this->SetCaption("");

	this->focusable = false;

	this->SetVisible(true);
	this->SetEnabled(true);

	this->containeroffsetx = 0;
	this->containeroffsety = 0;

	this->SetRect(this->x, this->y, this->x + this->width, this->y + this->height);
	this->SetClip(this->left, this->top, this->right, this->bottom);

    virtualsize.x = 0;
    virtualsize.y = 0;

    virtualpos.x = 0;
    virtualpos.y = 0;

	//printf("Container created.\n");

}

/**
  * Currently does nothing.
  */
glictContainer::~glictContainer() {

}

/**
  * Sets to identity matrix by pushing GL matrix, loading an identity matrix,
  * calling RememberTransformations() and popping the GL matrix.
  */
void glictContainer::ResetTransformations() {
	glPushMatrix();
		glLoadIdentity();
		this->RememberTransformations();
	glPopMatrix();
}
/**
  * \param obj Pointer to object to add as a child.
  * Simply inserts the passed object pointer into a vector/list of objects, also
  * giving the child object a "this" as a parent, so the child knows who gave
  * birth to it. Also tells it to ResetTransformations, which'd be incorrect
  * at display, but lets us get rid of it at init time.
  * (Oh, dear, no more biology...)
  * \sa RemoveObject
  */
void glictContainer::AddObject(glictContainer* obj) {
	obj->parent = this;
	obj->ResetTransformations();
	this->objects.insert(this->objects.end(), obj);

	//this->SetPos(x,y); // just force the clipping code to execute
	this->RecursiveBoundaryFix();
    //if (parent) parent->SetPos(parent->x, parent->y);
}
/**
  * \param obj Pointer to object to remove.
  *
  * Adds the object to delayed-remove vector, which means the object will be
  * removed upon next call to CPaint() before real painting.
  * Note: object stored in pointer is not touched and if dynamically
  * created it must be manually cleaned up.
  *
  * \sa AddObject(glictContainer* obj)
  */
void glictContainer::RemoveObject(glictContainer* object) {
	/*vector<glictContainer*>::iterator it;

	for (it=objects.begin(); it!=objects.end(); it++) {
		if ((*it)==object) {
			//delete *it;
			objects.erase(it);
			return;
		}
	}*/

	delayedremove.insert(delayedremove.end(), object);
	//printf("ADDED an object to DELAYEDREMOVE list -- new size %s is %d\n", GetCaption().c_str(), delayedremove.size() );
}

/**
  * Executes the delayedremoval.
  */
void glictContainer::DelayedRemove() {
	//printf("Delayed removal of %d objects %s\n", delayedremove.size(), GetCaption().c_str());
	if (delayedremove.size()) {

		for (vector<glictContainer*>::iterator it = delayedremove.begin(); delayedremove.size(); ) {
			for (std::vector<glictContainer*>::iterator it2 = objects.begin(); it2 != objects.end(); ) {
				if ((*it) == (*it2)) {
					objects.erase(it2);
					it2 = objects.begin();
				}
				else {
					it2++;
				}
			}
			delayedremove.erase(it);

		}
	}
}

/**
  * \param h Height to which the object should be set.
  *
  * This function sets up the current object's height and the object's
  * boundaries properly, to whatever values they should be set.
  * \sa SetWidth(int w), GetSize(glictSize* size)
  */
void glictContainer::SetHeight(int h) {
	this->height = h;
    this->RecursiveBoundaryFix();
}

/**
  * \param w Width to which the object should be set.
  *
  * This function sets up the current object's height and the object's
  * boundaries properly, to whatever values they should be set.
  * \sa SetHeight(int h), GetSize(glictSize* size)
  */
void glictContainer::SetWidth(int w) {
	this->width = w;
	this->RecursiveBoundaryFix();
}

/**
  * \param x X coordinate of the object, relative to parent
  * \param y Y coordinate of the object, relative to parent
  *
  * This function sets up the current object's position and the object's
  * boundaries properly, to whatever values they should be set.
  * \sa SetPos(glictPos pos), GetPos(int *x, int *y)
  */
void glictContainer::SetPos(int x, int y) {

	//printf("Postavka pozicije %s (%s) %s na %d %d\n", objtype, (parent ? parent->objtype : "NULL"), this->caption.c_str(), x, y);
	//printf("This->x %d This->y %d\n", this->x, this->y);

    this->FixContainerOffsets(); // in case the skin got just turned on, we'll have to fix the container offsets

    if (!parent) {
	    this->SetRect(x, y, x+width, y+height);

        this->x = x;
        this->y = y;
    } else {
        this->SetRect(
            parent->left + x + parent->containeroffsetx - parent->virtualpos.x,
            parent->top + y + parent->containeroffsety  - parent->virtualpos.y,
            parent->left + x + width+ parent->containeroffsetx -  parent->virtualpos.x + containeroffsetx*2,
            parent->top + y + height+ parent->containeroffsety -  parent->virtualpos.y + containeroffsety*2 // FIXME containeroffsety*2 should be replaced by containeroffsety + bottomheight
        );


        this->x = x;
        this->y = y;
    }



	glictSize size;
	for (std::vector<glictContainer*>::iterator it=objects.begin(); it!=objects.end(); it++) {

		(*it)->GetPos(&x, &y);
#if 1
        (*it)->SetPos(x,y);
#else
// FIXME: this way, we have less ops but it's not recursive properly and does not fix the boundaries as it ought to.
//  check if we can still keep this way of doing things instead of sicko "setpos" recursively
		(*it)->GetSize(&size);



		(*it)->SetRect(this->left + x, this->top, this->left + x + size.w, this->top + y + size.h );


#endif
	}
}

/**
  * \param pos Position specified in a struct, relative to parent.
  *
  * This function sets up the current object's position and the object's
  * boundaries properly, to whatever values they should be set.
  * \sa SetPos(int x, int y), GetPos(glictPos *pos)
  */
void glictContainer::SetPos(glictPos pos) {
	this->SetPos(pos.x,pos.y);
}

/**
  * \param *x Pointer where X coordinate should be saved.
  * \param *y Pointer where Y coordinate should be saved.
  *
  * This function puts the current object's position (relative to
  * parent) into specified memory locations.
  * \sa SetPos(int x, int y), GetPos(glictPos *pos)
  * \bug Possible containeroffset*-based problem, manifests upon moving of a
  *      window which is a child of another window by dragging. See also,
  *      bugnote in glictWindow::CastEvent()
  */
void glictContainer::GetPos(int* x, int* y) {
	*x = this->x;
	*y = this->y;
}

/**
  * \param *pos Pointer to struct where coordinates should be saved.
  *
  * This function puts the current object's position (relative to
  * parent) into specified memory locations.
  * \sa SetPos(glictPos pos), GetPos(int *x, int *y)
  */
void glictContainer::GetPos(glictPos* pos) {
	pos->x = this->x;
	pos->y = this->y;
}
/* TODO (Khaos#2#): GetSize(w,h); */

/**
  * \param *size Pointer to which the size of object should be saved.
  *
  * Retrieves width and height and puts it into a structure specified by
  * the pointer.
  * \sa SetWidth(int w), SetHeight(int h)
  */
void glictContainer::GetSize(glictSize* size) {
	size->w = this->width;
	size->h = this->height;
}

/**
  * \return Current width of the object.
  *
  * Retrieves width of the object and returns it as a value.
  * \sa SetWidth(int w), GetSize(glictSize* size)
  */
unsigned int glictContainer::GetWidth() {
    return width;
}

/**
  * \return Current height of the object.
  *
  * Retrieves height of the object and returns it as a value.
  * \sa SetHeight(int h), GetSize(glictSize* size)
  */
unsigned int glictContainer::GetHeight() {
    return height;
}

/**
  * \param left Left boundary of the object.
  * \param top Top boundary of the object.
  * \param right Right boundary of the object.
  * \param bottom Bottom boundary of the object.
  *
  * Internal use function that sets the boundaries of the object. Note, these
  * are not the display-related boundaries, they are imaginary boundaries.
  * To set the display-related boundaries use SetClip().
  *
  * These functions can be overridden, always use SetPos(), SetWidth(),
  * SetHeight() and other related functions!
  * \sa SetClip(int left, int top, int right, int bottom)
  * \sa SetPos(int x, int y),
  *     SetPos(glictPos pos),
  *     SetWidth(int w),
  *     SetHeight(int h)
  */
void glictContainer::SetRect(int left, int top, int right, int bottom) {
	//printf("%s s parentom %s postaje %d %d %d %d\n", this->objtype, (parent ? parent->objtype : "NULL"), left, right, top, bottom);
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;


    //printf("Sada, %s s parentom %s sebe postavlja na clip %d %d %d %d\n", this->objtype, (parent ? parent->objtype : "NULL"), left, right, top, bottom);
    this->SetClip(
        max(left, (parent ? parent->clipleft : 0)),
        max(top, (parent ? parent->cliptop:0)),
        min(right, (parent ? parent->clipright: right)),
        min(bottom, (parent ? parent->clipbottom:bottom)) );

}

/**
  * \param left Left clipping boundary of the object.
  * \param top Top clipping boundary of the object.
  * \param right Right clipping boundary of the object.
  * \param bottom Bottom clipping boundary of the object.
  *
  * Internal use function that sets the display related, clipping boundaries
  * of the object. Note, these are the display-related boundaries,
  * they are visible on screen. To set internal boundaries use SetRect().
  *
  * These functions can be overridden, always use SetPos(), SetWidth(),
  * SetHeight() and other related functions!
  * \sa SetRect(int left, int top, int right, int bottom)
  * \sa SetPos(int x, int y),
  *     SetPos(glictPos pos),
  *     SetWidth(int w),
  *     SetHeight(int h)
  */
void glictContainer::SetClip(int left, int top, int right, int bottom) {

	this->clipleft = left;
	this->clipright = right ;
	this->cliptop = top ;
	this->clipbottom = bottom ;

	//printf("%s s parentom %s clippa kao %d %d %d %d\n", this->objtype, (parent ? parent->objtype : "NULL"), clipleft, clipright, cliptop, clipbottom);

/*	glictSize size;
	int x, y;
	if (objects.size()) for (vector<glictContainer*>::iterator it=objects.begin(); it!=objects.end(); it++) {
		(*it)->GetPos(&x, &y);
		(*it)->GetSize(&size);


		(*it)->SetRect(this->left + x, this->top + y, this->left + x + size.w, this->top + y + size.h);
		(*it)->SetClip(max(this->left + (*it)->x, this->clipleft), max(this->top + (*it)->y, this->cliptop), min(this->left + (*it)->x + size.w, this->clipright), min(this->top + (*it)->y + size.h, this->clipbottom));
	}
*/

}

/**
  * Sets up scissoring of the current object. Should be called withing Paint()
  * function. If necessary, it can be manually called by program that uses the
  * library, however in most cases it should not be necessary.
  *
  * Note: although named SetScissor, GLICT can be set to use either scissor
  * or stencil testing. This is so that special transformations can be
  * painlessly used on entire GUI, which was one of goals in making the library.
  * Default mode is scissor test for performance, scissoring/clipping can be
  * turned off in glictGlobals's member clippingTest, or can be set to scissor
  * testing.
  *
  * It is up to main program to enable or disable appropriate stencil/scissor
  * test.
  *
  * If using scissor test, then you MUST update the glictGlobals.h, although
  * that is always recommended. glictGlobals.h must contain current viewport
  * height.
  *
  * \sa Paint(), CPaint(), glictGlobals::clippingMode
  */
void glictContainer::SetScissor() {
	//printf("SCISSOR SET\n");

	if (glictGlobals.clippingMode==GLICT_SCISSORTEST) {
		//printf("Scissor testing %s (%s), %d %d %d %d\n", this->objtype, (this->parent ? this->parent->objtype : "NULL"), clipleft, clipright, cliptop, clipbottom);
		//glMatrixMode(GL_MODELVIEW);
		//glPushMatrix();
		//glLoadIdentity();
		glScissor(
			this->clipleft,
			(int)glictGlobals.h - this->clipbottom,
			this->clipright - this->clipleft,
			this->clipbottom - this->cliptop
		);
		//glPopMatrix();

	} else
	if (glictGlobals.clippingMode==GLICT_STENCILTEST) {
		//glLoadName(this->guid);
		//printf("Stencil testing %s (%s); %d %d %d %d\n", this->objtype, (this->parent ? this->parent->objtype : "NULL"), clipleft, clipright, cliptop, clipbottom);
		glClear(GL_STENCIL_BUFFER_BIT);
		//glDisable(GL_SCISSOR_TEST);
		//glEnable(GL_STENCIL_TEST); // was Disable
		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glColorMask(0,0,0,0);
		glDepthMask(0);

		//glColor3b(rand(), rand(), rand());
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
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


	}// else printf("CLIP TEST IS OFF!!\n");
}

/**
  * (This description applies for the glictContainer::Paint(). If you are
  * reading it for some other class, then its documentation is not written
  * yet for this function.)
  *
  * In glictContainer class, Paint does nothing except calling SetScissor() and
  * CPaint(). This is because as a widget the container class does not render
  * anything; it serves as a point of divergence for other classes.
  * Other widgets should define their own procedure with these two calls
  * as a minimum.
  *
  * When writing a widget, put SetScissor() on the beginning of this
  * function, and CPaint() on the end.
  *
  * \sa SetScissor(), CPaint()
  */
void glictContainer::Paint() {
	if (!GetVisible()) return;
	this->SetScissor();

	this->CPaint();

}

/**
  * CPaint() is used to render "all the small things". All children are
  * properly rendered here, with calls to SetScissor() and then to Paint(),
  * and although the call to SetScissor() might be unnecessary it's always
  * good to do things twice if you're not certain. After that Paint() of each
  * object is called.
  *
  * So, we have a loop that goes through the objects vector/list, then calls for
  * each object the SetScissor() and then Paint().
  *
  * Prior to rendering, however, the delayedremove vector is checked out
  * and all objects requested are removed.
  *
  * This f. is used within Paint(); actually there should be no reason for this
  * function to be used except internally when writing a widget. You may be
  * more interested in the Paint() method.
  *
  * \sa Paint()
  */
void glictContainer::CPaint() {

	//printf("Rendering %s (child of %s)\n", objtype, parent ? parent->objtype : "NULL");

	DelayedRemove();

#if 0

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();


	if (glictGlobals.clippingMode==GLICT_SCISSORTEST) glDisable(GL_SCISSOR_TEST);
	if (glictGlobals.clippingMode==GLICT_STENCILTEST) glDisable(GL_STENCIL_TEST);


    glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(this->clipleft,this->clipbottom);
		glVertex2f(this->clipleft,this->cliptop);

		glColor3f(0.0,1.0,0.0);
		glVertex2f(this->clipright,this->clipbottom);
		glVertex2f(this->clipleft,this->clipbottom);


		glColor3f(0.0,0.0,1.0);
		glVertex2f(this->clipright,this->cliptop);
		glVertex2f(this->clipright,this->clipbottom);


		glColor3f(1.0,1.0,0.0);
		glVertex2f(this->clipleft,this->cliptop);
		glVertex2f(this->clipright,this->cliptop);

	glEnd();

	if (glictGlobals.clippingMode==GLICT_SCISSORTEST) glEnable(GL_SCISSOR_TEST);
	if (glictGlobals.clippingMode==GLICT_STENCILTEST) glEnable(GL_STENCIL_TEST);
	glPopMatrix();
#endif

	if (objects.size()) {
		glPushMatrix();
			glTranslatef(this->x + containeroffsetx - virtualpos.x, this->y + containeroffsety - virtualpos.y,0.0);

			std::vector<glictContainer*>::iterator it;
			for (it=objects.begin(); it!=objects.end(); it++) {
				(*it)->SetScissor();
				(*it)->Paint();
			}
			glTranslatef(-this->x - containeroffsetx + virtualpos.x, -this->y - containeroffsety + virtualpos.y,0.0);
		glPopMatrix();
	}
}

/**
  * \param evt Event that occured
  * \param *wparam Any kind of value that needs to be passed on to the function.
  *               (For example if a structure needs to be passed.)
  * \param lparam Numeric value that needed to be passed for event processing.
  * \param *returnvalue Return value, if any, will be stored here.
  *                     If NULL, no return value shall be stored.
  * \return Boolean value specifying if the event was processed at all. True
  *         means an event was caught by the widget, or by a child.
  *
  *
  * This function does default processing of event, so that each widget
  * does not have to reinvent the wheel. If the widget does not want to
  * process an event, it simply can call upon this function.
  *
  * There is very little practical use for this function apart from internal use
  * when making a widget. You may be more interested in classical CastEvent().
  *
  * For external use you should only use CastEvent().
  *
  * For more information on how the event system works, see the CastEvent() as
  * well. You'll also read of parameters there, as well as return value.
  *
  * \sa CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue)
  * \todo Clicking must be handled via verifying if an element has been rendered on
  *       certain pixel, not the way it's done right now. Currently we cannot do
  *       custom-shaped widgets, only rectangle widgets! (This todo is not strictly
  *       related to this function.)
  *
  */
bool glictContainer::DefaultCastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
	//printf("Default event of type %s passing through %s (%s) with %d children\n", EvtTypeDescriptor(evt), objtype, parent ? parent->objtype : "NULL", this->objects.size());
	switch (evt) {
		case GLICT_KEYPRESS:
			// as default behaviour is that widget doesnt know what to do with
			// a key, it'll pass exec to the top focused item, whatever it
			// might be
			if (glictGlobals.topFocused)
				if (this!=glictGlobals.topFocused)
					return glictGlobals.topFocused->CastEvent(evt, wparam, lparam, returnvalue);
			// if it cant find top focused item, or that it is actually the
			// focused item, then it will report that it
			// doesnt know how to proc the event
			return false;
		case GLICT_KEYDOWN:

			return false;
		case GLICT_KEYUP:

			return false;

		case GLICT_MOUSEUP:
		case GLICT_MOUSEDOWN:
		case GLICT_MOUSECLICK:
			{
			std::vector<glictContainer*>::reverse_iterator it;
			//vector<glictContainer*>::iterator it;


			if  (((glictPos*)wparam)->x > this->clipleft &&
                 ((glictPos*)wparam)->x < this->clipright &&
                 ((glictPos*)wparam)->y > this->cliptop &&
                 ((glictPos*)wparam)->y < this->clipbottom) {
					if (objects.size()) {
						for (it=objects.rbegin(); it != objects.rend(); it++) {
						//for (it=objects.begin(); it!=objects.end(); it++) {
							if (it != objects.rend() && *it ) {

								//printf("Testing %s (%s)\n", (*it)->objtype, this->objtype );


								if ((*it)->CastEvent(evt, wparam, lparam, returnvalue)) {
									//printf("%s (%s) returned true\n", (*it)->objtype, this->objtype );
									return true;
								}


							}
						}
					}

			}


			//printf("Passing on to specific mouseevent processing in %s (%s).\n", objtype, parent ? parent->objtype : "NULL");
			if (evt == GLICT_MOUSEDOWN) {
				//printf("Mouse down\n");
				glictGlobals.lastMousePos.x = ((glictPos*)wparam)->x ; // remembers x and y when pressing the mouse down
				glictGlobals.lastMousePos.y = ((glictPos*)wparam)->y ;
				if (focusable) {
					if (((glictPos*)wparam)->x > this->clipleft &&
					 ((glictPos*)wparam)->x < this->clipright &&
					 ((glictPos*)wparam)->y > this->cliptop &&
					 ((glictPos*)wparam)->y < this->clipbottom ) {
						//MessageBox(0,"MouseDown",this->GetCaption().c_str(),0);
						this->Focus(NULL);
						return true;
					 }
				}
			} else if (evt == GLICT_MOUSEUP) {
				//printf("Mouse up on %s\n", objtype);
				if (abs (((glictPos*)wparam)->x - glictGlobals.lastMousePos.x) < 3 && // upon release verifies the location of mouse, and if nearby then it's a click - cast a click event
					abs (((glictPos*)wparam)->y - glictGlobals.lastMousePos.y) < 3 ) { // if up to 2 pixels diff
					//printf("Considering it a click\n");
					//if (!parent) {
						//printf("Casting click event.\n");

						return this->CastEvent(GLICT_MOUSECLICK, wparam, lparam, returnvalue);
					//} else {
						//Not toplevel! Has a parent. Thus ignoring a click, letting toplevel parse properly.
					//}

				} else {
					//Considering it dragging. Ignoring!
				}
			} else { // not mousedown , not mouseup? mouseclick!


				if (((glictPos*)wparam)->x > this->clipleft &&
					((glictPos*)wparam)->x < this->clipright &&
					((glictPos*)wparam)->y > this->cliptop &&
					((glictPos*)wparam)->y < this->clipbottom ) {
					if (this->OnClick) {
						//printf("Click on %s.\n", objtype);
						glictPos relpos;
						relpos.x = ((glictPos*)wparam)->x - this->left - this->containeroffsetx + this->virtualpos.x;
						relpos.y = ((glictPos*)wparam)->y - this->top - this->containeroffsety + this->virtualpos.y;
						this->OnClick(&relpos, this);
						return true;
					}
					// if it happened within our boundaries, let it be as if we proc'ed it!
					// of course, only if we're not a container

					if (strcmp(objtype, "Container")) {// FIXME this is ugly lowperformance check, we should make it a bool or sth
                        //printf("Announcing click in %s\n", objtype);
					    return true;
					}
				}
				// it didnt? then lets ignore it

				return false;

			}
			return false; // came here? defaultcastevent caught nothing
			}


		default:
			printf("Unhandled event\n");
			return false; // unprocessed, unknown event
	}

	// should never come here, but just in case:
	return false;
}

/**
  * \param evt Event that occured
  * \param *wparam Any kind of value that needs to be passed on to the function.
  *               (For example if a structure needs to be passed.)
  * \param lparam Numeric value that needed to be passed for event processing.
  * \return Boolean value specifying if the event was processed at all. True
  *         means an event was caught by the widget, or by a child.
  *
  * Function that can be used in case returnvalue is not necessary, meaning
  * that caller is not interested in a result of the function.
  * Actually just calls to regular CastEvent with returnvalue set to NULL.
  * See the regular CastEvent for more info.
  *
  * \sa CastEvent(glictEvents, void*, long, void*);
  */
bool glictContainer::CastEvent(glictEvents evt, void* wparam, long lparam) {
   return this->CastEvent(evt, wparam, lparam, NULL);
}

/**
  * \param evt Event that occured
  * \param *wparam Any kind of value that needs to be passed on to the function.
  *               (For example if a structure needs to be passed.)
  * \param lparam Numeric value that needed to be passed for event processing.
  * \param *returnvalue Return value, if any, will be stored here.
  *                     If NULL, no return value shall be stored.
  * \return Boolean value specifying if the event was processed at all. True
  *         means an event was caught by the widget, or by a child.
  *
  * Unlike other UI libraries, this library has one philosophy and that is
  * it does not like hooking to any of the windowing system's functions;
  * it does not depend upon either GLUT, Windows, Linux or MacOS. Widgets
  * are receiving user feedback upon being told that user made some. They
  * are told this by the program that uses this library. Example of this
  * action is that in, let's say, GLUT the program receives a call to mouse()
  * function. Upon this, the program casts an event using this function to
  * the widget which then processes what should actually happen.
  * A button, for example, would upon a mouseclick call to OnClick function.
  *
  * The thing is, this CastEvent function first tries to tell its children that
  * the event happened, and upon being told by each of them that they didn't
  * do anything with the event, goes on with processing it itself.
  *
  * Container class does not do anything except pushing events on to
  * DefaultCastEvent.
  *
  * \sa DefaultCastEvent()
  */
bool glictContainer::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue) {
	if (!GetVisible() || !GetEnabled()) return false;
	//printf("Event of type %s passing through %s (%s)\n", EvtTypeDescriptor(evt), objtype, parent ? parent->objtype : "NULL");
	switch (evt) {
		default:
			return DefaultCastEvent(evt, wparam, lparam, returnvalue); // use default processing for all events
		case GLICT_MOUSECLICK:
			return DefaultCastEvent(evt, wparam, lparam, returnvalue); // we never catch a mouse click in glictContainer... but some child might do so
			break;
	}
}

/**
  * Sets an OnClick function that would a, for example, button use.
  */
void glictContainer::SetOnClick(void(*f)(glictPos* relmousepos, glictContainer* callerclass)) {
	this->OnClick = f;
}

/**
  * Sets an OnPaint function that would a, for example, button use.
  *
  * \todo Not all widgets obey OnPaint.
  */
void glictContainer::SetOnPaint(void(*f)(glictRect* real, glictRect* clipped, glictContainer* callerclass)) {
	this->OnPaint = f;
}

/**
  * Function collects the OpenGL's modelview matrix, and remembers whatever
  * state it currently is in. It is later used in TransformScreenCoords().
  * It is not necessary to call this function unless you wish to perform
  * a transformation on user interface, for example rotation. If you do not
  * do this kind of thing, you do not need to call this function.
  *
  * This also makes all children remember their transformation.
  *
  * \sa TransformScreenCoords(glictPos *pos)
  */
void glictContainer::RememberTransformations() {

	std::vector<glictContainer*>::iterator it;

	glGetFloatv(GL_MODELVIEW_MATRIX, ModelviewMatrix);
	//printf("Remembering %s's modelview matrix (child of %s)\n", objtype, parent ? parent->objtype : "NULL");

	for (it=objects.begin(); it!=objects.end(); it++) {
		(*it)->RememberTransformations();
	}
}

/**
  * Debugging function that reports some info about the current object, such
  * as its name, height, width, position et al.
  */
void glictContainer::ReportDebug() {

	printf("--%s--\n", objtype);
	printf("Height: %d\n", height);
	printf("Width: %d\n", width);
	printf("Pos: %d %d\n", x, y);
	printf("Rect TBLR: %d %d %d %d\n", top, bottom, left, right);
	printf("Clip TBLR: %d %d %d %d\n", cliptop, clipbottom, clipleft, clipright);
	printf("GUID: %d\n", guid);
	std::vector<glictContainer*>::iterator it;
	for (it=objects.begin(); it!=objects.end(); it++) {
		(*it)->ReportDebug();
	}
}

/**
  * \param *pos Coordinates to transform. Result gets placed here as well.
  *
  * Using the modelview matrix that was stored using the
  * RememberTransformations() this function will create an inverse matrix
  * of the modelview matrix and multiply the original coordinates by it,
  * storing the result in the input parameter.
  *
  * \warning Watch out, *pos gets overwritten!
  * \sa RememberTransformations()
  * \bug This function does not work properly the way it should, it should be
  *      inspected whether the float/double precision is causing problems
  *      or something else. It is also certain that there is some other
  *      problem within this function as certain translation transfo's don't
  *      work.
  * \todo Scale is not operational, look into that. (Mathematicians,
  *       please come to aid.)
  * \todo This only takes modelview matrix into account, while we should take
  *       projection matrix into account as well.
  */
void glictContainer::TransformScreenCoords(glictPos *pos) {


	int i,j,k;

	/*printf("original\n");
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++)
			printf("%.02f ", ModelviewMatrix[i*4 + j]);
		printf("\n");
	}
	//system("pause");
*/

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
	double m[32];
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
	double result[16];
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





	/*printf("inverse\n");
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++)
			printf("%.02f ", result[i*4 + j]);
		printf("\n");
	}
	//system("pause");
*/


	// multiply matrix by coordinates
	pos->x = (int)((double)(pos->x) * result[0 * 4 + 0]
			+ (double)(pos->y) * result[1 * 4 + 0]
			+ (double)(0     ) * result[2 * 4 + 0]
			+ (double)(1     ) * result[3 * 4 + 0]);
	pos->y = (int)((double)(pos->x) * result[0 * 4 + 1]
			+ (double)(pos->y) * result[1 * 4 + 1]
			+ (double)(0     ) * result[2 * 4 + 1]
			+ (double)(1     ) * result[3 * 4 + 1]);

}

/**
  * \param caption Caption that's to be set
  *
  * If the object that derived from this class supports captions, then this
  * function will change the caption being displayed on it.
  */
void glictContainer::SetCaption(const std::string caption) {
	this->caption = caption;
}
/**
  * \return Caption of the object.
  *
  * If the object that derived from this class supports captions, then this
  * function will get the caption being displayed on it.
  */
std::string glictContainer::GetCaption() {
	return caption;
}

/**
  * Obtains the current object's parent, as stored in parent variable.
  */
glictContainer* glictContainer::GetParent() {
	return parent;
}

/**
  * \param callerchild This object's child that asks this object to focus.
  *
  * Sets the focus to the current object. Parameter is either the
  * child that has called the focusing function, or NULL to signify
  * that there is no child to refocus here, that the widget is supposed only
  * to refocus itself.
  *
  * In applications, you should mostly use NULL argument.
  */
void glictContainer::Focus(glictContainer* callerchild) {
    //printf("FOCUSING ON %s\n", this->objtype);
	if (callerchild && callerchild->focusable && objects.size()) {
		std::vector<glictContainer*>::iterator it;
		bool heredone=false;
		for (it=objects.begin(); it!=objects.end(); it++) {
			if ((*it)==callerchild) {
				objects.erase(it);
				heredone=true;
				break;
			}
		}
		if (heredone)
			objects.insert(objects.end(), callerchild);
	}


	if (parent) {
		parent->Focus(this);
	} else { // we're on top level
	}

	//if (!callerchild) printf("Focused on %s\n", this->objtype);
	glictGlobals.topFocused = this;
}

/**
  * \param vsbl New value for visibility
  *
  * Sets whether the object is visible or not.
  */

void glictContainer::SetVisible(bool vsbl) {
	this->visible = vsbl;
}

/**
  * \return Boolean, object's visibility
  *
  * Retrieves whether the object is visible or not.
  */

bool glictContainer::GetVisible() {
	return this->visible;
}

/**
  * \param enabled Object's "enabledness"
  *
  * Sets whether the object is enabled or not.
  */

void glictContainer::SetEnabled(bool enabled) {
	this->enabled = enabled;
}

/**
  * \return Boolean, object's "enabledness"
  *
  * Retrieves whether the object is enabled or not.
  *
  *
  */

bool glictContainer::GetEnabled() {
	return this->enabled;
}

/**
  * This function goes all the way up to the root of the tree and fixes the
  * boundaries from up there, making sure clipping occurs properly.
  *
  * This function should be used only internally.
  */


/**
  * Describes event being passed as the parameter and returns its name as a
  * string. For debugging purposes.
  */

const char* glictContainer::EvtTypeDescriptor(glictEvents evt) {

	if (evt==GLICT_MOUSEDOWN) return "mousedown";
	if (evt==GLICT_MOUSEUP) return "mouseup";
	if (evt==GLICT_MOUSECLICK) return "mouseclick";
	return "UNKNOWN";
}




/**
  * This function goes all the way up to the root of the tree and fixes the
  * boundaries from up there, making sure clipping occurs properly.
  *
  * This function should be used only internally.
  */
void glictContainer::RecursiveBoundaryFix() {
    if (parent)
        parent->RecursiveBoundaryFix();
    else
        this->SetPos(x,y);
}

/**
  * In case user sets a new skin, we need to fix container offsets.
  * Each widget should redefine it and use appropriate skin element's properties.
  *
  * This function should be used only internally.
  */
void glictContainer::FixContainerOffsets() {

}



/**
  * \todo Document this function
  */
void glictContainer::SetCustomData(void *param) {
    this->customdata = param;
}

/**
  * \todo Document this function
  */
void* glictContainer::GetCustomData() {
    return customdata;
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
void glictContainer::SetVirtualSize(int w, int h) {
    virtualsize.w = w;
    virtualsize.h = h;
}
/**
  * Scrolls to the virtual area's bottom.
  * Container has it EMPTY because it does not directly support virtual area.
  */
void glictContainer::VirtualScrollBottom() {

}

