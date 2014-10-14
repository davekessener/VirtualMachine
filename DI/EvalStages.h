#ifndef DAV_DIL_EVALSTAGES_H
#define DAV_DIL_EVALSTAGES_H

#include <memory>
#include <vector>
#include <sstream>
#include <cassert>

namespace dav
{
	namespace dil
	{
		class EvalStage
		{
			public:
			typedef std::vector<std::string> vec_t;
			typedef std::shared_ptr<EvalStage> Eval_ptr;
		
			public:
				inline EvalStage( ) : next_(nullptr) { }
				inline ~EvalStage( ) { }
				virtual void parse(const std::string&) = 0;
				virtual void out(std::ostream&) const { }
				inline EvalStage *getNext( ) { return next_; }
				inline bool hasNext( ) const { return next_; }
			protected:
				void next(EvalStage *i) { assert(!next_); next_ = i; }
			private:
				EvalStage *next_;
		};
		typedef EvalStage::Eval_ptr Eval_ptr;
		
		class Stage0_Name : public EvalStage
		{
			public:
				void parse(const std::string&);
		};

		class Stage1_Vars : public EvalStage
		{
			using EvalStage::vec_t;
		
			public:
				Stage1_Vars(const std::string& name) : name_(name) { }
				void parse(const std::string&);
			private:
				const std::string& name_;
				vec_t vars_;
		};
		
		class Stage2_Equation : public EvalStage
		{
			using EvalStage::vec_t;
		
			public:
				Stage2_Equation(const std::string& name, vec_t&& v)
					: name_(name), vars_(v) { }
				void parse(const std::string&);
			private:
				std::string name_;
				vec_t vars_, eqs_;
		};
		
		class Stage3_Evaluation : public EvalStage
		{
			using EvalStage::vec_t;
		
			public:
				template<typename I1, typename I2>
					Stage3_Evaluation(const std::string&, I1, I1, I2, I2);
				void parse(const std::string&) { assert(!"Cannot add token to finalized Evaluator!"); }
				void out(std::ostream& os) const { for(const auto& s : out_) os << s << "\n"; }
			private:
				vec_t out_;
		};
	}
}

#endif

