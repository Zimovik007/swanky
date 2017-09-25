#include <map>
#include <iostream>
#include <cstring>
#include "parser.hpp"

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

void Parser::PushFromTableSymbols(string name, Symbol* symbol){
	if (!table_symbols.find(name)->second){
		table_symbols[name] = symbol;
	}
	else{
		Error("identifier '" + name + "' duplicated");
	}
}

int Parser::IsType(string type){
	string upper = ToUpper(type);
	for (int i = 0; i < types.size(); i++){
		if (types[i] == upper){
			return 1;
		}
	}
	return 0;
}

int Parser::ProcessingTypes(int type, vector<string> idents, int is_var){
	if (!type){
		Error("It is not a type");
		return 0;
	}
	if (ToUpper(cur_token.value) == "ARRAY"){
		int start_i, end_i;
		SetNextToken();
		if (cur_token.value == "["){
			SetNextToken();
			if (cur_token.GetType() == T_integer){
				start_i = stoi(cur_token.value);
				SetNextToken();
				if (cur_token.value == ".."){
					SetNextToken();
					if (cur_token.GetType() == T_integer){
						end_i = stoi(cur_token.value);
						SetNextToken();
						if (cur_token.value == "]"){
							SetNextToken();
							if (ToUpper(cur_token.value) == "OF"){
								SetNextToken();
								if ((IsType(cur_token.value)) && (ToUpper(cur_token.value) != "ARRAY")){
									Symbol* type_elem_array = GetSymbolTypeOfArray();
									if (is_var){
										for (int i = 0; i < idents.size(); i++){
											Symbol* symbol = new VarArraySymbol(start_i, end_i, type_elem_array);
											PushFromTableSymbols(idents[i], symbol);
										}
									}
									else{
										for (int i = 0; i < idents.size(); i++){
											Symbol* symbol = new ConstArraySymbol(start_i, end_i, type_elem_array);
											PushFromTableSymbols(idents[i], symbol);
										}
									}
								}
								else{
									Error("in can not create multidimensional arrays in this version of the compiler");
								}
							}
							else{
								Error("expected 'OF' word");
							}
						}
						else{
							Error("expected ']' symbol");
						}
					}
					else{
						Error("bound of the array must be a number");
					}
				}
				else{
					Error("split the array bounds must be two dots");
				}
			}
			else{
				Error("bound of the array must be a number");
			}
		}
		else{
			Error("expected '[' symbol");
		}
	}
	else if (ToUpper(cur_token.value) == "INTEGER"){
		for (int i = 0; i < idents.size(); i++){
			if (is_var){
				Symbol* symbol = new VarIntSymbol();
				PushFromTableSymbols(idents[i], symbol);
			}
			else{
				Symbol* symbol = new ConstIntSymbol();
				PushFromTableSymbols(idents[i], symbol);
			}
		}
	}
	else if (ToUpper(cur_token.value) == "REAL"){
		for (int i = 0; i < idents.size(); i++){
			if (is_var){
				Symbol* symbol = new VarFloatSymbol();
				PushFromTableSymbols(idents[i], symbol);
			}
			else{
				Symbol* symbol = new ConstFloatSymbol();
				PushFromTableSymbols(idents[i], symbol);
			}
		}
	}
	else if (ToUpper(cur_token.value) == "STRING"){
		for (int i = 0; i < idents.size(); i++){
			if (is_var){
				Symbol* symbol = new VarStringSymbol();
				PushFromTableSymbols(idents[i], symbol);
			}
			else{
				Symbol* symbol = new ConstStringSymbol();
				PushFromTableSymbols(idents[i], symbol);
			}
		}
	}
	else{
		for (int i = 0; i < idents.size(); i++){
			map<string, Symbol*> r_fields = record_fields.find(cur_token.value)->second;
			if (is_var){
				Symbol* symbol = new VarRecordSymbol(r_fields);
				PushFromTableSymbols(idents[i], symbol);
			}
			else{
				Symbol* symbol = new ConstRecordSymbol(r_fields);
				PushFromTableSymbols(idents[i], symbol);
			}
		}		 
	}
	SetNextToken();
	return 1;
}

