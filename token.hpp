#pragma once
#include <string>
#include <map>

enum types{
	_OPERATOR_,
	_LITERAL_,
	_INTEGER_,
	_FLOAT_,
	_IDENT_,
	_SEPARATOR_,
	_RESERVED_WORD_,
	_EOF_,
};

static std::map<int, std::string> tp = {
	{_OPERATOR_, "operator"},
	{_LITERAL_, "string literal"},
	{_INTEGER_, "integer literal"},
	{_FLOAT_, "float literal"},
	{_IDENT_, "identifier"},
	{_SEPARATOR_, "separator"},
	{_RESERVED_WORD_, "reserved"},
	{_EOF_, "[end of file]"},
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
	int PrintToken(int endl = 1);
	int GetType();
	pos token_pos;
	std::string value;
	std::string source;
	int type;
};