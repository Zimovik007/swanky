#pragma once
#include <string>
#include <fstream>
#include <map>
#include <deque>

typedef struct Position{
	int x, y;
} pos;

class Token{
public:
	Token();
	Token& operator=(const Token& right){
		value = right.value;
		token_pos = right.token_pos;
		type = right.type;
	}
	int PrintToken();
	pos token_pos;
	std::string value;
	int type;
};

class Scanner{
public:
	Scanner(std::string text);
	int ChangePos(int change_y, int change_x);
	pos GetPos();
	Token Next();
	Token GetLiteral(char c);
	Token GetNumber(char c);
	Token GetIdent(char c);
	int GetLengthDeque();
	Token GetToken(int index);
private:
	std::deque<Token> dTokens;
	int max_length_literal;
	int max_length_ident;
	std::fstream fin;
	char cur_symbol;
	pos cur_pos;
};