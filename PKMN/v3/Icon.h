#ifndef DAV_PKMN_V3_SCREEN_ICON_H
#define DAV_PKMN_V3_SCREEN_ICON_H

#include "Surface.h"

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			class Icon : public Surface
			{
				public:
				typedef std::shared_ptr<Icon> Icon_ptr;

				public:
					Icon(const std::string& s = "") : path_(s) { img_.id = 0; }
					void load(const std::string&);
					const std::string& path( ) const { return path_; }
					uint size( ) const
						{ if(!img_.id) throw std::string("Icon not jet loaded!"); return img_.size; }
				private:
					void i_doPrerender( );
					void i_doRender( );
				private:
					std::string path_;
					text_info img_;
			};

			typedef Icon::Icon_ptr Icon_ptr;
		}
	}
}

#endif

