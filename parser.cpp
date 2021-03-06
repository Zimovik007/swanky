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
#define T_function 8
#define T_record   9
#define T_array    10

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

int Parser::GetTypeNode(Node* node, map<string, Symbol*> variables){
	int type_node = node->GetType();
	if (type_node == T_integer) return T_integer;
	if (type_node == T_float)   return T_float;
	if (type_node == T_literal) return T_literal;
	if (type_node == T_ident){
		if (IssetIdent(node->GetName(), variables))
			return ((variables.find(node->GetName())->second)->GetType());
		return -1;
	}
	if (type_node == T_array){
		if (IssetIdent(node->GetName(), variables)){
			Node* index = node->GetIndexNode();
			if (GetTypeNode(index, variables) != T_integer){
				Error("index of array must be integer", -1);
				return -1;
			}
			return ((variables.find(node->GetName())->second)->GetTypeElements()->GetType());
		}
		return -1;	
	}
	if (type_node == T_function){
		if (IssetIdent(node->GetName(), variables)){
			
			return ((variables.find(node->GetName())->second)->GetReturnType());
		}
		return -1;
	}
	if (type_node == T_operator){
		int left_type =  GetTypeNode(node->GetLeftChild(),  variables);
		int right_type = GetTypeNode(node->GetRightChild(), variables); 
		if (left_type == right_type) 
			return left_type;
		return -1;
	}
	return -1;
}

int Parser::CheckTypes(Node* tree, map<string, Symbol*> variables){
	int left_type = -1;
	int right_type = -1;
	if (tree->GetType() == T_operator){
		left_type  = GetTypeNode(tree->GetLeftChild(),  variables);
		right_type = GetTypeNode(tree->GetRightChild(), variables);
		if (left_type == right_type) return 1;
		Error("Wrong types. See: '" + IntTypeToString(left_type) + "' & '" + IntTypeToString(right_type) + "'", -1);
		return 0;
	}
	else if (tree->GetType() == T_function){
		GetTypeNode(tree, variables);
	}
	else{
		Error("expected a function call or expression", -1);
	}
	return 1;
}

int Parser::GetTypeOfToken(map<string, Symbol*> variables){
	if (cur_token.GetType() == T_integer)
		return cur_token.GetType();
	if (cur_token.GetType() == T_float)
		return cur_token.GetType();
	if (cur_token.GetType() == T_literal)
		return cur_token.GetType();	
	if (cur_token.GetType() == T_ident)
		return (variables.find(cur_token.value)->second)->GetType();
	if (cur_token.GetType() == T_function)
		return (variables.find(cur_token.value)->second)->GetReturnType();
	return -1;
}

int Parser::StringTypeToInt(string type){
	if (ToUpper(type) == "INTEGER")  return T_integer;
	if (ToUpper(type) == "STRING")   return T_literal;
	if (ToUpper(type) == "REAL")     return T_float;
	if (ToUpper(type) == "FLOAT")    return T_float;
	if (ToUpper(type) == "FUNCTION") return T_literal;
	if (ToUpper(type) == "ARRAY")    return T_array;
	if (ToUpper(type) == "RECORD")   return T_record;
	return -1;
}

string Parser::IntTypeToString(int type){
	if (type == T_integer)  return "INTEGER";
	if (type == T_float)    return "REAL";
	if (type == T_literal)  return "STRING";
	if (type == T_function) return "FUNCTION";
	if (type == T_array)    return "ARRAY";
	if (type == T_record)   return "RECORD";
	return "UNDEFINED";
}

void Parser::PushFromTableSymbols(string name, Symbol* symbol, map<string, Symbol*>& table){
	for(auto& item : table)
    	if (item.first == name){
    		Error("identifier '" + name + "' duplicated");
    		return;
    	}
    table[name] = symbol;
    last_push_symbol_from_table_symbols = symbol;
}

int Parser::IsType(string type){
	string upper = ToUpper(type);
	for(auto& item : types)
    	if (item.first == upper)
    		return 1;
	return 0;
}

