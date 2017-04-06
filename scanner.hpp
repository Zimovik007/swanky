#pragma once
#include <string>
#include <fstream>
#include <map>
#include <deque>
#include "token.hpp"

class Scanner{
public:
	Scanner(std::string text);
	int ChangePos(int change_y, int change_x);
	Token Next();
	Token GetLiteral(char c);
	Token GetNumber(char c);
	Token GetIdent(char c);
	Token GetBinaryNumber();
	Token GetHexNumber();
	int GetLengthDeque();
	Token GetToken(int index);
	int ErrorHandler(Token token, std::string msg);
	int AddError();
	int GetCntErrors();
private:
	std::deque<Token> dTokens;
	int max_length_ident;
	std::fstream fin;
	char cur_symbol;
	pos cur_pos;
	int errors;
};