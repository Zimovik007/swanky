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
	int ParseVar();
	int ParseConst();
	int ParseDefinitionIdent(int is_var);
	int ParseBodyProgramm();
	int ParseDefinitionRecord();
	int ParseDefinitionFunction();
	Node* ParsePrimary();
	Node* ParseExpression();
	Node* ParseInt();
	Node* ParseFloat();
	Node* ParseIdent();
	Node* ParseBinary(int priority, Node* left);
	Node* ParseParen(string paren);
	void SetNextToken();
	int GetPriorityToken();
	Symbol* GetSymbolTypeOfArray();
	int ProcessingTypes(int type, vector<string> idents, int is_var);
	int IsType(string type);
	void PushFromTableSymbols(string name, Symbol* symbol);
	string ToUpper(string str);
	//Handle Error
	Node* Error(string str);
	Token cur_token;
	Scanner* scanner;
	int used_var, used_const, used_begin, used_type, used_function, is_end;
	vector<string> types;
	map<string, map<string, Symbol*>> record_fields;
	map<string, Symbol*> table_symbols;
};