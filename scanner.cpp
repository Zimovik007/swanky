#include "scanner.hpp"
#include <iostream>
#include <algorithm>

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

map<int, string> tp = {
	{_OPERATOR, "OPERATOR"},
	{_LITERAL, "LITERAL"},
	{_NUMBER, "NUMBER"},
	{_IDENT, "IDENT"},
	{_SEPARATOR, "SEPARATOR"},
	{_RESERVED_WORD, "RESERVED WORDS"}
};



Token::Token(){
	value = "";
}

int Token::PrintToken(){
	int offset_pos = 5 + to_string(token_pos.y).length() + to_string(token_pos.x).length();
	cout << "(" << token_pos.y << ", " << token_pos.x << ") ";
	for (int i = 0; i < 10 - offset_pos; i++){
		cout << " ";
	}
	cout << tp[type] << " ";
	for (int i = 0; i < 17 - tp[type].length(); i++){
		cout << " ";
	}
	cout << value << "  ~" << source << "~" << endl; 
	return 0;
}

Scanner::Scanner(string text){
	fin.open(text);
	cur_pos.y = 1;
	cur_pos.x = 0;
	max_length_ident = 16;
	max_length_literal = 256;
	cur_symbol = fin.get();
	while(cur_symbol != EOF)
		dTokens.push_back(Next());
}

int Scanner::GetLengthDeque(){
	return dTokens.size();
}

Token Scanner::GetToken(int index){
	return dTokens[index];
}

int Scanner::ChangePos(int change_y, int change_x){
	if (change_y > 0){
		cur_pos.x = 0;
		cur_pos.y += change_y;
		cur_pos.x += change_x;	
	}
	else
		cur_pos.x += change_x;
	return 0;
}

pos Scanner::GetPos(){
	return cur_pos;
}

Token Scanner::Next(){
	Token token;
	while ((token.value == "") && (cur_symbol != EOF)){
		ChangePos(0, 1);
		token.token_pos = GetPos();
		switch(cur_symbol){
			case ' ': cur_symbol = fin.get(); break;
			case '\t': cur_symbol = fin.get(); break;
			case '\n': cur_symbol = fin.get(); ChangePos(1, 0); break;
			case '+': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); 
				switch(cur_symbol){
					ChangePos(0, 1);
					case '=': token.value += cur_symbol; token.source = token.value; cur_symbol = fin.get(); break;
				}
				break;
			case '-': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); 
				switch(cur_symbol){
					ChangePos(0, 1);
					case '=': token.value += cur_symbol; token.source = token.value; cur_symbol = fin.get(); break;
				}
				break;
			case '*': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); 
				switch(cur_symbol){
					ChangePos(0, 1);
					case '=': token.value += cur_symbol; token.source = token.value; cur_symbol = fin.get(); break;
				}
				break;
			case '/': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); 
				switch(cur_symbol){
					ChangePos(0, 1);
					case '=': token.value += cur_symbol; token.source = token.value; cur_symbol = fin.get(); break;
					case '/': token.value = ""; cur_symbol = fin.get(); 
						while((cur_symbol != '\n') && (cur_symbol != EOF))
							cur_symbol = fin.get(); 
						break;
				}
				break;
			case '<': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); 
				switch(cur_symbol){
					ChangePos(0, 1);
					case '=': token.value += cur_symbol; token.source = token.value; cur_symbol = fin.get(); break;
					case '>': token.value += cur_symbol; token.source = token.value; cur_symbol = fin.get(); break;
				}
				break;
			case '>': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); 
				switch(cur_symbol){
					ChangePos(0, 1);
					case '=': token.value += cur_symbol; token.source = token.value; cur_symbol = fin.get(); break;
				}
				break;
			case '{': 
				while((cur_symbol != '}') && (cur_symbol != EOF)){
					cur_symbol = fin.get();
					if (cur_symbol != '\n') ChangePos(0, 1);
					else  ChangePos(1, 0);
				} 
				cur_symbol = fin.get(); 
				break;
			case '(': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); break;
			case '[': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); break; 
			case ']': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); break;
			case ')': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); break;
			case ',': token.value += cur_symbol; token.source = token.value; token.type = _SEPARATOR; cur_symbol = fin.get(); break;
			case '.': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); 
				switch(cur_symbol){
					ChangePos(0, 1);
					case '.': token.value += cur_symbol; token.source = token.value; token.type = _SEPARATOR; cur_symbol = fin.get(); break;
				}
				break;
			case ':': token.value += cur_symbol; token.source = token.value; token.type = _SEPARATOR; cur_symbol = fin.get();
				switch(cur_symbol){
					ChangePos(0, 1);
					case '=': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); break;
				}
				break;
			case '=': token.value += cur_symbol; token.source = token.value; token.type = _OPERATOR; cur_symbol = fin.get(); break;
			case '"': token = GetLiteral(cur_symbol); token.type = _LITERAL; break;
			case '\'': token = GetLiteral(cur_symbol); token.type = _LITERAL; break;
			case ';': token.value += cur_symbol; token.source = token.value; token.type = _SEPARATOR;  cur_symbol = fin.get(); break;
			default: 
				if (((cur_symbol >= 'a') && (cur_symbol <= 'z')) || ((cur_symbol >= 'A') && (cur_symbol <= 'Z'))){
					token = GetIdent(cur_symbol);
					string check_rw = token.value;
					transform(check_rw.begin(), check_rw.end(), check_rw.begin(), ::toupper);
					if (rw[check_rw] > 0)
						token.type = _RESERVED_WORD;
					else
						token.type = _IDENT;
					break; 
				}	
				else if ((cur_symbol >= '0') && (cur_symbol <= '9')){
					token = GetNumber(cur_symbol);
					break;
				}
				exit(-1);
		}		
	}	
	return token;
}

