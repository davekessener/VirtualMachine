#ifndef PKMN_EDITOR_SURFACE_STRINGLIST_H
#define PKMN_EDITOR_SURFACE_STRINGLIST_H

#include "../../common.h"
#include "Surface.h"
#include <functional>

namespace editor
{
	namespace surface
	{
		class StringList : public Surface
		{
			typedef std::function<void(const std::string&)> act_fn;
			typedef std::vector<std::string> vec_t;

			public:
				StringList(act_fn a) : activate_(a) { }
				template<typename I>
					void load(I i1, I i2) { vec_t(i1, i2).swap(choices_); i_doInit(); }
			private:
				void i_doInit( );
				void i_doUpdate(int) { }
				void i_doRender( ) const;
				void i_doMouseOver(bool);
				void i_doMouseMove(int, int);
				void i_doMouseDown(MouseButtons, int, int);
				void i_doScroll(int);
			private:
				int idx_, o_;
				vec_t choices_;
				act_fn activate_;
		};
	}
}

#endif

