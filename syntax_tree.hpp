#pragma once

#include <string>
#include <vector>

using namespace std;

class Node{
public:
	virtual ~Node(){};
	virtual void PrintNode(){};
};

class NumberNode: public Node{
public:
	NumberNode(string val){ value = atof(val.c_str()); };
	void PrintNode();
private:	
	double value;
};

class IdentNode: public Node{
public:
	IdentNode(string var){ name = var; };
	void PrintNode();
private:
	string name;
};

class BinaryNode: public Node{
public:
	BinaryNode(string c, Node *left, Node *right){op = c; leftChild = left; rightChild = right;}
	void PrintNode();
private:
	int flag;
	string op;
	Node *leftChild, *rightChild;
};

class CallFuncNode: public Node{
public:
	CallFuncNode(string name, vector<Node*> arguments){func_name = name; args = arguments;};
	void PrintNode();
private:
	string func_name;
	vector<Node*> args;
};