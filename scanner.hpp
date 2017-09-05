#pragma once
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "token.hpp"

class Scanner{
public:
	Scanner(std::string text);
	Token Next();
	int GetLengthDeque();
	Token GetToken(int index);
	int GetCntErrors();
	Token GetNextToken();
	int GetCurIndex();
private:
	int ChangePos(int change_y, int change_x);
	bool GetExp(Token& token, bool& error, bool& e);
	void GetASCIICharacter(Token& token, bool& error);
	Token GetLiteral(char c);
	Token GetNumber(char c, int cnt_dots);
	Token GetIdent(char c);
	Token GetBinaryNumber();
	Token GetHexNumber();
	int AddError();
	int HandleError(Token token, std::string msg);
	void AddSymbol(Token& token, char c);
	void SetToken(Token& token, char symbol, int type);
	std::vector<Token> dTokens;
	int max_length_ident;
	std::fstream fin;
	char cur_symbol;
	pos cur_pos;
	int errors;
	int index_token;
};