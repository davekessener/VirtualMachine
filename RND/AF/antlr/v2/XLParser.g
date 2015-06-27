class XLParser extends Parser;

options {
	defaultErrorHandler = true;
	k = 2;
}


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
}

program
	: "program" IDENT EQUALS subprogramBody DOT
	;

subprogramBody
	: (basicDecl)* (procedureDecl)* "begin" (statement)* "end" IDENT
	;

basicDecl
	: varDecl
	| constDecl
	| typeDecl
	;

varDecl
	: "var" identList COLON typeName (BECOMES constantValue)? SEMI
	;

constDecl
	: "constant" identList COLON typeName BECOMES constantValue SEMI
	;

identList
	: IDENT (COMMA IDENT)*
	;

constantValue
	: INTLIT
	| STRINGLIT
	| IDENT
	;

typeDecl
	: "type" IDENT EQUALS (arrayDecl | recordDecl) SEMI
	;

arrayDecl
	: "array" LBRACKET integerConstant DOTDOT integerConstant RBRACKET "of" typeName
	;

integerConstant
	: INTLIT
	| IDENT
	;

recordDecl
	: "record" (identList COLON typeName SEMI)+ "end" "record"
	;

typeName
	: IDENT
	| "Integer"
	| "Boolean"
	;

procedureDecl
	: "procedure" IDENT (formalParameters)? EQUALS subprogramBody SEMI
	;

formalParameters
	: LPAREN parameterSpec (SEMI parameterSpec)* RPAREN
	;

parameterSpec
	: ("var")? identList COLOR typeName
	;

statement
	: exitStatement
	| returnStatement
	| ifStatement
	| loopStatement
	| ioStatement
	| procedureCallStatement
	| assignmentStatement
	;

assignmentStatement
	: variableReference BECOMES expression SEMI
	;

exitStatement
	: "exit" "when" expression
	;

procedureCallStatement
	: IDENT (actualParameters)? SEMI
	;

actualParameters
	: LPAREN (expression (COMMA expression)*)? RPAREN
	;

returnStatement
	: "return" SEMI
	;

ifStatement
	: "if" expression "then"
		(statement)*
	  ("elsif" expression "then"
	  	(statement)*
	  )*
	  ("else"
	  	(statement)*
	  )?
	  "end" "if" SEMI
	;

loopStatement
	: ("while" expression)? "loop"
		(statement)*
	  "end" "loop" SEMI
	;

ioStatement
	: "put" LPAREN expression RPAREN SEMI
	| "get" LPAREN variableReference RPAREN SEMI
	| "newLine" (LPAREN RPAREN)? SEMI
	| "skipLine" (LPAREN RPAREN)? SEMI
	;

primitiveElement
	: constantValue
	| variableReference
	| LPAREN expression RPAREN
	;

booleanNegationExpression
	: ("not")* primitiveElement
	;

signExpression
	: ((PLUS|MINUS))* booleanNegationExpression
	;

multiplyExpression
	: signExpression ((TIMES|DIV|"mod") signExpression)*
	;

addingExpression
	: multiplyExpression ((PLUS|MINUS) multiplyExpression)*
	;

relationalExpression
	: addingExpression ((EQUALS|NOT_EQUALS|GT|GTE|LT|LTE) addingExpression)*
	;

expression
	: relationalExpression (("and"|"or") relationalExpression)*
	;


variableReference
	: IDENT (LBRACKET expression RBRACKET | DOT IDENT)*
	;

