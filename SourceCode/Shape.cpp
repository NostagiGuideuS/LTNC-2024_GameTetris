#include "Shape.h"



FrameSize FrameSize :: operator* (int k)
{
	FrameSize res;
	res.x = x * k;
	res.y = y * k;
	res.w = w;
	res.h = res.h;
	return res;
}
void FrameSize :: operator*= (int k) {
	x *= k;
	y *= k;
}
void FrameSize :: operator= (FrameSize sample)
{
	x = sample.x;
	y = sample.y;
	h = sample.h;
	w = sample.w;
}
FrameSize::FrameSize() { ; }
FrameSize :: FrameSize(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}
FrameSize :: ~FrameSize() { ; }



Shape::Shape()
{
	empty = NULL;
	bottom = NULL;
	frame = NULL;
	pos.x = g_w / 2 - 2;
	pos.y = 0;
}
Shape::~Shape()
{
	Free();
	delete[] frame;
	for (int i = 0; i < n_frame; i++)
	{
		delete[] bottom[i];
		if (e != 0)delete[] empty[i];
	}
	delete[] bottom;
	delete[] empty;
}

void Shape::createFrame(int status_, FrameSize *sample, int k, location **mt, int e_, std::string path, SDL_Renderer* screen)
{
	renderer = screen;
	status = status_ - 1;

	n_frame = k;
	frame = new FrameSize[k];
	for (int i = 0; i < n_frame; i++)
	{
		frame[i] = sample[i];
	}
	// Copy frame information

	e = e_;
	createEmpty(mt);
	createBottom();

	for (int i = 0; i < n_frame; i++)
	{
		frame[i] *= BlockSize_Input;
	}
	// Hieu chinh

	if (!BaseObject::LoadImage(path, screen)) std::cout << "Load Layer " << status+1 << " image failed\n";
	currentFrame = 0;
}

void Shape :: createFrame(int status_, FrameSize* sample, int k, std::string path, SDL_Renderer* screen)
{
	renderer = screen;
	status = status_ - 1;

	n_frame = k;
	frame = new FrameSize[k];
	for (int i = 0; i < n_frame; i++)
	{
		frame[i] = sample[i];
	}
	// copy frame information

	e = 0;
	createBottom();

	for (int i = 0; i < n_frame; i++)
	{
		frame[i] *= BlockSize_Input;
	}
	// Hieu chinh

	if (!BaseObject::LoadImage(path, screen)) std::cout << "Load Layer " << status+1 << " image failed\n";
	currentFrame = 0;
}

void Shape:: createBottom()
{
	bottom = new location * [n_frame];
	location temp{ 0, 1 };
	for (int i = 0; i < n_frame; i++)
	{
		bottom[i] = new location[frame[i].w];
		for (int j = 0; j < frame[i].w; j++)
		{
			bottom[i][j] = { j, frame[i].h };
			if (e != 0)
			{
				while (true)
				{
					for (int k = 0; k < e; k++)
					{
						if (bottom[i][j] - temp == empty[i][k])
						{
							bottom[i][j].y -= 1;
							goto again;
						}
					}
					break; // skip here
				again:
					{; }
				}
			}
			bottom[i][j].y--;
		}
	}
}

void Shape:: createEmpty(location ** mt)
{
	empty = new location * [n_frame];
	for (int i = 0; i < n_frame; i++)
	{
		empty[i] = new location[e];
		for (int j = 0; j < e; j++)
		{
			empty[i][j] = mt[i][j];
		}
	}
}

void Shape::Show()
{
	// Render
	SDL_Rect copyFrame = { frame[currentFrame].x, frame[currentFrame].y, frame[currentFrame].w * BlockSize_Input, frame[currentFrame].h * BlockSize_Input };
	
	SDL_Rect renderFrame = { (pos.x + 1) * BlockSize_Output, (pos.y + 1) * BlockSize_Output, frame[currentFrame].w * BlockSize_Output, frame[currentFrame].h * BlockSize_Output };
	
	SDL_RenderCopy(renderer, p_object, &copyFrame , &renderFrame);
}

void Shape::nextFrame()
{
	currentFrame = next();
}
void Shape::prevFrame()
{
	currentFrame = prev();
}

void Shape::move(char m)
{
	switch (m)
	{
	case 'l': if (pos.x > 0) pos.x -= 1;
		break;
	case 'r': if ( pos.x < g_w - frame[currentFrame].w) pos.x += 1;
		break;
	case 'd': pos.y += 1;
		break;
	case 'u': pos.y -= 1;
	}
}

int Shape :: currentWidth() { return frame[currentFrame].w; }
int Shape :: currentHeight() { return frame[currentFrame].h; }
int Shape :: prev() { return (currentFrame == 0) ? n_frame - 1 : currentFrame - 1; }
int Shape :: next() { return (currentFrame == n_frame - 1) ? 0 : currentFrame + 1; }


bool Shape::isEmpty(location pos)
{
	if (empty == NULL) return false;
	for (int i = 0; i < e; i++)
	{
		if (empty[currentFrame][i] == pos) return true;
	}
	return false;
}

void Shape::spawn()
{
	currentFrame = 0;
	pos.x = g_w / 2 - 2;
	pos.y = 0;
	//currentFrame = 0;
}

location Shape::aBottom(int k)
{
	location res = pos + bottom[currentFrame][k];
	return res;
}

void Shape::Shadow_need_Host(Shape* Layer, std::string path)
{
	status = -2;
    n_frame = Layer->n_frame;
	currentFrame = Layer->currentFrame;

	frame = new FrameSize[n_frame];
	bottom = new location * [n_frame];
	for (int i = 0; i < n_frame; i++)
	{
		frame[i] = Layer->frame[i];
		bottom[i] = new location[frame[i].w];
		for (int j = 0; j < frame[i].w; j++)
		{
			bottom[i][j] = Layer->bottom[i][j];
		}
	}
	renderer = Layer->renderer;
	if (!BaseObject::LoadImage(path, renderer))
		std::cout << "Load Shadow of " << Layer->status + 1 << " failed\n";
	// Shadow no need empty
}

void Shape::showOnBench()
{
	// Erase the old
	SDL_SetRenderDrawColor(renderer, 50, 55, 60, 255);
	SDL_Rect whatLeft{ (g_w + 6) * BlockSize_Output, 6 * BlockSize_Output, 4 * BlockSize_Output, 4 * BlockSize_Output };
	SDL_RenderFillRect(renderer, &whatLeft);

	// draw the new
	SDL_Rect copyFrame = { frame[0].x, frame[0].y, frame[0].w * BlockSize_Input, frame[0].h * BlockSize_Input };

	SDL_Rect renderFrame = { (g_w + 6) * BlockSize_Output, 6 * BlockSize_Output, frame[0].w * BlockSize_Output, frame[0].h * BlockSize_Output };

	SDL_RenderCopy(renderer, p_object, &copyFrame, &renderFrame);
}