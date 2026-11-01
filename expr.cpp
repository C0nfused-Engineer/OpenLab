#include "expr.h"
#include "builtins.h"
#include <sstream>
#include <cmath>
#include <stdexcept>

std::string BinaryExpr::toString() const {
	return "(" + opSymbol(op) + " " + left->toString() + " " + right->toString() + ")";
}

Value BinaryExpr::eval(Environment& env) const {
	return binaryOp(op, left->eval(env), right->eval(env));
}

std::string VariableExpr::toString() const {
	return name;
}

Value VariableExpr::eval(Environment& env) const {
	auto it = env.find(name);
	if(it == env.end()) {
		throw std::runtime_error("undefined variable '" + name + "'");
	}
	return it->second;
}

std::string UnaryExpr::toString() const {
	return "(" + opSymbol(op) + " " + operand->toString() + ")";
}

Value UnaryExpr::eval(Environment& env) const {
	return unaryOp(op, operand->eval(env));
}

std::string AssignExpr::toString() const {
	return "(= "  + name + " " + value->toString() + ")";
}

Value AssignExpr::eval(Environment& env) const {
	Value v = value->eval(env);
	env[name] = v;
	return v;
}

std::string CallExpr::toString() const {
	std::string s = "(" + name;
	for(const auto& arg : args) s += " " + arg->toString();
	return s + ")";
}

Value CallExpr::eval(Environment& env) const {
	const auto& table = getBuiltins();
	auto it = table.find(name);
	if(it == table.end()) {
		throw std::runtime_error("undefined function '" + name + "'");
	}
	const Builtin& b = it->second;
	
	int n = static_cast<int>(args.size());
	if(n < b.minArgs || (b.maxArgs >= 0 && n > b.maxArgs)) {
		throw std::runtime_error(name + "() got " + std::to_string(n) + " argument(s), expected " + (b.minArgs == b.maxArgs ? std::to_string(b.minArgs) : b.maxArgs < 0 ? "at least " + std::to_string(b.minArgs) : std::to_string(b.minArgs) + " to " + std::to_string(b.maxArgs)));
	}
	
	std::vector<double> values;
	values.reserve(args.size());
	for(const auto& arg : args) values.push_back(toDouble(arg->eval(env)));
	
	return Value(b.fn(values));
}

std::string LiteralExpr::toString() const {
	return formatValue(value);
}

Value LiteralExpr::eval(Environment&) const {
	return value;
}