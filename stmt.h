// stmt.h
#ifndef STMT_H
#define STMT_H

#include <memory>
#include <string>
#include "expr.h"

enum class Flow { NORMAL, BREAK, CONTINUE, RETURN };

struct [[nodiscard]] ExecResult {
	Flow flow = Flow::NORMAL;
	Value value{};
};

struct Stmt {
	virtual ~Stmt() = default;
	virtual ExecResult execute(Environment& env) const = 0;
	virtual std::string toString() const = 0;
};

struct ExprStmt : Stmt {
	std::unique_ptr<Expr> expr;
	bool print;
	
	ExprStmt(std::unique_ptr<Expr> e, bool p) : expr(std::move(e)), print(p) {}
	
	ExecResult execute(Environment& env) const override;
	std::string toString() const override;
};

struct BlockStmt : Stmt {
	std::vector<std::unique_ptr<Stmt>> statements;
	
	explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> s) : statements(std::move(s)) {}
	
	ExecResult execute(Environment& env) const override;
	std::string toString() const override;
};

struct IfStmt : Stmt {
	std::unique_ptr<Expr> condition;
	std::unique_ptr<BlockStmt> thenBranch;
	std::unique_ptr<Stmt> elseBranch;
	
	IfStmt(std::unique_ptr<Expr> c, std::unique_ptr<BlockStmt> t, std::unique_ptr<Stmt> e) : condition(std::move(c)), thenBranch(std::move(t)), elseBranch(std::move(e)) {}
	
	ExecResult execute(Environment& env) const override;
	std::string toString() const override;
};

struct WhileStmt : Stmt {
	std::unique_ptr<Expr> condition;
	std::unique_ptr<BlockStmt> body;
	
	WhileStmt(std::unique_ptr<Expr> c, std::unique_ptr<BlockStmt> b) : condition(std::move(c)), body(std::move(b)) {}
	
	ExecResult execute(Environment& env) const override;
	std::string toString() const override;
};

struct BreakStmt : Stmt {
	ExecResult execute(Environment& env) const override;
	std::string toString() const override;
};

struct ContinueStmt : Stmt {
	ExecResult execute(Environment& env) const override;
	std::string toString() const override;
};

#endif