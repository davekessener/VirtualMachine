#include <iostream>

#define TEST_CASE_NAME MyTest
#include <test/unit_test.hpp>

BEGIN

struct DATA
{
	int i;
};

BEFORE
{
	i = 7;
}

TEST(my_test)
{
	std::cout << "Hello, World! x" << i << std::endl;
}

END

