#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

/*!
	@file semantic_analysis.h
	@brief Semantic Analysis Header
*/

#include "stl.h"

/* type_expr.h begin */

//! The Basic Types
enum basic_types {
	INTEGER,
	CHARACTER,
	REAL,
	VOID_TYPE,
	RECORD,
	ARRAY,
	POINTER,
	FUNCTION,
	LABEL,
	CONSTAN,
	TYPE_ERROR
};


//! Representing a Type
class type_expr
{
   public :
	//basic_types kind;

	//! Pointer to the Next in the Type Expression Chain
	type_expr *next;

	//! Constructor
	type_expr()
	{
		next=NULL;
	};

	//! Get the Size of the Type by traversing the Type Expression Chain
	int getsize()
	{
		int sz=1,k;
		type_expr *t;
		int ptr_type=0;

		t=this;
		while( t -> next != NULL ){
			k = t->getwidth();
			if(t->gettype() == POINTER ){
				ptr_type = 1;
			}
			sz = sz * k; 
			t = t ->next;
		}

		if(ptr_type != 1 ){ 
			/* if it is not a pointer ..then take the 
			   size of basic type into consideration
			*/
			k= t->getwidth();
			sz = sz * k;
		}

		return(sz);
	}
	//! Print the Type Expression Chain
	void print_it()
	{
		type_expr *n;

		print();
		n=next;
		while(n != NULL ){
			printf("->");
			n->print();
			n=n->next;
		}
	}
	//! Get the size of the Type. Pure Virtual Function
	virtual int getwidth()=0;
	//! Get the Base Type. Pure Virtual Function
	virtual int gettype()=0;
	//! Print the Base Type. Pure Virtual Function
	virtual void print()=0;

	void chain(type_expr *next);
};

//! Representing a pointer
class pointer : public type_expr
{
	public :

	pointer()
	{
	}

	int gettype()
	{
		return(POINTER);
	}

	int getwidth()
	{
		return(4);
	}

	void print()
	{
		printf("POINTER");
	}
	virtual ~pointer()
	{
	}
};

		
//! Representing a array
class array : public type_expr
{
	public:

	//! Dimension of an array
	int limit;

	//! Constructor given the Dimension of an array
	array(int lim)
	{
		limit=lim;
	}

	int gettype()
	{
		return(ARRAY);
	}
	int getwidth()
	{
		return(limit);
	};
	void print()
	{
		printf("ARRAY(%d)",limit);
	}

	//! Get the Size of an element in the Array
	int get_basic_type_size()
	{
		int sz;
		type_expr *t;

		t=this;
		while( t -> next != NULL ){
			t = t ->next;
		}

		sz= t->getwidth();

		return(sz);
	}
	virtual ~array()
	{
	}
};

class sym_tab;
class sym_tab_entry;

//! Representing a record
class record : public type_expr
{
   public:
	//! The fields in a record
   sym_tab *fields;

	//! The tagname of the record
   string tagname;

   record(sym_tab *t);

   record(sym_tab *t,string tagname);

   int gettype()
   {
      return(RECORD);
   }
   int getwidth();
	void print()
	{
		printf("RECORD(%s)",tagname.c_str());
	}
   virtual ~record()
   {
   }
};

//! Representing a Function
class function : public type_expr
{
	public:
	//! Getting the Type Expression of the Function's return value
	type_expr *ret_type;

	//! Number of arguments to the function
	int no_of_args;

	//! The arguments list to the function
	vector<sym_tab_entry *> args_list ;

	type_expr *get_arg_type(int ind);

	//! Constructing a Function, given the Type Expression of the Return Value 
	function(type_expr *r)
	{
		ret_type=r;
		no_of_args=0;
	}
	//! Adding an Argument to the Argument List
	void add_arg(sym_tab_entry *t)
	{
		args_list.push_back(t);
		no_of_args ++;
	}
	//! Fetch an Argument at a given position from Argument List
	sym_tab_entry *get_arg(int ind)
	{
		return(args_list[ind]);
	}

	int gettype()
	{
		return(FUNCTION);
	}
	int getwidth()
	{
		return(4);
	}

	void print()
	{
		printf("FUNCTION");
	}

	virtual ~function()
	{
	}
};

//! Representing a Real
class real : public type_expr
{
	public:

	real()
	{
	}
	int gettype()
	{
		return(REAL);
	}
	void print()
	{
		printf("REAL");
	}
	int getwidth()
	{
		return(8);
	}
};


//! Representing a char
class character : public type_expr
{
	public:

	character()
	{
	}
	int gettype()
	{
		return(CHARACTER);
	}
	int getwidth()
	{
		return(1);
	}
	void print()
	{
		printf("CHAR");
	}
};


//! Representing an Integer
class integer : public type_expr
{
	public:

	integer()
	{
	}
	int gettype()
	{
		return(INTEGER);
	}
	int getwidth()
	{
		return(4);
	}
	void print()
	{
		printf("INTEGER");
	}
};

