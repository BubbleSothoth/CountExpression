// CountExpression.cpp : 定义静态库的函数。
//

#define __DLL_EXPORTS__

#include "pch.h"
#include "CountExpression.h"
bool fnCountExpression(float* answer, char* expression)
{
	using std::stack;
	using std::string;

	string sexpression;
	stack<string::iterator> priority_stack;
	//printf_s("算式: %s\n", expression);
	if (expression == nullptr) return false;
	if (!isLegal(expression)) return false;

	sexpression = expression;
	for (string::iterator it1 = sexpression.begin(); it1 != sexpression.end(); it1++) {
		if (*it1 == '(') {
			priority_stack.push(it1);
			//printf_s("STACK PUSH\n");
		}
		if (*it1 == ')') {
			float rslt;
			string::iterator it2 = priority_stack.top();
			priority_stack.pop();
			//printf_s("STACK POP\n");
			string sub_exp(it2 + 1, it1);
			fnCountExpression(&rslt, (char*)sub_exp.c_str());
			char temp[64] = "";
			sprintf_s(temp, "%f", rslt);
			it1 = sexpression.erase(it2, it1 + 1);

			stack<size_t>* temp_offset_stack = new stack<size_t>();
			//保护现场
			while (!priority_stack.empty()) {
				temp_offset_stack->push(priority_stack.top() - sexpression.begin());
				priority_stack.pop();
			}
			//修改sexpression
			string _ = temp;
			it1 = sexpression.insert(it1, _.begin(), _.end());
			//恢复现场
			while (!temp_offset_stack->empty()) {
				priority_stack.push(sexpression.begin() + temp_offset_stack->top());
				temp_offset_stack->pop();
			}
			delete temp_offset_stack;
		}
	}

	std::vector<string> f_exp;
	format_expression(f_exp, sexpression);	// 格式化算式串
	for (std::vector<std::string>::iterator it = f_exp.begin(); it != f_exp.end(); it++) {
		if (*it == "*" || *it == "/") {
			float l_val = atof((it - 1)->c_str()), r_val = atof((it + 1)->c_str()), rslt = 0.0f;
			if (*it == "*") rslt = l_val * r_val;
			else rslt = l_val / r_val;
			char temp[64] = "";
			sprintf_s(temp, "%f", rslt);
			*(it - 1) = temp;
			it = f_exp.erase(it, it + 2);
			it--;
		}
	}
	for (std::vector<string>::iterator it = f_exp.begin(); it != f_exp.end(); it++) {
		if (*it == "+" || *it == "-") {
			float l_val = atof((it - 1)->c_str()), r_val = atof((it + 1)->c_str()), rslt = 0.0f;
			if (*it == "+") rslt = l_val + r_val;
			else rslt = l_val - r_val;
			char temp[64] = "";
			sprintf_s(temp, "%f", rslt);
			*(it - 1) = temp;
			it = f_exp.erase(it, it + 2);
			it--;
		}
	}
	*answer = atof(f_exp.front().c_str());
	//printf_s("%s=%f\n", sexpression.c_str(), *answer);
	return true;
}

void format_expression(std::vector<std::string>& fexpression, std::string expression) {
	using std::string;
	using std::vector;
	string number = "";
	string single = "";
	for (int i = 0; i <= expression.length(); i++) {
		if (isNumber(expression[i])) number += expression[i];
		else {
			fexpression.push_back(number);
			single += expression[i];
			fexpression.push_back(single);
			single.clear();
			number.clear();
		}
	}
	return;
}

inline  bool isNumber(char ch) {
	return (ch >= '0' && ch <= '9') || (ch == '.');
}

inline bool isBracket(char ch) {
	return (ch >= '(') || (ch <= ')');
}

inline bool isCalculation(char ch) {
	return ch == (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/');
}

bool isLegal(const std::string expression) {
	std::stack<char> bracket_stack;
	for (auto it = expression.begin(); it != expression.end(); it++) {
		if (isNumber(*it) || isBracket(*it) || isCalculation(*it))
		{
			switch (*it)
			{
			case '(':
				bracket_stack.push('#');
				if (it + 1 == expression.end() ||
					isCalculation(*(it + 1)))
					return false;
				if (it != expression.begin() && isNumber(*(it - 1))) return false;
				break;
			case ')':
				if (it == expression.begin() ||
					isCalculation(*(it - 1)))
					return false;
				if (it + 1 != expression.end() && isNumber(*(it + 1))) return false;
				if (!bracket_stack.empty()) bracket_stack.pop();
				else return false;
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				if (it == expression.begin() || it + 1 == expression.end()) return false;
				if (isCalculation(*(it + 1)) || isCalculation(*(it - 1))) return false;
				break;
			default:
				break;
			}
		}
		else return false;
	}
	if (!bracket_stack.empty()) return false;
	return true;

}