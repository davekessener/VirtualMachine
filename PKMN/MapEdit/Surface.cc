#include "Surface.h"

SDL_Renderer *Surface::renderer = NULL;
SDLImage *Surface::border_art = NULL;

Surface::Surface(int _x, int _y, int _w, int _h)
	: client(renderer, _w - 16, _h - 32), sysButtons(0), b_lock(-1),
	  title(), x(_x), y(_y), width(_w), height(_h), scroll_x(0), scroll_y(0),
	  scroll_down(renderer, _w - 16, 16), scroll_right(renderer, 16, _h - 32)
{
	title_r.x = 0;
	title_r.y = 0;
	title_r.w = width;
	title_r.h = 16;

	LOG("Created surface @(%d|%d) %d x %d", x, y, width, height);

	createScrolls();

	buttons.push_back(Button(16, height - 16, width - 48, 16, [this](button_t b, int _x, int _y)
		{
			_x = _x < 16 ? 0 : _x > width - 48 - 16 ? width - 48 - 32 : _x - 16;
			scroll_x = (getScrollSize().first * _x + (width - 48 - 32) / 2) / (width - 48 - 32);
		}));
	buttons.push_back(Button(width - 16, 32, 16, height - 64, [this](button_t b, int _x, int _y)
		{
			_y = _y < 16 ? 0 : _y > height - 64 - 16 ? height - 64 - 32 : _y - 16;
			scroll_y = (getScrollSize().second * _y + (height - 64 - 32) / 2) / (height - 64 - 32);
		}));
	buttons.push_back(Button(0, 16, width - 16, height - 32, [this](button_t b, int _x, int _y)
		{
			onClick(b, _x + scroll_x, _y + scroll_y);
		}));
}

Surface::~Surface(void)
{
}

void Surface::render(void)
{
	SDL_Rect ro, rt;

	client.erase();

	draw(client, scroll_x, scroll_y);

	ro.x = ro.y = 0;
	ro.w = client.width();
	ro.h = client.height();

	rt.x = x;
	rt.y = y + 16;
	rt.w = ro.w;
	rt.h = ro.h;

	SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(client), &ro, &rt);

	if(title.opened()) 
	{
		rt = ro = title_r;
		ro.x = ro.y = 0;

		SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(title), &ro, &rt);
	}

	rt = ro = scroll_down_r;
	ro.x = ro.y = 0;

	SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(scroll_down), &ro, &rt);

	rt = ro = scroll_right_r;
	ro.x = ro.y = 0;

	SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(scroll_right), &ro, &rt);

#define max(i,j) ((i)>(j)?(i):(j))
	std::pair<int, int> scroll = getScrollSize();

	if(scroll.first)
	{
		int dx = (width - 80) * scroll_x / scroll.first + 16;

		ro.x = 64;
		ro.y = 0;
		rt.x = x + 16 + max(0, dx - 16);
		rt.y = y + height - 16;
		ro.w = ro.h = rt.w = rt.h = 16;

		SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(*border_art), &ro, &rt);

		ro.x += 16;
		rt.x += 16;

		SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(*border_art), &ro, &rt);
	}

	if(scroll.second)
	{
		int dy = (height - 96) * scroll_y / scroll.second + 16;

		ro.x = 96;
		ro.y = 0;
		rt.y = y + 32 + max(0, dy - 16);
		rt.x = x + width - 16;
		ro.w = ro.h = rt.w = rt.h = 16;

		SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(*border_art), &ro, &rt);

		ro.x += 16;
		rt.y += 16;

		SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(*border_art), &ro, &rt);
	}
#undef max

	if(sysButtons)
	{
		ro.x = 160;
		ro.y = 0;
		rt.x = x + width;
		rt.y = y;
		ro.w = ro.h = rt.w = rt.h = 16;

		for(int i = 0 ; i < 32 ; ++i)
		{
			ro.x = i * 16;
			rt.x -= 16;

			SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(*border_art), &ro, &rt);
		}
	}
}

void Surface::click(button_t btn, int _x, int _y)
{
	_x -= x;
	_y -= y;

	if(b_lock >= 0)
	{
		buttons.at(b_lock)(btn, _x, _y);
	}
	else throw SDLException("ERR: clicked without lock! @(%d|%d)", _x + x, _y + y);
}

bool Surface::lock(int _x, int _y)
{
	_x -= x;
	_y -= y;

	for(int i = 0 ; i < buttons.size() ; ++i)
	{
		if(buttons.at(i).hit(_x, _y))
		{
			b_lock = i;
			return true;
		}
	}

	return false;
}

void Surface::init(SDL_Renderer *r)
{
	renderer = r;
	border_art = new SDLImage(r, "border.png");
}

void Surface::freeResources(void)
{
	delete border_art;

	border_art = NULL;

	LOG("Freed resources [DONE]");
}

void Surface::addButton(int _x, int _y, int _w, int _h, Button::button_fn_t _fn)
{
	buttons.push_back(Button(_x, _y + 16, _w, _h, _fn));
}

void Surface::addQuitButton(void)
{
	int _x = width - 16;

	int i;
	for(i = 0 ; sysButtons & (1 << i) ; ++i)
	{
		_x -= 16;
	}
	sysButtons |= SB_QUIT;

	buttons.push_back(Button(_x, 0, 16, 16, 
		[this](button_t b, int _x, int _y) { if(b & Key::LEFT) quit(); }));
}

void Surface::quit(void)
{
	SDL_Event e;

	e.type = SDL_QUIT;

	SDL_PushEvent(&e);
}

void Surface::createScrolls(void)
{
	LOG("Creating scrolls [...]");

	SDL_Rect ro, rt;

	ro.x = ro.y = rt.x = rt.y = 0;
	ro.w = ro.h = rt.w = rt.h = 16;

	// # -------------------------------------------------------------------------

	scroll_down_r.x = x;
	scroll_down_r.y = y + height - 16;
	scroll_down_r.w = width - 16;
	scroll_down_r.h = 16;

	scroll_down.startBlit();

	ro.x = 48;
	scroll_down.blit(*border_art, ro, rt);
	ro.x = 128;

	for(int i = 16 ; i < scroll_down_r.w - 16 ; i += 16)
	{
		rt.x = i;
		scroll_down.blit(*border_art, ro, rt);
	}

	rt.x = scroll_down_r.w - 16;
	ro.x = 32;
	scroll_down.blit(*border_art, ro, rt);

	scroll_down.endBlit();

	// # -------------------------------------------------------------------------

	rt.w = ro.w = 16;
	rt.x = rt.y = 0;

	scroll_right_r.x = x + width - 16;
	scroll_right_r.y = y + 16;
	scroll_right_r.w = 16;
	scroll_right_r.h = height - 32;

	scroll_right.startBlit();

	ro.x = 16;
	scroll_right.blit(*border_art, ro, rt);
	ro.x = 144;

	for(int i = 16 ; i < scroll_right_r.h - 16 ; i += 16)
	{
		rt.y = i;
		scroll_right.blit(*border_art, ro, rt);
	}

	rt.y = scroll_right_r.h - 16;
	ro.x = 0;
	scroll_right.blit(*border_art, ro, rt);

	scroll_right.endBlit();
}

