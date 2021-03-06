#include "scanner.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

enum reserved_words{
	_AND, _ASM, _ARRAY,	_BEGIN,	_CASE, _CONST, _CONSTRUCTOR,
	_DESTRUCTOR, _DIV, _DO,	_DOWNTO, _ELSE,	_END, _EXPORTS,
	_FILE, _FOR, _FUNCTION,	_GOTO, _IF, _IMPL,
	_IN, _INHERITED, _INLINE, _INTERFACE, _INTEGER, _LABEL, _LIBRARY,
	_MOD, _NIL, _NOT, _OBJECT, _OF, _OR, _PACKED, _PROCEDURE,
	_PROGRAM, _RECORD, _REPEAT, _REAL, _SET, _SHL, _SHR, _STRING,
	_THEN, _TO, _TYPE, _UNIT, _UNTIL, _USES, _VAR, _WHILE, _WITH, _XOR
};

map<string, int> rw = {
	{"AND", _AND},               {"ASM", _ASM},             {"ARRAY", _ARRAY}, 
	{"BEGIN", _BEGIN}, 
	{"CASE", _CASE},             {"CONST", _CONST},         {"CONSTRUCTOR", _CONSTRUCTOR}, 
	{"DESTRUCTOR", _DESTRUCTOR}, {"DIV", _DIV},             {"DO", _DO},                   
	{"DOWNTO", _DOWNTO}, 
	{"ELSE", _ELSE},             {"END", _END},             {"EXPORTS", _EXPORTS}, 
	{"FILE", _FILE},             {"FOR", _FOR},             {"FUNCTION", _FUNCTION}, 
	{"GOTO", _GOTO},   
	{"IF", _IF},                 {"IMPLEMENTATION", _IMPL}, {"IN", _IN}, 
	{"INHERITED", _INHERITED},   {"INLINE", _INLINE},       {"INTERFACE", _INTERFACE}, 
	{"INTEGER", _INTEGER},
	{"LABEL", _LABEL},           {"LIBRARY", _LIBRARY}, 
	{"MOD", _MOD}, 
	{"NIL", _NIL},               {"NOT", _NOT}, 
	{"OBJECT", _OBJECT},         {"OF", _OF},               {"OR", _OR}, 
	{"PACKED", _PACKED},         {"PROCEDURE", _PROCEDURE}, {"PROGRAM", _PROGRAM}, 
	{"RECORD", _RECORD},         {"REPEAT", _REPEAT}, 		{"REAL", _REAL},
	{"SET", _SET},               {"SHL", _SHL},             {"SHR", _SHR},                 
	{"STRING", _STRING}, 
	{"THEN", _THEN},             {"TO", _TO},               {"TYPE", _TYPE}, 
	{"UNIT", _UNIT},             {"UNTIL", _UNTIL},         {"USES", _USES}, 
	{"VAR", _VAR}, 
	{"WHILE", _WHILE},           {"WITH", _WITH}, 
	{"XOR", _XOR}
};

string ToUpper(string str){
	for (int i = 0; i < str.length(); i++)
		str[i] = toupper(str[i]);
	return str;
}

void Scanner::SetToken(Token& token, char symbol, int type = -1){
	token.value += symbol;
	token.source = token.value;
	if (type >= 0)
		token.type = type;
	cur_symbol = fin.get();
}

void Scanner::AddSymbol(Token& token, char c){
	token.value += c;
	token.source += c;
	cur_symbol = fin.get();
}

Scanner::Scanner(string text){
	fin.open(text);
	errors = 0;
	index_token = 0;
	cur_pos.y = 1;
	cur_pos.x = 0;
	max_length_ident = 256;
	cur_symbol = fin.get();
	Token token_next;
	while(token_next.type != _EOF_){
		token_next = Next();
		dTokens.push_back(token_next);
	}
}

Token Scanner::GetNextToken(){
	if (index_token != dTokens.size())
		return dTokens[index_token++];
	else
		return dTokens[index_token];
}

