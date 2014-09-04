#ifndef PKMN_EDITOR_ADDMAPDLG_H
#define PKMN_EDITOR_ADDMAPDLG_H

#include "../common.h"
#include "surface/Surface.h"

namespace editor
{
	class AddMapDlg : public surface::Surface
	{
		public:
		private:
			void i_doInit( );
			void i_doUpdate(int) { }
			void i_doRender( ) const;
			void create( );
		private:
			surface::Surface_ptr id_in_, name_in_, width_in_, height_in_;
	};
}

#endif

