#pragma once
#ifdef __DLL_EXPORTS__
#define DLLAPI  __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif
#pragma once
#include <stack>
#include <string>
#include <vector>
#include <cstdlib>

bool fnCountExpression(float* answer, char* expression);
void format_expression(std::vector<std::string>& fexpression, std::string expression);
bool isLegal(const std::string expression);
inline  bool isNumber(char ch);
inline bool isBracket(char ch);
inline bool isCalculation(char ch);
