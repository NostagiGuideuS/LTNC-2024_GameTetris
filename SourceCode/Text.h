#pragma once

#include "CommonFunc.h"

class Text
{
private:
	SDL_Color fontColor;
	TTF_Font* font;
	SDL_Texture* Texture;
	SDL_Renderer* renderer;
	bool wide;

public:
	SDL_Rect renderRect;
	Text();
	~Text();

	bool LoadText (std::string content, std::string fontName, SDL_Renderer* des);
	void Render(SDL_Rect &Rect);
	void setColor(unsigned char r, unsigned char g, unsigned char b);

	void LoadMultipleText(std::string fontname, SDL_Renderer* des);
	void setText_Color(std::string content, SDL_Rect& Rect, unsigned char r, unsigned char g, unsigned char b);
	void wider();
	void unWider();
	int left();
	int right();
	int top();
	int dep();
	void move(SDL_Rect& rect);
	void clear();
};

class number
{
private:
	SDL_Rect rect;
	TTF_Font* font;
	SDL_Color white;
	SDL_Texture* Texture;
	SDL_Renderer* renderer;

public:
	std::string value;
	number();
	~number();

	void operator+= (int x);
	void LoadText(std::string fontName, SDL_Renderer* des);
	void Render();
	long long result();

};