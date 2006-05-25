#include <stdlib.h>
#include <time.h>
#include "globals.h"


glitGlobalsClass glitGlobals;

glitGlobalsClass::glitGlobalsClass() {
	windowTitleBgColor[0] = 0.0; windowTitleBgColor[1] = 0.0; windowTitleBgColor[2] = 1.0; windowTitleBgColor[3] = 1.0;
	windowTitleColor[0] = 1.0; windowTitleColor[1] = 1.0; windowTitleColor[2] = 1.0; windowTitleColor[3] = 1.0;
	
	renderMode = GLIT_RENDERING;
	
    srand(time(NULL));
}
glitGlobalsClass::~glitGlobalsClass() {
}

