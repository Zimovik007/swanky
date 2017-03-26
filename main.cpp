#include <iostream>
#include "scanner.hpp"
#include <map>

using namespace std;

int main(int argc, char* argv[]){
	Scanner lexical_analyzer(argv[1]);
	for (int i = 0; i < lexical_analyzer.GetLengthDeque(); i++){
		lexical_analyzer.GetToken(i).PrintToken();
	}
	return 0;
}