#ifndef NO_GL
	#ifdef WIN32
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#endif
	#ifdef __APPLE__
		#include <OpenGL/gl.h>
	#else
		#include <GL/gl.h>
	#endif
#endif
#include "skinner.h"
#include "globals.h"

#ifndef min
    #define min(a, b) ((a) < (b) ? (a) : (b))
#endif
glictSkinner::glictSkinner() {
    topleft = 0;
    topright = 0;
    bottomleft = 0;
    bottomright = 0;

    top = 0;
    left = 0;
    right = 0;
    bottom = 0;

    center = 0;


    topleftf = NULL;
    toprightf = NULL;
    bottomleftf = NULL;
    bottomrightf = NULL;

    topf = NULL;
    leftf = NULL;
    rightf = NULL;
    bottomf = NULL;

    centerf = NULL;



    topleftimg = NULL;
    toprightimg = NULL;
    bottomleftimg = NULL;
    bottomrightimg = NULL;

    topimg = NULL;
    leftimg = NULL;
    rightimg = NULL;
    bottomimg = NULL;

    centerimg = NULL;

}

glictSkinner::~glictSkinner() {
}

void glictSkinner::Paint(glictSize *size) {
    static int x1, y1, x2, y2;

#ifndef NO_GL
	if (center || left || right || top || bottom || topleft || topright || bottomleft || bottomright)  {
		glEnable(GL_TEXTURE_2D);
		glColor4f(1., 1., 1., 1.);
	}
#endif
	static glictColor col (1, 1, 1, 1);
    // first the corners...

    if (centerimg || center || centerf) {
        if (centerf)
            centerf(this, centerp);
		#ifndef NO_GL
        else if (!centerimg)
            glBindTexture(GL_TEXTURE_2D, center);
		#endif

        for (int pos = (left || leftf || leftimg) ? lefts.w : 0; pos < size->w - ((right || rightf || rightimg) ? rights.w : 0); pos+=centers.w) {
            for (int pos2 = (top || topf || topimg) ? tops.h : 0; pos2 < size->h - ((bottom || bottomf || bottomimg) ? bottoms.h : 0); pos2+=centers.h) {

                y1 = pos2;
                y2 = min(size->h - ((bottom || bottomf) ? bottoms.h : 0), pos2 + centers.h);

                x1 = pos;
                x2 = min(size->w - ((right || rightf) ? rights.w : 0), pos + centers.w);

                if (centerimg)
					centerimg->Paint(x1, y1, x2-x1, y2-y1);
				else
					glictGlobals.PaintRect(x1, x2, y1, y2, col);
            }
        }
    }




    if (topimg || top || topf) {

        if (topf)
            topf(this, topp);
		#ifndef NO_GL
        else if (!topimg)
            glBindTexture(GL_TEXTURE_2D, top);
		#endif

        for (int pos = (topleft || topleftf || topleftimg) ? toplefts.w : 0; pos < size->w - ((topright || toprightf || toprightimg) ? toprights.w : 0); pos+=tops.w) {
            x1 = pos; y1 = 0;
            x2 = min(size->w - ((topright || toprightf || toprightimg) ? toprights.w : 0) , pos + tops.w); y2 = tops.h;
            if (topimg)
				topimg->Paint(x1, y1, x2-x1, y2-y1);
			else
				glictGlobals.PaintRect(x1, x2, y1, y2, col);
        }
    }

    if (bottomimg || bottom || bottomf) {
        if (bottomf)
            bottomf(this, bottomp);
		#ifndef NO_GL
        else if (!bottomimg)
            glBindTexture(GL_TEXTURE_2D, bottom);
		#endif
        for (int pos = (bottomleft || bottomleftf || bottomleftimg) ? bottomlefts.w : 0; pos < size->w - ((bottomright || bottomrightf || bottomrightimg) ? bottomrights.w : 0); pos+=bottoms.w) {
            x1 = pos; y1 = size->h - bottoms.h;
            x2 = min(size->w - ((bottomleft || bottomleftf || bottomrightimg) ? bottomlefts.w : 0), pos + bottoms.w); y2 = size->h;
            if (bottomimg)
				bottomimg->Paint(x1, y1, x2-x1, y2-y1);
			else
				glictGlobals.PaintRect(x1, x2, y1, y2, col);

        }
    }



    if (leftimg || left || leftf) {
        if (leftf)
            leftf(this, leftp);
		#ifndef NO_GL
        else if (!leftimg)
            glBindTexture(GL_TEXTURE_2D, left);
		#endif
        for (int pos = (topleft || topleftf || topleftimg) ? toplefts.h : 0; pos < size->h - ((bottomleft || bottomleftf || bottomleftimg) ? bottomlefts.w : 0); pos+=lefts.h) {
            x1 = 0; y1 = pos;
            x2 = lefts.w; y2 = min(size->h - ((bottomleft || bottomleftf || bottomleftimg) ? bottomlefts.h : 0), pos + lefts.h);
            if (leftimg)
				leftimg->Paint(x1, y1, x2-x1, y2-y1);
			else
				glictGlobals.PaintRect(x1, x2, y1, y2, col);
        }
    }


    if (rightimg || right || rightf) {
        if (rightf)
            rightf(this, rightp);
		#ifndef NO_GL
        else if (!rightimg)
            glBindTexture(GL_TEXTURE_2D, right);
		#endif
        for (int pos = (topright || toprightf || toprightimg) ? toprights.h : 0; pos < size->h - ((bottomright || bottomrightf || bottomrightimg) ? bottomrights.w : 0); pos+=rights.h) {
            x1 = size->w - rights.w; y1 = pos;
            x2 = size->w; y2 = min(size->h - ((bottomright || bottomrightf || bottomrightimg) ? bottomrights.w : 0), pos + rights.h);
            if (rightimg)
				rightimg->Paint(x1, y1, x2-x1, y2-y1);
			else
				glictGlobals.PaintRect(x1, x2, y1, y2, col);
        }
    }




    if (topleftimg || topleft || topleftf) {
        x1 = y1 = 0;
        x2 = toplefts.w+1; y2 = toplefts.h;
        if (topleftimg)
			topleftimg->Paint(x1, y1, x2-x1, y2-y1);
		else {
			if (topleftf)
				topleftf(this, topleftp);
			#ifndef NO_GL
			else
				glBindTexture(GL_TEXTURE_2D, topleft);
			#endif
			glictGlobals.PaintRect(x1, x2, y1, y2, col);
		}
    }

    if (toprightimg || topright || toprightf) {
        x1 = size->w - toprights.w; y1 = 0;
        x2 = size->w; y2 = toprights.h;
        if (toprightimg)
			toprightimg->Paint(x1, y1, x2-x1, y2-y1);
		else {
			if (toprightf)
				toprightf(this, toprightp);
			#ifndef NO_GL
			else
				glBindTexture(GL_TEXTURE_2D, topright);
			#endif
			glictGlobals.PaintRect(x1, x2, y1, y2, col);
		}
    }

    if (bottomleftimg || bottomleft || bottomleftf) {
        x1 = 0; y1 = size->h - bottomlefts.h;
        x2 = bottomlefts.w ; y2 = size->h;
        if (bottomleftimg)
			bottomleftimg->Paint(x1, y1, x2-x1, y2-y1);
		else {
			if (bottomleftf)
				bottomleftf(this, bottomleftp);
			#ifndef NO_GL
			else
				glBindTexture(GL_TEXTURE_2D, bottomleft);
			#endif
			glictGlobals.PaintRect(x1, x2, y1, y2, col);
		}
    }

    if (bottomrightimg || bottomright || bottomrightf) {
        x1 = size->w - bottomrights.w; y1 = size->h - bottomrights.h;
        x2 = size->w - 1; y2 = size->h;
        if (bottomrightimg)
			bottomrightimg->Paint(x1, y1, x2-x1, y2-y1);
		else {
			if (bottomrightf)
				bottomrightf(this, bottomrightp);
			#ifndef NO_GL
			else
				glBindTexture(GL_TEXTURE_2D, bottomright);
			#endif
			glictGlobals.PaintRect(x1, x2, y1, y2, col);
		}
    }



#ifndef NO_GL
	if (center || left || right || top || bottom || topleft || topright || bottomleft || bottomright)  {
		glDisable(GL_TEXTURE_2D);
	}
#endif

}


