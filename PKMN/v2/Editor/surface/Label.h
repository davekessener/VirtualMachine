#ifndef PKMN_EDITOR_SURFACE_LABEL_H
#define PKMN_EDITOR_SURFACE_LABEL_H

#include "../../common.h"
#include "Surface.h"

namespace editor
{
	namespace surface
	{
		class Label : public Surface
		{
			public:
				Label(const std::string& m) : msg_(m) { }
				uint getEstWidth( ) const;
			private:
				void i_doInit( );
				void i_doUpdate(int) { }
				void i_doRender( ) const;
			private:
				std::string msg_;
		};
	}
}

#endif

