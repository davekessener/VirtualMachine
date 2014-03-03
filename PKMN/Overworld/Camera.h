#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "Moving.h"

class Camera : public Moving
{
	public:
		Camera( ) : _lock(NULL) { }
		void update( );
		void walk(Direction);
		void lock(const Moving *);
		inline void unlock( ) { lock(NULL); }
	private:
		const Moving *_lock;
};

#endif

