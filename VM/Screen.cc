//#include <SDL_image.h>
#include <cassert>
#include "Screen.h"
#include "Logger.h"
//#include "Image.h"
//#include "SDLException.h"

namespace vm { namespace sdl {

//void Screen::aquire(const std::string& title, size_t width, size_t height)
//{
//	if(window_) return;
//
//	if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();
//
//	if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == -1) throw SDLException(IMG_GetError());
//
//	window_ = SDL_CreateWindow(title.c_str(), 
//		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
//		width, height, 
//		SDL_WINDOW_SHOWN);
//
//	if(!window_) throw SDLException();
//
//	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_TARGETTEXTURE);
//
//	if(!renderer_) throw SDLException();
//
//	SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
//	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0xff);
//
//	Image::init(renderer_);
//}

//void Screen::release(void)
//{
//	if(window_)
//	{
//		SDL_DestroyRenderer(renderer_);
//		SDL_DestroyWindow(window_);
//		IMG_Quit();
//		SDL_Quit();
//		window_ = NULL;
//		renderer_ = NULL;
//	}
//
//	quit_ = quitFn_t();
//}

//void Screen::update(void)
//{
//	SDL_Event e;
//
//	while(SDL_PollEvent(&e))
//	{
//		switch(e.type)
//		{
//			case SDL_QUIT:
//				if(static_cast<bool>(quit_)) quit_(); else exit(0);
//				break;
//			case SDL_KEYDOWN:
//				buffer_.push_back(Key(e.key.keysym.sym));
//				LOG("Key press! [0x%04lx]", e.key.keysym.sym);
//				break;
//			case SDL_KEYUP:
//				break;
//		}
//	}
//}

//void Screen::refresh(void)
//{
//	if(!window_) throw SDLException();
//	SDL_RenderPresent(renderer_);
//	SDL_UpdateWindowSurface(window_);
//}

//void Screen::clear(void)
//{
//	SDL_RenderClear(renderer_);
//}

//void Screen::onQuit(quitFn_t q)
//{
//	quit_ = q;
//}

void Screen::pressKey(const Key& k)
{
	buffer_.push_back(k);
	LOG("Key press! [0x%04lx]", static_cast<size_t>(k));
}

bool Screen::keyPressed(void) const
{
	return !buffer_.empty();
}

Screen::Key Screen::keyPoll(void)
{
	assert(keyPressed());
	Key k(buffer_.front());
	buffer_.pop_back();
	return k;
}

// # ---------------------------------------------------------------------------

Screen& Screen::instance(void)
{
	static Screen s;

	return s;
}

// # ===========================================================================

Screen::Screen(void)
{
}

Screen::~Screen(void)
{
//	release();
}

}}

