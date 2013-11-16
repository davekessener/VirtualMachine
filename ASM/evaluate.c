#define EVAL_C
#define SM_OPS
#include "evaluate.h"
#undef SM_OPS
#undef EVAL_C

int evaluate(const char *src)
{
	P parser;
	P_init(&parser);

// Add all viable tokens with their string-based
// identified to the symboltable
#define TADD(s,i) TABLE_add(&parser.symtable,s,i)
	TADD("+", TOK_PLUS);
	TADD("-", TOK_MINUS);
	TADD("*", TOK_AST);
	TADD("/", TOK_SLASH);
	TADD("**", TOK_EXP);
	TADD("log", TOK_LOG);
	TADD("lg", TOK_LOG);
	TADD("ln", TOK_LN);
	TADD("sin", TOK_SIN);
	TADD("cos", TOK_COS);
	TADD("tan", TOK_TAN);
	TADD("(", TOK_OP);
	TADD(")", TOK_CP);
	TADD("pi", TOK_PI);
	TADD("e", TOK_E);
	TADD("#^", TOK_RUP);
	TADD("#v", TOK_RDOWN);
	TADD("#-", TOK_ROUND);
	TADD("##", TOK_ROUND);
	TADD("&&", TOK_LAND);
	TADD("||", TOK_LOR);
	TADD("!", TOK_LNOT);
	TADD("&", TOK_AND);
	TADD("|", TOK_OR);
	TADD("~", TOK_NOT);
	TADD("^", TOK_XOR);
	TADD("%", TOK_PERC);
	TADD("<<", TOK_SHL);
	TADD(">>", TOK_SHR);
#undef TADD

	// Load the input string into the tokenizer
	TKN_load(&parser.tokenizer, src);

	// Evaluate the expression
	evalLAO(&parser);

	// If the read buffer isn't empty
	// (The tkn couldn't read the entire input string)
	if(*parser.tokenizer.cp)
	{
		fprintf(stderr, "ERR: Expected EOS, but found '%.16s' ...\nAbort.\n", parser.tokenizer.cp);
		P_dispose(&parser);
		return 1;
	}

	P_dispose(&parser);

	// Return status == OK
	return 0;
}

void setEvalOutput(logfPtr fn)
{
	// Assign the static function pointer from
	// 'pFn' the new output function (if it isn't a NULL pointer)
	if(fn != NULL)
	{
		*pFn() = fn;
	}
}

// # --------------------------------------------------------------------------- 

void stdEvalPrint(const char *out)
{
	printf("%s\n", out);
}

// # =========================================================================== 

// Print a token with id 'i' via parser 'p'
void printToken(P *p, int i) { TOK t; t.type = i; t.val = 0.0; P_print(p, t); }
// Read next token from the tokenizer of parser 'p'
T getToken(P *p) { return TKN_get(&p->tokenizer, &p->symtable); }

// Simple grammar for mathematical expressions:
// MATH-EXPR = LAO-EXPR
//  LAO-EXPR = AS-EXPR ([&&,||] AS-EXPR)*
//   AS-EXPR = MD-EXPR ([+-] MD-EXPR)*
//   MD-EXPR =  E-EXPR ([*/%] E-EXPR)*
//    E-EXPR = AOX-EXPR ('**'  E-EXPR)?
//  AOX-EXPR =  U-EXPR ([&^|] U-EXPR)*
//    U-EXPR = [+-!~]? TL-EXPR
//   TL-EXPR = (sin|cos|tan|log|lg|ln|...)? C-EXPR
//    C-EXPR =   number-constant  // ((0[xb]?)?[0-9a-fA-F]*\.?[0-9a-fA-F]+)
//             | '(' LAO-EXPR ')'
void evalLAO(P *p)
{
	T t;

	evalAS(p);

	t = getToken(p);

	while(t.type == TAG_TAG)
	{
		if(t.data.tag == TOK_LAND)
		{
			evalAS(p);
			printToken(p, SM_OP_LAND);
		}
		else if(t.data.tag == TOK_LOR)
		{
			evalAS(p);
			printToken(p, SM_OP_LOR);
		}
		else
		{
			break;
		}
	
		t = getToken(p);
	}

	TKN_unget(&p->tokenizer);
}

void evalAS(P *p)
{
	T t;

	evalMD(p);

	t = getToken(p);

	while(t.type == TAG_TAG)
	{
		if(t.data.tag == TOK_PLUS)
		{
			evalMD(p);
			printToken(p, SM_OP_ADD);
		}
		else if(t.data.tag == TOK_MINUS)
		{
			evalMD(p);
			printToken(p, SM_OP_SUB);
		}
		else
		{
			break;
		}
	
		t = getToken(p);
	}

	TKN_unget(&p->tokenizer);
}