int Scanner::GetCurIndex(){
	return index_token;
}

int Scanner::GetCntErrors(){
	return errors;
}

int Scanner::AddError(){
	errors++;
	return 0;
}

int Scanner::HandleError(Token token, string msg){
	AddError();
	cout << "(" << token.token_pos.y << ":" << token.token_pos.x << ") " << "bad token \"" << token.source << "\": " << msg << endl;
	return 0;
}

int Scanner::GetLengthDeque(){
	return dTokens.size();
}

Token Scanner::GetToken(int index){
	return dTokens[index];
}

int Scanner::ChangePos(int change_y, int change_x){
	if (change_y > 0){
		cur_pos.y += change_y;
		cur_pos.x = change_x;	
	}
	else
		cur_pos.x += change_x;
	return 0;
}

bool CheckCharOrNum(char c){
	return (((c >= 'a') && (c <= 'z')) || 
			((c >= 'A') && (c <= 'Z')) ||
			((c >= '0') && (c <= '9')));
} 

Token Scanner::Next(){
	Token token;
	while ((token.value == "") && (cur_symbol != EOF)){
		ChangePos(0, 1);
		token.token_pos = cur_pos;
		switch(cur_symbol){
			case ' ': cur_symbol = fin.get();                   break;
			case '\t': cur_symbol = fin.get();                  break;
			case '\n': cur_symbol = fin.get(); ChangePos(1, 0); break;
			case '(': SetToken(token, cur_symbol, _OPERATOR_);   break;
			case '[': SetToken(token, cur_symbol, _OPERATOR_);   break; 
			case ']': SetToken(token, cur_symbol, _OPERATOR_);   break;
			case ')': SetToken(token, cur_symbol, _OPERATOR_);   break;
			case ',': SetToken(token, cur_symbol, _SEPARATOR_);  break;
			case '=': SetToken(token, cur_symbol, _OPERATOR_);   break;
			case ';': SetToken(token, cur_symbol, _SEPARATOR_);  break;
			case '\'': token = GetLiteral(cur_symbol);          break;
			case '+': SetToken(token, cur_symbol, _OPERATOR_); 
				switch(cur_symbol){
					case '=': ChangePos(0, 1); AddSymbol(token, cur_symbol); break;
				}
				break;
			case '-': SetToken(token, cur_symbol, _OPERATOR_); 
				switch(cur_symbol){
					case '=': ChangePos(0, 1); AddSymbol(token, cur_symbol); break;
				}
				break;
			case '*': SetToken(token, cur_symbol, _OPERATOR_); 
				switch(cur_symbol){
					case '=': ChangePos(0, 1); AddSymbol(token, cur_symbol); break;
				}
				break;
			case '/': SetToken(token, cur_symbol, _OPERATOR_); 
				switch(cur_symbol){
					case '=': ChangePos(0, 1); AddSymbol(token, cur_symbol); break;
					case '/': token.value = ""; cur_symbol = fin.get(); 
						while((cur_symbol != '\n') && (cur_symbol != EOF))
							cur_symbol = fin.get(); 
						break;
				}
				break;
			case '<': SetToken(token, cur_symbol, _OPERATOR_);
				switch(cur_symbol){
					case '=': ChangePos(0, 1); AddSymbol(token, cur_symbol); break;
					case '>': ChangePos(0, 1); AddSymbol(token, cur_symbol); break;
				}
				break;
			case '>': SetToken(token, cur_symbol, _OPERATOR_);
				switch(cur_symbol){
					case '=': ChangePos(0, 1); AddSymbol(token, cur_symbol); break;
				}
				break;
			case '{': 
				while((cur_symbol != '}') && (cur_symbol != EOF)){
					cur_symbol = fin.get();
					if (cur_symbol != '\n') ChangePos(0, 1);
					else  ChangePos(1, 0);
				}
				if (cur_symbol == EOF){
					Token error_comment("{", "{", cur_pos, _SEPARATOR_);
					HandleError(error_comment, "comment not closed");
				}
				else
					cur_symbol = fin.get(); 
				break;			
			case '.': SetToken(token, cur_symbol, _OPERATOR_); 
				switch(cur_symbol){
					case '.': ChangePos(0, 1); SetToken(token, cur_symbol, _OPERATOR_); break;
					default: 
						if (cur_symbol <= '9' && cur_symbol >= '0'){
							token = GetNumber(cur_symbol, 1);
							token.value = "0." + token.value;
							token.source = "." + token.source;
						}
				}
				break;
			case ':': SetToken(token, cur_symbol, _SEPARATOR_);
				switch(cur_symbol){
					case '=': ChangePos(0, 1); SetToken(token, cur_symbol, _OPERATOR_); break;
				}
				break;
			default: 
				if (cur_symbol == '%'){ token = GetBinaryNumber(); break; }
				if (cur_symbol == '$'){ token = GetHexNumber();    break; }
				if (((cur_symbol >= 'a') && (cur_symbol <= 'z')) || 
					((cur_symbol >= 'A') && (cur_symbol <= 'Z')) || 
					 (cur_symbol == '_')){
					token = GetIdent(cur_symbol);
					if (rw.find(ToUpper(token.value))->second)
						token.type = _RESERVED_WORD_;
					break; 
				}	
				else if ((cur_symbol >= '0') && (cur_symbol <= '9')){
					token = GetNumber(cur_symbol, 0);
					break;
				}
				AddSymbol(token, cur_symbol);
				HandleError(token, "unknown symbol");
				break;
		}		
	}
	if (token.source == "") token = Token("EOF", "EOF", cur_pos, _EOF_);
	return token;
}

