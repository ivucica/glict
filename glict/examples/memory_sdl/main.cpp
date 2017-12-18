/*
    GLICT - Graphics Library Interface Creation Toolkit
    Copyright (C) 2006 OBJECT Networks

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
  * \example examples/memory_sdl/main.cpp
  * This is a variant on the "memory/" example program, modified to
  * demonstrate how to use GLICT with other rendering APIs such as SDL.
  *
  *  Being that the author of this code is a beginner SDL programmer,
  *  he asks you to forgive him on the newbie code, and for the lack
  *  of comments. Those who need this code will probably understand it.
  *
  * \bug Palette is not correctly set when in 8bit mode.
  */

// GLICTMemory_SDL
//  simple game made with GLICT
//  to compile you'll need SDL

#include <time.h>

// SDL includes
// we use sdl because it's cross platform compatible
#include <SDL/SDL.h>



// GLICT includes
// following includes should, in your project, actually look a
// bit different
// this project, as it is, should be placed inside the GLICT's folder so
// its includes is relative
#include <GLICT/container.h>
#include <GLICT/button.h>
#include <GLICT/globals.h>
#include <GLICT/window.h>
#include <GLICT/panel.h>
#include <GLICT/messagebox.h>
#include <GLICT/fonts.h>
#include "sdlfont.h"
// some nice strings for the cards... ;)
char cardtitles[16][16] = { "Gecko", "Smygflik", "mips", "the fike", "Pekay", "Yorick", "tliff", "SimOne"};
// and some colors for the cards
float cardcolor[16][3] = { { 1., 0., 0.}, { 0., 0., .9 }, {.8, .8, 0.}, {.7, 0., .7},
                           {0., .85, 0.},  { .55, 0., 0. }, {0., 75., .75}, {.5, 1., .5}};
// other game related vars
char matrix[16]; // matrix specifies where is each card
char taken[16]; // taken specifies if we've used a card in generation, so we dont generate same pair again
char solved[16]; // solved specifies if a piece has been solved
char flipd[2]; // which cards are currently flipped


int totalsolved = 0, totalopens = 0;

glictContainer desktop;
glictWindow window;
glictButton cards[16];
glictPanel pnlSolveds;
glictMessageBox* msgSuccess;
glictButton btnClose;
int windowhandle; // glut's window identifier

bool running = true;




// this happens when msgbox is dismissed; serves for memory cleanup
void OnDismissSuccess (glictPos* relmousepos, glictContainer* caller) {
    delete caller;
}


// CardOnClick is executed upon click on a card with mouse
// we'll identify which card is used by pointer difference
void CardOnClick(glictPos* relmousepos, glictContainer* callerclass) {
    glictButton *button = dynamic_cast<glictButton*>(callerclass);
    int cardclicked = button - cards;
    //char c[256];
    //sprintf(c, "%d", cardclicked);


	SDL_WM_SetCaption("GLICTMemory", "");


    if (flipd[1]!=-1 ) {

        if (!solved[flipd[0]]) {
            char tmp[16];
            sprintf(tmp, "%d", flipd[0]); cards[flipd[0]].SetCaption(tmp); cards[flipd[0]].SetBGColor(0.5, 0.5, 0.5, 1.);
            sprintf(tmp, "%d", flipd[1]); cards[flipd[1]].SetCaption(tmp); cards[flipd[1]].SetBGColor(0.5, 0.5, 0.5, 1.);

        }
        flipd[0]=-1;
        flipd[1]=-1;

    }

    if (solved[cardclicked]) {
       SDL_WM_SetCaption("GLICTMemory - Card's been solved already.", "");
       return;
    }

    if (flipd[0]!=-1 && cardclicked==flipd[0]) {
        return;
    }


    button->SetCaption(cardtitles[matrix[cardclicked]]);
    button->SetBGColor(cardcolor[matrix[cardclicked]][0], cardcolor[matrix[cardclicked]][1], cardcolor[matrix[cardclicked]][2], 1.);
    if (flipd[0]==-1) flipd[0]=cardclicked; else {
        flipd[1]=cardclicked;


        totalopens++;
        if (matrix[flipd[0]] == matrix[flipd[1]]) {
            SDL_WM_SetCaption("GLICTMemory - Correct!", "");
            solved[flipd[0]] = 1;
            solved[flipd[1]] = 1;
            totalsolved++;

            if (totalsolved == 8) {
                msgSuccess = new glictMessageBox;
                msgSuccess->SetOnDismiss(OnDismissSuccess);
                msgSuccess->SetCaption("Congrats!");
                msgSuccess->SetMessage("You've solved the game!");
                msgSuccess->SetPos(32,32);
                window.AddObject(msgSuccess);

            }
        } else {
            SDL_WM_SetCaption("GLICTMemory - Wrong!", "");
        }
    }


    char solveds[256];
    sprintf(solveds, "Solved: %d\nOpens: %d", totalsolved, totalopens);
    pnlSolveds.SetCaption(solveds);

}

