// stmt.h
#ifndef STMT_H
#define STMT_H

#include <memory>
#include <string>
#include "expr.h"

struct Stmt {
	virtual ~Stmt() = default;
	virtual void execute(Environment& env) const = 0;
	virtual std::string toString() const = 0;
};

struct ExprStmt : Stmt {
	std::unique_ptr<Expr> expr;
	bool print;
	
	ExprStmt(std::unique_ptr<Expr> e, bool p) : expr(std::move(e)), print(p) {}
	
	void execute(Environment& env) const override;
	std::string toString() const override;
};

struct BlockStmt : Stmt {
	std::vector<std::unique_ptr<Stmt>> statements;
	
	explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> s) : statements(std::move(s)) {}
	
	void execute(Environment& env) const override;
	std::string toString() const override;
};

struct IfStmt : Stmt {
	std::unique_ptr<Expr> condition;
	std::unique_ptr<BlockStmt> thenBranch;
	std::unique_ptr<Stmt> elseBranch;
	
	IfStmt(std::unique_ptr<Expr> c, std::unique_ptr<BlockStmt> t, std::unique_ptr<Stmt> e) : condition(std::move(c)), thenBranch(std::move(t)), elseBranch(std::move(e)) {}
	
	void execute(Environment& env) const override;
	std::string toString() const override;
};

#endif