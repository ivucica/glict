#include <SDL/SDL.h>
#include <string.h>
extern SDL_Surface* screen;
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

