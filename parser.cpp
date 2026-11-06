#include "parser.h"

const Token& Parser::peek() const {
	return tokens[current];
}

const Token& Parser::peekNext() const {
	if(check(TokenType::END)) return peek();
	return tokens[current + 1];
}

const Token& Parser::previous() const {
	return tokens[current - 1];
}

const Token& Parser::advance() {
	const Token& tok = peek();
	if(!check(TokenType::END)) current++;
	return tok;
}

bool Parser::check(TokenType t) const {
	return peek().type == t;
}

bool Parser::match(TokenType t) {
	if(check(t)) {
		advance();
		return true;
	}
	
	return false;
}

bool Parser::endStatement() {
	if(match(TokenType::SEMI)) {
		return false;
	}
	if(match(TokenType::NEWLINE)) {
		return true;
	}
	if(check(TokenType::END) || check(TokenType::RBRACE)) {
		return true;
	}
	throw std::runtime_error("expected ';' or end of line at line " + std::to_string(peek().line) + ", column " + std::to_string(peek().col));
}

const Token& Parser::expect(TokenType t, const std::string& msg) {
	if(check(t)) return advance();
	throw std::runtime_error(msg + " at line " + std::to_string(peek().line) + ", column " + std::to_string(peek().col));
}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
	std::vector<std::unique_ptr<Stmt>> program;
	while(!check(TokenType::END)) {
		if(match(TokenType::NEWLINE) || match(TokenType::SEMI)) continue;
		program.push_back(statement());
	}
	return program;
}

std::unique_ptr<Stmt> Parser::statement() {
	if(match(TokenType::IF)) return ifStatement();
	if(match(TokenType::WHILE)) return whileStatement();
	if(match(TokenType::LBRACE)) return block();
	
	if(match(TokenType::BREAK)) {
		if(loopDepth == 0) throw std::runtime_error("'break' outside of a loop");
		endStatement();
		return std::make_unique<BreakStmt>();
	}
	
	if(match(TokenType::CONTINUE)) {
		if(loopDepth == 0) throw std::runtime_error("'continue' outside of a loop");
		endStatement();
		return std::make_unique<ContinueStmt>();
	}
	
	if(check(TokenType::ELSE)) {
		throw std::runtime_error("'else' must be on the same line as the closing '}'");
	}
	return expressionStatement();
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
	auto expr = assignment();
	bool print = endStatement();
	return std::make_unique<ExprStmt>(std::move(expr), print);
}

std::unique_ptr<BlockStmt> Parser::block() {
	std::vector<std::unique_ptr<Stmt>> statements;
	
	while(!check(TokenType::RBRACE) && !check(TokenType::END)) {
		if(match(TokenType::NEWLINE) || match(TokenType::SEMI)) continue;
		statements.push_back(statement());
	}
	
	expect(TokenType::RBRACE, "expected '}'");
	return std::make_unique<BlockStmt>(std::move(statements));
}

