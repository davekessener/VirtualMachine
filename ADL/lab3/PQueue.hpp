#ifndef DAV_PQUEUE_H
#ifndef DAV_PQUEUE_H

namespace dav
{
	template<typename T, typename S = std::less<T>>
	class PQueue
	{
		public:
		typedef typename std::decay<T>::type value_type;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		public:
			void insert(const_reference);
			void remove( );
			reference top( );
			const_reference top( ) const;
			size_type size( ) const;
			bool empty( ) const;
		private:
	};
}

#endif

