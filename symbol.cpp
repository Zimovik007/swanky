#include "symbol.hpp"

using namespace std;

//VARIABLE INTEGER

int VarIntSymbol::GetType(){
	return type;
}

int VarIntSymbol::IsVar(){
	return is_var;
}

//VARIABLE FLOAT

int VarFloatSymbol::GetType(){
	return type;
}

int VarFloatSymbol::IsVar(){
	return is_var;
}

//VARIABLE STRING

int VarStringSymbol::GetType(){
	return type;
}

int VarStringSymbol::IsVar(){
	return is_var;
}

//VARIABLE RECORD

int VarRecordSymbol::GetType(){
	return type;
}

int VarRecordSymbol::IsVar(){
	return is_var;
}

map<string, Symbol*> VarRecordSymbol::GetMapElements(){
	return elems;
}

//VARIABLE ARRAY

int VarArraySymbol::GetType(){
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

int ConstIntSymbol::GetType(){
	return type;
}

int ConstIntSymbol::IsVar(){
	return is_var;
}

//CONST FLOAT

int ConstFloatSymbol::GetType(){
	return type;
}

int ConstFloatSymbol::IsVar(){
	return is_var;
}

//CONST STRING

int ConstStringSymbol::GetType(){
	return type;
}

int ConstStringSymbol::IsVar(){
	return is_var;
}

//CONST RECORD

int ConstRecordSymbol::GetType(){
	return type;
}

int ConstRecordSymbol::IsVar(){
	return is_var;
}

map<string, Symbol*> ConstRecordSymbol::GetMapElements(){
	return elems;
}

//CONST ARRAY

int ConstArraySymbol::GetType(){
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

int FuncSymbol::GetType(){
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

int FuncSymbol::GetReturnType(){
	return return_type;
}