#include "SDL.h"
#include <iostream>
#include "SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
	//initalize to nullptr to avoid pointer issues
	SDL_Texture *texture = nullptr;
	//load image
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	// if we are loaded, convert to texture and return it
	if (loadedImage != nullptr) {
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//make sure converting went all good
		if (texture == nullptr) {
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	//setup dest rect to where we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//query texture to get wight and height
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

int main(int argc, char *argv[]) {

	// -----------------------------------------------------------------
	// setup window/renderer
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		SDL_DestroyRenderer(ren);
		SDL_Quit();
		return 1;
	}
	// -----------------------------------------------------------------

	SDL_Texture *background = loadTexture("safeway.bmp", ren);
	SDL_Texture *image = loadTexture("safeway.bmp", ren);
	if (background == nullptr || image == nullptr) {
		SDL_DestroyTexture(background);
		SDL_DestroyTexture(image);
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	bool closing = false;
	while (!closing) {
		SDL_RenderClear(ren);

		int bW, bH;
		SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
		renderTexture(background, ren, 0, 0);
		renderTexture(background, ren, bW, 0);
		renderTexture(background, ren, 0, bH);
		renderTexture(background, ren, bW, bH);

		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;
		renderTexture(image, ren, x, y);
		SDL_RenderPresent(ren);
		SDL_Delay(5000);

		closing = true;
	}

	

	// clean up time yayyy
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}