#include <iostream>
#include <vector>
#include "Manager.h"
//#include <set>
//#include <memory>
//#include <algorithm>
//#include "inc.h"
//#include "gl.h"
//#include "sdl.h"
//#include "image.h"

//#define MXT_WINDOWNAME "DaViewer"
//#define MXT_WIDTH 1240
//#define MXT_HEIGHT 720
//#define MXT_STEP 16
//#define MXT_HIDEIMAGE "hidden.bmp.gz"
//
//typedef unsigned int uint;
//
//void do_init(int, int);
//bool do_update(int);
//void do_keyboard(sdl::Controls, bool);
//void do_mouse(uint, uint, int, int);
//
//Image img_;
//std::vector<std::string> files_;
//std::vector<std::vector<std::string>::const_iterator> random_;
//std::vector<std::string>::const_iterator idx_;
//std::vector<std::vector<std::string>::const_iterator>::const_iterator ridx_;
//std::set<sdl::Controls> modifier_;
//bool running_ = true, scale_ = true, isRand_ = false;
//int dx_ = 0, dy_ = 0;
//
//void incImage();
//void decImage();
//void nextRand();
//void prevRand();
//void loadImage();
//void shuffle();
//void hide();
//
//inline bool pressed(sdl::Controls c) { return modifier_.find(c) != modifier_.cend(); }

//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <iomanip>
//#include <aux>
//#include "dav_header.h"

#include <boost/filesystem.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	using namespace boost::filesystem;

	const std::string inp(args.size() > 1 ? args.at(1) : ".");
	std::string fp(inp.substr(0, inp.find_last_of('/')));
	if(fp.empty()) fp = "/";
	path p(fp);
	std::string f;
	if(inp != "/") f = inp.substr(inp.find_last_of('/') + 1);

	for(directory_iterator i(p), e ; i != e ; ++i)
	{
		std::string fn(i->path().filename().generic_string());
		if(fn.length() >= f.length() && (f.empty() || fn.substr(0, f.length()) == f)) std::cout << fn << std::endl;
	}

	return 0;

//	std::ifstream in("list.txt");
//	dav::dataheader_t h;
//	h.id = dav::DAV_MAGIC;
//	h.hash = dav::hash("password");
//	std::vector<dav::data_t> v;
//	while(!in.eof())
//	{
//		std::string s("");
//		unsigned long hash = 0;
//		in >> s;
//		if(s.length() != 19) continue;
//		dav::data_t t;
//		std::stringstream ss;
//		ss << std::string(s.cbegin(), s.cbegin() + 16);
//		ss << std::setbase(16);
//		ss >> hash;
//		t.name = hash;
//		t.viewed_ms = t.viewed_c = 0;
//		v.push_back(t);
//	}
//	in.close();
//	h.imgcount = v.size();
//	std::ofstream out("test.cfg");
//	out << lib::aux::write_to(h);
//	out << lib::aux::write_to(*v.cbegin(), v.size() * sizeof(dav::data_t));
//	out.close();

	return Manager::instance().run(args);

//	std::vector<std::string>(argv + 1, argv + argc).swap(files_);
//	random_.reserve(files_.size());
//	for(auto i = files_.cbegin() ; i != files_.cend() ; ++i) random_.push_back(i);
//	idx_ = files_.cbegin();
//
//	shuffle();
//
//	sdl::set_init(&do_init);
//	sdl::set_update(&do_update);
//	sdl::set_input(&do_keyboard, &do_mouse);
//
//	sdl::start(MXT_WINDOWNAME, MXT_WIDTH, MXT_HEIGHT);
//
//	return 0;
}

//void do_init(int w, int h)
//{
//	gl::init2d(w, h);
//}
//
//bool do_update(int d)
//{
//	gl::start_draw();
//
//	if(!img_.loaded())
//	{
//		loadImage();
//		img_.create();
////		img_.bind();
//	}
//
//	if(scale_)
//	{
//		img_.center(MXT_WIDTH, MXT_HEIGHT);
//	}
//	else
//	{
//		img_.draw(MXT_WIDTH, MXT_HEIGHT, dx_, dy_);
//	}
//
//	gl::update();
//
//	return running_;
//}
//
//void do_keyboard(sdl::Controls c, bool down)
//{
//	using sdl::Controls;
//
//	if(down)
//	{
//		int w = img_.width(), h = img_.height();
//		int mx = -(MXT_WIDTH - w) / 2;
//		int my = -(MXT_HEIGHT - h) / 2;
//		bool s = pressed(Controls::SHIFT);
//		if(mx < 0) mx = 0;
//		if(my < 0) my = 0;
//
//		switch(c)
//		{
//			case Controls::SPACE:
//			case Controls::X:
//				if(!isRand_) incImage(); else nextRand();
//				break;
//			case Controls::Z:
//				if(!isRand_) decImage(); else prevRand();
//				break;
//			case Controls::L:
//				scale_ = !scale_;
//				break;
//			case Controls::R:
//				if(s)
//					shuffle();
//				else
//					isRand_ = !isRand_;
//				break;
//			case Controls::S:
//				dy_ -= s ? h / 10 : MXT_STEP;
//				if(dy_ < -my) dy_ = -my;
//				break;
//			case Controls::W:
//				dy_ += s ? h / 10 : MXT_STEP;
//				if(dy_ > my) dy_ = my;
//				break;
//			case Controls::D:
//				dx_ -= s ? w / 10 : MXT_STEP;
//				if(dx_ < -mx) dx_ = -mx;
//				break;
//			case Controls::A:
//				dx_ += s ? w / 10 : MXT_STEP;
//				if(dx_ > mx) dx_ = mx;
//				break;
//			case Controls::Q:
//				hide();
//				break;
//			case Controls::ESCAPE:
//				running_ = false;
//				break;
//			default:
//				break;
//		}
//	}
//
//	if(down)
//	{
//		modifier_.insert(c);
//	}
//	else
//	{
//		auto i = modifier_.find(c);
//		if(i != modifier_.end()) modifier_.erase(i);
//	}
//}
//
//void do_mouse(uint x, uint y, int dx, int dy)
//{
//}
//
//void decImage(void)
//{
//	if(idx_ == files_.cbegin()) idx_ = files_.cend();
//	--idx_;
//	if(img_.loaded()) img_.erase();
//}
//
//void incImage(void)
//{
//	++idx_;
//	if(idx_ == files_.cend()) idx_ = files_.cbegin();
//	if(img_.loaded()) img_.erase();
//}
//
//void nextRand(void)
//{
//	++ridx_;
//	if(ridx_ == random_.cend()) ridx_ = random_.cbegin();
//	if(img_.loaded()) img_.erase();
//	idx_ = *ridx_;
//}
//
//void prevRand(void)
//{
//	if(ridx_ == random_.cbegin()) ridx_ = random_.cend();
//	--ridx_;
//	if(img_.loaded()) img_.erase();
//	idx_ = *ridx_;
//}
//
//void loadImage(void)
//{
//	img_.load(*idx_);
//	dx_ = dy_ = 0;
//}
//
//void shuffle(void)
//{
//	std::random_shuffle(random_.begin(), random_.end());
//	ridx_ = std::find(random_.cbegin(), random_.cend(), idx_);
//}
//
//void hide(void)
//{
//	if(img_.loaded()) img_.erase();
//	img_.load(MXT_HIDEIMAGE);
//	dx_ = dy_ = 0;
//	img_.create();
////	img_.bind();
//}

