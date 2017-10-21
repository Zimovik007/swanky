#include "syntax_tree.hpp"
#include <iostream>

using namespace std;

void IntNode::PrintNode(){
	cout << value << " ";
}

int IntNode::GetType(){
	return type;
}

void FloatNode::PrintNode(){
	cout << value << " ";
}

int FloatNode::GetType(){
	return type;
}

void LiteralNode::PrintNode(){
	cout << literal << " ";
}

int LiteralNode::GetType(){
	return type;
}

void IdentNode::PrintNode(){
	if (is_negative)
		cout << "-" << name << " ";
	else
		cout << name << " ";
}

int IdentNode::GetType(){
	return type;
}

string IdentNode::GetName(){
	return name;
}

void BinaryNode::PrintNode(){
	cout << "[ ";
	leftChild->PrintNode(); 
	cout << op << " ";
	rightChild->PrintNode();
	cout << "] ";
}

int BinaryNode::GetType(){
	return type;
}

Node* BinaryNode::GetLeftChild(){
	return leftChild;
}

Node* BinaryNode::GetRightChild(){
	return rightChild;
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

int CallFuncNode::GetType(){
	return type;
}

string CallFuncNode::GetName(){
	return func_name;
}

void ArrayNode::PrintNode(){
	if (is_negative) 
		cout << "-";
	cout << name_array << "[ ";
	index->PrintNode();
	cout << "] ";
}

int ArrayNode::GetType(){
	return type;
}

Node* ArrayNode::GetIndexNode(){
	return index;
}