import sys
from antlr4 import *
from MiniPLexer import MiniPLexer
from MiniPParser import MiniPParser
from Interpreter import Interpreter

def main(args):
	input = FileStream(args[1])
	lexer = MiniPLexer(input)
	stream = CommonTokenStream(lexer)
	parser = MiniPParser(stream)
	tree = parser.program()

	interpreter = Interpreter()
	interpreter.visit(tree)

if __name__ == '__main__':
	main(sys.argv)

