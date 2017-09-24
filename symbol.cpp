#include "symbol.hpp"

using namespace std;

//VARIABLE INTEGER

string VarIntSymbol::GetType(){
	return type;
}

int VarIntSymbol::IsVar(){
	return is_var;
}

//VARIABLE FLOAT

string VarFloatSymbol::GetType(){
	return type;
}

int VarFloatSymbol::IsVar(){
	return is_var;
}

//VARIABLE STRING

string VarStringSymbol::GetType(){
	return type;
}

int VarStringSymbol::IsVar(){
	return is_var;
}

//VARIABLE RECORD

string VarRecordSymbol::GetType(){
	return type;
}

int VarRecordSymbol::IsVar(){
	return is_var;
}

map<string, Symbol*> VarRecordSymbol::GetMapElements(){
	return elems;
}

//VARIABLE ARRAY

string VarArraySymbol::GetType(){
	return type;
}

int VarArraySymbol::IsVar(){
	return is_var;
}

int VarArraySymbol::IndexStart(){
	return index_start;
}

int VarArraySymbol::IndexEnd(){
	return index_end;
}

Symbol* VarArraySymbol::GetTypeElements(){
	return type_elems;
}

//CONST INTEGER

string ConstIntSymbol::GetType(){
	return type;
}

int ConstIntSymbol::IsVar(){
	return is_var;
}

//CONST FLOAT

string ConstFloatSymbol::GetType(){
	return type;
}

int ConstFloatSymbol::IsVar(){
	return is_var;
}

//CONST STRING

string ConstStringSymbol::GetType(){
	return type;
}

int ConstStringSymbol::IsVar(){
	return is_var;
}

//CONST RECORD

string ConstRecordSymbol::GetType(){
	return type;
}

int ConstRecordSymbol::IsVar(){
	return is_var;
}

map<string, Symbol*> ConstRecordSymbol::GetMapElements(){
	return elems;
}

//CONST ARRAY

string ConstArraySymbol::GetType(){
	return type;
}

int ConstArraySymbol::IsVar(){
	return is_var;
}

int ConstArraySymbol::IndexStart(){
	return index_start;
}

int ConstArraySymbol::IndexEnd(){
	return index_end;
}

Symbol* ConstArraySymbol::GetTypeElements(){
	return type_elems;
}

//FUNCTION

string FuncSymbol::GetType(){
	return type;
}

int FuncSymbol::IsVar(){
	return is_var;
}

map<string, Symbol*> FuncSymbol::GetMapElements(){
	return map_params;
}

vector<Symbol*> FuncSymbol::GetVecElements(){
	return vec_params;
}

string FuncSymbol::GetReturnType(){
	return return_type;
}