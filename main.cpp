#include <iostream>
#include "scanner.hpp"
#include <map>

using namespace std;

int main(int argc, char* argv[]){
	string lex = "--lex";
	Scanner lexical_analyzer(argv[1]);
	if (argv[2] == lex){
		if (!lexical_analyzer.GetCntErrors())
			for (int i = 0; i < lexical_analyzer.GetLengthDeque(); i++){
				lexical_analyzer.GetToken(i).PrintToken();
			}
	}
	return 0;
}
