#ifndef ENCRYPTSTREAM_H
#define ENCRYPTSTREAM_H

template<typename S>
class encrypt_stream
{
	public:
		encrypt_stream(S& s) : s_(&s), last_(0), cleanup_(false) { }
		encrypt_stream(const std::string& s) : s_(new S(s)), last_(0), cleanup_(true) { }
		~encrypt_stream( ) { if(cleanup_) delete s_; }
		int get( );
		void read(void *, size_t);
		void put(int);
		void write(const void *, size_t);
		void close( ) { s_->close(); }
	private:
		S *s_;
		int last_;
		bool cleanup_;
};

template<typename S>
int encrypt_stream<S>::get(void)
{
	int t(last_);
	last_ = s_->get();
	return last_ ^ t;
}

template<typename S>
void encrypt_stream<S>::read(void *dst, size_t n)
{
	unsigned char *o = static_cast<unsigned char *>(dst);

	while(n--)
	{
		*o++ = static_cast<unsigned char>(get());
	}
}

template<typename S>
void encrypt_stream<S>::put(int c)
{
	s_->put(last_ ^= c);
}

template<typename S>
void encrypt_stream<S>::write(const void *src, size_t n)
{
	const unsigned char *i = static_cast<const unsigned char *>(src);

	while(n--)
	{
		put(*i++);
	}
}

#endif

