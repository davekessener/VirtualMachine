#include "Test.h"

void Test::testSingleton(void)
{
	Singleton<A>::Instance().test();
}

