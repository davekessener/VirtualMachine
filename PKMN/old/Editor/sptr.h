#ifndef __SPTR_H
#define __SPTR_H

#include <memory>
#include <cassert>
#include <Misc/Logger.h>

namespace
{
	template<typename T>
	class _sptr_deref
	{
		public:
			_sptr_deref(T *t) : _v(t) { LOG("Dereference @%p", t); }
			_sptr_deref(const _sptr_deref<T>& p) : _v(p._v) { LOG("Copied dereference @%p", _v); }
			operator T( ) { LOG("Implicit conversion: @%p yields %d", _v, static_cast<int>(*_v)); return *_v; }
			T& operator=(const T& t) { LOG("Assigned dereference: @%p from %d to %d", _v, static_cast<int>(*_v), static_cast<int>(t)); return *_v = t; }
			T *operator&( ) { LOG("Address of reference @%p", _v); return _v; }
		private:
			_sptr_deref<T>& operator=(const _sptr_deref<T>&);
			T *_v;
	};
}

template<typename T>
class sptr
{
	public:
		sptr(T *t) : _v(t) { LOG("New smart-pointer to %p(%d)", t, static_cast<int>(*t)); }
		sptr(const sptr<T>& p) : _v(p._v) { LOG("Copied smart-pointer to %p(%d)", &*_v, getval()); }
		sptr<T>& operator=(const sptr<T>& p) { LOG("Assigned smart-pointer from %p(%d) to %p(%d)", &*_v, getval(), &*p._v, p.getval()); _v = p._v;  return *this; }
		void reset(T *t) { LOG("Reset smart-ptr from %p(%d) to %p(%d)", &*_v, getval(), t, static_cast<int>(*t)); _v.reset(t); }
		void set(const T& t) { LOG("Used value-set @%p: from %d to %d", &*_v, getval(), static_cast<int>(t)); *_v = t; }
		_sptr_deref<T> operator*( ) { return _sptr_deref<T>(&*_v); }
//		T *operator->( );
		explicit operator bool( ) { LOG("Explicit bool-cast @%p: %d", static_cast<bool>(_v)); return static_cast<bool>(_v); }
	private:
		int getval( ) const { return static_cast<bool>(_v) ? static_cast<int>(*_v) : 0; }
		std::shared_ptr<T> _v;
};

#endif

