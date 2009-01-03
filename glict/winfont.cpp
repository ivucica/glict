// This code is  licensed under GNU Lesser General Public License v2.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include "winfont.h"
WinFontStruct *WinFontCreate(char* fontname, char style, char size) {
	HFONT	font;						// Windows Font ID
	GLuint	base;
	GLYPHMETRICSFLOAT gmf[256];
	base = glGenLists(256);					// Storage For 256 Characters


	// Height
	//By putting a minus, we're telling windows to find us a font based on the CHARACTER height.
	//If we use a positive number we match the font based on the CELL height.

	// Width
	//Then we specify the cell width. You'll notice I have it set to 0.
	//By setting values to 0, windows will use the default value.
	//You can play around with this value if you want. Make the font wide, etc.

	// Angle
	//Angle of Escapement will rotate the font. Orientation Angle quoted from MSDN help
	//Specifies the angle, in tenths of degrees, between each character's base line and
	//the x-axis of the device.
	//Unfortunately I have no idea what that means :(

	// Weight
	//Font weight is a great parameter. You can put a number from 0 - 1000 or you can use
	//one of the predefined values. FW_DONTCARE is 0, FW_NORMAL is 400, FW_BOLD is 700
	//and FW_BLACK is 900. There are alot more predefined values, but those 4 give some
	//good variety. The higher the value, the thicker the font (more bold).

	// Style
	//Italic, Underline and Strikeout can be either TRUE or FALSE. Basically if underline
	//is TRUE, the font will be underlined. If it's FALSE it wont be. Pretty simple :)

	// Charset
	//Character set Identifier describes the type of Character set you wish to use.
	//There are too many types to explain. CHINESEBIG5_CHARSET, GREEK_CHARSET, RUSSIAN_CHARSET,
	//DEFAULT_CHARSET, etc. ANSI is the one I use, although DEFAULT would probably work
	//just as well.
	//If you're interested in using a font such as Webdings or Wingdings, you need to use
	//SYMBOL_CHARSET instead of ANSI_CHARSET.

	// Precision
	//Output Precision is very important. It tells Windows what type of character set
	//to use if there is more than one type available. OUT_TT_PRECIS tells Windows that
	//if there is more than one type of font to choose from with the same name, select
	//the TRUETYPE version of the font. Truetype fonts always look better, especially
	//when you make them large. You can also use OUT_TT_ONLY_PRECIS, which ALWAYS trys
	//to use a TRUETYPE Font.

	// Clipping Precision
	//is the type of clipping to do on the font if it goes outside the clipping region.
	//Not much to say about this, just leave it set to default.

	// Output Quality
	//is very important.you can have PROOF, DRAFT, NONANTIALIASED, DEFAULT or ANTIALIASED.
	//We all know that ANTIALIASED fonts look good :) Antialiasing a font is the same effect
	//you get when you turn on font smoothing in Windows. It makes everything look less jagged.

	// Family and Pitch
	//Next we have the Family and Pitch settings. For pitch you can have DEFAULT_PITCH,
	//FIXED_PITCH and VARIABLE_PITCH, and for family you can have FF_DECORATIVE, FF_MODERN,
	//FF_ROMAN, FF_SCRIPT, FF_SWISS, FF_DONTCARE. Play around with them to find out what
	//they do. I just set them both to default.

	// Font name
	//Finally... We have the actual name of the font. Boot up Microsoft Word or some other
	//text editor. Click on the font drop down menu, and find a font you like. To use the
	//font, replace 'Comic Sans MS' with the name of the font you'd rather use.

	font = CreateFont(	-12,												// Height Of Font
						0,													// Width Of Font
						0,													// Angle Of Escapement
						0,													// Orientation Angle
						(style & WINFONT_BOLD) ? FW_BOLD : FW_DONTCARE,		// Font Weight
						style & WINFONT_ITALIC,								// Italic
						style & WINFONT_UNDERLINE,							// Underline
						style & WINFONT_STRIKEOUT,							// Strikeout
						ANSI_CHARSET,										// Character Set Identifier
						OUT_TT_PRECIS,										// Output Precision
						CLIP_DEFAULT_PRECIS,								// Clipping Precision
						ANTIALIASED_QUALITY,								// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,							// Family And Pitch
						fontname);											// Font Name

	if (!font) {
		MessageBox(HWND_DESKTOP, "Failed to create font.", 0,0);
		return NULL;
	}

	HDC hDC = CreateCompatibleDC(GetDC(HWND_DESKTOP)); //"DISPLAY", NULL, NULL, NULL);
	if (!hDC) {
		MessageBox(HWND_DESKTOP, "Failed to create font.", 0,0);
		return NULL;
	}
	SelectObject(hDC, font);				// Selects The Font We Created

	//Now for the new code. We build our Outline font using a new command wglUseFontOutlines.
	//We select our DC, the starting character, the number of characters to create and the
	//'base' display list value. All very similar to the way we built our Bitmap font.


	//That's not all however. We then set the deviation level. The closer to 0.0f, the
	//smooth the font will look. After we set the deviation, we get to set the font
	//thickness. This describes how thick the font is on the Z axis. 0.0f will produce
	//a flat 2D looking font and 1.0f will produce a font with some depth.

	//The parameter WGL_FONT_POLYGONS tells OpenGL to create a solid font using polygons.
	//If we use WGL_FONT_LINES instead, the font will be wireframe (made of lines).
	//It's also important to note that if you use GL_FONT_LINES, normals will not be
	//generated so lighting will not work properly.

	//The last parameter gmf points to the address buffer for the display list data.


	wglUseFontOutlines(	hDC,				// Select The Current DC
						0,					// Starting Character
						255,				// Number Of Display Lists To Build
						base,				// Starting Display Lists
						1.0f,				// Deviation From The True Outlines
						0.0f,				// Font Thickness In The Z Direction
						WGL_FONT_POLYGONS,	// Use Polygons, Not Lines
						gmf);				// Address Of Buffer To Recieve Data

	//wglUseFontBitmaps (hDC, 0, 255, base);
	DeleteObject(font);
	DeleteDC(hDC);

	WinFontStruct *font_struct;
	font_struct = (WinFontStruct*)malloc(sizeof(WinFontStruct));
	font_struct->base = base;
	memcpy(font_struct->gmf, gmf, sizeof(gmf[0])*256);
	return font_struct;
}

