#include "scanner.hpp"

using namespace std;

enum reserved_words{
	_AND,
	_ASM,
	_ARRAY,
	_BEGIN,
	_CASE,
	_CONST,
	_CONSTRUCTOR,
	_DESTRUCTOR,
	_DIV,
	_DO,
	_DOWNTO,
	_ELSE,
	_END,
	_EXPORTS,
	_FILE,
	_FOR,
	_FUNCTION,
	_GOTO,
	_IF,
	_IMPLEMENTATION,
	_IN,
	_INHERITED,
	_INLINE,
	_INTERFACE,
	_LABEL,
	_LIBRARY,
	_MOD,
	_NIL,
	_NOT,
	_OBJECT,
	_OF,
	_OR,
	_PACKED,
	_PROCEDURE,
	_PROGRAM,
	_RECORD,
	_REPEAT,
	_SET,
	_SHL,
	_SHR,
	_STRING,
	_THEN,
	_TO,
	_TYPE,
	_UNIT,
	_UNTIL,
	_USES,
	_VAR,
	_WHILE,
	_WITH,
	_XOR
};

map<string, int> rw = {
	{"AND", _AND}, 
	{"ASM", _ASM}, 
	{"ARRAY", _ARRAY}, 
	{"BEGIN", _BEGIN}, 
	{"CASE", _CASE}, 
	{"CONST", _CONST}, 
	{"CONSTRUCTOR", _CONSTRUCTOR}, 
	{"DESTRUCTOR", _DESTRUCTOR}, 
	{"DIV", _DIV}, 
	{"DO", _DO}, 
	{"DOWNTO", _DOWNTO}, 
	{"ELSE", _ELSE}, 
	{"END", _END}, 
	{"EXPORTS", _EXPORTS}, 
	{"FILE", _FILE}, 
	{"FOR", _FOR}, 
	{"FUNCTION", _FUNCTION}, 
	{"GOTO", _GOTO}, 
	{"IF", _IF}, 
	{"IMPLEMENTATION", _IMPLEMENTATION}, 
	{"IN", _IN}, 
	{"INHERITED", _INHERITED}, 
	{"INLINE", _INLINE}, 
	{"INTERFACE", _INTERFACE}, 
	{"LABEL", _LABEL}, 
	{"LIBRARY", _LIBRARY}, 
	{"MOD", _MOD}, 
	{"NIL", _NIL}, 
	{"NOT", _NOT}, 
	{"OBJECT", _OBJECT}, 
	{"OF", _OF}, 
	{"OR", _OR}, 
	{"PACKED", _PACKED}, 
	{"PROCEDURE", _PROCEDURE}, 
	{"PROGRAM", _PROGRAM}, 
	{"RECORD", _RECORD}, 
	{"REPEAT", _REPEAT}, 
	{"SET", _SET}, 
	{"SHL", _SHL}, 
	{"SHR", _SHR}, 
	{"STRING", _STRING}, 
	{"THEN", _THEN}, 
	{"TO", _TO}, 
	{"TYPE", _TYPE}, 
	{"UNIT", _UNIT}, 
	{"UNTIL", _UNTIL}, 
	{"USES", _USES}, 
	{"VAR", _VAR}, 
	{"WHILE", _WHILE}, 
	{"WITH", _WITH}, 
	{"XOR", _XOR}
};

enum types{
	_OPERATOR,
	_LITERAL,
	_COMMENT,
	_NUMBER,
	_IDENT,
	_SEPARATOR,
	_RESERVED_WORD,
};

map<string, int> tp = {
	{"OPERATOR", _OPERATOR},
	{"LITERAL", _LITERAL},
	{"COMMENT", _COMMENT},
	{"NUMBER", _NUMBER},
	{"IDENT", _IDENT},	
	{"SEPARATOR", _SEPARATOR},
	{"RESERVED WORD", _RESERVED_WORD}
};

Token::Token(){
	value = "";
}

Scanner::Scanner(string text){
	fin.open(text);
	cur_pos.y = 1;
	cur_pos.x = 0;
	max_length_ident = 16;
	max_length_literal = 256;
}

int Scanner::change_pos(int change_y, int change_x){
	if (change_y > 0){
		cur_pos.x = 1;
		cur_pos.y += change_y;	
	}
	else
		cur_pos.x += change_x;
	return 0;
}

