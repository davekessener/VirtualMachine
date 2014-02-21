#ifndef FRAME_H
#define FRAME_H

#ifdef FRAME_MAIN
#endif

class Frame
{
	public:
		virtual inline ~Frame( ) { }
		virtual void input(int) = 0;
		virtual void update(int) = 0;
		virtual void refresh( ) = 0;
	private:
};

#endif

