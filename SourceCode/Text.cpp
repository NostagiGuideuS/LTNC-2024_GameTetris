#include "Text.h"

Text::Text()
{
	fontColor = { 255, 255, 255};
}
Text :: ~Text()
{
	SDL_DestroyTexture(Texture);
	TTF_CloseFont(font);
	TTF_Quit();
}

bool Text::LoadText(std::string content, std::string fontName, SDL_Renderer* des)
{
	if (TTF_Init()) std::cout << "Load Text Init failed\n";

	font = TTF_OpenFont(fontName.c_str(), 500);
	if (font == NULL) std::cout << "Cannot open font.ttf\n";

	SDL_Surface* surfaceMessage =
		TTF_RenderText_Solid(font, content.c_str(), fontColor);

	// now you can convert it into a texture
	renderer = des;
	Texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_FreeSurface(surfaceMessage);
	return (Texture != NULL);
}
void Text::Render(SDL_Rect &Rect)
{
	renderRect = Rect;
	SDL_RenderCopy(renderer, Texture, NULL, &Rect);
}

void Text::setColor(unsigned char r, unsigned char g, unsigned char b)
{
	fontColor = { r, g, b };
}

void Text :: LoadMultipleText(std::string fontName, SDL_Renderer* des)
{
	if (TTF_Init()) std::cout << "Load Text Init failed\n";

	renderer = des;

	font = TTF_OpenFont(fontName.c_str(), 500);
	if (font == NULL) std::cout << "Cannot open font.ttf\n";
}
void Text::setText_Color(std::string content, SDL_Rect& Rect, unsigned char r, unsigned char g, unsigned char b)
{
	this->setColor(r, g, b);
	SDL_Surface* surfaceMessage =
		TTF_RenderText_Solid(font, content.c_str(), fontColor);

	Texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_FreeSurface(surfaceMessage);
	Render(Rect);
}

int Text::left() { return renderRect.x; }
int Text::right() { return renderRect.x + renderRect.w; }
int Text::top() { return renderRect.y; }
int Text::dep() { return renderRect.y + renderRect.h; }

void Text:: wider()
{
	if (!wide)
	{
		SDL_RenderFillRect(renderer, &renderRect);
		renderRect.x -= renderRect.w / 4;
		renderRect.y -= renderRect.h / 4;
		renderRect.w *= 1.5;
		renderRect.h *= 1.5;
		this->Render(renderRect);
		SDL_RenderPresent(renderer);
		wide = true;
	}
}

void Text::unWider()
{
	if (wide)
	{
		SDL_RenderFillRect(renderer, &renderRect);
		renderRect.x += renderRect.w / 6;
		renderRect.y += renderRect.h / 6;
		renderRect.w /= 1.5;
		renderRect.h /= 1.5;
		this->Render(renderRect);
		SDL_RenderPresent(renderer);
		wide = false;
	}
}

void Text:: move(SDL_Rect& rect)
{
	clear();
	renderRect = rect;
	this->Render(renderRect);
	SDL_RenderPresent(renderer);
}

void Text::clear()
{
	SDL_RenderFillRect(renderer, &renderRect);
}

number::number()
{
	value = "0";

	rect.x = (g_w + 4) * BlockSize_Output;
	rect.y = g_Height / 2 + 2 * BlockSize_Output;
	rect.w = BlockSize_Output * 2;
	rect.h = BlockSize_Output * 3;

	white = { 255, 255, 255 };
}
number::~number() {
	SDL_DestroyTexture(Texture);
	TTF_CloseFont(font);
	TTF_Quit();
}

void number:: operator+= (int x)
{
	int n = value.size() - 1;
	int remember = 0;
	while ( (x != 0 || remember != 0) && n >= 0)
	{
		value[n] += x % 10 + remember;
		x /= 10;
		remember = 0;
		while (value[n] > '9')
		{
			value[n] -= 10;
			remember++;
		}
		n--;
	}
	while (x != 0 || remember != 0)
	{
		char temp = x % 10 + '0' + remember;
		remember = 0;
		while (temp > '9') {
			temp -= 10;
			remember++;
		}
		value = temp + value;
		x /= 10;
	}
	//std::cout << value << '\n';
	n = value.size();
	rect.w = n * BlockSize_Output * 2;

}

void number::LoadText(std::string fontName, SDL_Renderer* des)
{
	value = '0';
	if (TTF_Init()) std::cout << "Load Text Init failed\n";

	font = TTF_OpenFont(fontName.c_str(), 500);
	if (font == NULL) std::cout << "Cannot open font.ttf\n";

	renderer = des;
}

void number::Render()
{
	SDL_SetRenderDrawColor(renderer, 50, 55, 60, 255);
	SDL_RenderFillRect(renderer, &rect);


	SDL_Surface* surfaceMessage =
		TTF_RenderText_Solid(font, value.c_str(), white);

	Texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_FreeSurface(surfaceMessage);

	SDL_RenderCopy(renderer, Texture, NULL, &rect);
}

long long number::result()
{
	long long res = 0;
	int n = value.size();
	for (int i = 0; i < n; i++)
	{
		res = 10 * res + value[i] - '0';
	}
	return res;
}