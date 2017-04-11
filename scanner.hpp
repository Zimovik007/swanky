#pragma once
#include <string>
#include <fstream>
#include <map>
#include <deque>
#include "token.hpp"

class Scanner{
public:
	Scanner(std::string text);
	Token Next();
	int GetLengthDeque();
	Token GetToken(int index);
	int GetCntErrors();
private:
	int ChangePos(int change_y, int change_x);
	Token GetLiteral(char c);
	Token GetNumber(char c);
	Token GetIdent(char c);
	Token GetBinaryNumber();
	Token GetHexNumber();
	int AddError();
	int HandleError(Token token, std::string msg);
	void AddSymbol(Token& token, char c);
	void SetToken(Token& token, char symbol, int type);
	std::deque<Token> dTokens;
	int max_length_ident;
	std::fstream fin;
	char cur_symbol;
	pos cur_pos;
	int errors;
};