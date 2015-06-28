from MiniPParser import MiniPParser
from MiniPVisitor import MiniPVisitor

class Interpreter(MiniPVisitor):
	def __init__(self):
		self.vars = {}
		self.ops = {
			MiniPParser.PLUS:  lambda l, r : l + r,
			MiniPParser.MINUS: lambda l, r : l - r,
			MiniPParser.MUL:   lambda l, r : l * r,
			MiniPParser.DIV:   lambda l, r : l / r,
			MiniPParser.LT:    lambda l, r : l < r,
			MiniPParser.LE:    lambda l, r : l <= r,
			MiniPParser.GT:    lambda l, r : l > r,
			MiniPParser.GE:    lambda l, r : l >= r,
			MiniPParser.EQ:    lambda l, r : l == r,
			MiniPParser.NE:    lambda l, r : not (l == r),
			MiniPParser.AND:   lambda l, r : l and r,
			MiniPParser.OR:    lambda l, r : l or r,
			}

	def visitDecl(self, ctx):
		ids = ctx.ID()
		type = ctx.datatype.text
		for id in ids:
#			print('registering var \'%s\'' % id.getText())
			self.vars[id.getText()] = None

	def visitAssignment(self, ctx):
		id = ctx.ID().getText()
		value = self.visit(ctx.expr())
		self.vars[id] = value
#		print('trying to assign \'%s\' (%s) to %s in line %d' % (ctx.expr().getText(), str(value), id, ctx.start.line))
#		print('Assigned \'%s\' value %s' % (id, str(value)))

	def visitIfStatement(self, ctx):
		if self.visit(ctx.expr()):
			self.visit(ctx.statements(0))
		elif len(ctx.statements()) > 1:
			self.visit(ctx.statements(1))

	def visitWhileStatement(self, ctx):
		while self.visit(ctx.expr()):
			for s in ctx.statement():
				self.visit(s)
	
	def visitRepeatStatement(self, ctx):
		while True:
			for s in ctx.statement():
				self.visit(s)
			if self.visit(ctx.expr()):
				break

	def visitReadStatement(self, ctx):
		id = ctx.ID().getText()
		s = input('%s = ' % id)
		self.vars[id] = s

	def visitPrintlnStatement(self, ctx):
		v = self.visit(ctx.expr())
		if type(v) is float:
			print('%.3f' % v)
		else:
			print(v)

	def visitUnaryExpr(self, ctx):
		return -self.visit(ctx.expr())

	def visitMulExpr(self, ctx):
		return self.binOp(ctx)

	def visitAddExpr(self, ctx):
		return self.binOp(ctx)

	def visitRelExpr(self, ctx):
		return self.binOp(ctx)

	def visitEqExpr(self, ctx):
		return self.binOp(ctx)

	def visitAndExpr(self, ctx):
		return self.binOp(ctx)

	def visitOrExpr(self, ctx):
		return self.binOp(ctx)

	def binOp(self, ctx):
		l = self.visit(ctx.expr(0))
		r = self.visit(ctx.expr(1))
		return self.ops[ctx.op.type](l, r)

	def visitParExpr(self, ctx):
		return self.visit(ctx.expr())

	def visitStrExpr(self, ctx):
		s = ctx.STR().getText()[1:-1]
#		print('read string \'%s\'' % s)
		return s

	def visitNumExpr(self, ctx):
		v = float(ctx.val.text)
#		print('read number %f' % v)
		return v

	def visitIdExpr(self, ctx):
		v = self.vars[ctx.ID().getText()]
		if v is None:
			print('empty var %s' % ctx.ID().getText())
#		print('read var \'%s\' containing \'%s\'' % (ctx.ID().getText(), str(v)))
		return v

