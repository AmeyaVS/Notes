#ifndef SYNTAX_ANALYSIS_H
#define SYNTAX_ANALYSIS_H

#include <stdio.h>
#include <stdlib.h>
#include "stl.h"

/*!
	@file syntax_analysis.h
	@brief Syntax Analysis Header
*/

//! The 'node' and its derivations are used to pass info during parsing process
class node 
{
   public:
	//! The Type of node (e.g. "type specifier", "identifier" etc)
   string type; 
};

//! The  type specification node
class type_spec_node : public node 
{

   public:

	//! Data Type (e.g int,char,float )
   type_expr *data_type; /* e.g int,char,float*/

	//! Name of the identifier (e.g. i,j )
   string name;     /* e.g name of the identifier like i,j*/

	//! Constructor for type_spec_node, given its data type
   type_spec_node(type_expr *data_typ)
   {
      type += "type specifier node";
      data_type = data_typ;
   };

	//! Constructor for type_spec_node, given its data type and name
   type_spec_node(type_expr *data_typ,char *nam)
   {
      type += "type specifier node";
      data_type = data_typ;
      name += nam;
   };
	//! Constructor for type_spec_node, given its data type and name
   type_spec_node(char *nam)
   {
      type += "type specifier node";
      name += nam;
   };       
};

//! The  Identifier node
class IDENTIFIER_node : public node 
{

   public:

	//! Name of the identifier (e.g. i,j )
   string name;     /* name of the identifier like i,j*/

	//! Constructor for Identifier node, given its name
   IDENTIFIER_node(char *nam)
   {
      type += "identifier node";
      name += nam;
   };
};

class parameter_list_node ;

//! declarator 
class declarator_node : public node 
{

   public:
	//! name of the declarator
   string name;     /* name of the identifier like i,j*/

	//! Data Type of the declarator
	type_expr *data_type;

   declarator_node()
   {
      type += "declarator node";
   };
};

//! parameter node
class parameter_node : public node 
{

   public:
	//! Data Type of the parameter
	type_expr *expr_type;

	//! Name of the parameter
	string name;

	//! Constructor for parameter node, given its name and Expression Type
   parameter_node(string nam,type_expr *t)
   {
      type += "parameter node";
		expr_type = t ;
		name = nam;
   };
};

//! Function declaration node
class func_decl_node : public node
{
   public:
	//! Name of the Function
   string name;     /* name of the func */

	//! The Function's Type
	type_expr *expr_type; /* function type expression */

	//! Constructor for Function declaration node
   func_decl_node(string nm)
   {
      type += "func_decl node";
		expr_type = NULL;
		name=nm;
   };

	//! Constructor for Function declaration given its name and Type Expression
   func_decl_node(string nm,type_expr *t)
   {
      type += "func_decl node";
		expr_type = t;
		name=nm;
	}
};


//! Parameter List
class parameter_list_node : public node 
{

   public:
	//! All the parameters list
	vector<sym_tab_entry *> parameters ;

	//! Number of parameters
	int no_of_args;

	//! Constructor for parameters list, given one of the parameters
   parameter_list_node(string nam,type_expr *t)
   {
		sym_tab_entry *p;

      type += "parameter_list node";
		p = new sym_tab_entry(nam,t);

		parameters.push_back(p);
		no_of_args =1 ;
   };

	//! Fetch the type of the n'th argument
	type_expr *get_arg_type(int ind)
	{
		return(parameters[ind]->type);
	};

	//! Add a parameter given its name and type
	void add_arg(string nam,type_expr *t)
	{
		sym_tab_entry *p;

		p = new sym_tab_entry(nam,t);
		parameters.push_back(p);
		no_of_args ++ ;
	};
	//! Fetch the name of n'th argument
	sym_tab_entry * get_arg(int ind)
	{
		return(parameters[ind]);
	}
};

//! Argument List
class args_list_node : public node 
{

   public:
	//! Arguments types
	vector<type_expr *> args ;
	#ifdef ICGEN
	//! Arguments names
	vector<sym_tab_entry *> args_sym_tab_entries ;
	#endif
	//! Number of arguments
	int no_of_args;

	//! Constructor for arguments list, given an argument type
   args_list_node(type_expr *p)
   {
      type += "args_list node";

		args.push_back(p);
		no_of_args =1 ;
   };

	//! Fetch the type of n'th argument
	type_expr *get_arg_type(int ind)
	{
		return(args[ind]);
	};

