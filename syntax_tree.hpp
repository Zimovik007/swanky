#pragma once

#include <string>

using namespace std;

class Node{
public:
	virtual ~Node(){};
};

class NumberNode: public Node{
public:
	NumberNode(string val){ value = atof(val.c_str()); };
private:	
	double value;
};

class VariableNode: public Node{
public:
	VariableNode(string var){ name = var; };
private:
	string name;
};

class BinaryNode: public Node{
public:
	BinaryNode(char c, Node *left, Node *right){op = c; leftChild = left; rightChild = right;}
private:
	char op;
	Node *leftChild, *rightChild;
};