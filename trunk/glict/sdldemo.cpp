#include <SDL/SDL.h>
#include <GLICT/container.h>
#include <GLICT/window.h>
#include <GLICT/button.h>
#include <GLICT/scrollbar.h>
#include <GLICT/fonts.h>
#include <GLICT/globals.h>
//#include "sdlfont.h"
glictContainer desktop;
glictWindow win;
glictButton btn;
glictScrollbar scb;
SDL_Surface* screen;
SDL_Surface* sysfontpic;
void onclick(glictPos *a, glictContainer* callclass) {
	printf("...\n");
}
void onclickscb(glictPos *a, glictContainer* callclass) {
	printf("SCB %d\n", scb.GetValue());
}
void SDLFontDrawChar(char t, SDL_Surface* img, int x1, int y1) {
	t -= 32;
    int x = (int)(t % 32)*16.;
    int y = (int)(t / 32)*16.;

    int w = 10;
    int h = 10;

	SDL_Rect dest;
	dest.x = x1;
	dest.y = y1;
	SDL_Rect src;
	src.x = x;
	src.y = y;
	src.w = w;
	src.h = h;
	SDL_BlitSurface(img, &src, screen, &dest);
}
void SDLFontDraw(const char* txt, const void* font, float x, float y){

	SDL_Surface *img = (SDL_Surface*)font;

	float cx=x*10,cy=y*10;

    volatile register float sizesofar = 0.;
	volatile register float linessofar = 0.;
	for (volatile register unsigned char *t = (unsigned char*)txt; *t; ++t) {
		switch (*t) {
			default:

				SDLFontDrawChar(*t, img, cx, cy);
				cx += 10;
				sizesofar += 10;
				break;
			case '\n':
			case '\r':
				cx -= sizesofar;
				cy += 10;
				linessofar += 1.;
				sizesofar = 0;
				if (*t == '\n' && *(t+1)=='\r' || *t == '\r' && *(t+1)=='\n' ) t++;
                break;

		}
	}


}
float SDLFontSize(const char* txt, const void* font) {
	return strlen(txt);
}

void SDLRectDraw(float left, float right, float top, float bottom, const glictColor &col) {
	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	int color = SDL_MapRGB(vi->vfmt, (int)(col.r * 255), (int)(col.g * 255), (int)(col.b * 255));
	SDL_Rect rect = {left, top, right-left, bottom-top};
	SDL_FillRect(screen, &rect, color);
}

void SDLRectLinesDraw(float left, float right, float top, float bottom, const glictColor &col) {
	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	int color = SDL_MapRGB(vi->vfmt, (int)(col.r * 255), (int)(col.g * 255), (int)(col.b * 255));
	//SDL_Rect rect = {left, top, right-left, bottom-top};
	{
		SDL_Rect rect = {left, top, 1, bottom-top};
		SDL_FillRect(screen, &rect, color);
	}
	{
		SDL_Rect rect = {right-1, top, 1, bottom-top};
		SDL_FillRect(screen, &rect, color);
	}
	{
		SDL_Rect rect = {left, top, right-left, 1};
		SDL_FillRect(screen, &rect, color);
	}
	{
		SDL_Rect rect = {left, bottom-1,right-left, 1};
		SDL_FillRect(screen, &rect, color);
	}
}


int main () {
	int videoflags = SDL_HWSURFACE  | SDL_ANYFORMAT| SDL_DOUBLEBUF | SDL_RESIZABLE;
	int width = 640;
	int height = 480;
	int video_bpp = 32;


	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}



	screen = SDL_SetVideoMode(width, height, video_bpp, videoflags);

	if (!screen){
		fprintf(stderr, "Could not set %dx%d video mode: %s\n", width, height, SDL_GetError());
		exit(1);
	}

    printf("Set 640x480 at %d bits-per-pixel mode\n",
           screen->format->BitsPerPixel);

    glictFont* sysfont = glictCreateFont("system");
	sysfontpic = SDL_LoadBMP("font.bmp");

	SDL_SetColorKey(sysfontpic, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(SDL_GetVideoInfo()->vfmt, 0xFF, 0, 0xFF)); // magneta is transparent

    sysfont->SetFontParam(sysfontpic);
    sysfont->SetRenderFunc(SDLFontDraw);
    sysfont->SetSizeFunc(SDLFontSize);


	desktop.AddObject(&win);
	win.AddObject(&btn);
	win.SetPos(50, 50);
	btn.SetOnClick(onclick);
	btn.SetBGColor(.5,.5,.5,1.);

    win.AddObject(&scb);
    scb.SetPos(0,50);
    scb.SetWidth(100);
    scb.SetHeight(15);
    scb.SetMin(0);
    scb.SetMax(100);
    scb.SetValue(0);
    scb.SetBGColor(.5,.5,.5,1.);
    scb.SetOnClick(onclickscb);

	glictGlobals.w = width;
	glictGlobals.h = height;
	desktop.SetWidth(width);
	desktop.SetHeight(height);

	glictGlobals.paintrectCallback = SDLRectDraw;
	glictGlobals.paintrectlinesCallback = SDLRectLinesDraw;
	glictGlobals.enableGlTranslate = false;

	SDL_WM_SetCaption("GLICT/SDL Demo", "");
	bool running = true;
	SDL_Event event;

	while(running){
		while(SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_KEYDOWN:

					break;
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:{
					glictPos p = {event.button.x, event.button.y};
					printf("%d %d\n", event.button.x, event.button.y);
					desktop.CastEvent(event.type == SDL_MOUSEBUTTONUP ? GLICT_MOUSEUP : GLICT_MOUSEDOWN, &p,0);

					break;
				}
				case SDL_QUIT:
						running = false;
					break;
				default:
					break;
			}
		}

		// blanks the screen..
		glictColor c(0,0,0,1);
		SDLRectDraw(0,640,0,480,c);

		desktop.Paint();
		SDL_Flip(screen);

	}

	return 0;
}
