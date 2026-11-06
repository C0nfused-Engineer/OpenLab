#include "stmt.h"
#include <iostream>

ExecResult ExprStmt::execute(Environment& env) const {
	Value result = expr->eval(env);
	if(print) {
		if(auto* assign = dynamic_cast<AssignExpr*>(expr.get())) {
			std::cout << assign->name << " = " << formatValue(result) << "\n";
		} else {
			env["ans"] = result;
			std::cout << "ans = " << formatValue(result) << "\n";
		}
	}
	return {};
}

std::string ExprStmt::toString() const {
	return expr->toString() + (print ? "" : ";");
}

ExecResult BlockStmt::execute(Environment& env) const {
	for(const auto& stmt : statements) {
		ExecResult r = stmt->execute(env);
		if(r.flow != Flow::NORMAL) return r;
	}
	return {};
}

std::string BlockStmt::toString() const {
	std::string s = "{";
	for(const auto& stmt : statements) s += " " + stmt->toString() + ";";
	return s + "}";
}

ExecResult IfStmt::execute(Environment& env) const {
	if(toCondition(condition->eval(env))) return thenBranch->execute(env);
	if(elseBranch) return elseBranch->execute(env);
	return {};
}

std::string IfStmt::toString() const {
	std::string s = "(if " + condition->toString() + " " + thenBranch->toString();
	if(elseBranch) s += " else " + elseBranch->toString();
	return s + ")";
}

ExecResult WhileStmt::execute(Environment& env) const {
	while(toCondition(condition->eval(env))) {
		ExecResult r = body->execute(env);
		if(r.flow == Flow::BREAK) break;
		if(r.flow == Flow::RETURN) return r;
	}
	return {};
}

std::string WhileStmt::toString() const {
	return "(While " + condition->toString() + " " + body->toString() + ")";
}

ExecResult BreakStmt::execute(Environment&) const {
	return {Flow::BREAK};
}

std::string BreakStmt::toString() const {
	return "break";
}

ExecResult ContinueStmt::execute(Environment&) const {
	return {Flow::CONTINUE};
}

std::string ContinueStmt::toString() const {
	return "continue";
}