void WinFontDelete(void* fontvoid) {
	if (fontvoid) {
		WinFontStruct *font = (WinFontStruct*)fontvoid;
		glDeleteLists(font->base, 256);
		free(font);
	}
}

void WinFontDraw(const char* txt, const void* fontvoid, float x, float y) {
	WinFontStruct *font = ((WinFontStruct*)fontvoid);

	glPushAttrib(GL_LIST_BIT|GL_POLYGON_BIT);				// Pushes The Display List Bits
	glListBase(font->base);					// Sets The Base Character to 0

	glCullFace(GL_BACK);
	glEnable (GL_CULL_FACE);

	glPushMatrix();

	glTranslatef(x,y,0);
	glScalef(1.5,1.5,1.);
	//glCallLists(strlen(txt), GL_UNSIGNED_BYTE, txt);	// Draws The Display List Text
	unsigned int sizesofar = 0.;
	for (unsigned char *t = (unsigned char*)txt; *t; ++t) {
		switch (*t) {
			default:
				glCallLists(1, GL_UNSIGNED_BYTE, t);
				sizesofar += (font->gmf[*t].gmfCellIncX * (1 << 16));
				break;
			case '\n':
			case '\r':
				glTranslatef(-(sizesofar / (float)(1 << 16)), -(float)(font->gmf['a'].gmfBlackBoxY) * 1.6 ,0);
				sizesofar = 0;
				if (*t == '\n' && *(t+1)=='\r' || *t == '\r' && *(t+1)=='\n' ) t++;
                break;


		}
	}
	glPopMatrix();

	glPopAttrib();
}

// possibly it might be a good idea to rewrite this?
float WinFontSize(const char* text, const void* fontvoid) {
	WinFontStruct* font = (WinFontStruct*)fontvoid;

	float length=0.;
	for (char *t=(char*)text;*t;++t)	// Loop To Find Text Length
	{
		length+=font->gmf[*t].gmfCellIncX;		// Increase Length By Each Characters Width
	}
	return length*1.5;
}
