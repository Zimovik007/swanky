#include "block.hpp"
#include "symbol.hpp"

void Block::AddToBlock(Node* node){
	body_function.push_back(node);	
}

int Block::GetSizeBlock(){
	return body_function.size();
}

Node* Block::GetElemFromBlock(int index){
	return body_function[index];
}

void Block::PrintNode(){
	for (int i = 0; i < body_function.size(); i++)
		body_function[i]->PrintNode();
}

int Block::AddVariable(string name, Symbol* symbol){
	if (local_variables.find(name)->second){
		return 0;
	}
	local_variables[name] = symbol;
	return 1;
}

map<string, Symbol*> Block::GetLocalVariables(){
	return local_variables;
}

void Block::PushLocalVariables(map<string, Symbol*> l_vars){
	local_variables = l_vars;
}