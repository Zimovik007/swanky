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
};

map<string, int> tp = {
	{"OPERATOR", _OPERATOR},
	{"LITERAL", _LITERAL},
	{"COMMENT", _COMMENT},
	{"NUMBER", _NUMBER},
	{"IDENT", _IDENT},	
	{"SEPARATOR", _SEPARATOR}
};

Token::Token(){
	value = "";
}

Scanner::Scanner(string text){
	fin.open(text);
	cur_pos.y = 1;
	cur_pos.x = 0;
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
	fin >> cur_symbol;
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
			if (((cur_symbol >= 65) && (cur_symbol <= 90)) || ((cur_symbol >= 97) && (cur_symbol <= 122))){
				token = GetIdent(cur_symbol);
				token.type = _IDENT; 
			}	
			else if ((cur_symbol >= 48) && (cur_symbol <= 57)){
				token = GetNumber(cur_symbol);
				token.type = _NUMBER;
			}
			break;
	}
}

Token Scanner::GetNumber(char c){

}

Token Scanner::GetIdent(char c){

}

Token Scanner::GetLiteral(char c){

}