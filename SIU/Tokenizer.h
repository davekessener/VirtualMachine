#ifndef SIU_TOKENIZER_H
#define SIU_TOKENIZER_H

namespace siu
{
	class Tokenizer
	{
		public:
			Tokenizer(const std::string&);
			~Tokenizer( );
			std::string poll( );
			bool isEmpty( );
		private:
			const char *_c, *_i;
	};
}

#endif

