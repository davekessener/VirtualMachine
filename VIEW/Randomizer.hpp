#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <vector>
#include <random>
#include <algorithm>
#include <aux>

struct RNGStraight
{
	template<typename I>
		void shuffle(I i1, I i2) const { }
};

struct RNGDefault
{
	template<typename I>
		void shuffle(I i1, I i2) const { std::random_shuffle(i1, i2); }
};

template<typename I>
class Randomizer
{
	typedef std::vector<I> ivec;
	typedef typename ivec::const_iterator const_iterator;
	typedef typename lib::aux::deref_trait<I>::value_type value_type;

	public:
		void load(I, I);
		template<typename RNG>
			void shuffle(const RNG&);
		void clear( ) { ivec().swap(vec_); beg_ = end_; }
		const_iterator begin( ) const { return vec_.cbegin(); }
		const_iterator end( ) const { return vec_.cend(); }
		const_iterator cbegin( ) const { return vec_.cbegin(); }
		const_iterator cend( ) const { return vec_.cend(); }
		const value_type& get( ) const { return **cur_; }
		void next( );
		void back( );
	private:
		ivec vec_;
		I beg_, end_;
		const_iterator cur_;
};

template<typename I>
void Randomizer<I>::load(I i1, I i2)
{
	clear();
	beg_ = i1;
	end_ = i2;
	while(i1 != i2)
	{
		vec_.push_back(i1);
		++i1;
	}
	cur_ = vec_.cbegin();
}

template<typename I>
template<typename RNG>
void Randomizer<I>::shuffle(const RNG& rng)
{
	I ti(*cur_);

	Randomizer<I> t;
	t.load(beg_, end_);
	rng.shuffle(t.vec_.begin(), t.vec_.end());
	vec_.swap(t.vec_);
	
	cur_ = std::find(vec_.cbegin(), vec_.cend(), ti);
}

template<typename I>
void Randomizer<I>::next(void)
{
	if(++cur_ == vec_.cend()) cur_ = vec_.cbegin();
}

template<typename I>
void Randomizer<I>::back(void)
{
	if(cur_ == vec_.cbegin()) cur_ = vec_.cend();
	--cur_;
}

#endif

