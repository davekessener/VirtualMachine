#include "OWSpritePool.h"
#include <sstream>

#define OWS_POOL_MAXSIZE 24

template<typename T>
class _pool_validation_age  : public PoolCounted<T, OWS_POOL_MAXSIZE>
{
	public:
		typedef typename PoolSimple<T>::return_type return_type;

		_pool_validation_age( ) { }
		_pool_validation_age(return_type v) : PoolSimple<T>(v), PoolCounted<T, OWS_POOL_MAXSIZE>(v) { }
};

SDLImage *OWSpritePool::getSprite(int id)
{
	SDLImage *r = pool::instance().getFromPool(id);

	if(!r->opened())
	{
		std::ostringstream ss;
		ss << "./sprites/" << id << ".png";
		r->setRenderer(Screen::instance().getRenderer());
		r->open(ss.str());
	}

	return r;
}

