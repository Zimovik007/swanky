#include <map>
#include <iostream>
#include "parser.hpp"
#include "syntax_tree.hpp"

#define T_operator 0
#define T_literal  1
#define T_integer  2
#define T_float    3
#define T_ident    4
#define T_separate 5
#define T_reserved 6
#define T_eof      7

using namespace std;

map<int, string> priority = {
	{1, "@"},
	{1, "NOT"},
	{2, "*"},
	{2, "/"},
	{2, "DIV"},
	{2, "MOD"},
	{2, "AND"},
	{3, "+"},
	{3, "-"},
	{3, "OR"},
	{3, "XOR"},
	{4, "="},
	{4, "<>"},
	{4, "<"},
	{4, ">"},
	{4, "<="},
	{4, ">="},
	{4, "IN"},
};

Parser::Parser(){

}

void Parser::SetCurrentToken(Token token){
	cur_token = token;
}

Node* Parser::Parse(Node* left){

}

Node* Parser::Expression(){

}

Node* Parser::Term(){

}

Node* Parser::Factor(){
	if ((cur_token.GetType() == T_integer) or (cur_token.GetType() == T_float)){
		Node* node = new NumberNode(cur_token.value);
		return node;
	}
	else
	if (cur_token.GetType() == T_ident){
		Node* node = new VariableNode(cur_token.value);
		return node;
	}
}