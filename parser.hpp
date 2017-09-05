#pragma once

#include "token.hpp"
#include "scanner.hpp"
#include "syntax_tree.hpp"

class Parser{
public:
	Parser();
	Node* Parse(Node* left);
	Node* Expression();
	Node* Term();
	Node* Factor();
	void SetCurrentToken(Token token);
private:
	Token cur_token;
	int cnt_tokens_in_line;
};