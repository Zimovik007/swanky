#include <iostream>
#include "scanner.hpp"
#include "syntax_tree.hpp"
#include "parser.hpp"
#include <map>

using namespace std;

int main(int argc, char* argv[]){
	string lex = "--lex";
	string syntax = "--syntax";
	string compile = "--compile";
	if (argc <= 2) 
		return -1;
	if (argv[2] == lex){
		Scanner lexical_analyzer(argv[1]);
		if (!lexical_analyzer.GetCntErrors())
			for (int i = 0; i < lexical_analyzer.GetLengthDeque() - 1; i++){
				lexical_analyzer.GetToken(i).PrintToken();
			}
	}
	if (argv[2] == syntax){
		Scanner lexical_analyzer(argv[1]);
		if (lexical_analyzer.GetCntErrors()){
			cout << "Лексер отработал с ошибками, синтаксический анализатор не начал работу";
			return -1;
		}
		Parser syntax_analyzer;
		Node* left = new Node();
		while (lexical_analyzer.GetLengthDeque() != lexical_analyzer.GetCurIndex()){
			syntax_analyzer.SetCurrentToken(lexical_analyzer.GetNextToken());
			syntax_analyzer.Parse(left);
		}
	}
	return 0;
}
