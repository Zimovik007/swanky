#include <map>
#include <iostream>
#include <cstring>
#include "parser.hpp"
#include "syntax_tree.hpp"

#define T_operator 0
#define T_literal  1
#define T_integer  2
#define T_float    3
#define T_ident    4
#define T_separate 5
#define T_reserved 6
#define T_eof      7

using namespace std;


string paren_open = "(";
string paren_close = ")";
string comma = ",";

map<string, int> priority = {
	{"@", 5}, 
	{"NOT", 5}, 
	{"*", 4}, 
	{"/",  4}, 
	{"DIV", 4}, 
	{"MOD", 4},
	{"AND", 4}, 
	{"+", 3}, 
	{"-", 3}, 
	{"OR", 3}, 
	{"XOR", 3}, 
	{"=", 2}, 
	{"<>", 2}, 
	{"<", 2}, 
	{">", 2}, 
	{"<=", 2}, 
	{">=", 2},
	{"IN", 2},
	{":=", 1} 
};

int Parser::GetPriorityToken(){
	if (priority.find(cur_token.value)->second)
		return priority.find(cur_token.value)->second;
	else
		return -1;
}

Node* Parser::Error(string str){
	cout << "Error: " << str << endl;
	return 0;
}

void Parser::SetNextToken(){
	cur_token = scanner->GetNextToken();
}

Parser::Parser(Scanner* scan){
	scanner = scan;
	SetNextToken();
}

Node* Parser::ParsePrimary(){
	int type = cur_token.GetType();
	
	if (type == T_operator){
		if (cur_token.value == paren_open) return ParseParen();
		return ParseExpression();
	}
	if (type == T_integer) return ParseNumber();
	if (type == T_float  ) return ParseNumber();
	if (type == T_ident  ) return ParseIdent();

	return 0;
}

Node* Parser::ParseExpression(){
	Node* left_sub_tree = ParsePrimary();
	
	if (!left_sub_tree) return 0;

	return ParseBinary(0, left_sub_tree);
}

Node* Parser::ParseBinary(int exp_priority, Node* left){
	while (true){
		int cur_token_priority = GetPriorityToken();
		if (cur_token_priority < exp_priority)
			return left;

		string op = cur_token.value;
		SetNextToken();

		Node* right = ParsePrimary();
		if (!right)
			return 0;

		int next_token_priority = GetPriorityToken();
		if (cur_token_priority < next_token_priority){
			right = ParseBinary(cur_token_priority, right);
			if (!right)
				return 0;
		}

		left = new BinaryNode(op, left, right);
	}
}

Node* Parser::ParseNumber(){
	Node* node = new NumberNode(cur_token.value);
	SetNextToken();
	return node;
}

Node* Parser::ParseIdent(){
	string name = cur_token.value;
	SetNextToken();
	if (cur_token.value != paren_open){
		Node* result = new IdentNode(name);
		return result;
	}
	SetNextToken();
	vector<Node*> args;
	if (cur_token.value != paren_close){
		while (true){
			Node* argument = ParseExpression();
			if (!argument)
				return 0;
			args.push_back(argument);

			if (cur_token.value == paren_close)
				break;
			if (cur_token.value != comma)
				return Error("expected ')' or ',' in arguments list");
			SetNextToken();
		}
	}
	SetNextToken();
	Node* func = new CallFuncNode(name, args);
	return func;
}

Node* Parser::ParseParen(){
	SetNextToken();
	Node* result = ParseExpression();

	if (!result) return 0;
	if (cur_token.value != paren_close) return Error("expected )");

	SetNextToken();
	return result;
}