void evalMD(P *p)
{
	T t;

	evalE(p);

	t = getToken(p);

	while(t.type == TAG_TAG)
	{
		if(t.data.tag == TOK_AST)
		{
			evalE(p);
			printToken(p, SM_OP_MUL);
		}
		else if(t.data.tag == TOK_SLASH)
		{
			evalE(p);
			printToken(p, SM_OP_DIV);
		}
		else if(t.data.tag == TOK_PERC)
		{
			evalE(p);
			printToken(p, SM_OP_MOD);
		}
		else if(t.data.tag == TOK_SHL)
		{
			evalE(p);
			printToken(p, SM_OP_SHL);
		}
		else if(t.data.tag == TOK_SHR)
		{
			evalE(p);
			printToken(p, SM_OP_SHR);
		}
		else
		{
			break;
		}
	
		t = getToken(p);
	}

	TKN_unget(&p->tokenizer);
}

void evalE(P *p)
{
	T t;

	evalAOX(p);

	t = getToken(p);

	if(t.type == TAG_TAG && t.data.tag == TOK_EXP)
	{
		evalE(p);
		printToken(p, SM_OP_EXP);
	}
	else
	{
		TKN_unget(&p->tokenizer);
	}
}

void evalAOX(P *p)
{
	T t;

	evalU(p);

	t = getToken(p);

	while(t.type == TAG_TAG)
	{
		if(t.data.tag == TOK_AND)
		{
			evalU(p);
			printToken(p, SM_OP_AND);
		}
		else if(t.data.tag == TOK_OR)
		{
			evalU(p);
			printToken(p, SM_OP_OR);
		}
		else if(t.data.tag == TOK_XOR)
		{
			evalU(p);
			printToken(p, SM_OP_XOR);
		}
		else
		{
			break;
		}
	
		t = getToken(p);
	}

	TKN_unget(&p->tokenizer);
}

void evalU(P *p)
{
	T t = getToken(p);

	if(t.type == TAG_TAG)
	{
		switch(t.data.tag)
		{
			case TOK_PLUS:
				evalTL(p);
				return;
			case TOK_MINUS:
				evalTL(p);
				printToken(p, SM_OP_NEG);
				return;
			case TOK_LNOT:
				evalTL(p);
				printToken(p, SM_OP_LNOT);
				return;
			case TOK_NOT:
				evalTL(p);
				printToken(p, SM_OP_NOT);
				return;
		}
	}

	TKN_unget(&p->tokenizer);

	evalTL(p);
}

void evalTL(P *p)
{
	T t = getToken(p);

	if(t.type == TAG_TAG)
	{
		switch(t.data.tag)
		{
#define TLP(i) evalC(p);printToken(p,i)
			case TOK_RUP:
				TLP(SM_OP_RUP);
				return;
			case TOK_RDOWN:
				TLP(SM_OP_RDWN);
				return;
			case TOK_ROUND:
				TLP(SM_OP_RND);
				return;
			case TOK_SIN:
				TLP(SM_OP_SIN);
				return;
			case TOK_COS:
				TLP(SM_OP_COS);
				return;
			case TOK_TAN:
				TLP(SM_OP_TAN);
				return;
			case TOK_LOG:
				TLP(SM_OP_LOG);
				return;
			case TOK_LN:
				TLP(SM_OP_LN);
				return;
#undef TLP
		}
	}

	TKN_unget(&p->tokenizer);

	evalC(p);
}

void evalC(P *p)
{
	T t;
	TOK tok;

	tok.type = SM_OP_PUSH;

	t = getToken(p);

	if(t.type == TAG_NONE) return;

	if(t.type == TAG_VALUE)
	{
		tok.val = t.data.val;
		P_print(p, tok);
	}
	else
	{
		if(t.data.tag == TOK_PI)
		{
			tok.val = PI;
			P_print(p, tok);
		}
		else if(t.data.tag == TOK_E)
		{
			tok.val = E;
			P_print(p, tok);
		}
		else if(t.data.tag == TOK_OP)
		{
			evalLAO(p);
			t = getToken(p); 
			assert(t.type==TAG_TAG&&t.data.tag==TOK_CP);
		}
		else if(t.data.tag == TOK_CP)
		{
			TKN_unget(&p->tokenizer);
		}
		else
		{
			fprintf(stderr, "ERR: Unexpected token '%d'.\nAbort.\n", t.data.tag);
			exit(1);
		}
	}
}

// # --------------------------------------------------------------------------- 

void P_init(P *this)
{
	TABLE_init(&this->symtable);
	TKN_init(&this->tokenizer);
}

void P_print(P *this, TOK t)
{
	char buf[512];

	if(t.type == SM_OP_PUSH)
	{
		sprintf(buf, "%s %.*Lf", SM_OP_Instructions(SM_OP_PUSH), *MAX_DIGITS(), t.val);
	}
	else
	{
		sprintf(buf, "%s", SM_OP_Instructions(t.type));
	}

	(*pFn())(buf);
}

void P_dispose(P *this)
{
	TABLE_dispose(&this->symtable);
	TKN_dispose(&this->tokenizer);
}

logfPtr *pFn(void)
{
	static logfPtr ptr = stdEvalPrint;

	return &ptr;
}

