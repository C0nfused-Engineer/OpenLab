// builtins.h
#ifndef BUILTINS_H
#define BUILTINS_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using BuiltinFn = std::function<double(const std::vector<double>&)>;

struct Builtin {
	int minArgs;
	int maxArgs; // -1 meaning any number
	BuiltinFn fn;
	std::string help;
};

const std::unordered_map<std::string, Builtin>& getBuiltins();

#endif