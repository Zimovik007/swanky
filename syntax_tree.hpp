#pragma once

#include <string>
#include <vector>

using namespace std;

class Node{
public:
	virtual ~Node(){};
	virtual void PrintNode(){};
	virtual int GetType(){};
	virtual Node* GetLeftChild(){};
	virtual Node* GetRightChild(){};
	virtual string GetName(){};
	virtual Node* GetIndexNode(){};
};

class IntNode: public Node{
public:
	IntNode(string val){ value = atoi(val.c_str()); type = 2; };
	void PrintNode();
	int GetType();
private:	
	int value;
	int type;
};

class FloatNode: public Node{
public:
	FloatNode(string val){ value = atof(val.c_str()); type = 3; };
	void PrintNode();
	int GetType();
private:	
	double value;
	int type;
};

class LiteralNode: public Node{
public:
	LiteralNode(string val){ literal = val; type = 1; };
	void PrintNode();
	int GetType();
private:
	string literal;
	int type;
};

class IdentNode: public Node{
public:
	IdentNode(string var, int sign){ name = var; is_negative = sign; type = 4; };
	void PrintNode();
	int GetType();
	string GetName();
private:
	string name;
	int is_negative;
	int type;
};

class BinaryNode: public Node{
public:
	BinaryNode(string c, Node *left, Node *right){op = c; leftChild = left; rightChild = right; type = 0; }
	void PrintNode();
	int GetType();
	Node* GetLeftChild();
	Node* GetRightChild();
private:
	int flag;
	string op;
	Node *leftChild, *rightChild;
	int type;
};

class CallFuncNode: public Node{
public:
	CallFuncNode(string name, vector<Node*> arguments, int sign){func_name = name; args = arguments; is_negative = sign; type = 8; };
	void PrintNode();
	int GetType();
	string GetName();
private:
	string func_name;
	int is_negative;
	vector<Node*> args;
	int type;
};

class ArrayNode: public Node{
public:
	ArrayNode(string name, Node* i, int sign){name_array = name; index = i; is_negative = sign; type = 10; };
	void PrintNode();
	int GetType();
	Node* GetIndexNode();
	string GetName();
private:
	string name_array;
	int is_negative;
	Node* index;
	int type;
};