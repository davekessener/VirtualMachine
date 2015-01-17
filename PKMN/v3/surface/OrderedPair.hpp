#ifndef DAV_PKMN_V3_SCREEN_ORDEREDPAIR
#define DAV_PKMN_V3_SCREEN_ORDEREDPAIR

#include <algorithm>

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			template<typename K, typename V, typename S = std::less<K>>
			class OrderedPair
			{
				typedef OrderedPair<K, V, S> self;

				public:
					OrderedPair(const K& k, V v) : key_(k), value_(v) { }
					bool operator<(const self& p) const { return op_(key_, p.key_); }
					operator V( ) const { return value_; }
					V operator->( ) { return value_; }
					const V operator->( ) const { return value_; }
				private:
					K key_;
					V value_;
					S op_;
			};
		}
	}
}

#endif

