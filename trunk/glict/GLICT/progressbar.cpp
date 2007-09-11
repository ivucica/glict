#include <GLICT/progressbar.h>

glictProgressBar::glictProgressBar () {
	percent = 0;
}

glictProgressBar::~glictProgressBar () {

}

void glictProgressBar::Paint() {
	glictGlobals.PaintRect(this->x+glictGlobals.translation.x, this->x+this->width+glictGlobals.translation.x,
		this->y+glictGlobals.translation.y, this->y+this->height+glictGlobals.translation.y, glictColor(1,1,1,1));

	glictGlobals.PaintRect(this->x+glictGlobals.translation.x +2, this->x+this->width+glictGlobals.translation.x -2,
		this->y+glictGlobals.translation.y +2,  this->y+this->height+glictGlobals.translation.y -2, glictColor(0,1,0,1));


	this->CPaint();

	// this is here so that scissoring resumes properly
	this->SetScissor();

}

void glictProgressBar::SetValue(float newpcnt) {
	percent = newpcnt;
}

void glictProgressBar::AddValue(float newpcnt) {
	percent += newpcnt;
}

float glictProgressBar::GetValue() {
	return percent;
}

