// $ANTLR 2.7.7 (20111217): "xl.g" -> "XLParser.java"$

import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;

import antlr.TokenBuffer;
import antlr.TokenStreamException;
import antlr.TokenStreamIOException;
import antlr.ANTLRException;
import antlr.LLkParser;
import antlr.Token;
import antlr.TokenStream;
import antlr.RecognitionException;
import antlr.NoViableAltException;
import antlr.MismatchedTokenException;
import antlr.SemanticException;
import antlr.ParserSharedInputState;
import antlr.collections.impl.BitSet;

public class XLParser extends antlr.LLkParser       implements XLLexerTokenTypes
 {

  // Define a main
  public static void main(String[] args) {
    // Use a try/catch block for parser exceptions
    try {
      // if we have at least one command-line argument
      if (args.length > 0 ) {
        System.err.println("Parsing...");

        // for each directory/file specified on the command line
        for(int i=0; i< args.length;i++)
          doFile(new File(args[i])); // parse it
      }
      else
        System.err.println("Usage: java XLRecogizer <directory name>");

    }
    catch(Exception e) {
      System.err.println("exception: "+e);
      e.printStackTrace(System.err);   // so we can get stack trace
    }
  }


  // This method decides what action to take based on the type of
  //   file we are looking at
  public static void doFile(File f) throws Exception {
    // If this is a directory, walk each file/dir in that directory
    if (f.isDirectory()) {
      String files[] = f.list();
      for(int i=0; i < files.length; i++)
        doFile(new File(f, files[i]));
    }

    // otherwise, if this is a java file, parse it!
    else if ((f.getName().length()>3) &&
             f.getName().substring(f.getName().length()-3).equals(".xl")) {
      System.err.println("-------------------------------------------");
      System.err.println(f.getAbsolutePath());
      parseFile(new FileInputStream(f));
    }
  }

  // Here's where we do the real work...
  public static void parseFile(InputStream s) throws Exception {
    try {
      // Create a scanner that reads from the input stream passed to us
      XLLexer lexer = new XLLexer(s);

      // Create a parser that reads from the scanner
      XLParser parser = new XLParser(lexer);

      // start parsing at the compilationUnit rule
      parser.program();
    }
    catch (Exception e) {
      System.err.println("parser exception: "+e);
      e.printStackTrace();   // so we can get stack trace		
    }
  }

protected XLParser(TokenBuffer tokenBuf, int k) {
  super(tokenBuf,k);
  tokenNames = _tokenNames;
}

public XLParser(TokenBuffer tokenBuf) {
  this(tokenBuf,2);
}

protected XLParser(TokenStream lexer, int k) {
  super(lexer,k);
  tokenNames = _tokenNames;
}

public XLParser(TokenStream lexer) {
  this(lexer,2);
}

public XLParser(ParserSharedInputState state) {
  super(state,2);
  tokenNames = _tokenNames;
}

	public final void program() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_program);
			match(IDENT);
			match(EQUALS);
			subprogramBody();
			match(DOT);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_0);
		}
	}
	
	public final void subprogramBody() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			{
			_loop44:
			do {
				if (((LA(1) >= LITERAL_var && LA(1) <= LITERAL_type))) {
					basicDecl();
				}
				else {
					break _loop44;
				}
				
			} while (true);
			}
			{
			_loop46:
			do {
				if ((LA(1)==LITERAL_procedure)) {
					procedureDecl();
				}
				else {
					break _loop46;
				}
				
			} while (true);
			}
			match(LITERAL_begin);
			{
			_loop48:
			do {
				if ((_tokenSet_1.member(LA(1)))) {
					statement();
				}
				else {
					break _loop48;
				}
				
			} while (true);
			}
			match(LITERAL_end);
			match(IDENT);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_2);
		}
	}
	
	public final void basicDecl() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			switch ( LA(1)) {
			case LITERAL_var:
			{
				varDecl();
				break;
			}
			case LITERAL_constant:
			{
				constDecl();
				break;
			}
			case LITERAL_type:
			{
				typeDecl();
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_3);
		}
	}
	
	public final void procedureDecl() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_procedure);
			match(IDENT);
			{
			switch ( LA(1)) {
			case LPAREN:
			{
				formalParameters();
				break;
			}
			case EQUALS:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(EQUALS);
			subprogramBody();
			match(SEMI);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_4);
		}
	}
	
	public final void statement() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			switch ( LA(1)) {
			case LITERAL_exit:
			{
				exitStatement();
				break;
			}
			case LITERAL_return:
			{
				returnStatement();
				break;
			}
			case LITERAL_if:
			{
				ifStatement();
				break;
			}
			case LITERAL_while:
			case LITERAL_loop:
			{
				loopStatement();
				break;
			}
			case LITERAL_put:
			case LITERAL_get:
			case LITERAL_newLine:
			case LITERAL_skipLine:
			{
				ioStatement();
				break;
			}
			default:
				if ((LA(1)==IDENT) && (LA(2)==SEMI||LA(2)==LPAREN)) {
					procedureCallStatement();
				}
				else if ((LA(1)==IDENT) && (LA(2)==DOT||LA(2)==BECOMES||LA(2)==LBRACKET)) {
					assignmentStatement();
				}
			else {
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_5);
		}
	}
	
	public final void varDecl() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_var);
			identList();
			match(COLON);
			typeName();
			{
			switch ( LA(1)) {
			case BECOMES:
			{
				match(BECOMES);
				constantValue();
				break;
			}
			case SEMI:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(SEMI);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_3);
		}
	}
	
	public final void constDecl() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_constant);
			identList();
			match(COLON);
			typeName();
			match(BECOMES);
			constantValue();
			match(SEMI);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_3);
		}
	}
	
	public final void typeDecl() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_type);
			match(IDENT);
			match(EQUALS);
			{
			switch ( LA(1)) {
			case LITERAL_array:
			{
				arrayDecl();
				break;
			}
			case LITERAL_record:
			{
				recordDecl();
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(SEMI);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_3);
		}
	}
	
	public final void identList() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(IDENT);
			{
			_loop55:
			do {
				if ((LA(1)==COMMA)) {
					match(COMMA);
					match(IDENT);
				}
				else {
					break _loop55;
				}
				
			} while (true);
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_6);
		}
	}
	
	public final void typeName() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			switch ( LA(1)) {
			case IDENT:
			{
				match(IDENT);
				break;
			}
			case LITERAL_Integer:
			{
				match(LITERAL_Integer);
				break;
			}
			case LITERAL_Boolean:
			{
				match(LITERAL_Boolean);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_7);
		}
	}
	
	public final void constantValue() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			switch ( LA(1)) {
			case INTLIT:
			{
				match(INTLIT);
				break;
			}
			case STRINGLIT:
			{
				match(STRINGLIT);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_8);
		}
	}
	
	public final void arrayDecl() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_array);
			match(LBRACKET);
			integerConstant();
			match(DOTDOT);
			integerConstant();
			match(RBRACKET);
			match(LITERAL_of);
			typeName();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_9);
		}
	}
	
	public final void recordDecl() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_record);
			{
			int _cnt63=0;
			_loop63:
			do {
				if ((LA(1)==IDENT)) {
					identList();
					match(COLON);
					typeName();
					match(SEMI);
				}
				else {
					if ( _cnt63>=1 ) { break _loop63; } else {throw new NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt63++;
			} while (true);
			}
			match(LITERAL_end);
			match(LITERAL_record);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_9);
		}
	}
	
	public final void integerConstant() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			switch ( LA(1)) {
			case INTLIT:
			{
				match(INTLIT);
				break;
			}
			case IDENT:
			{
				match(IDENT);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_10);
		}
	}
	
	public final void formalParameters() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LPAREN);
			parameterSpec();
			{
			_loop69:
			do {
				if ((LA(1)==SEMI)) {
					match(SEMI);
					parameterSpec();
				}
				else {
					break _loop69;
				}
				
			} while (true);
			}
			match(RPAREN);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_11);
		}
	}
	
	public final void parameterSpec() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case LITERAL_var:
			{
				match(LITERAL_var);
				break;
			}
			case IDENT:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			identList();
			match(COLOR);
			typeName();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_12);
		}
	}
	
	public final void exitStatement() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_exit);
			match(LITERAL_when);
			expression();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_5);
		}
	}
	
	public final void returnStatement() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_return);
			match(SEMI);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_5);
		}
	}
	
	public final void ifStatement() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LITERAL_if);
			expression();
			match(LITERAL_then);
			{
			_loop84:
			do {
				if ((_tokenSet_1.member(LA(1)))) {
					statement();
				}
				else {
					break _loop84;
				}
				
			} while (true);
			}
			{
			_loop88:
			do {
				if ((LA(1)==LITERAL_elsif)) {
					match(LITERAL_elsif);
					expression();
					match(LITERAL_then);
					{
					_loop87:
					do {
						if ((_tokenSet_1.member(LA(1)))) {
							statement();
						}
						else {
							break _loop87;
						}
						
					} while (true);
					}
				}
				else {
					break _loop88;
				}
				
			} while (true);
			}
			{
			switch ( LA(1)) {
			case LITERAL_else:
			{
				match(LITERAL_else);
				{
				_loop91:
				do {
					if ((_tokenSet_1.member(LA(1)))) {
						statement();
					}
					else {
						break _loop91;
					}
					
				} while (true);
				}
				break;
			}
			case LITERAL_end:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(LITERAL_end);
			match(LITERAL_if);
			match(SEMI);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_5);
		}
	}
	
	public final void loopStatement() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			{
			switch ( LA(1)) {
			case LITERAL_while:
			{
				match(LITERAL_while);
				expression();
				break;
			}
			case LITERAL_loop:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(LITERAL_loop);
			{
			_loop95:
			do {
				if ((_tokenSet_1.member(LA(1)))) {
					statement();
				}
				else {
					break _loop95;
				}
				
			} while (true);
			}
			match(LITERAL_end);
			match(LITERAL_loop);
			match(SEMI);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_5);
		}
	}
	
	public final void ioStatement() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			switch ( LA(1)) {
			case LITERAL_put:
			{
				match(LITERAL_put);
				match(LPAREN);
				expression();
				match(RPAREN);
				match(SEMI);
				break;
			}
			case LITERAL_get:
			{
				match(LITERAL_get);
				match(LPAREN);
				variableReference();
				match(RPAREN);
				match(SEMI);
				break;
			}
			case LITERAL_newLine:
			{
				match(LITERAL_newLine);
				{
				switch ( LA(1)) {
				case LPAREN:
				{
					match(LPAREN);
					match(RPAREN);
					break;
				}
				case SEMI:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				match(SEMI);
				break;
			}
			case LITERAL_skipLine:
			{
				match(LITERAL_skipLine);
				{
				switch ( LA(1)) {
				case LPAREN:
				{
					match(LPAREN);
					match(RPAREN);
					break;
				}
				case SEMI:
				{
					break;
				}
				default:
				{
					throw new NoViableAltException(LT(1), getFilename());
				}
				}
				}
				match(SEMI);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_5);
		}
	}
	
	public final void procedureCallStatement() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(IDENT);
			{
			switch ( LA(1)) {
			case LPAREN:
			{
				actualParameters();
				break;
			}
			case SEMI:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(SEMI);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_5);
		}
	}
	
	public final void assignmentStatement() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			variableReference();
			match(BECOMES);
			expression();
			match(SEMI);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_5);
		}
	}
	
	public final void variableReference() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(IDENT);
			{
			_loop125:
			do {
				switch ( LA(1)) {
				case LBRACKET:
				{
					match(LBRACKET);
					expression();
					match(RBRACKET);
					break;
				}
				case DOT:
				{
					match(DOT);
					match(IDENT);
					break;
				}
				default:
				{
					break _loop125;
				}
				}
			} while (true);
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_13);
		}
	}
	
	public final void expression() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			relationalExpression();
			{
			_loop122:
			do {
				if ((LA(1)==LITERAL_and||LA(1)==LITERAL_or)) {
					{
					switch ( LA(1)) {
					case LITERAL_and:
					{
						match(LITERAL_and);
						break;
					}
					case LITERAL_or:
					{
						match(LITERAL_or);
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					relationalExpression();
				}
				else {
					break _loop122;
				}
				
			} while (true);
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_14);
		}
	}
	
	public final void actualParameters() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			match(LPAREN);
			{
			switch ( LA(1)) {
			case INTLIT:
			case STRINGLIT:
			case IDENT:
			case LPAREN:
			case PLUS:
			case MINUS:
			case LITERAL_not:
			{
				expression();
				{
				_loop80:
				do {
					if ((LA(1)==COMMA)) {
						match(COMMA);
						expression();
					}
					else {
						break _loop80;
					}
					
				} while (true);
				}
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(RPAREN);
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_9);
		}
	}
	
	public final void primitiveElement() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			switch ( LA(1)) {
			case INTLIT:
			case STRINGLIT:
			{
				constantValue();
				break;
			}
			case IDENT:
			{
				variableReference();
				break;
			}
			case LPAREN:
			{
				match(LPAREN);
				expression();
				match(RPAREN);
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_8);
		}
	}
	
	public final void booleanNegationExpression() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			{
			_loop102:
			do {
				if ((LA(1)==LITERAL_not)) {
					match(LITERAL_not);
				}
				else {
					break _loop102;
				}
				
			} while (true);
			}
			primitiveElement();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_8);
		}
	}
	
	public final void signExpression() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			{
			_loop106:
			do {
				if ((LA(1)==PLUS||LA(1)==MINUS)) {
					{
					switch ( LA(1)) {
					case PLUS:
					{
						match(PLUS);
						break;
					}
					case MINUS:
					{
						match(MINUS);
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
				}
				else {
					break _loop106;
				}
				
			} while (true);
			}
			booleanNegationExpression();
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_8);
		}
	}
	
	public final void multiplyExpression() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			signExpression();
			{
			_loop110:
			do {
				if ((LA(1)==TIMES||LA(1)==DIV||LA(1)==LITERAL_mod)) {
					{
					switch ( LA(1)) {
					case TIMES:
					{
						match(TIMES);
						break;
					}
					case DIV:
					{
						match(DIV);
						break;
					}
					case LITERAL_mod:
					{
						match(LITERAL_mod);
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					signExpression();
				}
				else {
					break _loop110;
				}
				
			} while (true);
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_15);
		}
	}
	
	public final void addingExpression() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			multiplyExpression();
			{
			_loop114:
			do {
				if ((LA(1)==PLUS||LA(1)==MINUS)) {
					{
					switch ( LA(1)) {
					case PLUS:
					{
						match(PLUS);
						break;
					}
					case MINUS:
					{
						match(MINUS);
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					multiplyExpression();
				}
				else {
					break _loop114;
				}
				
			} while (true);
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_16);
		}
	}
	
	public final void relationalExpression() throws RecognitionException, TokenStreamException {
		
		
		try {      // for error handling
			addingExpression();
			{
			_loop118:
			do {
				if ((_tokenSet_17.member(LA(1)))) {
					{
					switch ( LA(1)) {
					case EQUALS:
					{
						match(EQUALS);
						break;
					}
					case NOT_EQUALS:
					{
						match(NOT_EQUALS);
						break;
					}
					case GT:
					{
						match(GT);
						break;
					}
					case GTE:
					{
						match(GTE);
						break;
					}
					case LT:
					{
						match(LT);
						break;
					}
					case LTE:
					{
						match(LTE);
						break;
					}
					default:
					{
						throw new NoViableAltException(LT(1), getFilename());
					}
					}
					}
					addingExpression();
				}
				else {
					break _loop118;
				}
				
			} while (true);
			}
		}
		catch (RecognitionException ex) {
			reportError(ex);
			recover(ex,_tokenSet_18);
		}
	}
	
	
	public static final String[] _tokenNames = {
		"<0>",
		"EOF",
		"<2>",
		"NULL_TREE_LOOKAHEAD",
		"WS",
		"COMMENT",
		"INTLIT",
		"CHARLIT",
		"STRINGLIT",
		"IDENT",
		"DOT",
		"BECOMES",
		"COLON",
		"SEMI",
		"COMMA",
		"EQUALS",
		"LBRACKET",
		"RBRACKET",
		"DOTDOT",
		"LPAREN",
		"RPAREN",
		"NOT_EQUALS",
		"LT",
		"LTE",
		"GT",
		"GTE",
		"PLUS",
		"MINUS",
		"TIMES",
		"DIV",
		"\"program\"",
		"\"begin\"",
		"\"end\"",
		"\"var\"",
		"\"constant\"",
		"\"type\"",
		"\"array\"",
		"\"of\"",
		"\"record\"",
		"\"Integer\"",
		"\"Boolean\"",
		"\"procedure\"",
		"COLOR",
		"\"exit\"",
		"\"when\"",
		"\"return\"",
		"\"if\"",
		"\"then\"",
		"\"elsif\"",
		"\"else\"",
		"\"while\"",
		"\"loop\"",
		"\"put\"",
		"\"get\"",
		"\"newLine\"",
		"\"skipLine\"",
		"\"not\"",
		"\"mod\"",
		"\"and\"",
		"\"or\""
	};
	
	private static final long[] mk_tokenSet_0() {
		long[] data = { 2L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_0 = new BitSet(mk_tokenSet_0());
	private static final long[] mk_tokenSet_1() {
		long[] data = { 71046043340374528L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_1 = new BitSet(mk_tokenSet_1());
	private static final long[] mk_tokenSet_2() {
		long[] data = { 9216L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_2 = new BitSet(mk_tokenSet_2());
	private static final long[] mk_tokenSet_3() {
		long[] data = { 2261300281344L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_3 = new BitSet(mk_tokenSet_3());
	private static final long[] mk_tokenSet_4() {
		long[] data = { 2201170739200L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_4 = new BitSet(mk_tokenSet_4());
	private static final long[] mk_tokenSet_5() {
		long[] data = { 71890472565473792L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_5 = new BitSet(mk_tokenSet_5());
	private static final long[] mk_tokenSet_6() {
		long[] data = { 4398046515200L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_6 = new BitSet(mk_tokenSet_6());
	private static final long[] mk_tokenSet_7() {
		long[] data = { 1058816L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_7 = new BitSet(mk_tokenSet_7());
	private static final long[] mk_tokenSet_8() {
		long[] data = { 1080837527657701888L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_8 = new BitSet(mk_tokenSet_8());
	private static final long[] mk_tokenSet_9() {
		long[] data = { 8192L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_9 = new BitSet(mk_tokenSet_9());
	private static final long[] mk_tokenSet_10() {
		long[] data = { 393216L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_10 = new BitSet(mk_tokenSet_10());
	private static final long[] mk_tokenSet_11() {
		long[] data = { 32768L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_11 = new BitSet(mk_tokenSet_11());
	private static final long[] mk_tokenSet_12() {
		long[] data = { 1056768L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_12 = new BitSet(mk_tokenSet_12());
	private static final long[] mk_tokenSet_13() {
		long[] data = { 1080837527657703936L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_13 = new BitSet(mk_tokenSet_13());
	private static final long[] mk_tokenSet_14() {
		long[] data = { 72031210055033344L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_14 = new BitSet(mk_tokenSet_14());
	private static final long[] mk_tokenSet_15() {
		long[] data = { 936722338776539648L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_15 = new BitSet(mk_tokenSet_15());
	private static final long[] mk_tokenSet_16() {
		long[] data = { 936722338575213056L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_16 = new BitSet(mk_tokenSet_16());
	private static final long[] mk_tokenSet_17() {
		long[] data = { 65044480L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_17 = new BitSet(mk_tokenSet_17());
	private static final long[] mk_tokenSet_18() {
		long[] data = { 936722338510168576L, 0L};
		return data;
	}
	public static final BitSet _tokenSet_18 = new BitSet(mk_tokenSet_18());
	
	}
