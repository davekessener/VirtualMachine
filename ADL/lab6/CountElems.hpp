#ifndef DAV_COUNTELEMS_H
#define DAV_COUNTELEMS_H

template<typename I, typename HM>
void countElements(I i1, I i2, HM&& map)
{
	while(i1 != i2)
	{
		++map[*i1];
		++i1;
	}
}

#endif

