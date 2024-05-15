#pragma once
#include "CommonFunc.h"

class BaseObject
{
public:
	BaseObject();
	~BaseObject();

	void setRect(int x, int y) { rect.x = x; rect.y = y; }
	SDL_Rect getRect() const { return rect; }
	SDL_Texture* getObject() const { return p_object; }

	virtual bool LoadImage (std::string path, SDL_Renderer* screen);
	void Render (SDL_Renderer* des, const SDL_Rect* clip /*= NULL*/);
	virtual void Free();


protected: // for Nested class
	SDL_Texture *p_object;
	SDL_Rect rect;
	SDL_Renderer* renderer;
};

class location
{
public:
	int x;
	int y;

	location();
	location(int x_, int y_);
	~location();

	location operator- (location r);
	bool operator== (location r);
	bool operator!= (location r);
	location operator+ (location r);
	void operator= (location r);
};