void CloseOnClick(glictPos* relmousepos, glictContainer* callerclass) {
    // when close button is clicked on ingame window
    running=false;
}


// game related, board generation function
void generate() {
     bool gen;
     int x,y;
     srand(time(NULL));
     for (int i=0;i<16;i++) taken[i]=false;
     for (int i=0;i<8;i++) {
         gen=false;
         while (!gen) {
               x = (int)(rand() / (float)RAND_MAX * (float)16);
               if (!taken[x]) {taken[x]=true;  matrix[x]=i; gen=true;}
         }

         gen=false;
         while (!gen) {
               x = (int)(rand() / (float)RAND_MAX * (float)16);
               if (!taken[x]) {taken[x]=true;  matrix[x]=i; gen=true;}

         }
     }
     flipd[0]=-1;
     flipd[1]=-1;
}
// MainWidgets initializes the widgets on main screen
void MainWidgets() {
    generate();
    window.SetCaption("Board");
    desktop.AddObject(&window);
    window.AddTitlebarObject(&btnClose);
    btnClose.SetWidth(12);
    btnClose.SetHeight(12);
    btnClose.SetPos(96*4-12, 0);
    btnClose.SetOnClick(CloseOnClick);
    btnClose.SetCaption("X");

    for (int i=0; i<16;i++) {

        cards[i].SetPos((i % 4) * 96, (i / 4) * 96);
        cards[i].SetWidth(96);
        cards[i].SetHeight(96);
        cards[i].SetBGColor(0.5, 0.5, 0.5, 1.);

        char tmp[3];
        sprintf(tmp,"%d", i);
        cards[i].SetCaption(tmp);
        cards[i].SetOnClick(CardOnClick);

        window.AddObject(&cards[i]);
    }

    desktop.AddObject(&pnlSolveds);
    pnlSolveds.SetCaption("Solved: 0\nOpens: 0");
    pnlSolveds.SetBGColor(0.,0.,0.,1.);
    pnlSolveds.SetWidth(100);
}

void reshape(int x, int y) {
    // set desktop's width and height to what we got
    desktop.SetWidth(x);
    desktop.SetHeight(y);

    window.SetWidth(96*4);
    window.SetHeight(96*4);

    window.SetPos(x/2-96*2, y/2-96*2);
}

SDL_Surface * screen;
SDL_Surface * sysfontpic;

