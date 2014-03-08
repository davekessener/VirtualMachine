#include "Application.h"
#include "Window.h"

class App : public surface::Scrollable
{
	public:
		App(Image *i, int x, int y, int w, int h) : surface::Scrollable(i, x, y, w, h) { img = new Image(w, h); img->gradientFill(0xff0000ff, 0x00ff00ff, 0x0000ffff, 0xffffffff); }
		~App( ) { delete img; }
	protected:
		void draw(Image *i, int dx, int dy)
		{
			i->blit(img, Point(0, 0), Rect(0, 0, W(), H()));
		}
		std::pair<int, int> getScrollRanges( ) { return std::make_pair(10, 0); }
	private:
		Image *img;
};

class Win : public surface::Window<App>
{
	public:
		Win(Image *i) : surface::Window<App>(i, "Application") { }
};

int main(void)
{
	surface::Application<Win> a(0, 24, 0, -24);

	a.run();
	
	return 0;
}