Token Scanner::GetNumber(char c){
	Token num_token;
	num_token.source = c;
	num_token.value = c;
	num_token.token_pos = cur_pos;
	num_token.type = _NUMBER;
	int cnt_dot = 0;
	while(true){
		cur_symbol = fin.get();
		if ((cur_symbol >= '0') && (cur_symbol <= '9')){
			ChangePos(0, 1);
			num_token.source += cur_symbol;
			num_token.value += cur_symbol;
		}
		else
		if (cur_symbol == '.'){
			ChangePos(0, 1);
			cnt_dot++;
			string num = num_token.value;
			string num_source = num_token.source;
			num_token.value += cur_symbol;
			num_token.source += cur_symbol;
			cur_symbol = fin.get();
			if (cur_symbol == '.'){
				Token n_token;
				n_token.value = num;
				n_token.source = num_source;
				n_token.type = _NUMBER;
				n_token.token_pos = num_token.token_pos;
				dTokens.push_back(n_token);
				num_token.value = "..";
				num_token.source = num_token.value;
				num_token.type = _SEPARATOR;
				num_token.token_pos = cur_pos;
				ChangePos(0, 1);
				cur_symbol = fin.get();
				return num_token;
			}
			else
			if ((cur_symbol >= '0') && (cur_symbol <= '9')){
				num_token.value += cur_symbol;
				num_token.source += cur_symbol;
			}
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
	ident_token.source = c;
	ident_token.token_pos = cur_pos;
	int cnt = 1;
	while(cnt <= max_length_ident){
		cnt++;
		cur_symbol = fin.get();
		if (((cur_symbol >= 'a') && (cur_symbol <= 'z')) || ((cur_symbol >= 'A') && (cur_symbol <= 'Z'))){
			ident_token.value += cur_symbol;
			ident_token.source += cur_symbol;
		}
		else 
		if ((cur_symbol >= '0') && (cur_symbol <= '9')){
			ident_token.value += cur_symbol;
			ident_token.source += cur_symbol;
		}
		else{
			break;
		}
	}
	if (cnt > max_length_ident){
		exit(-1);
	}
	ChangePos(0, ident_token.value.length() - 1);
	return ident_token;
}

Token Scanner::GetLiteral(char c){
	Token literal_token;
	literal_token.token_pos = cur_pos;
	literal_token.source = c;
	int len_symbol = 0;
	int cnt = 1;
	while(cnt <= max_length_literal){
		cnt++;
		cur_symbol = fin.get();
		if (((c == '\'') && (cur_symbol != '\'')) || ((c == '"') && (cur_symbol != '"'))){
			literal_token.source += cur_symbol;
			literal_token.value += cur_symbol;
		}
		else{
			char _c = cur_symbol;
			cur_symbol = fin.get();
			if ((_c == c) && (c == '"')) break;
			if (cur_symbol == '\''){
				literal_token.source += cur_symbol;
				literal_token.source += cur_symbol;
				literal_token.value += cur_symbol;
			}
			else
			if (cur_symbol == '#'){
				literal_token.source += '\'';
				literal_token.source += cur_symbol;
				len_symbol = 2;
				string ascii_symbol = "";
				cur_symbol = fin.get();
				len_symbol++;
				while (cur_symbol != '\''){
					if ((cur_symbol >= '0') && (cur_symbol <= '9')){
						literal_token.source += cur_symbol;	
						ascii_symbol += cur_symbol;
						cur_symbol = fin.get();
					}
					else{
						exit(-1);
					}
				}
				literal_token.source += '\'';
				char ascii = stoi(ascii_symbol);
				literal_token.value += ascii; 
			}
			else{
				break;
			}
		}
	}
	literal_token.source += c;
	if (cnt > max_length_literal){
		exit(-1);
	}
	ChangePos(0, literal_token.value.length() + len_symbol);
	return literal_token;
}
