#pragma once
#include "CommonFunc.h"
#include "BaseObject.h"
#include "Shape.h"


class BoardBase : public BaseObject
{
private:
	int* blockValue;

public:
	int width;
	int height;
	int** block;
	BoardBase();
	~BoardBase();

	void createBoardBase(std::string path, SDL_Renderer* screen);

	void LocalRender(location pos, SDL_Renderer* des, const SDL_Rect* clip);
	void change(int k, SDL_Renderer* des);
	void eraseLine(int line);
	void drawErase (int line, int width_x);
	void setupErase();
	void setupBackgroundColor();
	bool GameOver();
	void clear();
	void Free();
	void createNew();

	friend bool stopLayer_whenDown(Shape&);
	friend bool stopLayer_whenRight(Shape&);
	friend bool stopLayer_whenLeft(Shape&);
};