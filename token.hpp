#pragma once
#include <string>
#include <map>

enum types{
	_OPERATOR,
	_LITERAL,
	_INTEGER,
	_FLOAT,
	_IDENT,
	_SEPARATOR,
	_RESERVED_WORD,
	_EOF,
};

static std::map<int, std::string> tp = {
	{_OPERATOR, "operator"},
	{_LITERAL, "string literal"},
	{_INTEGER, "integer literal"},
	{_FLOAT, "float literal"},
	{_IDENT, "identifier"},
	{_SEPARATOR, "separator"},
	{_RESERVED_WORD, "reserved"},
	{_EOF, "[end of file]"},
};

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