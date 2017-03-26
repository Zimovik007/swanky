#pragma once
#include <string>
#include <fstream>
#include <map>

typedef struct Position{
	int x, y;
} pos;

class Token{
public:
	Token();
	pos token_pos;
	std::string value;
	int type;
};

class Scanner{
public:
	Scanner(std::string text);
	int change_pos(int change_y, int change_x);
	Token Next();
	Token GetLiteral(char c);
	Token GetNumber(char c);
	Token GetIdent(char c);
private:
	int max_length_literal;
	int max_length_ident;
	std::fstream fin;
	char cur_symbol;
	pos cur_pos;
};