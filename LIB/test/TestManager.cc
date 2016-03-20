#include <map>
#include "TestManager.h"

namespace lib { namespace test {

class TestManager::TestManagerImpl
{
	friend class TestManager;
	typedef std::map<std::string, testFn> tests_t;
	typedef std::map<std::string, tests_t> units_t;

	units_t units_;
	tests_t *tests_;
	std::string cur_unit_;

	tests_t *getUnit(const std::string&);
};

void TestManager::setUnit(const std::string& id)
{
	impl_->tests_ = impl_->getUnit(id);
}

void TestManager::addTest(const std::string& id, testFn test)
{
	if(!impl_->tests_)
		throw std::string("ERR: no unit selected!");

	if(impl_->tests_->find(id) != impl_->tests_->end())
		throw std::string("ERR: tried to register duplicate tests: " + id);

	(*impl_->tests_)[id] = test;
}

int TestManager::run(void)
{
	typedef TestManagerImpl::units_t::iterator uiter_t;
	typedef TestManagerImpl::tests_t::iterator titer_t;

	for(uiter_t i1 = impl_->units_.begin(), i2 = impl_->units_.end() ; i1 != i2 ; ++i1)
	{
		for(titer_t j1 = i1->second.begin(), j2 = i1->second.end() ; j1 != j2 ; ++j1)
		{
			(*j1->second)();
		}
	}

	return 0;
}

TestManager::TestManager(void) : impl_(new TestManagerImpl)
{
	impl_->tests_ = NULL;
}

TestManager::~TestManager(void)
{
	delete impl_;
}

TestManager& TestManager::Instance(void)
{
	static TestManager tm;

	return tm;
}

TestManager::TestManagerImpl::tests_t *TestManager::TestManagerImpl::getUnit(const std::string& id)
{
	if(id == cur_unit_ || units_.find(id) != units_.end())
		throw std::string("ERR: unit named '" + id + "' already exists!");

	cur_unit_ = id;

	return &units_[cur_unit_];
}

}}

