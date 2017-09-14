#include "syntax_tree.hpp"
#include <iostream>

using namespace std;

void NumberNode::PrintNode(){
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
	cout << " " << func_name << "( ";
	for (int i = 0; i < args.size() - 1; i++){
		args[i]->PrintNode(); 
		cout << ", ";
	}
	args[args.size() - 1]->PrintNode();
	cout << ") ";
}