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
	if(auto i = std::get_if<int64_t>(&v)) return static_cast<double>(*i);
	if(auto d = std::get_if<double>(&v)) return *d;
	throw std::runtime_error("expected a number, got " + typeName(v));
}

bool toCondition(const Value& v) {
	if(auto b = std::get_if<bool>(&v)) return *b;
	throw std::runtime_error("expected a bool, got " + typeName(v));
}

bool isComparison(TokenType op) {
	switch(op) {
		case TokenType::EQUAL_EQUAL: return true;
		case TokenType::GREATER: return true;
		case TokenType::GREATER_EQUAL: return true;
		case TokenType::LESSER: return true;
		case TokenType::LESSER_EQUAL: return true;
		case TokenType::NOT_EQUAL: return true;
		default: return false;
	}
}

int64_t intPow(int64_t base, int64_t exp) {
	int64_t result = 1;
	while(exp-- > 0) result *= base;
	return result;
}

Value binaryOp(TokenType op, const Value& l, const Value& r) {
	if(isComparison(op)) return compareOp(op, l, r);
	
	if(std::holds_alternative<bool>(l) || std::holds_alternative<bool>(r)) {
		throw std::runtime_error("cannot do arithmetic on bool");
	}
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
		int64_t a = std::get<int64_t>(l), b = std::get<int64_t>(r);
		switch(op) {
			case TokenType::PLUS: return a + b;
			case TokenType::MINUS: return a - b;
			case TokenType::ASTR: return a * b;
			case TokenType::SLASH: return static_cast<double>(a) / b;
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
			if(auto i = std::get_if<int64_t>(&v)) return -*i;
			if(auto d = std::get_if<double>(&v)) return -*d;
			throw std::runtime_error("cannot negate a bool");
		case TokenType::NOT:
			return !toCondition(v);
		default:
			throw std::runtime_error("unary operator not supported for " + typeName(v));
	}
}

Value compareOp(TokenType op, const Value& l, const Value& r) {
	bool lBool = std::holds_alternative<bool>(l);
	bool rBool = std::holds_alternative<bool>(r);
	
	if(lBool && rBool) {
		bool a = std::get<bool>(l), b = std::get<bool>(r);
		if(op == TokenType::EQUAL_EQUAL) return a == b;
		if(op == TokenType::NOT_EQUAL) return a != b;
		throw std::runtime_error("cannot order bools");
	}
	
	if(lBool || rBool) {
		throw std::runtime_error("cannot compare " + typeName(l) + " and " + typeName(r));
	}
	
	if(std::holds_alternative<double>(l) || std::holds_alternative<double>(r)) {
		double a = toDouble(l), b = toDouble(r);
		switch(op) {
			case TokenType::EQUAL_EQUAL: return a == b;
			case TokenType::NOT_EQUAL: return a != b;
			case TokenType::LESSER: return a < b;
			case TokenType::LESSER_EQUAL: return a <= b;
			case TokenType::GREATER: return a > b;
			case TokenType::GREATER_EQUAL: return a >= b;
			default: break;
		}
	} else {
		int64_t a = std::get<int64_t>(l), b = std::get<int64_t>(r);
		switch(op) {
			case TokenType::EQUAL_EQUAL: return a == b;
			case TokenType::NOT_EQUAL: return a != b;
			case TokenType::LESSER: return a < b;
			case TokenType::LESSER_EQUAL: return a <= b;
			case TokenType::GREATER: return a > b;
			case TokenType::GREATER_EQUAL: return a >= b;
			default: break;
		}
	}
	throw std::runtime_error("unknown comparison");
}