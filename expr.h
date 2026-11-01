// expr.h
#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "token.h"
#include "value.h"

using Environment = std::unordered_map<std::string, Value>;

struct Expr {
	virtual ~Expr() = default;
	virtual std::string toString() const = 0;
	virtual Value eval(Environment& env) const = 0;
};

struct BinaryExpr : Expr {
	TokenType op;
	std::unique_ptr<Expr> left, right;
	
	BinaryExpr(TokenType op, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : op(op), left(std::move(l)), right(std::move(r)) {}
	
	std::string toString() const override;
	
	Value eval(Environment& env) const override;
};

struct VariableExpr : Expr {
	std::string name;
	
	explicit VariableExpr(std::string n) : name(n) {}
	
	std::string toString() const override;
	
	Value eval(Environment& env) const override;
};

struct UnaryExpr : Expr {
	TokenType op;
	std::unique_ptr<Expr> operand;
	
	UnaryExpr(TokenType t, std::unique_ptr<Expr> oper) : op(t), operand(std::move(oper)) {}
	
	std::string toString() const override;
	
	Value eval(Environment& env) const override;
};

struct AssignExpr : Expr {
	std::string name;
	std::unique_ptr<Expr> value;
	
	AssignExpr(std::string name, std::unique_ptr<Expr> val) : name(std::move(name)), value(std::move(val)) {}
	
	std::string toString() const override;
	
	Value eval(Environment& env) const override;
};

struct CallExpr : Expr {
	std::string name;
	std::vector<std::unique_ptr<Expr>> args;
	
	CallExpr(std::string n, std::vector<std::unique_ptr<Expr>> a) : name(std::move(n)), args(std::move(a)) {}
	
	std::string toString() const override;
	
	Value eval(Environment& env) const override;
};

struct LiteralExpr : Expr {
	Value value;
	
	explicit LiteralExpr(Value v) : value(std::move(v)) {}
	
	std::string toString() const override;
	
	Value eval(Environment& env) const override;
};

#endif