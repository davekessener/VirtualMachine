#include "File.h"
#include "Controller.h"
#include <dav/Logger.h>
#include <nbt/NBT.h>

#define MXT_NBT_MAPS "Maps"

namespace editor
{
	class FileHelper
	{
		friend class File;

		private:
			static File *get( ) { return instance().file_; }
			static void set(File *f)
			{
				if(instance().file_) delete instance().file_;
				instance().file_ = f;
			}
		private:
			FileHelper( ) : file_(nullptr) { }
			~FileHelper( ) { if(file_) delete file_; }
			static FileHelper& instance( ) { static FileHelper fh; return fh; }
		private:
			File *file_;
		private:
			FileHelper(const FileHelper&) = delete;
			FileHelper& operator=(const FileHelper&) = delete;
	};

// # ===========================================================================

	struct File::Impl
	{
		public:
			Impl(const std::string&);
			inline iterator begin( ) { return v_.begin(); }
			inline iterator end( ) { return v_.end(); }
			void insert(iterator, const std::string&, uint, uint);
			inline void erase(iterator i) { v_.erase(i); changed_ = true; }
			inline map_t& get(QWORD id)
				{ for(map_t& m : v_) if(m.ID() == id) return m; assert(!"ID doesn't exist!"); }
			void save(const std::string&);
			inline bool hasName( ) const { return !fn_.empty(); }
			inline bool hasChanged( ) const
				{ for(const map_t& m : v_) if(m.hasChanged()) return true; return changed_; }
			inline void change( ) { changed_ = true; }
		private:
			std::string fn_;
			vec_t v_;
			bool changed_;
	};

// # ===========================================================================

	File::iterator File::begin(void)
	{
		return instance().impl_->begin();
	}

	File::iterator File::end(void)
	{
		return instance().impl_->end();
	}

	void File::insert(iterator i, const std::string& name, uint w, uint h)
	{
		instance().impl_->insert(i, name, w ? w : 1, h ? h : 1);
	}

	void File::erase(iterator i)
	{
		instance().impl_->erase(i);
	}

	File::map_t& File::get(QWORD id)
	{
		return instance().impl_->get(id);
	}

	bool File::hasName(void)
	{
		return instance().impl_->hasName();
	}

	void File::save(const std::string& fn)
	{
		if(Controller::isLoaded()) Controller::save();
		instance().impl_->save(fn);
	}

	bool File::hasChanged(bool set)
	{
		if(set)
		{
			instance().impl_->change();
			return true;
		}
		else
		{
			return     (Controller::isLoaded() && Controller::hasChanged())
					||  instance().impl_->hasChanged();
		}
	}

// # ---------------------------------------------------------------------------

	void File::load(const std::string& fn)
	{
		FileHelper::set(new File(fn));
	}

	void File::close(void)
	{
		FileHelper::set(nullptr);
	}

	bool File::isLoaded(void)
	{
		return FileHelper::get();
	}

	File::File(const std::string& fn) : impl_(new Impl(fn))
	{
	}

	File& File::instance(void)
	{
		return *FileHelper::get();
	}

// # ===========================================================================

	File::Impl::Impl(const std::string& fn) : fn_(fn), changed_(true)
	{
		if(fn_.empty()) return;

		auto err = [this]()
		{
			LOG("ERR: File '%s' has no valid map-data.", fn_.data());
			throw std::string("no valid map-data!");
		};

		nbt::TAG_Compound::ptr_t root = nbt::readFile(fn_);

		if(!root || !root->hasTag(MXT_NBT_MAPS)) err();

		nbt::TAG_List::ptr_t list = root->getTagList(MXT_NBT_MAPS);

		if(list->tagID() != nbt::TAG_Compound::ID) err();

		std::map<QWORD, std::shared_ptr<map_t>> mm;

		for(auto i = list->begin<nbt::TAG_Compound>(), e = list->end<nbt::TAG_Compound>() ; i != e; ++i)
		{
			std::shared_ptr<map_t> m(new map_t(*i));
			mm[m->ID()] = m;
		}

		v_.reserve(mm.size());

		for(QWORD i = 0, c = 0, s = mm.size() ; c < s ; ++i)
		{
			auto j = mm.find(i);

			if(j != mm.end())
			{
				v_.push_back(std::move(*j->second));
				++c;
			}
		}

		changed_ = false;
	}

	void File::Impl::insert(iterator i, const std::string& name, uint w, uint h)
	{
		if(v_.empty())
		{
			v_.push_back(map_t(0, name, w, h));
		}
		else if(i == v_.end())
		{
			v_.push_back(map_t((*--i).ID() + 1, name, w, h));
		}
		else
		{
			for(auto j = i, e = v_.end() ; j != e ; ++j)
			{
				j->setID(j->ID() + 1);
			}

			v_.insert(i, map_t(i->ID() - 1, name, w, h));
		}
	}

	void File::Impl::save(const std::string& fn)
	{
		if(!fn.empty()) fn_ = fn;

		if(fn_.empty())
		{
			LOG("ERR: Cannot save file without name.");
			throw std::string("cannot save file wothout name!");
		}

		nbt::TAG_Compound::ptr_t tag = nbt::Make<nbt::TAG_Compound>();
		nbt::TAG_List::ptr_t list = nbt::Make<nbt::TAG_List>();

		for(const map_t& m : v_)
		{
			list->addTag(m.save());
		}

		tag->setTagList(MXT_NBT_MAPS, list);

		nbt::writeFile(fn_, tag);

		changed_ = false;
	}
}