Token Scanner::GetBinaryNumber(){
	Token token(0, cur_symbol, cur_pos, _INTEGER_);
	cur_symbol = fin.get();
	bool error = false;
	while (CheckCharOrNum(cur_symbol) && cur_symbol != EOF){
		if (cur_symbol >= '2') error = true;
		AddSymbol(token, cur_symbol);
		ChangePos(0, 1);
	}
	if (token.source.length() > 32) HandleError(token, "integer overflow");
	else if (error || token.source.length() == 1) HandleError(token, "invalid binary number");
	else token.value = to_string(stoi(token.value, 0, 2));
	return token;
}

Token Scanner::GetHexNumber(){
	Token token(0, cur_symbol, cur_pos, _INTEGER_);
	cur_symbol = fin.get();
	bool error = false;
	while (CheckCharOrNum(cur_symbol) && cur_symbol != EOF){
		if ((cur_symbol > 'f' && cur_symbol <= 'z') ||
			(cur_symbol > 'F' && cur_symbol <= 'Z'))
				error = true;
		AddSymbol(token, cur_symbol);
		ChangePos(0, 1);
	}
	if (error) HandleError(token, "invalid hex number");
	else{
		try{ token.value = to_string(stoi(token.value, 0, 16)); }
		catch(out_of_range exep){
			token.value = "";
			HandleError(token, "integer overflow");
		}
	} 
	return token;
}

