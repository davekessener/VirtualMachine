#include "Template.h"
#include "TemplateManager.h"

namespace sim
{
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
			const std::vector<int>& in( ) { return in_; }
			const std::vector<int>& out( ) { return out_; }
		private:
			std::string name_;
			std::vector<int> in_, out_;
			int state;

			void scan(const std::string& s)
			{
//				std::cout << "##Scan(" << state << ") '" << s << "'" << std::endl;

				if(s[0] >= '0' && s[0] <= '9')
				{
					(state ? out_ : in_).push_back(std::atoi(s.c_str()));
				}
				else
				{
					assert(!state);

					state = 1;

					name_ = s;
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

		void readLineOfInts(std::vector<int>& v, const std::string& s)
		{
			check__(s);

			std::istringstream iss(s);

			while(iss)
			{
				int i;
				iss >> i;
				if(!iss) break;
				v.push_back(i);
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

		for(int in : ins_)
		{
			ch->setInput(in);
		}

		for(int out : outs_)
		{
			ch->setOutput(out);
		}

		for(Scanner *s : components_)
		{
			Chip::Chip_ptr c = TemplateManager::getTemplate(s->name())->instantiate();

			for(int i = 0, m = s->in().size() ; i < m ; ++i)
			{
				int in = s->in().at(i);
				if(in < 0) continue;
				c->getInput(i)->setInput(ch->getNode(in));
			}

			for(int i = 0, m = s->out().size() ; i < m ; ++i)
			{
				int out = s->out().at(i);
				if(out < 0) continue;
				ch->getNode(out)->connect(*c->getOutput(i));
			}

			ch->addChip(c);
		}

		return ch;
	}
}