void glictSkinner::SetTL(glictImage* img ) {
    this->topleftimg = img;
    if (img)
		this->toplefts = img->GetSize();
}
void glictSkinner::SetTL(GLuint textureid, glictSize *size ) {
    this->topleftf = NULL;
    this->topleftimg = NULL;
    this->topleft = textureid;
    if (size) this->toplefts = *size;
}
void glictSkinner::SetTL(glictBindCallback bindf, void* param, glictSize *size) {
    this->topleftimg = NULL;
    this->topleftf = bindf;
    this->topleftp = param;
    if (size) this->toplefts = *size;
}


void glictSkinner::SetTR(glictImage* img ) {
    this->toprightimg = img;
    if (img)
		this->toprights = img->GetSize();
}
void glictSkinner::SetTR(GLuint textureid, glictSize *size ) {
    this->toprightf = NULL;
    this->toprightimg = NULL;
    this->topright = textureid;
    if (size) this->toprights = *size;
}
void glictSkinner::SetTR(glictBindCallback bindf, void* param, glictSize *size) {
	this->toprightimg = NULL;
    this->toprightf = bindf;
    this->toprightp = param;
    if (size) this->toprights = *size;
}



void glictSkinner::SetBL(glictImage* img ) {
    this->bottomleftimg = img;
    if (img)
		this->bottomlefts = img->GetSize();
}
void glictSkinner::SetBL(GLuint textureid, glictSize *size ) {
    this->bottomleftf = NULL;
    this->bottomleftimg = NULL;
    this->bottomleft = textureid;
    if (size) this->bottomlefts = *size;
}
void glictSkinner::SetBL(glictBindCallback bindf, void* param, glictSize *size) {
	this->bottomleftimg = NULL;
    this->bottomleftf = bindf;
    this->bottomleftp = param;
    if (size) this->bottomlefts = *size;
}


