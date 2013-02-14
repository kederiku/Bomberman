#include "image.h"

SDL_Surface*	load_img(const char* file)
{
	Uint32		color;
	SDL_Surface*	img;
	SDL_Surface*	img_opti;

	img_opti = NULL;
	img = NULL;
	color = 0;
	img = SDL_LoadBMP(file);
	if (img == NULL)
		return NULL;
	img_opti = SDL_DisplayFormat(img);
	SDL_FreeSurface(img);
	if (img_opti == NULL)
		return NULL;
	color = SDL_MapRGB(img_opti->format, 0, 255, 0);
	SDL_SetColorKey(img_opti, SDL_SRCCOLORKEY | SDL_RLEACCEL, color);
	return img_opti;
}
