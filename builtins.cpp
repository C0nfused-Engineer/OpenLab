#include "builtins.h"
#include <algorithm>
#include <cmath>
#include <numeric>

static Builtin fn1(double (*f)(double), std::string help) {
	return {1, 1, [f](const std::vector<double>& a) {return f(a[0]);}, std::move(help)};
}

static Builtin fn2(double (*f)(double, double), std::string help) {
	return {2, 2, [f](const std::vector<double>& a) { return f(a[0], a[1]); }, std::move(help)};
}

const std::unordered_map<std::string, Builtin>& getBuiltins() {
	static const std::unordered_map<std::string, Builtin> table = {
		// one argument
		{"sqrt", fn1([](double x) {return std::sqrt(x); }, "square root")},
		{"abs", fn1([](double x) {return std::fabs(x); }, "absolute value")},
		{"exp", fn1([](double x) {return std::exp(x); }, "e^x")},
		{"log", fn1([](double x) {return std::log(x); }, "natural log")},
		{"log10", fn1([](double x) {return std::log10(x); }, "base-10 log")},
		{"sin", fn1([](double x) {return std::sin(x); }, "sine (radians)")},
		{"cos", fn1([](double x) {return std::cos(x); }, "cosine (radians)")},
		{"tan", fn1([](double x) {return std::tan(x); }, "tangent (radians)")},
		// Add SIN COS TAN degree variants
		{"floor", fn1([](double x) {return std::floor(x); }, "round down")},
		{"ceil", fn1([](double x) {return std::ceil(x); }, "round up")},
		{"round", fn1([](double x) {return std::round(x); }, "round to nearest")},
		
		// two arguments
		{"atan2", fn2([](double y, double x) {return std::atan2(y, x); }, "angle of (x, y) (radians)")},
		
		// any number of args
		{"max", {1, -1, [](const std::vector<double>& a) {return *std::max_element(a.begin(), a.end()); }, "largest argument"}},
		{"min", {1, -1, [](const std::vector<double>& a) {return *std::min_element(a.begin(), a.end()); }, "smallest argument"}},
		{"sum", {1, -1, [](const std::vector<double>& a) {return std::accumulate(a.begin(), a.end(), 0.0); }, "sum of elements"}},
	};
	
	return table;
}