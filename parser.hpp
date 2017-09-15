#pragma once

#include "token.hpp"
#include "scanner.hpp"
#include "syntax_tree.hpp"
#include "symbol.hpp"

class Parser{
public:
	Parser(Scanner* scan);
	void Parse();
private:
	Node* ParseVar();
	Node* ParseConst();
	Node* ParseBodyProgramm();
	Node* ParseDefinitionRecord();
	Node* ParseDefinitionFunction();
	Node* ParsePrimary();
	Node* ParseExpression();
	Node* ParseNumber();
	Node* ParseIdent();
	Node* ParseBinary(int priority, Node* left);
	Node* ParseParen(string paren);
	void SetNextToken();
	int GetPriorityToken();
	string ToUpper(string str);
	//Handle Error
	Node* Error(string str);
	Token cur_token;
	Scanner* scanner;
	int used_var, used_const, used_begin, is_end;
};