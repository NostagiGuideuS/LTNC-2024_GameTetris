#include "BoardBase.h"

BoardBase::BoardBase()
{
	width = g_w;
	height = g_h;
	createNew();
}
BoardBase :: ~BoardBase()
{
	Free();
}

void BoardBase::Free()
{
	delete[] blockValue;
	delete[] block;
}

void BoardBase::createNew()
{
	blockValue = new int[width * (1+height)];
	block = new int* [width];
	for (int i = 0; i < width; i++) block[i] = blockValue + i * (height + 1);
	for (int i = 0; i < width * (height + 1); i++) blockValue[i] = -1;
}

void BoardBase :: createBoardBase(std::string path, SDL_Renderer* screen)
{
	if (!BaseObject::LoadImage(path, screen)) std::cout << "Load Board image failed\n";
}

void BoardBase :: LocalRender(location pos, SDL_Renderer* des, const SDL_Rect* clip)
{
	renderer = des;
	if (block[pos.x][pos.y] != -1)
	{
		SDL_Rect copyFrame = { BlockSize_Input * block[pos.x][pos.y] , 0, BlockSize_Input, BlockSize_Input };

		SDL_Rect renderFrame = { (pos.x+1) * BlockSize_Output, (pos.y+1) * BlockSize_Output, BlockSize_Output, BlockSize_Output };

		SDL_RenderCopy(des, p_object, &copyFrame, &renderFrame);
	}
}

void BoardBase::change(int k, SDL_Renderer *des)
{ // useless // Actually i use this for test when making base
	blockValue[width*height - 2] = k-1;
	blockValue[width * height - 1] = k - 1;
	LocalRender({ width - 1, height - 2 }, des, NULL);
	LocalRender({ width - 1, height - 1 }, des, NULL);
}

void BoardBase::eraseLine(int line)
{
	int target;
	SDL_Rect currentBlock;
	for (int i = 0; i < width; i++)
	{
		target = line;
		while (target > 0)
		{
			block[i][target] = block[i][target - 1];
			if (block[i][target] == -1) // Because when block empty, it's not draw anything at all, so we need to FillRect
			{
				currentBlock = {(i+1)*BlockSize_Output, (target+1) * BlockSize_Output, BlockSize_Output, BlockSize_Output};
				SDL_RenderFillRect(renderer, &currentBlock);
			}
			else LocalRender({ i, target }, renderer, NULL);
			target--;
		}
	}
}

void BoardBase::drawErase(int line, int width_x)
{
	SDL_Rect currentBlock{ (width_x + 1) * BlockSize_Output, (line + 1) * BlockSize_Output, BlockSize_Output, BlockSize_Output };
	SDL_RenderFillRect(renderer, &currentBlock);
}

void BoardBase::setupErase()
{
	SDL_SetRenderDrawColor(renderer, 50, 150, 200, 250);
}
void BoardBase::setupBackgroundColor()
{
	SDL_SetRenderDrawColor(renderer, playGroundColor_R, playGroundColor_G, playGroundColor_B, 255);
}

bool BoardBase:: GameOver()
{
	for (int i = 0; i < width; i++)
		if (block[i][0] != -1) return true;
	return false;
}

void BoardBase:: clear()
{
	for (int i = 0; i < width * (height + 1); i++) blockValue[i] = -1;
}
