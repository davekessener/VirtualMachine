#ifndef DAV_ADL_ALLOCATION_H
#define DAV_ADL_ALLOCATION_H

namespace dav
{
	namespace lib
	{
		namespace allocation
		{
			template<typename T>
			class Default
			{
				public:
				static constexpr size_t size = sizeof(T);

				public:
					static void *Allocate( ) { return operator new(size); }
					static void *Deallocate(void *ptr) { operator delete(ptr, size); }
				private:
					Default( ) = delete;
					Default(const Default&) = delete;
					Default& operator=(const Default&) = delete;
					~Default( ) = delete;
			};
		}
	}
}

#endif

