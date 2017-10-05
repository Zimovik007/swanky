#pragma once
#include <map>
#include <vector>

#include "block.hpp"

using namespace std;

class Symbol{
public:
	Symbol(){};
	virtual int GetType(){};
	virtual int IsVar(){};
	virtual Symbol* GetTypeElements(){};
	virtual int GetReturnType(){};
};

class VarIntSymbol: public Symbol{
public:
	VarIntSymbol(){type = 2; is_var = 1;};
	int GetType() override;
	int IsVar() override;
private:
	int type;
	int is_var;
};

class VarFloatSymbol: public Symbol{
public:
	VarFloatSymbol(){type = 3; is_var = 1;};
	int GetType() override;
	int IsVar() override;
private:
	int type;
	int is_var;
};

class VarStringSymbol: public Symbol{
public:
	VarStringSymbol(){type = 1; is_var = 1;};
	int GetType() override;
	int IsVar() override;
private:
	int type;
	int is_var;
};

class VarRecordSymbol: public Symbol{
public:
	VarRecordSymbol(map<string, Symbol*> elements){
		type = 9; 
		is_var = 0; 
		elems = elements;
	};
	int GetType() override;
	int IsVar() override;
	map<string, Symbol*> GetMapElements();
private:
	int type;
	int is_var;
	map<string, Symbol*> elems;
};

class VarArraySymbol: public Symbol{
public:
	VarArraySymbol(int i_start, int i_end, Symbol* type_e){
		type = 10;
		type_elems = type_e; 
		is_var = 1;
		index_start = i_start;
		index_end = i_end;
	};
	int GetType() override;
	Symbol* GetTypeElements();
	int IsVar() override;
	int IndexStart();
	int IndexEnd();
private:
	int type;
	Symbol* type_elems;
	int is_var, index_start, index_end;
};

class ConstIntSymbol: public Symbol{
public:
	ConstIntSymbol(){type = 2; is_var = 0;};
	int GetType() override;
	int IsVar() override;
private:
	int type;
	int is_var;
};

class ConstFloatSymbol: public Symbol{
public:
	ConstFloatSymbol(){type = 3; is_var = 0;};
	int GetType() override;
	int IsVar() override;
private:
	int type;
	int is_var;
};

class ConstStringSymbol: public Symbol{
public:
	ConstStringSymbol(){type = 1; is_var = 0;};
	int GetType() override;
	int IsVar() override;
private:
	int type;
	int is_var;
};

class ConstRecordSymbol: public Symbol{
public:
	ConstRecordSymbol(map<string, Symbol*> elements){
		type = 9; 
		is_var = 0; 
		elems = elements;
	};
	int GetType() override;
	int IsVar() override;
	map<string, Symbol*> GetMapElements();
private:
	int type;
	int is_var;
	map<string, Symbol*> elems;
};

class ConstArraySymbol: public Symbol{
public:
	ConstArraySymbol(int i_start, int i_end, Symbol* type_e){
		type = 10;
		is_var = 0;
		type_elems = type_e;
		index_start = i_start;
		index_end = i_end;
	};
	int GetType() override;
	Symbol* GetTypeElements();
	int IsVar() override;
	int IndexStart();
	int IndexEnd();
private:
	int type;
	Symbol* type_elems;
	int is_var, index_start, index_end;
};

class FuncSymbol: public Symbol{
public:
	FuncSymbol(map<string, Symbol*> map_p, vector<Symbol*> vec_p, int re_type, Block* body){
		map_params = map_p; 
		vec_params = vec_p; 
		is_var = 1; 
		type = 8;
		return_type = re_type;
		body_function = body;
	};
	map<string, Symbol*> GetMapElements();
	vector<Symbol*> GetVecElements();
	int GetType() override;
	int GetReturnType();
	int IsVar() override;
private:
	int is_var;
	int type, return_type;
	map<string, Symbol*> map_params;
	vector<Symbol*> vec_params;
	Block* body_function;
};