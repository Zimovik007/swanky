#pragma once

#include "token.hpp"
#include "scanner.hpp"
#include "syntax_tree.hpp"
#include "symbol.hpp"
#include "block.hpp"

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
	Block* ParseBodyFunction(map<string, Symbol*> type);
	Node* ParsePrimary(map<string, Symbol*> variables);
	Node* ParseExpression(map<string, Symbol*> variables);
	Node* ParseInt(int sign);
	Node* ParseFloat(int sign);
	Node* ParseIdent(map<string, Symbol*> variables, int sign);
	Node* ParseLiteral();
	Node* ParseBinary(int priority, Node* left, map<string, Symbol*> variables);
	Node* ParseParen(string paren);
	void SetNextToken();
	int IssetIdent(string name, map<string, Symbol*> variables);
	int GetPriorityToken();
	Symbol* GetSymbolTypeOfArray();
	int IsType(string type);
	int StringTypeToInt(string type);
	int GetTypeOfToken(map<string, Symbol*> variables);
	int CheckTypes(Node* tree, map<string, Symbol*> variables);
	int GetTypeNode(Node* node, map<string, Symbol*> variables);
	map<string, Symbol*> ProcessingTypes(int type, vector<string> idents, int is_var, map<string, Symbol*> table);
	map<string, Symbol*> PushFromTableSymbols(string name, Symbol* symbol, map<string, Symbol*> table);
	string ToUpper(string str);
	//Handle Error
	Node* Error(string str, int move = 1);
	Token cur_token;
	Scanner* scanner;
	int used_var, used_const, used_begin, used_type, used_function, is_end;
	map<string, map<string, Symbol*>> types;
	map<string, map<string, Symbol*>> record_fields;
	map<string, Symbol*> table_symbols;
	Symbol* last_push_symbol_from_table_symbols;
};