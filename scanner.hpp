#pragma once
#include <string>
#include <fstream>
#include <map>

class Scanner{
public:
	Scanner(std::string text);
	std::string getToken();
private:
	std::fstream fin;
	std::string source;
	std::string* tokens;
};