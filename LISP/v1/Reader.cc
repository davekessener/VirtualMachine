#include "Reader.h"
#include <readline/readline.h>
#include <readline/history.h>

namespace reader
{
	template<typename T>
	void eraseArr(T *t)
	{
		free(t);
	}

	template<typename T, void (&DEALLOC)(T*) = eraseArr<T>>
	class tmpBuf
	{
		public:
			tmpBuf( ) : v_(NULL) { }
			tmpBuf(T *t) : v_(t) { }
			tmpBuf(tmpBuf<T>& t) : v_(t.v_) { t.v_ = NULL; }
			~tmpBuf( ) { DEALLOC(v_); v_ = NULL; }
			tmpBuf<T>& operator=(tmpBuf<T>& t) { DEALLOC(v_); v_ = t.v_; t.v_ = NULL; }
			operator T *( ) { return v_; }
			operator const T *( ) const { return v_; }
			T& operator[](int i) { return v_[i]; }
			const T& operator[](int i) const { return v_[i]; }
			operator bool( ) const { return v_ != NULL; }
		private:
			T *v_;
	};

	const std::string readline(void)
	{
		tmpBuf<char> buf(::readline(""));

		if(buf && buf[0])
		{
			::add_history(buf);
		}

		return std::string(buf);
	}
}