std::unique_ptr<Stmt> Parser::ifStatement() {
	expect(TokenType::LPAREN, "expected '(' after 'if'");
	auto condition = expression();
	expect(TokenType::RPAREN, "expected ')' after condition");
	expect(TokenType::LBRACE, "expected '{' on the same line as 'if'");
	auto thenBranch = block();
	
	std::unique_ptr<Stmt> elseBranch = nullptr;
	if(match(TokenType::ELSE)) {
		if(match(TokenType::IF)) {
			elseBranch = ifStatement();
		} else {
			expect(TokenType::LBRACE, "expected '{' on the same line as 'else'");
			elseBranch = block();
		}
	}
	
	return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::whileStatement() {
	expect(TokenType::LPAREN, "expected '(' after 'while'");
	auto condition = expression();
	expect(TokenType::RPAREN, "expected ')' after condition");
	expect(TokenType::LBRACE, "expected '{' on the same line as 'while'");
	
	loopDepth++;
	auto body = block();
	loopDepth--;
	
	return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<Expr> Parser::assignment() {
	if(check(TokenType::IDENT) && peekNext().type == TokenType::ASSIGN) {
		std::string name = advance().text;
		advance();
		auto value = expression();
		return std::make_unique<AssignExpr>(name, std::move(value));
	}
	
	return expression();
}

std::unique_ptr<Expr> Parser::expression() {
	return logicOr();
}

std::unique_ptr<Expr> Parser::logicOr() {
	auto left = logicAnd();
	while(match(TokenType::OR)) {
		auto right = logicAnd();
		left = std::make_unique<BinaryExpr>(TokenType::OR, std::move(left), std::move(right));
	}
	return left;
}

std::unique_ptr<Expr> Parser::logicAnd() {
	auto left = equality();
	while(match(TokenType::AND)) {
		auto right = equality();
		left = std::make_unique<BinaryExpr>(TokenType::AND, std::move(left), std::move(right));
	}
	return left;
}

std::unique_ptr<Expr> Parser::equality() {
	auto left = comparison();
	while(match(TokenType::EQUAL_EQUAL) || match(TokenType::NOT_EQUAL)) {
		TokenType op = previous().type;
		auto right = comparison();
		left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
	}
	return left;
}

std::unique_ptr<Expr> Parser::comparison() {
	auto left = additive();
	while(match(TokenType::LESSER) || match(TokenType::LESSER_EQUAL) || match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL)) {
		TokenType op = previous().type;
		auto right = additive();
		left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
	}
	return left;
}

std::unique_ptr<Expr> Parser::additive() {
	auto left = term();
	
	while(match(TokenType::PLUS) || match(TokenType::MINUS)) {
		TokenType op = previous().type;
		auto right = term();
		left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
	}
	
	return left;
}

std::unique_ptr<Expr> Parser::term() {
	auto left = unary();
	
	while(match(TokenType::ASTR) || match(TokenType::SLASH)) {
		TokenType op = previous().type;
		auto right = unary();
		left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
	}
	
	return left;
}

std::unique_ptr<Expr> Parser::unary() {
	if(match(TokenType::MINUS) || match(TokenType::NOT)) {
		TokenType op = previous().type;
		auto operand = unary();
		return std::make_unique<UnaryExpr>(op, std::move(operand));
	}
	return power();
}

std::unique_ptr<Expr> Parser::power() {
	auto base = primary();
	
	if(match(TokenType::CARET)) {
		auto exponent = unary();
		return std::make_unique<BinaryExpr>(TokenType::CARET, std::move(base), std::move(exponent));
	}
	
	return base;
}

std::unique_ptr<Expr> Parser::primary() {
	if(match(TokenType::NUM)) {
		const std::string& t = previous().text;
		if(t.find('.') != std::string::npos) {
			return std::make_unique<LiteralExpr>(Value(std::stod(t)));
		}
		return std::make_unique<LiteralExpr>(Value(static_cast<int64_t>(std::stoll(t))));
	}
	if(match(TokenType::TRU)) return std::make_unique<LiteralExpr>(Value(true));
	if(match(TokenType::FAL)) return std::make_unique<LiteralExpr>(Value(false));
	
	if (match(TokenType::IDENT)) {
		std::string name = previous().text;

		if (match(TokenType::LPAREN)) {
			std::vector<std::unique_ptr<Expr>> args;
			if (!check(TokenType::RPAREN)) {
				do {
					args.push_back(expression());
				} while (match(TokenType::COMMA));
			}
			expect(TokenType::RPAREN, "expected ')' after arguments");
			return std::make_unique<CallExpr>(name, std::move(args));
		}

		return std::make_unique<VariableExpr>(name);
	}
	
	if(match(TokenType::LPAREN)) {
		auto inner = expression();
		expect(TokenType::RPAREN, "expected ')'");
		return inner;
	}
	
	throw std::runtime_error("expected expression at at line " + std::to_string(peek().line) + ", column " + std::to_string(peek().col));
}