/**********************************
 *           THE OUTCAST          *
 *       Codename: Tibia3d        *
 *                                *
 * copyright 2005 OBJECT Networks *
 *********************************/
 
// Glut-helper.cpp
// contains glut helper

// objnet glut helper
// (c) 2005 object networks
//
// contains some helper code for glut
// e.g. entire string printing

#ifndef _glutx_
#define _glutx_
void glutxStrokeString(const char* txt, void* font, float x, float y) ;
char* glutxStrokeStringExpert(const char* txt, void* font);
void glutxBitmapString(char* txt, void* font,int x, int y) ;
float glutxBitmapSize(char* txt, void* font);
float glutxStrokeSize(const char* txt, void* font);
int glutxNumberOfLines(char* txt);
#endif
