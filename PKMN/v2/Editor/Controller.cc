#include "Controller.h"
#include <stack>
#include <functional>
#include <cassert>

bool editor::Controller::GRID = false;
bool editor::Controller::SPLIT = false;

namespace editor
{
	namespace
	{
		class History
		{
			typedef std::pair<DWORD, DWORD> value_t;
			struct data_t { uint l, x, y; value_t v; };
			typedef std::vector<data_t> vec_t;
			typedef std::shared_ptr<vec_t> data_ptr;
			typedef std::function<DWORD(const value_t&)> access_fn;
			typedef std::function<void(uint, uint, uint, DWORD)> set_fn;

			struct bftor
			{
				bftor(data_ptr dp, access_fn afn, set_fn s)
					: data_(dp), acc_(afn), set_(s) { }
				void operator()( ) const
				{
					for(const data_t& d : *data_)
					{
						set_(d.l, d.x, d.y, acc_(d.v));
					}
				}

				private:
					data_ptr data_;
					access_fn acc_;
					set_fn set_;
			};

			public:
				History( ) : data_(new vec_t) { }
				void add(uint l, uint x, uint y, uint n, uint o)
				{
					auto i = std::find_if(data_->begin(), data_->end(),
						[x, y](const data_t& d){ return x == d.x && y == d.y; });

					if(i == data_->end())
					{
						data_->push_back(data_t{l, x, y, std::make_pair(n, o)});
					}
					else
					{
						*i = data_t{l, x, y, std::make_pair(n, i->v.second)};
					}
				}
				void clear( ) { data_.reset(new vec_t); }
				bftor undo(set_fn fn) const { return bftor(data_, [](const value_t& p){return p.second;}, fn); }
				bftor redo(set_fn fn) const { return bftor(data_, [](const value_t& p){return p.first;}, fn); }
				bool empty( ) const { return data_->empty(); }
			private:
				data_ptr data_;
		};
	}

// # ===========================================================================

	struct Controller::Impl
	{
		typedef std::function<void(void)> do_fn;
		typedef std::stack<std::pair<do_fn, do_fn>> dostack_t;

		public:
			Impl(QWORD id) : map_(File::get(id)), changed_(false) { }
			inline const map_t& getMap( ) const { return map_; }
			void set(uint, uint, uint, DWORD);
			inline DWORD get(uint l, uint x, uint y) const { return map_.get(l, x, y); }
			inline void undo( ) { if(!undo_.empty()) dodo(undo_, redo_); }
			inline void redo( ) { if(!redo_.empty()) dodo(redo_, undo_); }
			inline bool changed( ) const { return changed_; }
			inline void save( )
				{ if(changed_) { File::set(map_); changed_ = false; } }
			void addBuffer(uint, uint, uint, DWORD);
			void commitBuffer( );
		private:
			inline void dodo(dostack_t& s1, dostack_t& s2)
			{ auto a(s1.top()); s1.pop(); s2.push(std::make_pair(a.second, a.first)); a.first(); changed_ = true; }
		private:
			map_t map_;
			dostack_t undo_, redo_;
			bool changed_;
			History history;
	};

// # ===========================================================================

	void Controller::Impl::set(uint l, uint x, uint y, DWORD v)
	{
		if(!history.empty()) commitBuffer();

		DWORD old = map_.get(l, x, y);

		map_.set(l, x, y, v);

		undo_.push(std::make_pair([=]{ map_.set(l, x, y, old); }, [=]{ map_.set(l, x, y, v); }));
		while(!redo_.empty()) redo_.pop();

		changed_ = true;
	}

	void Controller::Impl::addBuffer(uint l, uint x, uint y, DWORD v)
	{
		try
		{
			DWORD o = map_.get(l, x, y);
			map_.set(l, x, y, v);
			history.add(l, x, y, v, o);

			changed_ = true;
		}
		catch(...) { }
	}

	void Controller::Impl::commitBuffer(void)
	{
		if(history.empty()) return;
		auto sfn = [this](uint l, uint x, uint y, DWORD v) { map_.set(l, x, y, v); };
		undo_.push(std::make_pair(history.undo(sfn), history.redo(sfn)));
		while(!redo_.empty()) redo_.pop();
		history.clear();
	}

// # ===========================================================================

	void Controller::load(QWORD id)
	{
		instance().doLoad(id);
	}

	bool Controller::isLoaded(void)
	{
		return instance().impl_;
	}

	const Controller::map_t& Controller::getMap(void)
	{
		assert(instance().impl_);
		return instance().impl_->getMap();
	}

	void Controller::set(uint l, uint x, uint y, DWORD v)
	{
		assert(instance().impl_);
		instance().impl_->set(l, x, y, v);
	}

	DWORD Controller::get(uint l, uint x, uint y)
	{
		assert(instance().impl_);
		return instance().impl_->get(l, x, y);
	}

	void Controller::undo(void)
	{
		assert(instance().impl_);
		instance().impl_->undo();
	}

	void Controller::redo(void)
	{
		assert(instance().impl_);
		instance().impl_->redo();
	}

	bool Controller::hasChanged(void)
	{
		assert(instance().impl_);
		return instance().impl_->changed();
	}

	void Controller::save(void)
	{
		assert(instance().impl_);
		instance().impl_->save();
	}

// # ---------------------------------------------------------------------------

	Controller::~Controller(void)
	{
		delete impl_;
	}

	void Controller::doLoad(QWORD id)
	{
		if(impl_)
		{
			delete impl_;
			impl_ = nullptr;
		}

		Impl *i = new Impl(id);
		
		impl_ = i;
	}

	void Controller::setBuffer(uint l, uint x, uint y, DWORD v)
	{
		assert(instance().impl_);
		instance().impl_->addBuffer(l, x, y, v);
	}

	void Controller::commitBuffer(void)
	{
		assert(instance().impl_);
		instance().impl_->commitBuffer();
	}
}