void glictSkinner::SetBR(glictImage* img ) {
    this->bottomrightimg = img;
    if (img)
		this->bottomrights = img->GetSize();
}
void glictSkinner::SetBR(GLuint textureid, glictSize *size ) {
    this->bottomrightf = NULL;
    this->bottomrightimg = NULL;
    this->bottomright = textureid;
    if (size) this->bottomrights = *size;
}
void glictSkinner::SetBR(glictBindCallback bindf, void* param, glictSize *size) {
	this->bottomrightimg = NULL;
    this->bottomrightf = bindf;
    this->bottomrightp = param;
    if (size) this->bottomrights = *size;
}


void glictSkinner::SetTop(glictImage* img ) {
    this->topimg = img;
    if (img)
		this->tops = img->GetSize();
}
void glictSkinner::SetTop(GLuint textureid, glictSize *size ) {
    this->topf = NULL;
    this->topimg = NULL;
    this->top = textureid;
    if (size) this->tops = *size;
}
void glictSkinner::SetTop(glictBindCallback bindf, void* param, glictSize *size) {
	this->topimg = NULL;
    this->topf = bindf;
    this->topp = param;
    if (size) this->tops = *size;
}



void glictSkinner::SetBottom(glictImage* img ) {
    this->bottomimg = img;
    if (img)
		this->bottoms = img->GetSize();
}
void glictSkinner::SetBottom(GLuint textureid, glictSize *size ) {
    this->bottomf = NULL;
    this->bottomimg = NULL;
    this->bottom = textureid;
    if (size) this->bottoms = *size;
}
void glictSkinner::SetBottom(glictBindCallback bindf, void* param, glictSize *size) {
	this->bottomimg = NULL;
    this->bottomf = bindf;
    this->bottomp = param;
    if (size) this->bottoms = *size;
}



void glictSkinner::SetLeft(glictImage* img ) {
    this->leftimg = img;
    if (img)
		this->lefts = img->GetSize();
}
void glictSkinner::SetLeft(GLuint textureid, glictSize *size ) {
    this->leftf = NULL;
    this->leftimg = NULL;
    this->left = textureid;
    if (size) this->lefts = *size;
}
void glictSkinner::SetLeft(glictBindCallback bindf, void* param, glictSize *size) {
	this->leftimg = NULL;
    this->leftf = bindf;
    this->leftp = param;
    if (size) this->lefts = *size;
}



void glictSkinner::SetRight(glictImage* img ) {
    this->rightimg = img;
    if (img)
		this->rights = img->GetSize();
}
void glictSkinner::SetRight(GLuint textureid, glictSize *size ) {
    this->rightf = NULL;
    this->rightimg = NULL;
    this->right = textureid;
    if (size) this->rights = *size;
}
void glictSkinner::SetRight(glictBindCallback bindf, void* param, glictSize *size) {
	this->rightimg = NULL;
    this->rightf = bindf;
    this->rightp = param;
    if (size) this->rights = *size;
}


void glictSkinner::SetCenter(glictImage* img ) {
    this->centerimg = img;
    if (img)
		this->centers = img->GetSize();
}
void glictSkinner::SetCenter(GLuint textureid, glictSize *size ) {
    this->centerf = NULL;
    this->centerimg = NULL;
    this->center = textureid;
    if (size) this->centers = *size;
}
void glictSkinner::SetCenter(glictBindCallback bindf, void* param, glictSize *size) {
	this->centerimg = NULL;
    this->centerf = bindf;
    this->centerp = param;
    if (size) this->centers = *size;
}


glictSize glictSkinner::GetLeftSize() {
    return this->lefts;
}
glictSize glictSkinner::GetTopSize() {
    return this->tops;
}
glictSize glictSkinner::GetRightSize() {
    return this->rights;
}
glictSize glictSkinner::GetBottomSize() {
    return this->bottoms;
}
glictSize glictSkinner::GetCenterSize() {
    return this->centers;
}