map<string, Symbol*> Parser::ProcessingTypes(int type, vector<string> idents, int is_var, map<string, Symbol*>& table){
	if (!type){
		Error("It is not a type");
		return table;
	}
	else
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
											PushFromTableSymbols(idents[i], symbol, table);
										}
									}
									else{
										for (int i = 0; i < idents.size(); i++){
											Symbol* symbol = new ConstArraySymbol(start_i, end_i, type_elem_array);
											PushFromTableSymbols(idents[i], symbol, table);
										}
									}
								}
								else{
									Error("in can not create multidimensional arrays in this version of the compiler");
									return table;
								}
							}
							else{
								Error("expected 'OF' word");
								return table;
							}
						}
						else{
							Error("expected ']' symbol");
							return table;
						}
					}
					else{
						Error("bound of the array must be a number");
						return table;
					}
				}
				else{
					Error("split the array bounds must be two dots");
					return table;
				}
			}
			else{
				Error("bound of the array must be a number");
				return table;
			}
		}
		else{
			Error("expected '[' symbol");
			return table;
		}
	}
	else if (ToUpper(cur_token.value) == "INTEGER"){
		for (int i = 0; i < idents.size(); i++){
			if (is_var){
				Symbol* symbol = new VarIntSymbol();
				PushFromTableSymbols(idents[i], symbol, table);
			}
			else{
				Symbol* symbol = new ConstIntSymbol();
				PushFromTableSymbols(idents[i], symbol, table);
			}
		}
	}
	else if (ToUpper(cur_token.value) == "REAL"){
		for (int i = 0; i < idents.size(); i++){
			if (is_var){
				Symbol* symbol = new VarFloatSymbol();
				PushFromTableSymbols(idents[i], symbol, table);
			}
			else{
				Symbol* symbol = new ConstFloatSymbol();
				PushFromTableSymbols(idents[i], symbol, table);
			}
		}
	}
	else if (ToUpper(cur_token.value) == "STRING"){
		for (int i = 0; i < idents.size(); i++){
			if (is_var){
				Symbol* symbol = new VarStringSymbol();
				PushFromTableSymbols(idents[i], symbol, table);
			}
			else{
				Symbol* symbol = new ConstStringSymbol();
				PushFromTableSymbols(idents[i], symbol, table);
			}
		}
	}
	else{
		for (int i = 0; i < idents.size(); i++){
			map<string, Symbol*> r_fields = record_fields.find(cur_token.value)->second;
			if (is_var){
				Symbol* symbol = new VarRecordSymbol(r_fields);
				PushFromTableSymbols(idents[i], symbol, table);
			}
			else{
				Symbol* symbol = new ConstRecordSymbol(r_fields);
				PushFromTableSymbols(idents[i], symbol, table);
			}
		}		 
	}
	SetNextToken();
	return table;
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
	is_end = 0;
	map<string, Symbol*> init;
	types["INTEGER"] = init;
	types["REAL"] = init;
	types["STRING"] = init;
	types["ARRAY"] = init;
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
			cout << "Типы данных:" << endl << "|";
			for(auto& item : types)
			{
    			cout << item.first << "|";
			}
			cout << endl;
			return;
		}
		else{
			Error("who knows what to do with it?");
			SetNextToken();
		}
	}
}

int Parser::ParseVar(){
	SetNextToken();
	return ParseDefinitionIdent(1);
}

int Parser::ParseConst(){
	SetNextToken();
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
				table_symbols = ProcessingTypes(IsType(cur_token.value), name_idents, is_var, table_symbols);
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
		tree = ParseExpression(table_symbols);
		if (tree){
			tree->PrintNode();
			CheckTypes(tree, table_symbols);
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
	while(true){
		if (cur_token.GetType() == T_ident){
			string record_name = cur_token.value;
			SetNextToken();
			if (cur_token.value == "="){
				SetNextToken();
				if (ToUpper(cur_token.value) == "RECORD"){
					map<string, Symbol*> record_fields;
					SetNextToken();
					while(ToUpper(cur_token.value) != "END"){
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
								record_fields = ProcessingTypes(IsType(cur_token.value), name_idents, 1, record_fields);
								SetNextToken();
								if (cur_token.value == ";")	SetNextToken();
							}
							else{
								Error("expected ':'");
							}
						}
						else{
							Error("it is not a identifier");
						}
					}
					types[ToUpper(record_name)] = record_fields;
					SetNextToken();
					if (cur_token.value == ";") SetNextToken();
				}
				else{
					Error("expected 'RECORD' keyword");
				}
			}
			else{
				Error("expected '='");
			}
		}
		else 
			if ((ToUpper(cur_token.value) == "VAR")      || 
				(ToUpper(cur_token.value) == "CONST")    || 
				(ToUpper(cur_token.value) == "FUNCTION") || 
				(ToUpper(cur_token.value) == "TYPE")     || 
				(ToUpper(cur_token.value) == "BEGIN"))
				return 1;
		else{				
			Error("it is uncorrect name of identifier");
		}
	}

	return 1;
}

