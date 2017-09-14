#pragma once

#include "token.hpp"
#include "scanner.hpp"
#include "syntax_tree.hpp"

class Parser{
public:
	Parser(Scanner* scan);
	Node* ParsePrimary();
	Node* ParseExpression();
	Node* ParseNumber();
	Node* ParseIdent();
	Node* ParseBinary(int priority, Node* left);
	Node* ParseParen();
	void SetNextToken();
	int GetPriorityToken();
	//Handle Error
	Node* Error(string str);
private:
	Token cur_token;
	Scanner* scanner;
};