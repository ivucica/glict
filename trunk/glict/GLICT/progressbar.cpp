#include <GLICT/progressbar.h>
#include <GLICT/globals.h>
#include <GLICT/types.h>
glictProgressBar::glictProgressBar () {
	percent = 0;
}

glictProgressBar::~glictProgressBar () {

}

void glictProgressBar::Paint() {
	static glictColor white(.75,.75,.75,.75);
	static glictColor green(0,1,0,1);
	glictGlobals.PaintRect(this->x+glictGlobals.translation.x, this->x+this->width+glictGlobals.translation.x,
		this->y+glictGlobals.translation.y, this->y+this->height+glictGlobals.translation.y, white);

	glictGlobals.PaintRect(this->x+glictGlobals.translation.x +1, this->x+(this->width* percent/100.)+glictGlobals.translation.x  -1,
		this->y+glictGlobals.translation.y +1,  this->y+this->height+glictGlobals.translation.y -1, green);


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

