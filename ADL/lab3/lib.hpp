#ifndef DAV_LIB_H
#define DAV_LIB_H

namespace dav
{
	namespace lib
	{
		struct SingleThreaded
		{
			typedef SingleThreaded self;

			class Lock
			{
				public:
					Lock(self&) { }
			};
		};

		template<typename MTX>
		class MTClass
		{
			typedef MTClass<MTX> self;

			public:
			class Lock
			{
				public:
					Lock(self&) { self::mtx_.aquire(); }
					~Lock( ) { self::mtx_.release(); }
				private:
					Lock(const Lock&) = delete;
			};

			private:
				static MTX mtx_;
		};

		template<typename MTX>
		MTX MTClass<MTX>::mtx_;

		template<typename MTX>
		class MTObject
		{
			typedef MTObject<MTX> self;

			public:
				class Lock
				{
					public:
						Lock(self& s) : s_(s) { s_.mtx_.aquire(); }
						~Lock( ) noexcept { s_.mtx_.release(); }
				};
			private:
				MTX mtx_;
		};
	}
}

#endif

