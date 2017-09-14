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

Parser::Parser(Scanner* scan){
	scanner = scan;
	SetNextToken();
	used_var = 0;
	used_const = 0;
	used_begin = 0;
	is_end = 0;
}

void Parser::Parse(){
	int type = cur_token.GetType(); 
	string upper = ToUpper(cur_token.value);

	if (type == T_reserved){
		if (upper == "VAR")
			ParseVar();
		if (upper == "CONST")
			ParseConst();
		if (upper == "BEGIN"){
			SetNextToken();
			ParseBodyProgramm();
		}
		if (upper == "TYPE")
			ParseDefinitionRecord();
		if (upper == "FUNCTION")
			ParseDefinitionFunction();
	}
	else
		Error("Not found the beginning of the program");
}

Node* Parser::ParseVar(){
	return 0;
}

Node* Parser::ParseConst(){
	return 0;	
}

Node* Parser::ParseBodyProgramm(){
	Node* tree = new Node();
	while (scanner->GetLengthDeque() - 1 > scanner->GetCurIndex()){
		tree = ParseExpression();
		if (tree){
			tree->PrintNode();
			cout << endl;
		}
		if (is_end) break;
	}
	if (!is_end) return Error("There is no end of the program");
	return 0;
}

Node* Parser::ParseDefinitionRecord(){
	return 0;
}

Node* Parser::ParseDefinitionFunction(){
	return 0;
}

string Parser::ToUpper(string str){
	for (int i = 0; i < str.length(); i++)
		str[i] = toupper(str[i]);
	return str;
}

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

Node* Parser::ParsePrimary(){
	if (cur_token.value == ";"){
		SetNextToken();
		return ParsePrimary();
	}

	int type = cur_token.GetType();

	if (cur_token.value == "(") return ParseParen("(");
	if (type == T_integer ) 	return ParseNumber();
	if (type == T_float   ) 	return ParseNumber();
	if (type == T_ident   ) 	return ParseIdent();
	if (ToUpper(cur_token.value) == "END"){
		SetNextToken();
		if (cur_token.value == "."){
			is_end = 1;
			return 0;
		}
		else
			return Error("expected '.'");
	}

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
		if (cur_token_priority < exp_priority){
			return left;
		}
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
	if (cur_token.value != "("){
		if (cur_token.value != "["){
			Node* result = new IdentNode(name);
			return result;
		}
		Node* index = ParseParen("[");
		Node* result = new ArrayNode(name, index);
		return result;
	}
	SetNextToken();
	vector<Node*> args;
	if (cur_token.value != ")"){
		while (true){
			Node* argument = ParseExpression();
			if (!argument)
				return 0;
			args.push_back(argument);

			if (cur_token.value == ")")
				break;
			if (cur_token.value != ",")
				return Error("expected ')' or ',' in arguments list");
			SetNextToken();
		}
	}
	SetNextToken();
	Node* func = new CallFuncNode(name, args);
	return func;
}

Node* Parser::ParseParen(string paren){
	SetNextToken();
	Node* result = ParseExpression();

	if (!result) return 0;
	if ((paren == "(") and (cur_token.value != ")")) return Error("expected ')'");
	if ((paren == "[") and (cur_token.value != "]")) return Error("expected ']' after index");

	SetNextToken();
	return result;
}

