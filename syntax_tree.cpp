#include "syntax_tree.hpp"
#include <iostream>

using namespace std;

void IntNode::PrintNode(){
	cout << value << " ";
}

void FloatNode::PrintNode(){
	cout << value << " ";
}

void IdentNode::PrintNode(){
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
	cout << name_array << "[ ";
	index->PrintNode();
	cout << "] ";
}