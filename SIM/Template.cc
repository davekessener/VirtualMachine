#include "Template.h"
#include "TemplateManager.h"

namespace sim
{
	namespace
	{
		void addList(std::vector<std::string>&, const std::string&);
	}

	struct Template::Scanner
	{
		public:
			Scanner(const std::string& l) : state(0)
			{
				std::istringstream iss(l);

				while(iss)
				{
					std::string s;
					iss >> s;
					if(!s.empty()) scan(s);
				}
			}
			std::string name( ) { return name_; }
			const std::vector<std::string>& in( ) { return in_; }
			const std::vector<std::string>& out( ) { return out_; }
		private:
			std::string name_;
			std::vector<std::string> in_, out_;
			int state;

			void scan(const std::string& s)
			{
//				std::cout << "##Scan(" << state << ") '" << s << "'" << std::endl;

				if(s[0] != '$')
				{
//					(state ? out_ : in_).push_back(s);
					addList(state ? out_ : in_, s);
				}
				else
				{
					assert(!state);

					state = 1;

					name_ = s.substr(1);
				}
			}
	};

	namespace
	{
		inline void check__(const std::string& _s_)
		{
			const char *s = _s_.c_str();

			do assert((*s>='0'&&*s<='9')||*s==' '||*s=='\t'); while(*++s);
		}

		void addList(std::vector<std::string>& v, const std::string& s)
		{
			const char *o = s.c_str();

			while(*o) if(*o++ == ':') break;

			std::string t(s.substr(0, o - s.c_str() - 1));
			int d = 0;

			if(*o)
			{
				d = std::atoi(o);

				assert(d>0);
	
				for(int i = 0 ; i < d ; ++i)
				{
					std::ostringstream oss;
					oss << t << "[" << i << "]";
					v.push_back(oss.str());
//					std::cout << "'" << s << "': '" << v.back() << "'" << std::endl;
				}
			}
			else
			{
				v.push_back(s);
			}
		}

		void readLineOfInts(std::vector<std::string>& v, const std::string& s)
		{
//			check__(s);

			std::istringstream iss(s);

			while(iss)
			{
				std::string i;
				iss >> i;
				if(!iss) break;
				addList(v, i);
//				v.push_back(i);
			}
		}
	}


	Template::Template(const std::vector<std::string>& data) : name_(data.at(0)), isOptimized_(false)
	{
		components_.reserve(data.size());

		for(const std::string &line : data)
		{
			if(line.empty()) continue;

			if(line[0] != ' ' && line[0] != '\t')
			{
				if(line[0] == '#')
				{
					switch(line[1])
					{
						case 'o':
							assert(outs_.empty());
							readLineOfInts(outs_, line.substr(2));
							break;
						case 'i':
							assert(ins_.empty());
							readLineOfInts(ins_, line.substr(2));
							break;
						case 'p':
							assert(pre_.empty());
							readLineOfInts(pre_, line.substr(2));
							break;
						case '!':
							isOptimized_ = true;
							break;
						default:
							assert(!"ERR: 'i' for input or 'o' for output expected!");
					}
				}
			}
			else
			{
				components_.push_back(new Scanner(line));
			}
		}
	}

	Template::~Template(void)
	{
		for(Scanner *s : components_)
		{
			delete s;
		}
	}

	Chip::Chip_ptr Template::instantiate(void) const
	{
		Chip::Chip_ptr ch(new Chip());

		ch->setName(name_);

		if(isOptimized_) ch->enableOptimization();

		for(const std::string& pre : pre_)
		{
			ch->getNode(pre)->preInit();
		}

		for(const std::string& in : ins_)
		{
			ch->setInput(in);
		}

		for(const std::string& out : outs_)
		{
			ch->setOutput(out);
		}

		for(Scanner *s : components_)
		{
			Chip::Chip_ptr c = TemplateManager::getTemplate(s->name())->instantiate();

			if(isOptimized_ && !c->isOptimized())
			{
				std::cout << "ERR: Chip '" << c->getName() << "' is not optimized." << std::endl;
			}

			if(isOptimized_) assert(c->isOptimized());

			for(int i = 0, m = s->in().size() ; i < m ; ++i)
			{
				const std::string &in = s->in().at(i);
				if(in == "_") continue;
				c->getInput(i)->setInput(ch->getNode(in));
			}

			for(int i = 0, m = s->out().size() ; i < m ; ++i)
			{
				const std::string& out = s->out().at(i);
				if(out == "_") continue;
				ch->getNode(out)->connect(*c->getOutput(i));
			}

			ch->addChip(c);
		}

		return ch;
	}
}

