#ifndef DAV_PKMN_V3_SCREEN_LABEL_H
#define DAV_PKMN_V3_SCREEN_LABEL_H

#include "Surface.h"

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			class Label : public Surface
			{
				public:
				typedef std::shared_ptr<Label> Label_ptr;

				public:
					Label(const std::string& msg = "")
						: msg_(msg)
						, sx_(MXT_DEFCHARSIZE)
						, sy_(MXT_DEFCHARSIZE * 3 / 2)
						, c_(0x000000)
						, center_(false)
					{ }
					void message(const std::string&);
					const std::string& message( ) const { return msg_; }
					void charsize(uint);
					uint charsize( ) const { return sx_; }
					uint labelWidth( ) { prerender(); return pieces_.size() ? pieces_.at(0).size() * sx_ : 0; }
					uint labelHeight( ) { prerender(); return pieces_.size() * sy_; }
					void spacing(uint dy) { sy_ = sx_ + dy; dirty(true); }
					uint spacing( ) const { return sy_ - sx_; }
					void color(color_t c) { c_ = c.rgb(); dirty(true); }
					color_t color( ) const { return color_t(c_ | 0xff000000); }
					void center(bool);
					bool center( ) const { return center_; }
				protected:
					void i_doPrerender( );
					void i_doRender( );
					void i_doResize( );
				private:
					std::string msg_;
					std::vector<std::string> pieces_;
					uint sx_, sy_;
					DWORD c_;
					bool center_;
			};

			typedef Label::Label_ptr Label_ptr;
		}
	}
}

#endif

