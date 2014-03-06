#include "Application.h"
#include "Scrollable.h"

class App : public surface::Scrollable
{
	public:
		App(Image *i) : surface::Scrollable(i, 0, 0, i->width(), i->height(), getScrollRanges()) { }
		~App( ) { }
		void draw(int dx, int dy) { }
		std::pair<int, int> getScrollRanges( ) { return std::make_pair(0, 0); }
	private:
};

int main(void)
{
	surface::Application::run<App>(24, 24, 1872, 1032);
	return 0;
}

