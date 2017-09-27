#pragma once

#include <vector>
#include <map>
#include "syntax_tree.hpp"

class Symbol;

class Block{
public:
	Block(){used_var = 0;};
	void PrintNode();
	void AddToBlock(Node* node);
	int AddVariable(string name, Symbol* symbol);
	map<string, Symbol*> GetLocalVariables();
	void PushLocalVariables(map<string, Symbol*> l_vars);
	int GetSizeBlock();
	Node* GetElemFromBlock(int index);
private:
	vector<Node*> body_function;
	map<string, Symbol*> local_variables;
	int used_var;
};