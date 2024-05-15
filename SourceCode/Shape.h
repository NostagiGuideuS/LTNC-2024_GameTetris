#pragma once

#include "CommonFunc.h"
#include "BaseObject.h"


class FrameSize
{
public:
	int x; int y;
	int w; int h;

	FrameSize();
	FrameSize(int _x, int _y, int _w, int _h);
	~FrameSize();

	FrameSize operator* (int k);
	void operator*= (int k);
	void operator= (FrameSize sample);
	
};

class Shape : public BaseObject
{
public:
	int status;
	location pos;
	Shape();
	~Shape();

	int currentWidth();
	int currentHeight();
	void createFrame(int status_,  FrameSize* sample, int k, location** mt, int e_, std::string path, SDL_Renderer* screen);
	void createFrame(int status_, FrameSize* sample, int k, std::string path, SDL_Renderer* screen); // only if there no empty, e_ = 0
	void Show();
	void nextFrame();
	void prevFrame();
	void move(char m);
	void spawn();
	void Shadow_need_Host (Shape* Layer, std::string path);
	void showOnBench();


	friend void add2Base (Shape& );
	friend bool stopLayer_whenDown (Shape& );
	friend bool stopLayer_whenRight(Shape&);
	friend bool stopLayer_whenLeft(Shape&);
	friend void localReDraw(Shape& );
	friend void get1Line();
	friend bool CannotChange();
	friend void foreShadow();

protected:   
	int n_frame;
	int currentFrame;
	FrameSize* frame;
	location **bottom;
	int e;
	location **empty;

	int prev();
	int next();

	bool isEmpty( location pos);
	location aBottom(int k);
	void createBottom();
	void createEmpty(location** mt);
};

