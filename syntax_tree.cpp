#include "syntax_tree.hpp"
#include <iostream>

using namespace std;

void IntNode::PrintNode(){
	cout << value << " ";
}

void FloatNode::PrintNode(){
	cout << value << " ";
}

void LiteralNode::PrintNode(){
	cout << literal << " ";
}

void IdentNode::PrintNode(){
	if (is_negative)
		cout << "-" << name << " ";
	else
		cout << name << " ";
}

void BinaryNode::PrintNode(){
	cout << "[ ";
	leftChild->PrintNode(); 
	cout << op << " ";
	rightChild->PrintNode();
	cout << "] ";
}

void CallFuncNode::PrintNode(){
	if (args.size()){
		if (is_negative)
			cout << " -" << func_name << "( ";
		else
			cout << " " << func_name << "( ";
		for (int i = 0; i < args.size() - 1; i++){
			args[i]->PrintNode(); 
			cout << ", ";
		}
		args[args.size() - 1]->PrintNode();
		cout << ") ";
	}
	else{
		cout << " " << func_name << "() ";
	}
}

void ArrayNode::PrintNode(){
	if (is_negative) 
		cout << "-";
	cout << name_array << "[ ";
	index->PrintNode();
	cout << "] ";
}