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

bool Parser::shouldPrint() const {
	return print;
}

const Token& Parser::expect(TokenType t, const std::string& msg) {
	if(check(t)) return advance();
	throw std::runtime_error(msg + " at column " + std::to_string(peek().col));
}

std::unique_ptr<Expr> Parser::parse() {
	auto tree = statement();
	
	print = !match(TokenType::SEMI);
	
	expect(TokenType::END, "unexpected token");
	
	return tree;
}

std::unique_ptr<Expr> Parser::statement() {
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
	
	throw std::runtime_error("expected expression at column " + std::to_string(peek().col));
}