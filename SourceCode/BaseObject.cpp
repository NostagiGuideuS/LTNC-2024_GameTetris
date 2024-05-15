#include "BaseObject.h"
//#include "stdafx.h"

// Constructor + Destructor
BaseObject:: BaseObject()
{
	p_object = NULL;
	rect = { 0, 0, 0, 0 };
}
BaseObject:: ~BaseObject()
{
	Free();
}


bool BaseObject:: LoadImage(std::string path, SDL_Renderer* screen)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* load_surface = IMG_Load(path.c_str());
	if (load_surface != NULL)
	{
		newTexture = SDL_CreateTextureFromSurface(screen, load_surface);
		if (newTexture != NULL)
		{
			rect.w = load_surface->w;
			rect.h = load_surface->h;
			// rect dang luu kich thuoc tam anh
		}
	}

	p_object = newTexture;
	
	return p_object != NULL; // true when p_object == newTexture
}

void BaseObject:: Render (SDL_Renderer* des, const SDL_Rect* clip /* = NULL*/)
{
	renderer = des;
	SDL_Rect renderSquad = { rect.x, rect.y, rect.w, rect.h };
	SDL_RenderCopy (des, p_object, clip, &renderSquad);
}

void BaseObject::Free()
{
	if (p_object != NULL)
	{
		p_object = NULL;
		rect.w = 0;
		rect.h = 0;
	}
}

location::location()
{
	x = 0; y = 0;
}
location::location(int x_, int y_)
{
	x = x_;
	y = y_;
}
location::~location() { ; }
location location:: operator- (location r)
{
	location res;
	res.x = x - r.x;
	res.y = y - r.y;
	return res;
}
bool location::operator== (location r)
{
	if (x == r.x && y == r.y) return true;
	return false;
}

bool location::operator!= (location r)
{
	if (x == r.x && y == r.y) return false;
	return true;
}

location location:: operator+ (location r)
{
	location res{ x + r.x, y + r.y };
	return res;
}

void location :: operator= (location r)
{
	x = r.x;
	y = r.y;
}
