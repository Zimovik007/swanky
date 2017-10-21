#pragma once

#include <string>
#include <vector>

using namespace std;

class Node{
public:
	virtual ~Node(){};
	virtual void PrintNode(){};
};

class IntNode: public Node{
public:
	IntNode(string val){ value = atoi(val.c_str()); };
	void PrintNode();
private:	
	int value;
};

class FloatNode: public Node{
public:
	FloatNode(string val){ value = atof(val.c_str()); };
	void PrintNode();
private:	
	double value;
};

class LiteralNode: public Node{
public:
	LiteralNode(string val){ literal = val; };
	void PrintNode();
private:
	string literal;
};

class IdentNode: public Node{
public:
	IdentNode(string var, int sign){ name = var; is_negative = sign; };
	void PrintNode();
private:
	string name;
	int is_negative;
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
	CallFuncNode(string name, vector<Node*> arguments, int sign){func_name = name; args = arguments; is_negative = sign; };
	void PrintNode();
private:
	string func_name;
	int is_negative;
	vector<Node*> args;
};

class ArrayNode: public Node{
public:
	ArrayNode(string name, Node* i, int sign){name_array = name; index = i; is_negative = sign; };
	void PrintNode();
private:
	string name_array;
	int is_negative;
	Node* index;
};