int Parser::ParseDefinitionFunction(){
	vector<string> name_idents;
	SetNextToken();
	if (cur_token.GetType() == T_ident){
		string func_name = cur_token.value;
		SetNextToken();
		if (cur_token.value == "("){
			SetNextToken();
			map<string, Symbol*> func_param_map;
			vector<Symbol*> func_param_vector;
			while(cur_token.value != ")"){
				if (cur_token.GetType() == T_ident){
					int cnt = 1;
					name_idents.push_back(cur_token.value);
					SetNextToken();
					while(cur_token.value == ","){
						SetNextToken();
						if (cur_token.GetType() == T_ident){
							cnt++;
							name_idents.push_back(cur_token.value);
							SetNextToken();
						}
						else{
							Error("expected identifier", 0);
							break;
						}
					}
					if (cur_token.value == ":"){
						SetNextToken();
						func_param_map = ProcessingTypes(IsType(cur_token.value), name_idents, 1, func_param_map);
						if (cur_token.value == ";")	SetNextToken();
						while (cnt){
							cnt--;
							func_param_vector.push_back(last_push_symbol_from_table_symbols);
						}
					}
					else{
						Error("expected ':'", 0);
						break;
					}
				}
				else{
					Error("it is not a identifier", 0);
					break;
				}
			}
			SetNextToken();
			if (cur_token.value == ":"){
				SetNextToken();
				if (IsType(cur_token.value)){
					string return_type = cur_token.value;
					vector<string> vars;
					vars = name_idents;
					vars.push_back("result");
					Block* body_function = new Block();
					map<string, Symbol*> local = body_function->GetLocalVariables();
					body_function->PushLocalVariables(ProcessingTypes(IsType(return_type), vars, 1, local));
					if (cur_token.value != ";"){
						Error("expected ';'");
					}
					else{
						SetNextToken();
						body_function = ParseBodyFunction(body_function->GetLocalVariables());
						if (body_function){
							Symbol* symbol = new FuncSymbol(func_param_map, func_param_vector, StringTypeToInt(return_type), body_function);
							PushFromTableSymbols(func_name, symbol, table_symbols);
						}
					}
				}
				else{
					Error("a nonexistent data type", 0);
				}
			}
			else{
				Error("expected ':'", 0);
			}
		}
	}
	else{
		Error("expected name of function", 0);
	}
	return 1;
}

Block* Parser::ParseBodyFunction(map<string, Symbol*> res){
	Block* body_function = new Block();
	body_function->PushLocalVariables(res);
	if (ToUpper(cur_token.value) == "VAR"){
		SetNextToken();
		while (ToUpper(cur_token.value) != "BEGIN"){
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
						Error("expected identifier", 0);
						break;
					}
				}
				if (cur_token.value == ":"){
					SetNextToken();
					map<string, Symbol*> local = body_function->GetLocalVariables();
					body_function->PushLocalVariables(ProcessingTypes(IsType(cur_token.value), name_idents, 1, local));
					if (cur_token.value == ";")	SetNextToken();
				}
				else{
					Error("expected ':'", 0);
					break;
				}
			}
			else{
				Error("it is not a identifier", 0);
				break;
			}
		}
	}
	if (ToUpper(cur_token.value) == "BEGIN"){
		SetNextToken();
		while (ToUpper(cur_token.value) != "END"){
			body_function->AddToBlock(ParseExpression(body_function->GetLocalVariables()));
			if (cur_token.value == ";"){
				SetNextToken();
			}
		}
		SetNextToken();
		if (cur_token.value != ";"){
			Error("expected ';'");
		}
		else{
			SetNextToken();
		}
	}
	else{
		Error("expected either 'begin' or 'var'", 0);
		return 0;
	}
	return body_function;
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

