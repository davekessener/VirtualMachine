#include <iostream>
#include <cstdint>
#include <vector>

typedef uint64_t (*QuickSortFn)(const void *, const void *);
typedef uint64_t (*QuickSortFnReentrant)(const void *, const void *, void *);

void QuickSort(void *, size_t, size_t, QuickSortFn);
void QuickSort(void *, size_t, size_t, QuickSortFnReentrant, void *);

template<typename T, typename F>
void QuickSortReentrant(T *data, uint64_t len, F f)
{
	auto cmp = [](const void *l, const void *r, void *c) -> uint64_t
		{
			return (*static_cast<F *>(c))(static_cast<const T *>(l), static_cast<const T *>(r));
		};
	
	QuickSort(data, len, sizeof(T), cmp, &f);
}

template<typename T, typename F>
void QuickSort(T *data, uint64_t len, F f)
{
	auto cmp = [f](const void *l, const void *r) -> uint64_t
		{
			return f(static_cast<const T *>(l), static_cast<const T *>(r));
		};
	
	QuickSort(data, len, sizeof(T), cmp);
}

int _main(int argc, char *argv[])
{
//	int v[] = {8, 5, 0, 2, 4, 1, 9, 3, 7, 6};
	std::vector<int> v = {8, 5, 0, 2, 4, 1, 9, 3, 7, 6};
	auto cmp = [](const void *l, const void *r) -> uint64_t { return *static_cast<const int *>(l) < *static_cast<const int *>(r); };

//	for(int i = 0 ; i < 10 ; ++i)
	for(int i : v)
	{
//		std::cout << v[i] << ' ';
		std::cout << i << ' ';
	}

	std::cout << std::endl;

//	QuickSort(v, 10, cmp);
//	QuickSort(v, 10, sizeof(int), cmp);

	QuickSort(&*v.begin(), v.size(), cmp);
//	QuickSort(&*v.begin(), v.size(), sizeof(*v.begin()), cmp);

//	for(int i = 0 ; i < 10 ; ++i)
	for(int i : v)
	{
//		std::cout << v[i] << ' ';
		std::cout << i << ' ';
	}

	std::cout << std::endl;

	return 0;
}

void QuickSort(void *_data, size_t len, size_t size, QuickSortFn sort)
{
	QuickSort(_data, len, size, [](const void *l, const void *r, void *c)->uint64_t{ return (*static_cast<QuickSortFn *>(c))(l, r); }, &sort);
}

void QuickSort(void *_data, size_t len, size_t size, QuickSortFnReentrant sort, void *context)
{
	uint8_t *data = static_cast<uint8_t *>(_data);

	for(int i = 0 ; i < len ; ++i)
	{
		for(int j = i + 1 ; j < len ; ++j)
		{
			if(!sort(data + i * size, data + j * size, context))
			{
				for(int k = 0 ; k < size ; ++k)
				{
					uint8_t t = (data + i * size)[k];
					(data + i * size)[k] = (data + j * size)[k];
					(data + j * size)[k] = t;
				}
			}
		}
	}
}

