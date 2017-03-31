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
	Token(std::string _value, std::string _source, pos _token_pos, int _type);
	Token(char _value, char _source, pos _token_pos, int _type);
	Token& operator=(const Token& right){
		value = right.value;
		token_pos = right.token_pos;
		type = right.type;
		source = right.source;
	}
	int PrintToken();
	pos token_pos;
	std::string value;
	std::string source;
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