//! Representing an void
class void_type : public type_expr
{
	public:

	void_type()
	{
	}
	int gettype()
	{
		return(VOID_TYPE);
	}
	int getwidth()
	{
		return(4);
	}
	void print()
	{
		printf("VOID");
	}
};

//! Representing an Label
class label : public type_expr
{
	public:

	label()
	{
	}
	int gettype()
	{
		return(LABEL);
	}
	int getwidth()
	{
		return(4);
	}
	void print()
	{
		printf("LABEL");
	}
};

//! Representing an Constant
class constant : public type_expr
{
	public:

	constant()
	{
	}
	int gettype()
	{
		return(CONSTAN);
	}
	int getwidth()
	{
		return(4);
	}
	void print()
	{
		printf("CONSTANT");
	}
};

//! Representing an Type Error
class type_error : public type_expr
{
	public:

	type_error()
	{
	}
	int gettype()
	{
		return(TYPE_ERROR);
	}
	int getwidth()
	{
		return(0);
	}
	void print()
	{
		printf("TYPE_ERROR");
	}
};

int type_equal(type_expr *ty1, type_expr *ty2);

//! Compare operator for type_expr_list
struct comp
{
	//! Compare operator for type_expr_list
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

//! Type Expression List used in Semantic Analysis 
typedef  map<const char *, type_expr *, comp> type_expr_list;


/* type_expr.h end */


/* sym_tab.h begin */

class sym_tab;

//! A symbol Table Entry
class sym_tab_entry
{
	public:

	//! Name of the Identifier
	string name;
	//! Type of the Identifier
	type_expr *type ;
	//! Offset in the Memory
	int offset;
	//! Size of the Identifier in Memory in Bytes
	int size;
	//! Backlink to the Symbol Table
	sym_tab *sym_tab_ptr;
	//! Used for Literals
	string literal;
	//! Used for Extending the Symbol table entry during optimization
	void *extension;

	//! Constructor, given name and Type of the Identifier
	sym_tab_entry(string nam,type_expr *typ)
	{
		name = nam ;
		type = typ ;
		sym_tab_ptr = NULL;
		extension=NULL;
	}

	//! Constructor, given name and Type of the Identifier
	sym_tab_entry(string nam,type_expr *typ,int offse)
	{
		name = nam ;
		type = typ ;
		sym_tab_ptr = NULL;
		offset = offse ;
		size=typ->getsize();
		extension=NULL;
	}

	//! Returns the Name of the Identifier
	string get_name()
	{
		return(name);
	}

	void print();
	int get_size();
	int is_tmp();
	int is_it_a_constant();
	int is_global();
};


//! Comparison Function used in stab
struct eqstr
{
	//! Comparison Function
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) == 0;
	}
};


//! A Hash Table with Key as the name of Identifier and symbol Table  Entry as the value
typedef hash_map<const char*, sym_tab_entry *, hash<const char*>, eqstr> stab;

//! A symbol Table 
class sym_tab
{
	public:

	//! Pointer to previous Symbol Table. Used in resolving scopes
	sym_tab *previous;

	//! Size of the Symbol Table
	int width;

#ifdef ICGEN
	/* Local space */

	//! Local Space to be created for the Symbol table
	sym_tab_entry *local_space;


	//! A Constant Literal for Parameter Space
	sym_tab_entry *param_space_sym_entry;

	//! Parameter Space
	int param_space;

	//! Entry Label
	sym_tab_entry *entry_lbl;

	//! End Label - Used during 'return'
	sym_tab_entry *end_lbl;

	//! Number of temporaries in the Symbol Table
	int no_of_temp;

	void set_param_space ();
	void update_fields(sym_tab_entry *endlbl);
	sym_tab_entry *make_temporary();
	void calculate_local_space();
	sym_tab_entry *make_temporary(type_expr *tmp_typ);
#endif

	//! The Actual Symbol table maintained as Hash
	stab sym_table;

	//! Name of the Symbol Table
	string name ;

	void get_list(list<sym_tab_entry *> *all_vars);
	//! Constructor
	sym_tab()
	{
		previous=NULL;
		width=0;
	#ifdef ICGEN
		local_space=NULL;
		param_space_sym_entry=NULL;
		param_space=0;
		entry_lbl=NULL;
		end_lbl = NULL;
	#endif
	};

	//! Constructor given the name of the Symbol Table
	sym_tab(string nm)
	{
		name=nm;
		previous=NULL;
		width=0;
	#ifdef ICGEN
		local_space=NULL;
		param_space_sym_entry=NULL;
		param_space=0;
		entry_lbl=NULL;
		end_lbl = NULL;
		no_of_temp=0;
	#endif
	}

	sym_tab_entry *add(string name,type_expr *type_ptr);
	void remove(string name);
	void print();
	sym_tab_entry *find(string name);
	//! Destructor
	~sym_tab()
	{
		sym_table.clear();
	}
};


/* sym_tab.h begin */

#endif
