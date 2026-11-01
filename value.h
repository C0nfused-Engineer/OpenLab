// value.h
#ifndef VALUE_H
#define VALUE_H

#include <cstdint>
#include <string>
#include <variant>
#include "token.h"

using Value = std::variant<bool, int64_t, double>;

std::string typeName(const Value& v);
std::string formatValue(const Value& v);
double toDouble(const Value& v);
bool toCondition(const Value& v);
bool isComparison(TokenType op);
int64_t intPow(int64_t base, int64_t exp);

Value binaryOp(TokenType op, const Value& l, const Value& r);
Value unaryOp(TokenType op, const Value& v);
Value binaryOp(TokenType op, const Value& l, const Value& r);
Value compareOp(TokenType op, const Value& l, const Value& r);

#endif