#ifndef __TEXLOAD_H
#define __TEXLOAD_H


extern "C" {

	#ifndef WIN32
	#warning Texload.c does not support non-Win32 platforms since they don't (officially) support GLAUX.
	#endif

    int BitmapLoad(const char* Filename);
}

#endif
