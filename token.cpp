#include "token.h"

std::string opSymbol(TokenType op) {
	switch(op) {
		case TokenType::PLUS: return "+";
		case TokenType::MINUS: return "-";
		case TokenType::ASTR: return "*";
		case TokenType::SLASH: return "/";
		case TokenType::CARET: return "^";
		case TokenType::LESSER: return "<";
		case TokenType::LESSER_EQUAL: return "<=";
		case TokenType::GREATER: return ">";
		case TokenType::GREATER_EQUAL: return ">=";
		case TokenType::EQUAL_EQUAL: return "==";
		case TokenType::NOT_EQUAL: return "!=";
		case TokenType::AND: return "&&";
		case TokenType::OR: return "||";
		case TokenType::NOT: return "!";
		default: return "?";
	}
}

std::string tokenTypeName(TokenType t) {
	switch(t) {
		case TokenType::IDENT: return "IDENT";
		case TokenType::NUM: return "NUM";
		case TokenType::PLUS: return "PLUS";
		case TokenType::MINUS: return "MINUS";
		case TokenType::ASTR: return "ASTR";
		case TokenType::SLASH: return "SLASH";
		case TokenType::LPAREN: return "LPAREN";
		case TokenType::RPAREN: return "RPAREN";
		case TokenType::LBRACK: return "LBRACK";
		case TokenType::RBRACK: return "RBRACK";
		case TokenType::LBRACE: return "LBRACE";
		case TokenType::RBRACE: return "RBRACE";
		case TokenType::SEMI: return "SEMI";
		case TokenType::CARET: return "CARET";
		case TokenType::ASSIGN: return "ASSIGN";
		case TokenType::IF: return "IF";
		case TokenType::ELSE: return "ELSE";
		case TokenType::WHILE: return "WHILE";
		case TokenType::INT: return "INT";
		case TokenType::FLOAT: return "FLOAT";
		case TokenType::STR: return "STR";
		case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
		case TokenType::LESSER: return "LESSER";
		case TokenType::LESSER_EQUAL: return "LESSER_EQUAL";
		case TokenType::GREATER: return "GREATER";
		case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
		case TokenType::NOT_EQUAL: return "NOT_EQUAL";
		case TokenType::NOT: return "NOT";
		case TokenType::AND: return "AND";
		case TokenType::OR: return "OR";
		case TokenType::NEWLINE: return "NEWLINE";
		case TokenType::END: return "END";
		default: return "ERROR";
	}
}