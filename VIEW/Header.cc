#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include <aux>
#include "Header.h"
#include "Viewer.h"
#include "Dialog.h"
#include "Manager.h"
#include "dav_header.h"

struct HImpl
{
	void read(std::istream&);

	dav::dataheader_t data_;
	std::vector<dav::data_t> imgs_;
};

struct Header::Impl
{
	Impl( ) : fwd_(new HImpl) { }
	std::shared_ptr<HImpl> fwd_;
};

void HImpl::read(std::istream& is)
{
	is >> lib::aux::read_from(data_);
	assert(data_.id==dav::DAV_MAGIC);
	imgs_.resize(data_.imgcount);
	is >> lib::aux::read_from(*imgs_.begin(), data_.imgcount * sizeof(dav::data_t));
}

Header::Header(std::istream& is) : impl_(new Impl)
{
	impl_->fwd_->read(is);
}

void Header::operator()(const std::string& s)
{
	QWORD pwd = dav::hash(s);

	if(pwd == impl_->fwd_->data_.hash)
	{
		Viewer *v = new Viewer;
		Manager::instance().pushScreen(Manager::screen_ptr(v));
		std::vector<std::string> fns(impl_->fwd_->imgs_.size());
		auto i = fns.begin();
		for(const auto& d : impl_->fwd_->imgs_)
		{
			*i = lib::aux::to_hex(d.name) + ".gz"; ++i;
		}
		v->load(fns.cbegin(), fns.cend());
		v->finalize();
	}
	else
	{
		Manager::instance().pushScreen(Manager::screen_ptr(new Dialog("ACCESS\nDENIED\n!!!")));
	}
}

Header::Header(const Header& h) : impl_(new Impl(*h.impl_))
{
}

Header::~Header(void) noexcept
{
	delete impl_;
}

Header& Header::operator=(const Header& h)
{
	Header t(h);
	swap(t);
	return *this;
}

void Header::swap(Header& h) noexcept
{
	Impl *i = impl_;
	impl_ = h.impl_;
	h.impl_ = i;
}