Token Scanner::GetNumber(char c, int cnt_dots){
	Token num_token(c, c, cur_pos, _INTEGER_);
	string num, num_source;
	int cnt_dot = cnt_dots;
	bool e = false;
	bool error = false;
	cur_symbol = fin.get();
	while(true){
		if ((cur_symbol >= '0') && (cur_symbol <= '9'))
			AddSymbol(num_token, cur_symbol);
		else if (GetExp(num_token, error, e)) 
			break;
		else if (cur_symbol == '.'){
			cnt_dot++;
			num = num_token.value;
			num_source = num_token.source;
			AddSymbol(num_token, cur_symbol);
			if (cur_symbol == '.'){
				Token n_token(num, num_source, num_token.token_pos, _INTEGER_);
				dTokens.push_back(n_token);
				ChangePos(0, n_token.source.length());
				Token double_dot("..", "..", cur_pos, _OPERATOR_);
				cur_symbol = fin.get();
				ChangePos(0, 1);
				return double_dot;
			}
			else if ((cur_symbol >= '0') && (cur_symbol <= '9'))
				AddSymbol(num_token, cur_symbol);
			else if (GetExp(num_token, error, e)) 
				break;		
			else 
				break;
		}
		else 
			break;
	}
	try{
		if (cnt_dot >= 1 || e){
			num_token.type = _FLOAT_;
			string check_float_overflow = to_string(stof(num_token.value));
			if (e) num_token.value = check_float_overflow;	
			if (num_token.value[num_token.value.length() - 1] == '.')
				num_token.value += '0';
		}
	}
	catch(out_of_range Exep){ HandleError(num_token, "float overflow"); }	
	if (cnt_dot > 1) HandleError(num_token, "invalid float");
	if (error) HandleError(num_token, "wrong exponent number");
	try{ if (num_token.type == _INTEGER_) int check_int_overflow = stoi(num_token.value); }
	catch(out_of_range Exep){ HandleError(num_token, "integer overflow"); }
	ChangePos(0, num_token.source.length() - 1);
	return num_token;
}

bool Scanner::GetExp(Token& token, bool& error, bool& e){
	if (cur_symbol == 'e'){
		e = true;
		error = true;
		AddSymbol(token, cur_symbol);
		if (cur_symbol == '-' || cur_symbol == '+')
			AddSymbol(token, cur_symbol);
		while((cur_symbol >= '0') && (cur_symbol <= '9')){
			error = false;
			AddSymbol(token, cur_symbol);
		}
		return true;
	}
	return false;
}

Token Scanner::GetIdent(char c){
	Token ident_token(c, c, cur_pos, _IDENT_);
	int cnt = 1;
	cur_symbol = fin.get();
	while(cur_symbol != EOF){
		cnt++;
		if (CheckCharOrNum(cur_symbol) || (cur_symbol == '_'))
			AddSymbol(ident_token, cur_symbol);
		else 
			break;
	}
	if (cnt > max_length_ident) HandleError(ident_token, "excess max length of indentifier");
	ChangePos(0, ident_token.value.length() - 1);
	return ident_token;
}

Token Scanner::GetLiteral(char c){
	bool error = false;
	Token literal_token(0, c, cur_pos, _LITERAL_);
	int cnt = 1;
	cur_symbol = fin.get();
	if (cur_symbol == '#')
		GetASCIICharacter(literal_token, error);
	else
		while(cur_symbol != EOF){
			cnt++;
			if (cur_symbol != '\'')
				AddSymbol(literal_token, cur_symbol);
			else{
				literal_token.source += cur_symbol;
				cur_symbol = fin.get();
				if (cur_symbol == '\'')
					AddSymbol(literal_token, cur_symbol);
				else if (cur_symbol == '#')
					GetASCIICharacter(literal_token, error);
				else 
					break;
			}	
		}
	if (error) HandleError(literal_token, "invalid string literal");
	ChangePos(0, literal_token.source.length());
	return literal_token;
}

void Scanner::GetASCIICharacter(Token& token, bool& error){
	while (cur_symbol == '#'){
		token.source += cur_symbol;
		string ascii_symbol = "";
		cur_symbol = fin.get();
		while (cur_symbol != '\'' && cur_symbol != '#'){
			token.source += cur_symbol;
			if ((cur_symbol >= '0') && (cur_symbol <= '9'))	
				ascii_symbol += cur_symbol;
			else{
				error = true;
				cur_symbol = fin.get();
				break;
			}
			cur_symbol = fin.get();
		}
		if (!error) token.value += stoi(ascii_symbol);
		else 
			break;
	}
	token.source += cur_symbol;
	cur_symbol = fin.get();
}