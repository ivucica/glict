#ifndef __SDLFONT_H
#define __SDLFONT_H

void SDLFontDrawChar(char t, SDL_Surface* img, int x1, int y1);
void SDLFontDraw(const char* txt, const void* font, float x, float y);
float SDLFontSize(const char* txt, const void* font);
#endif
