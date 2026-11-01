#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "value.h"

std::string typeName(const Value& v) {
	if(std::holds_alternative<bool>(v)) return "bool";
	if(std::holds_alternative<int64_t>(v)) return "int";
	return "float";
}

std::string formatValue(const Value& v) {
	if(auto b = std::get_if<bool>(&v)) return *b ? "true" : "false";
	if(auto i = std::get_if<int64_t>(&v)) return std::to_string(*i);
	
	double d = std::get<double>(v);
	std::ostringstream ss;
	ss << std::setprecision(15) << d;
	std::string s = ss.str();
	if(std::isfinite(d) && s.find_first_of(".e") == std::string::npos) s += ".0";
	return s;
}

double toDouble(const Value& v) {
	if(auto b = std::get_if<bool>(&v)) return *b ? 1.0 : 0.0;
	if(auto i = std::get_if<int64_t>(&v)) return static_cast<double>(*i);
	return std::get<double>(v);
}

static int64_t toInt(const Value& v) {
	if(auto b = std::get_if<bool>(&v)) return *b ? 1 : 0;
	return std::get<int64_t>(v);
}

static int64_t intPow(int64_t base, int64_t exp) {
	int64_t result = 1;
	while(exp-- > 0) result *= base;
	return result;
}

Value binaryOp(TokenType op, const Value& l, const Value& r) {
	bool useFloat = std::holds_alternative<double>(l) || std::holds_alternative<double>(r);
	
	if(useFloat) {
		double a = toDouble(l), b = toDouble(r);
		switch(op) {
			case TokenType::PLUS: return a + b;
			case TokenType::MINUS: return a - b;
			case TokenType::ASTR: return a * b;
			case TokenType::SLASH: return a / b;
			case TokenType::CARET: return std::pow(a, b);
			default: break;
		}
	} else {
		int64_t a = toInt(l), b = toInt(r);
		switch(op) {
			case TokenType::PLUS: return a + b;
			case TokenType::MINUS: return a - b;
			case TokenType::ASTR: return a * b;
			case TokenType::SLASH: 
				if(b == 0) throw std::runtime_error("integer division by zero");
				return a / b;
			case TokenType::CARET: 
				if(b < 0) return std::pow(static_cast<double>(a), static_cast<double>(b));
				return intPow(a, b);
			default: break;
		}
	}
	throw std::runtime_error("operator not supported for " + typeName(l) + " and " + typeName(r));
}

Value unaryOp(TokenType op, const Value& v) {
	switch(op) {
		case TokenType::MINUS:
			if(auto d = std::get_if<double>(&v)) return -*d;
			return -toInt(v);
		default:
			throw std::runtime_error("unary operator not supported for " + typeName(v));
	}
}