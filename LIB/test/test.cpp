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
	std::cout << "hello" << std::endl;
}

AFTER
{
	std::cout << "goodbye" << std::endl;
}

TEST(my_test)
{
	ASSERT_TRUE(false, "whatever");
	std::cout << "Hello, World! x" << i << std::endl;
}

TEST(another_test)
{
	ASSERT_TRUE(i == 7);
	std::cout << "i is indeed 7!" << std::endl;
}

END

