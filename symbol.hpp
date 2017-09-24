#pragma once
#include <map>
#include <vector>

using namespace std;

class Symbol{
public:
	Symbol(){};
	virtual string GetType(){};
	virtual int IsVar(){};
};

class VarIntSymbol: public Symbol{
public:
	VarIntSymbol(){type = "INTEGER"; is_var = 1;};
	string GetType() override;
	int IsVar() override;
private:
	string type;
	int is_var;
};

class VarFloatSymbol: public Symbol{
public:
	VarFloatSymbol(){type = "FLOAT"; is_var = 1;};
	string GetType() override;
	int IsVar() override;
private:
	string type;
	int is_var;
};

class VarStringSymbol: public Symbol{
public:
	VarStringSymbol(){type = "STRING"; is_var = 1;};
	string GetType() override;
	int IsVar() override;
private:
	string type;
	int is_var;
};

class VarRecordSymbol: public Symbol{
public:
	VarRecordSymbol(map<string, Symbol*> elements){
		type = "RECORD"; 
		is_var = 0; 
		elems = elements;
	};
	string GetType() override;
	int IsVar() override;
	map<string, Symbol*> GetMapElements();
private:
	string type;
	int is_var;
	map<string, Symbol*> elems;
};

class VarArraySymbol: public Symbol{
public:
	VarArraySymbol(int i_start, int i_end, Symbol* type_e){
		type = "ARRAY";
		//type_elems = type_e; 
		is_var = 1;
		index_start = i_start;
		index_end = i_end;
	};
	string GetType() override;
	Symbol* GetTypeElements();
	int IsVar() override;
	int IndexStart();
	int IndexEnd();
private:
	string type;
	Symbol* type_elems;
	int is_var, index_start, index_end;
};

class ConstIntSymbol: public Symbol{
public:
	ConstIntSymbol(){type = "INTEGER"; is_var = 0;};
	string GetType() override;
	int IsVar() override;
private:
	string type;
	int is_var;
};

class ConstFloatSymbol: public Symbol{
public:
	ConstFloatSymbol(){type = "FLOAT"; is_var = 0;};
	string GetType() override;
	int IsVar() override;
private:
	string type;
	int is_var;
};

class ConstStringSymbol: public Symbol{
public:
	ConstStringSymbol(){type = "STRING"; is_var = 0;};
	string GetType() override;
	int IsVar() override;
private:
	string type;
	int is_var;
};

class ConstRecordSymbol: public Symbol{
public:
	ConstRecordSymbol(map<string, Symbol*> elements){
		type = "RECORD"; 
		is_var = 0; 
		elems = elements;
	};
	string GetType() override;
	int IsVar() override;
	map<string, Symbol*> GetMapElements();
private:
	string type;
	int is_var;
	map<string, Symbol*> elems;
};

class ConstArraySymbol: public Symbol{
public:
	ConstArraySymbol(int i_start, int i_end, Symbol* type_e){
		type = "ARRAY";
		//type_elems = type_e; 
		is_var = 0;
		index_start = i_start;
		index_end = i_end;
	};
	string GetType() override;
	Symbol* GetTypeElements();
	int IsVar() override;
	int IndexStart();
	int IndexEnd();
private:
	string type;
	Symbol* type_elems;
	int is_var, index_start, index_end;
};

class FuncSymbol: public Symbol{
public:
	FuncSymbol(map<string, Symbol*> map_p, vector<Symbol*> vec_p, string re_type){
		map_params = map_p; 
		vec_params = vec_p; 
		is_var = 1; 
		type = "FUNCTION";
		return_type = re_type;
	};
	map<string, Symbol*> GetMapElements();
	vector<Symbol*> GetVecElements();
	string GetType() override;
	string GetReturnType();
	int IsVar() override;
private:
	int is_var;
	string type, return_type;
	map<string, Symbol*> map_params;
	vector<Symbol*> vec_params;
};