Token Scanner::Next(){
	Token token;
	switch(cur_symbol){
		case ' ': fin >> cur_symbol; while(cur_symbol == ' ') fin >> cur_symbol; break;
		case '+': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol; 
			switch(cur_symbol){
				case '=': token.value += cur_symbol; fin >> cur_symbol; break;
			}
			break;
		case '-': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol; 
			switch(cur_symbol){
				case '=': token.value += cur_symbol; fin >> cur_symbol; break;
			}
			break;
		case '*': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol; 
			switch(cur_symbol){
				case '=': token.value += cur_symbol; fin >> cur_symbol; break;
			}
			break;
		case '/': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol; 
			switch(cur_symbol){
				case '=': token.value += cur_symbol; fin >> cur_symbol; break;
			}
			break;
		case '<': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol; 
			switch(cur_symbol){
				case '=': token.value += cur_symbol; fin >> cur_symbol; break;
				case '>': token.value += cur_symbol; fin >> cur_symbol; break;
			}
			break;
		case '>': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol; 
			switch(cur_symbol){
				case '=': token.value += cur_symbol; fin >> cur_symbol; break;
			}
			break;
		case '[': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol;
		case ']': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol;
		case '(': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol;
		case ')': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol;
		case ',': token.value += cur_symbol; token.type = _SEPARATOR; fin >> cur_symbol;
		case ':': token.value += cur_symbol; token.type = _SEPARATOR; fin >> cur_symbol; 
			switch(cur_symbol){
				case '=': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol; break;
			}
			break;
		case '=': token.value += cur_symbol; token.type = _OPERATOR; fin >> cur_symbol; break;
		case '"': token = GetLiteral(cur_symbol); token.type = _LITERAL; fin >> cur_symbol; break;
		case '\'': token = GetLiteral(cur_symbol); token.type = _LITERAL; fin >> cur_symbol; break;
		case ';': token.value += cur_symbol; token.type = _SEPARATOR;  fin >> cur_symbol; break;
		default: 
			if (((cur_symbol >= 'a') && (cur_symbol <= 'z')) || ((cur_symbol >= 'A') && (cur_symbol <= 'Z'))){
				token = GetIdent(cur_symbol);
				token.type = _IDENT; 
			}	
			else if ((cur_symbol >= '0') && (cur_symbol <= '9')){
				token = GetNumber(cur_symbol);
				token.type = _NUMBER;
			}
			break;
	}
	return token;
}

Token Scanner::GetNumber(char c){
	Token num_token;
	num_token.value = c;
	int cnt_dot = 0;
	while(true){
		fin >> cur_symbol;
		if ((cur_symbol >= '0') && (cur_symbol <= '9')){
			num_token.value += cur_symbol;
		}
		else
		if (cur_symbol == '.'){
			cnt_dot++;
			num_token.value += cur_symbol;
		}
		else{
			break;
		}
	}
	return num_token;
}

Token Scanner::GetIdent(char c){
	Token ident_token;
	ident_token.value = c;
	int cnt = 1;
	while(cnt <= max_length_ident){
		cnt++;
		fin >> cur_symbol;
		if (((cur_symbol >= 'a') && (cur_symbol <= 'z')) || ((cur_symbol >= 'A') && (cur_symbol <= 'Z'))){
			ident_token.value += cur_symbol;
		}
		else 
		if ((cur_symbol >= '0') && (cur_symbol <= '9')){
			ident_token.value += cur_symbol;
		}
		else{
			break;
		}
	}
	if (cnt > max_length_ident){
		exit(-1);
	}
	return ident_token;
}

Token Scanner::GetLiteral(char c){
	Token literal_token;
	int cnt = 1;
	while(cnt <= max_length_literal){
		cnt++;
		fin >> cur_symbol;
		if ((cur_symbol != c) && (cur_symbol != '\'')){
			literal_token.value += cur_symbol;
		}
		else{
			char _c = cur_symbol;
			fin >> cur_symbol;
			if ((_c == c) && (c == '"')) break;
			if (cur_symbol == '\''){
				literal_token.value += cur_symbol;
			}
			else
			if (cur_symbol == '#'){
				string ascii_symbol = "";
				fin >> cur_symbol;
				while (cur_symbol != '\''){
					if ((cur_symbol >= '0') && (cur_symbol <= '9')){
						ascii_symbol += cur_symbol;
						fin >> cur_symbol;
					}
					else{
						exit(-1);
					}
				}
				char ascii = stoi(ascii_symbol);
				literal_token.value += ascii; 
			}
			else{
				break;
			}
		}
	}
	if (cnt > max_length_literal){
		exit(-1);
	}
	return literal_token;
}