#include "stmt.h"
#include <iostream>

void ExprStmt::execute(Environment& env) const {
	Value result = expr->eval(env);
	if(!print) return;
	
	if(auto* assign = dynamic_cast<AssignExpr*>(expr.get())) {
		std::cout << assign->name << " = " << formatValue(result) << "\n";
	} else {
		env["ans"] = result;
		std::cout << "ans = " << formatValue(result) << "\n";
	}
}

std::string ExprStmt::toString() const {
	return expr->toString() + (print ? "" : ";");
}

void BlockStmt::execute(Environment& env) const {
	for(const auto& stmt : statements) {
		stmt->execute(env);
	}
}

std::string BlockStmt::toString() const {
	std::string s = "{";
	for(const auto& stmt : statements) s += " " + stmt->toString() + ";";
	return s + "}";
}

void IfStmt::execute(Environment& env) const {
	if(toCondition(condition->eval(env))) {
		thenBranch->execute(env);
	} else if(elseBranch) {
		elseBranch->execute(env);
	}
}

std::string IfStmt::toString() const {
	std::string s = "(if " + condition->toString() + " " + thenBranch->toString();
	if(elseBranch) s += " else " + elseBranch->toString();
	return s + ")";
}