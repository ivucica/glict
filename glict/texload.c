
#include <stdio.h>


#ifndef WIN32 // unices
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>

static int power_of_two(int input)
{
        int value = 1;

        while ( value < input ) {
                value <<= 1;
        }
        return value;
}


GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord)
{
        GLuint texture;
        int w, h;
        SDL_Surface *image;
        SDL_Rect area;
        Uint32 saved_flags;
        Uint8  saved_alpha;

        /* Use the surface width and height expanded to powers of 2 */
        w = power_of_two(surface->w);
        h = power_of_two(surface->h);
        texcoord[0] = 0.0f;                     /* Min X */
        texcoord[1] = 0.0f;                     /* Min Y */
        texcoord[2] = (GLfloat)surface->w / w;  /* Max X */
        texcoord[3] = (GLfloat)surface->h / h;  /* Max Y */

        image = SDL_CreateRGBSurface(
                        SDL_SWSURFACE,
                        w, h,
                        32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
                        0x000000FF,
                        0x0000FF00,
                        0x00FF0000,
                        0xFF000000
#else
                        0xFF000000,
                        0x00FF0000,
                        0x0000FF00,
                        0x000000FF
#endif
                       );
        if ( image == NULL ) {
                return 0;
        }


        /* Save the alpha blending attributes */
        saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
        saved_alpha = surface->format->alpha;
        if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
                SDL_SetAlpha(surface, 0, 0);
        }

        /* Copy the surface into the GL texture image */
        area.x = 0;
        area.y = 0;
        area.w = surface->w;
        area.h = surface->h;
        SDL_BlitSurface(surface, &area, image, &area);

        /* Restore the alpha blending attributes */
        if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
                SDL_SetAlpha(surface, saved_flags, saved_alpha);
        }



        /* Create an OpenGL texture for the image */
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     w, h,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     image->pixels);
        SDL_FreeSurface(image); /* No longer needed */

        return texture;
}




int BitmapLoad(const char* Filename) {
	int w, h;
	SDL_Surface *image;
	GLfloat texcoord[4];

	/* Load the image (could use SDL_image library here) */
	image = SDL_LoadBMP(Filename);
	if ( image == NULL ) {
			return 0;
	}
	w = image->w;
	h = image->h;

	/* Convert the image into an OpenGL texture */
	return SDL_GL_LoadTexture(image, texcoord);

}
#else
#include <GL/glaux.h>
int BitmapLoad(const char* Filename) // 0 if success, else fail
    {
      GLuint texid;
      AUX_RGBImageRec *Bitmap = NULL;     // Pointer To AUX_RGBImageRec
      FILE *FileHandle = NULL;            // File Handle To Use
      char *pod;
      int i, j;

      printf("Ucitavanje grafike iz datoteke %s...\n", Filename);
      FileHandle = fopen(Filename,"rb");  // Check To See If The File Exists

      if(FileHandle != NULL)              // If File Exists
      {
        Bitmap = auxDIBImageLoad(Filename);  // Use glaux.h To Load Image
      } else                              // Else If Error Loading File
      {
        //fclose(FileHandle);               // Close File Handle
        return(1);                    // Return FALSE
      }

      fclose(FileHandle);                 // Close File Handle

      glGenTextures(1,                    // # Of Texture Names/ID To Create
                    &texid);  // Generated Texture ID

      glBindTexture(GL_TEXTURE_2D,        // Bind A 2D Texture
                    texid); // Bind Using This Texture ID

      pod = (char*)malloc(Bitmap->sizeX * Bitmap->sizeY * 4);

      for (i = 0 ; i < Bitmap->sizeX ; i++) {
          for (j = 0 ; j < Bitmap->sizeY ; j++) {
              if (Bitmap->data[((Bitmap->sizeY-j-1)*Bitmap->sizeX + i)*3] == 255 &&
                  Bitmap->data[((Bitmap->sizeY-j-1)*Bitmap->sizeX + i)*3 + 2] == 255 &&
                  Bitmap->data[((Bitmap->sizeY-j-1)*Bitmap->sizeX + i)*3 + 1] == 0) {
                      pod[(j*Bitmap->sizeX + i)*4 + 0] = 0;
                      pod[(j*Bitmap->sizeX + i)*4 + 1] = 0;
                      pod[(j*Bitmap->sizeX + i)*4 + 2] = 0;
                      pod[(j*Bitmap->sizeX + i)*4 + 3] = 0;
                  } else {
                      pod[(j*Bitmap->sizeX + i)*4 + 0] = Bitmap->data[((Bitmap->sizeY-j-1)*Bitmap->sizeX + i)*3];
                      pod[(j*Bitmap->sizeX + i)*4 + 1] = Bitmap->data[((Bitmap->sizeY-j-1)*Bitmap->sizeX + i)*3+1];
                      pod[(j*Bitmap->sizeX + i)*4 + 2] = Bitmap->data[((Bitmap->sizeY-j-1)*Bitmap->sizeX + i)*3+2];
                      pod[(j*Bitmap->sizeX + i)*4 + 3] = 255;
                  }
          }
      }


      gluBuild2DMipmaps(GL_TEXTURE_2D,    // Texture Is A 2D Texture
                        4,                // 3 Means "RGB" Texture...=)
                        Bitmap->sizeX,    // Bitmap's Width
                        Bitmap->sizeY,    // Bitmap's Height
                        GL_RGBA,           // Bitmap Is RGB Format
                        GL_UNSIGNED_BYTE, // Type Of Data Each Pixel Is Stored
                        pod);    // Actual Bitmap Data


      glTexParameteri(GL_TEXTURE_2D,          // Set 2D Texture Parameter
                      GL_TEXTURE_MIN_FILTER,  // Select Minification Filter Type
                      GL_NEAREST);            // GL_NEAREST (Blocky & Fastest)

      glTexParameteri(GL_TEXTURE_2D,          // Set 2D Texture Parameter
                      GL_TEXTURE_MAG_FILTER,  // Select Magnification Filter Type
                      GL_NEAREST);	      // GL_LINEAR (Smoothest & Slowest)

      if (Bitmap)
      {
        if (Bitmap->data)                 // If There Is Texture Data
        {
           free(Bitmap->data);            // Free Texture Data
        }
        free(Bitmap);                     // Free Structure
      }
      free(pod);
//      printf("Teksturni id: %d\n", texid);
      return(texid);                       // Return TRUE
}

#endif