	//! Adding an argument given its type
	void add_arg(type_expr *t)
	{
		args.push_back(t);
		no_of_args ++ ;
	};

	#ifdef ICGEN
	//! Adding an argument's name
	void add_arg_sym_tab_entry(sym_tab_entry *t)
	{
		args_sym_tab_entries.push_back(t);
	};

	//! Fetch the n'th argument's name
	sym_tab_entry *get_arg_sym_tab_entry(int ind)
	{
		return(args_sym_tab_entries[ind]);
	}

	#endif
};


//! Constant
class CONSTANT_node : public node 
{

   public:
	//! Value of the constant
   int val;     /* like 12345, 0x345 etc */
	//! Value of the constant stored in string form
	string str_val; /* stored in string form */

	//! Constructor for Constant, given its value
   CONSTANT_node(char *value)
   {
      type += "constant node";
		val = strtol(value,NULL,0);
		str_val += value ;
   };
};

//! Real Constant
class REAL_CONSTANT_node : public node 
{

   public:
	//! Value of the constant
   float val;     /* like 12345, 0x345 etc */
	//! Value of the constant stored in string form
	string str_val; /* stored in string form */

	//! Constructor for a Real Constant, given its value
   REAL_CONSTANT_node(char *value)
   {
      type += "real constant node";
		val = strtodf(value,NULL);
		str_val += value ;
   };
};

//! String Literal
class STRING_LIT_node : public node 
{

   public:
	//! string for a String Literal
	string str;

	//! Constructor for a String Literal, given the string
   STRING_LIT_node(char *str1)
   {
      type += "string literal node";
		str = str1 ;
   };
};




//! Unary Expression
class unary_expr_node : public node
{
   public:
	//! place
	sym_tab_entry *place;
	//! offset
	sym_tab_entry *offset;
	//! Type
	type_expr *expr_type;
	#ifdef ICGEN
	//! True List
	quad_list *truelist;
	//! False List
	quad_list *falselist;
	#endif
	//! Constructor 
   unary_expr_node()
   {
      type += "unary_expr_node";
		offset=NULL;
		expr_type = NULL;
   };
};

//! Binary Expression
class binary_expr_node : public node
{
   public:
	//! place
	sym_tab_entry *place;
	//! Type
	type_expr *expr_type;
	#ifdef ICGEN
	//! True List
	quad_list *truelist;
	//! False List
	quad_list *falselist;
	#endif

	//! Constructor 
   binary_expr_node()
   {
      type += "binary_expr_node";
		expr_type = NULL;
		#ifdef ICGEN
      truelist = new quad_list;
      falselist = new quad_list;
		#endif
   };
};

//! Expression 
class expr_node : public node
{
   public:
	//! place
	sym_tab_entry *place;
	//! Type
	type_expr *expr_type;
	#ifdef ICGEN
	//! True List
	quad_list *truelist;
	//! False List
	quad_list *falselist;
	#endif

	//! Constructor 
   expr_node()
   {
      type += "expr_node";
		expr_type = NULL;
   };
};

//! Marker used for backpatching
class M_node : public node
{
   public :
	//! quad
   int quad ;
	//! Label
	sym_tab_entry *lbl;
	//! Constructor 
   M_node()
   {
      type += "M_node" ;
   };
};

//! Marker used for backpatching
class N_node : public node
{
   public :
   #ifdef ICGEN
	//! Next statements depending on the marker
   quad_list *nextlist;
   #endif
      

	//! Constructor 
   N_node()
   {
      type += "N_node" ;
   };
};

//! Statement
class stmt_node : public node
{              
   public:     
	#ifdef ICGEN
	//! Next List
	quad_list *nextlist;
	#endif

	//! Constructor 
   stmt_node()
   {           
      type += "stmt_node";
		#ifdef ICGEN
		nextlist = new quad_list;
	   #endif
   };     
};

//! Statement List
class stmt_list_node : public node
{                       
   public:              
	//! Constructor 
   stmt_list_node()     
   {                    
      type += "stmt_list_node";
   };
}; 

//! Case Statement
class case_stmt_node : public node
{
	public:
	//! Constructor 
	case_stmt_node()
	{
		type += "case_stmt_node";
	};
};

//! Case Statement List
class case_stmt_list_node : public node
{
	public:
	//! Constructor 
	case_stmt_list_node()
	{
		type += "case_stmt_list_node";
	};
};

//! Associate node pointer as the information passing mechanism in parser
#define YYSTYPE node*
#endif