Node* Parser::Error(string str, int move){
	if (move > -1) cur_token.PrintToken(0); 
	cout <<  " |=> " << "Error: " << str;
	if (move > -1) cout << endl;
	if ((cur_token.value != ";") && (cur_token.GetType() != T_eof))
		SetNextToken();
	if (move){
		while ((cur_token.value != ";") && (cur_token.GetType() != T_eof) && (ToUpper(cur_token.value) != "VAR") && (ToUpper(cur_token.value) != "CONST") && (ToUpper(cur_token.value) != "FUNCTION") && (ToUpper(cur_token.value) != "TYPE") && (ToUpper(cur_token.value) != "BEGIN")){
			SetNextToken();
		}
	}
	else if (move == 0){
		while((cur_token.GetType() != T_eof) && (ToUpper(cur_token.value) != "VAR") && (ToUpper(cur_token.value) != "CONST") && (ToUpper(cur_token.value) != "FUNCTION") && (ToUpper(cur_token.value) != "TYPE") && (ToUpper(cur_token.value) != "BEGIN")){
			SetNextToken();
		}
	}
	return 0;
}

void Parser::SetNextToken(){
	cur_token = scanner->GetNextToken();
}

Node* Parser::ParsePrimary(map<string, Symbol*> variables){
	if (cur_token.value == ";"){
		SetNextToken();
		return ParsePrimary(variables);
	}

	int cur_type = cur_token.GetType();

	int sign = 0;
	if (cur_token.value == "(") return ParseParen("(");	
	if (cur_token.value == "-") { sign = 1; SetNextToken(); cur_type = cur_token.GetType();}
	if (cur_type == T_integer ) return ParseInt(sign);
	if (cur_type == T_float   ) return ParseFloat(sign);
	if (cur_type == T_ident   ) return ParseIdent(variables, sign);
	if (cur_type == T_literal ) return ParseLiteral();
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

Node* Parser::ParseExpression(map<string, Symbol*> variables){
	Node* left_sub_tree = ParsePrimary(variables);
	
	if (!left_sub_tree) return 0;

	Node* node = ParseBinary(0, left_sub_tree, variables);

	return node;
}

Node* Parser::ParseBinary(int exp_priority, Node* left, map<string, Symbol*> variables){
	while (true){
		int cur_token_priority = GetPriorityToken();
		if (cur_token_priority < exp_priority){
			return left;
		}
		string op = cur_token.value;
		SetNextToken();

		Node* right = ParsePrimary(variables);
		if (!right)
			return 0;
		int next_token_priority = GetPriorityToken();
		if (cur_token_priority < next_token_priority){
			right = ParseBinary(cur_token_priority, right, variables);
			if (!right)
				return 0;
		}

		left = new BinaryNode(op, left, right);
	}
}

Node* Parser::ParseLiteral(){
	Node* node = new LiteralNode(cur_token.value);
	SetNextToken();
	return node;
}

Node* Parser::ParseInt(int sign){
	if (sign){
		Node* node = new IntNode("-" + cur_token.value);
		SetNextToken();
		return node;
	}
	else{ 
		Node* node = new IntNode(cur_token.value);
		SetNextToken();
		return node;
	}
}

Node* Parser::ParseFloat(int sign){
	if (sign){
		Node* node = new FloatNode("-" + cur_token.value);
		SetNextToken();
		return node;
	}
	else{
		Node* node = new FloatNode(cur_token.value);
		SetNextToken();
		return node;
	}
}

Node* Parser::ParseIdent(map<string, Symbol*> variables, int sign){
	string name = cur_token.value;
	if (!IssetIdent(name, variables)){
		Error("identifier was not found");
		return 0;
	}
	SetNextToken();
	if (cur_token.value != "("){
		if (cur_token.value != "["){
			Node* result = new IdentNode(name, sign);
			return result;
		}
		Node* index = ParseParen("[");
		Node* result = new ArrayNode(name, index, sign);
		return result;
	}
	SetNextToken();
	vector<Node*> args;
	if (cur_token.value != ")"){
		while (true){
			Node* argument = ParseExpression(variables);
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
	Node* func = new CallFuncNode(name, args, sign);
	return func;
}

Node* Parser::ParseParen(string paren){
	SetNextToken();
	Node* result = ParseExpression(table_symbols);

	if (!result) return 0;
	if ((paren == "(") and (cur_token.value != ")")) return Error("expected ')'");
	if ((paren == "[") and (cur_token.value != "]")) return Error("expected ']' after index");

	SetNextToken();
	return result;
}

int Parser::IssetIdent(string name, map<string, Symbol*> variables){
	for(auto& item : variables)
    	if (item.first == name)
    		return 1;
    return 0;
}