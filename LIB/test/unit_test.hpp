#ifndef DAV_TEST_UNITTEST_H
#define DAV_TEST_UNITTEST_H

#include <test/TestManager.h>

#define MXT_STRINGIFY(a) #a

// # ---------------------------------------------------------------------------

#define BEGIN \
namespace TEST_CASE_NAME { \
struct Base { }; \
static ::lib::test::TestManager::Selector select_unit_##TEST_CASE_NAME(MXT_STRINGIFY(TEST_CASE_NAME)); \
template<typename T> struct Data { }; \
template<typename T> struct Before : public virtual Data<T> { void setup(void) { } }; \
template<typename T> struct After : public virtual Data<T> { void teardown(void) { } };

// # ---------------------------------------------------------------------------

#define END \
}

// # ---------------------------------------------------------------------------

#define DATA \
ThereCanBeOnlyOne { }; \
template<> \
struct Data<Base>

// # ---------------------------------------------------------------------------

#define BEFORE \
template<> \
struct Before<Base> : public virtual Data<Base> \
{ \
	void setup( ); \
}; \
void Before<Base>::setup(void)

// # ---------------------------------------------------------------------------

#define AFTER \
template<> \
struct After<Base> : public virtual Data<Base> \
{ \
	void teardown( ); \
}; \
void After<Base>::teardown(void)

// # ---------------------------------------------------------------------------

#define TEST(test_name) \
struct test_name : public Before<Base>, public After<Base> \
{ \
	void test( ); \
	static void wrapper( ); \
}; \
void test_name::wrapper(void) \
{ \
	test_name t; \
	t.setup(); \
	t.test(); \
	t.teardown(); \
} \
static ::lib::test::TestManager::Registrar register_##test_name(#test_name, &test_name::wrapper); \
void test_name::test(void)

// # ---------------------------------------------------------------------------

#endif

