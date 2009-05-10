#include <GLICT/progressbar.h>
#include <GLICT/globals.h>
glictProgressBar::glictProgressBar () : bgColor(.75,.75,.75,.75), fgColor(0,1,0,1), borderColor(0,0,0,0) {
	percent = 0;
}

glictProgressBar::~glictProgressBar () {

}

void glictProgressBar::Paint() {
	if(bgactive)
		glictGlobals.PaintRect(this->x+glictGlobals.translation.x, this->x+this->width+glictGlobals.translation.x,
			this->y+glictGlobals.translation.y, this->y+this->height+glictGlobals.translation.y, bgColor);

	if (percent>0)
		glictGlobals.PaintRect(this->x+glictGlobals.translation.x +1, this->x+((this->width-1)* percent/100.)+glictGlobals.translation.x,
			this->y+glictGlobals.translation.y +1,  this->y+this->height+glictGlobals.translation.y -1, fgColor);
	glictGlobals.PaintRectLines(this->x+glictGlobals.translation.x, this->x+this->width+glictGlobals.translation.x,
		this->y+glictGlobals.translation.y, this->y+this->height+glictGlobals.translation.y, borderColor);

	this->CPaint();

	// this is here so that scissoring resumes properly
	this->SetScissor();

}

void glictProgressBar::SetValue(float newpcnt) {
	percent = newpcnt;
	if (percent>100)
		printf("glictProgressBar::SetValue(): warning: percent > 100, set to %g\n", newpcnt);
}

void glictProgressBar::AddValue(float newpcnt) {
	percent += newpcnt;
}

float glictProgressBar::GetValue() {
	return percent;
}

