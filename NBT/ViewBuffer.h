#ifndef NBTEDIT_VIEWBUFFER_H
#define NBTEDIT_VIEWBUFFER_H

#include <string>
#include <vector>

class ViewBuffer
{
	public:
	struct position_t { int x, y; };
	struct line_t { position_t pos; std::string line; };
	typedef std::vector<line_t> vec_t;
	typedef vec_t::iterator iterator;
	typedef vec_t::const_iterator const_iterator;

	public:
		ViewBuffer( ) { }
		ViewBuffer(vec_t&& v) : vec_() { vec_.swap(v); }
		ViewBuffer(ViewBuffer&& vb) : vec_() { vec_.swap(vb.vec_); }
		ViewBuffer& operator=(ViewBuffer&& vb) { vec_.swap(vb.vec_); return *this; }
		void renderAt(int, int, int, int, int, int) const;
		void update(vec_t&& v) { vec_.swap(v); w_ = h_ = -1; }
		iterator begin( ) { return vec_.begin(); }
		iterator end( ) { return vec_.end(); }
		const_iterator begin( ) const { return vec_.cbegin(); }
		const_iterator end( ) const { return vec_.cend(); }
		const_iterator cbegin( ) const { return vec_.cbegin(); }
		const_iterator cend( ) const { return vec_.cend(); }
	private:
		vec_t vec_;
		mutable vec_t opt_;
		mutable int sx_, sy_, w_, h_;
};

#endif

