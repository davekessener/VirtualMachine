#ifndef BLOCK_H
#define BLOCK_H

enum class Direction
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	FRONT,
	BACK
};

class Block
{
	public:
		Block(int = 0);
		virtual ~Block( ) noexcept;
		virtual int getFace(Direction) const;
		inline int ID( ) const { return id_; }
	private:
		const int id_;
};

#endif

