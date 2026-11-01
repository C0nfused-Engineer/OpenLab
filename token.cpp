#include "token.h"

std::string opSymbol(TokenType op) {
	switch(op) {
		case TokenType::PLUS: return "+";
		case TokenType::MINUS: return "-";
		case TokenType::ASTR: return "*";
		case TokenType::SLASH: return "/";
		case TokenType::CARET: return "^";
		default: return "?";
	}
}

std::string tokenTypeName(TokenType t) {
	switch(t) {
		case TokenType::IDENT: return "IDENT"; break;
		case TokenType::NUM: return "NUM"; break;
		case TokenType::PLUS: return "PLUS"; break;
		case TokenType::MINUS: return "MINUS"; break;
		case TokenType::ASTR: return "ASTR"; break;
		case TokenType::SLASH: return "SLASH"; break;
		case TokenType::LPAREN: return "LPAREN"; break;
		case TokenType::RPAREN: return "RPAREN"; break;
		case TokenType::LBRACK: return "LBRACK"; break;
		case TokenType::RBRACK: return "RBRACK"; break;
		case TokenType::LBRACE: return "LBRACE"; break;
		case TokenType::RBRACE: return "RBRACE"; break;
		case TokenType::SEMI: return "SEMI"; break;
		case TokenType::CARET: return "CARET"; break;
		case TokenType::ASSIGN: return "ASSIGN"; break;
		case TokenType::IF: return "IF"; break;
		case TokenType::ELSE: return "ELSE"; break;
		case TokenType::WHILE: return "WHILE"; break;
		case TokenType::INT: return "INT"; break;
		case TokenType::FLOAT: return "FLOAT"; break;
		case TokenType::STR: return "STR"; break;
		case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL"; break;
		case TokenType::END: return "END"; break;
		default: return "ERROR"; break;
	}
}