void SDLRectDraw(float left, float right, float top, float bottom, const glictColor &col) {
	static const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	int color = SDL_MapRGB(vi->vfmt, (int)(col.r * 255), (int)(col.g * 255), (int)(col.b * 255));
	SDL_Rect rect = {left, top, right-left, bottom-top};
	SDL_FillRect(screen, &rect, color);
}
void SDLRectLDraw(float left, float right, float top, float bottom, const glictColor &color) {

	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	int col = SDL_MapRGBA(vi->vfmt, (int)(color.r), (int)(color.g), (int)(color.b), (int)(color.a));
	int height = bottom-top;
	int width = right-left;
	int x = left;
	int y = top;
	{
		SDL_Rect rect = {(int)x, (int)y, 1, (int)height};
		SDL_FillRect(screen, &rect, col);
	}

	{
		SDL_Rect rect = {(int)(x+width-1.f), (int)y, 1, (int)height};
		SDL_FillRect(screen, &rect, col);
	}

	{
		SDL_Rect rect = {(int)x, (int)y, (int)width, 1};
		SDL_FillRect(screen, &rect, col);
	}

	{
		SDL_Rect rect = {(int)x, (int)(y+height-1.f), (int)width, 1};
		SDL_FillRect(screen, &rect, col);
	}
}

// the main function initializes everything
int main(int argc, char** argv) {

	int videoflags = SDL_SWSURFACE  | SDL_ANYFORMAT|  SDL_DOUBLEBUF | SDL_RESIZABLE | SDL_SRCALPHA  ;
	int width = 640;
	int height = 480;
	int video_bpp = 32;


	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}


	//printf("'Best' video mode: %d\n", video_bpp = SDL_GetVideoInfo()->vfmt->BitsPerPixel);

	screen = SDL_SetVideoMode(width, height, video_bpp, videoflags);

	if (!screen){
		fprintf(stderr, "Could not set %dx%d video mode: %s\n", width, height, SDL_GetError());
		exit(1);
	}

    printf("Set 640x480 at %d bits-per-pixel mode\n",
           screen->format->BitsPerPixel);

	if (screen->format->BitsPerPixel==8) {

		SDL_Color colors[256];
		for (int i = 0 ; i < 256; i++) {
			colors[i].r = i; colors[i].g = i; colors[i].b = i;
		}

		SDL_SetColors(screen, colors, 0, sizeof(colors) / sizeof(SDL_Color));
	}
	SDL_WM_SetCaption("GLICTMemory", "");


    glictFont* sysfont = glictCreateFont("system");
	sysfontpic = SDL_LoadBMP("font.bmp");
	SDL_SetColorKey(sysfontpic, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(SDL_GetVideoInfo()->vfmt, 0xFF, 0, 0xFF)); // magneta is transparent

    sysfont->SetFontParam(sysfontpic);
    sysfont->SetRenderFunc(SDLFontDraw);
    sysfont->SetSizeFunc(SDLFontSize);


	//SDL_SetColors(sysfontpic, colors, 0, sizeof(colors) / sizeof(SDL_Color));

    MainWidgets();


	glictGlobals.w = width;
	glictGlobals.h = height;
	desktop.SetWidth(width);
	desktop.SetHeight(height);

	reshape(width,height);

	glictGlobals.paintrectCallback = SDLRectDraw;
	glictGlobals.paintrectlinesCallback = SDLRectLDraw;
	glictGlobals.enableGlTranslate = false;
	SDL_Event event;

	// blanks the screen..
	glictColor c(0,0,0,1);
	SDLRectDraw(0,640,0,480,c);

	desktop.Paint();
	SDL_Flip(screen);

	while(running){
		while(SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_KEYDOWN:

					break;
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEMOTION:{
					glictPos p = {event.button.x, event.button.y};
					desktop.CastEvent(event.type == SDL_MOUSEBUTTONUP ? GLICT_MOUSEUP : (event.type == SDL_MOUSEBUTTONDOWN ? GLICT_MOUSEDOWN : GLICT_MOUSEMOVE), &p,0);

					// blanks the screen..
					glictColor c(0,0,0,1);
					SDLRectDraw(0,640,0,480,c);

					desktop.Paint();
					SDL_Flip(screen);

					break;
				}

				case SDL_QUIT:
					running = false;
					break;
				default:
					break;
			}
		}
	}


	SDL_FreeSurface(screen);
	SDL_FreeSurface(sysfontpic);
	glictDeleteFont("system");
	SDL_Quit();

	return 0;
}
