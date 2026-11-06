// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "token.h"
#include "expr.h"
#include "stmt.h"

class Parser {
	public:
		explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {};
		std::vector<std::unique_ptr<Stmt>> parse();
	
	private:
		std::vector<Token> tokens;
		size_t current = 0;
		int loopDepth = 0;
		bool print = true;
		
		const Token& peek() const;
		const Token& peekNext() const;
		const Token& previous() const;
		const Token& advance();
		bool check(TokenType t) const;
		bool match(TokenType t);
		const Token& expect(TokenType t, const std::string& msg);
		bool endStatement();
		
		std::unique_ptr<Stmt> statement();
		std::unique_ptr<Stmt> expressionStatement();
		std::unique_ptr<Stmt> ifStatement();
		std::unique_ptr<Stmt> whileStatement();
		std::unique_ptr<BlockStmt> block();
		std::unique_ptr<Expr> assignment();
		std::unique_ptr<Expr> expression();
		std::unique_ptr<Expr> logicOr();
		std::unique_ptr<Expr> logicAnd();
		std::unique_ptr<Expr> equality();
		std::unique_ptr<Expr> comparison();
		std::unique_ptr<Expr> additive();
		std::unique_ptr<Expr> term();
		std::unique_ptr<Expr> unary();
		std::unique_ptr<Expr> power();
		std::unique_ptr<Expr> primary();
};

#endif