Symbol* Parser::GetSymbolTypeOfArray(){
	if (ToUpper(cur_token.value) == "INTEGER"){
		Symbol* symbol = new VarIntSymbol();
		return symbol;		
	}
	if (ToUpper(cur_token.value) == "REAL"){
		Symbol* symbol = new VarFloatSymbol();
		return symbol;
	}
	if (ToUpper(cur_token.value) == "STRING"){
		Symbol* symbol = new VarStringSymbol();
		return symbol;
	}
	else{
		map<string, Symbol*> r_fields = record_fields.find(cur_token.value)->second;
		Symbol* symbol = new VarRecordSymbol(r_fields);
		return symbol;
	}
}

Parser::Parser(Scanner* scan){
	scanner = scan;
	SetNextToken();
	used_var = 0;
	used_const = 0;
	used_begin = 0;
	used_type = 0;
	used_function = 0;
	is_end = 0;
	types.push_back("INTEGER");
	types.push_back("REAL");
	types.push_back("STRING");
	types.push_back("ARRAY");
}

void Parser::Parse(){
	int result = 1;
	while (result){
		int type = cur_token.GetType(); 
		string upper = ToUpper(cur_token.value);

		if (type == T_reserved){
			if (upper == "TYPE"    ) result = ParseDefinitionRecord();
			if (upper == "VAR"     ) result = ParseVar();
			if (upper == "CONST"   ) result = ParseConst();
			if (upper == "FUNCTION") result = ParseDefinitionFunction();
			if (upper == "BEGIN"   ) result = ParseBodyProgramm();
		}
		else if (type == T_eof){
			cout << "Таблица символов:" << endl << "|";
			for(auto& item : table_symbols)
			{
    			cout << item.first << "|";
			}
			cout << endl;
			return;
		}
	}
}

int Parser::ParseVar(){
	SetNextToken();
	if (used_var){
		Error("The 'var' previously");
		return 1;
	}
	if (used_function){
		Error("The 'var' should be above the function declarations");
		return 1;
	}
	used_var = 1;
	used_type = 1;
	return ParseDefinitionIdent(1);
}

int Parser::ParseConst(){
	SetNextToken();
	if (used_const){
		Error("The 'const' previously");
		return 1;
	}
	if (used_function){
		Error("The 'const' should be above the function declarations");
		return 1;
	}
	used_const = 1;
	used_type = 1;

	return ParseDefinitionIdent(0);
}

int Parser::ParseDefinitionIdent(int is_var){
	while(true){
		vector<string> name_idents;
		if (cur_token.GetType() == T_ident){
			name_idents.push_back(cur_token.value);
			SetNextToken();
			while(cur_token.value == ","){
				SetNextToken();
				if (cur_token.GetType() == T_ident){
					name_idents.push_back(cur_token.value);
					SetNextToken();
				}
				else{
					Error("expected identifier");
				}
			}
			if (cur_token.value == ":"){
				SetNextToken();
				ProcessingTypes(IsType(cur_token.value), name_idents, is_var);
				if (cur_token.value != ";"){
					Error("expected ';'");
				}
				else{
					SetNextToken();
				}
			}
			else{
				Error("expected ':'");
			}
		}
		else{
			break;
		}
	}

	return 1;	
}

int Parser::ParseBodyProgramm(){
	SetNextToken();
	Node* tree = new Node();
	while (scanner->GetLengthDeque() - 1 > scanner->GetCurIndex()){
		tree = ParseExpression();
		if (tree){
			tree->PrintNode();
			cout << endl;
		}
		if (is_end) break;
	}
	if (!is_end) Error("There is no end of the program");
	SetNextToken();
	return 1;
}

int Parser::ParseDefinitionRecord(){
	SetNextToken();
	if (used_type){
		Error("The 'type' previously or should be placed above");
		return 1;
	}
	if (used_function){
		Error("The 'type' should be above the function declarations");
		return 1;
	}
	if (used_const){
		Error("The 'type' should be above the function declarations");
		return 1;
	}
	if (used_var){
		Error("The 'type' should be above the function declarations");
		return 1;
	}
	used_type = 1;
	return 1;
}

int Parser::ParseDefinitionFunction(){
	SetNextToken();
	return 1;
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
	if (type == T_integer ) 	return ParseInt();
	if (type == T_float   ) 	return ParseFloat();
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

Node* Parser::ParseInt(){
	Node* node = new IntNode(cur_token.value);
	SetNextToken();
	return node;
}

Node* Parser::ParseFloat(){
	Node* node = new FloatNode(cur_token.value);
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

