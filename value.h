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

Value binaryOp(TokenType op, const Value& l, const Value& r);
Value unaryOp(TokenType op, const Value& v);

#endif