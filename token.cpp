#include <iostream>
#include "token.hpp"

using namespace std;

Token::Token(){
	value = "";
}

Token::Token(string _value, string _source, pos _token_pos, int _type){
	value = _value;
	source = _source;
	token_pos = _token_pos;
	type = _type;
}

Token::Token(char _value, char _source, pos _token_pos, int _type){
	if (_value)
		value += _value;
	source += _source;
	token_pos = _token_pos;
	type = _type;
}

int Token::PrintToken(int endl){
	int offset_pos = 5 + to_string(token_pos.y).length() + to_string(token_pos.x).length();
	cout << "(" << token_pos.y << ", " << token_pos.x << ") ";
	for (int i = 0; i < 10 - offset_pos; i++){
		cout << " ";
	}
	cout << tp[type] << " ";
	for (int i = 0; i < 17 - tp[type].length(); i++){
		cout << " ";
	}
	cout << value << "  ~" << source << "~";
	if (endl)
		cout << endl;
	return 0;
}

int Token::GetType(){
	return type;
}