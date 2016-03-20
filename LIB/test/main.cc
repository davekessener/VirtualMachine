#ifndef DAV_TEST_MAIN_H
#define DAV_TEST_MAIN_H

#include "TestManager.h"

int main(int argc, char *argv[])
{
	return lib::test::TestManager::Instance().run();
}

#endif

