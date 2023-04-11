#include "semantic_analysis.h"

/*!
	@file semantic_analysis.cc
	@brief Semantic Analysis Implementation
*/

/* type_expr.cc begin */

//! Gets the Size of the record
int
record::getwidth()
{
	if(fields == NULL ){
		printf("The record is NOT in Symbol Table \n");
		exit(1);
	}

   return(fields->width);
}

//! Constructor for a record, given its Symbol Table
record::record(sym_tab *fields_tab)
{
   fields = fields_tab ;
}

//! Constructor for a record, given its Symbol Table and tagname
record::record(sym_tab *fields_tab,string t_tag)
{
   fields = fields_tab ;
   tagname=t_tag;
}

//! Gets the type expression chain for 'ind' parameter to the function
type_expr *
function::	get_arg_type(int ind)
{
	return(args_list[ind]->type);
}


//! Adds a type expression tptr to the existing type expression chain
void 
type_expr ::chain(type_expr *tptr)
{
	type_expr *t,*last;

	if(next == NULL ){
		next=tptr;
	}else{
		t=next;
		while( t!= NULL ){
			last= t;
			t = t ->next;
		}
		last ->next = tptr ;
	}
	tptr->next=NULL;
}


//! Tests if two type expressions are equal
int type_equal(type_expr *ty1, type_expr *ty2)
{
	type_expr *t1,*t2;
	int b1,b2;

	t1 = ty1;
	t2 = ty2;

	/* reached the end of the type chain */
	if ( (t1 == NULL ) && ( t2 == NULL ) ){
		return(1);
	}

	/* One of them has terminated */
	if ( (t1 == NULL ) || ( t2 == NULL ) ){
		return(0);
	}

	b1 = t1->gettype();
	b2 = t2->gettype();

	if(b1 !=b2 ){
		return(0);
	}

	switch(b1)
	{
		case INTEGER:
   	case CHARACTER:
   	case REAL:
   	case VOID_TYPE :	break;
   	case RECORD :  {
								record *r1,*r2;
								r1 = (record *)t1;
								r2 = (record *)t2;
								if(r1->tagname != r2 ->tagname){
									return(0);
								}
								break;
							}
 
   	case ARRAY:  {
							 array *a1,*a2;
							 a1 = (array *)t1;
							 a2 = (array *)t2;
							 
							 if(a1->limit != a2->limit){
								return(0);
							 }
							 break;
						}
   	case POINTER: break;
   	case FUNCTION: {
								function *f1,*f2;
								int n,i;

								f1 = (function *)t1;
								f2 = (function *)t2;

								if(f1->no_of_args != f2 -> no_of_args){
									return(0);
								}
								n = f1->no_of_args ;
								for(i=0;i<n;i++){
									if(!type_equal(f1->args_list[i]->type,f2->args_list[i]->type)){
										return(0);
									}
								}
								if(!type_equal(f1->ret_type,f2->ret_type)){
									return(0);
								}
								break;
							}
   	case LABEL:
   	case CONSTAN:
   	case TYPE_ERROR: break;
	}
	return(type_equal(t1->next,t2->next) );

}

/* type_expr.cc end */


/* sym_tab.cc begin */

//! Add an Identifier into the symbol table 
sym_tab_entry *sym_tab::add(string name,type_expr *type_ptr)
{
	sym_tab_entry *sptr;

	sptr = new sym_tab_entry(name,type_ptr,width);
	sym_table[name.c_str()]=sptr;

	if(type_ptr != NULL ){
		width = width + type_ptr->getsize();
	}else{
		printf("Called with No type \n");
	}

	/* Have a back link to the symbol table */
	sptr->sym_tab_ptr = this;

	return(sptr);
}

//! Remove an Identifier from the symbol table 
void sym_tab::remove(string name)
{
	sym_tab_entry *sptr;
	stab :: iterator iter;
	pair <const char *,sym_tab_entry *> res;

	iter = sym_table.find(name.c_str() );

	if(iter == sym_table.end() ){
		printf("WARNING : Trying to Remove a non existing entry \n");
		return;
	} else {
		res = (*iter);
		sptr = res.second;
		sym_table.erase(iter);
		delete(sptr);
		return;
	}
}

//! Tells, if a given symbol is a constant literal
int
sym_tab_entry :: is_it_a_constant()
{
	if(sym_tab_ptr == NULL ){
		return(0);
	}

	if(strcmp(sym_tab_ptr->name.c_str(), "CONSTANT LITERALS" )== 0 ){
		return(1);
	}
	return(0);
}

//! Tells, if a given symbol is a Global variable
int
sym_tab_entry :: is_global()
{
	if(sym_tab_ptr == NULL ){
		return(0);
	}

	if(strcmp(sym_tab_ptr->name.c_str(), "GLOBALS" )== 0 ){
		return(1);
	}
	return(0);
}


//! Fetch a symbol table entry given the name of the identifier
sym_tab_entry *sym_tab::find(string name)
{
	sym_tab_entry *sptr;
	stab :: iterator iter;
	pair <const char *,sym_tab_entry *> res;

	iter = sym_table.find(name.c_str() );

	if(iter == sym_table.end() ){
		return(NULL);
	} else {
		res = (*iter);
		sptr = res.second;
		return(sptr);
	}
}

//! Print a Symbol table
void sym_tab::print()
{
	sym_tab_entry *sptr;
	stab :: iterator iter;
	pair <const char *,sym_tab_entry *> res;

	iter = sym_table.begin();

	printf("Name of the Table=%s Size=%d \n",name.c_str(),width);

	while( iter != sym_table.end() )
	{
		res = (*iter);
		sptr = res.second;
		sptr ->print();
		iter ++ ;
	}
	printf("\n");
	return;
}

//! Gets a list of all symbol table entries barring the temporary vars
void
sym_tab::get_list(list<sym_tab_entry *> *all_vars)
{
	sym_tab_entry *sptr;
	stab :: iterator iter;
	pair <const char *,sym_tab_entry *> res;

	iter = sym_table.begin();

	while( iter != sym_table.end() )
	{
		res = (*iter);
		sptr = res.second;

		if(sptr->is_tmp() == 0 ){
			all_vars->push_back(sptr);
		}
			
		iter ++ ;
	}

	return;
}

//! Print a Symbol table Entry
void sym_tab_entry :: print()
{
		printf("Name=%s ",name.c_str());
		printf("Type=");
		type->print_it();
		printf(" Size=%d ",size);
		printf(" Offset=%d",offset);
		printf("\n");
};

//! Tells, if the symbol table entry is a compiler generated Temporary
int sym_tab_entry :: is_tmp()
{
	if(name[0] == '_'){
		return(1);
	}else{
		return(0);
	}
}

#ifdef ICGEN

//! Makes a symbol table entry for a constant in the Constant Literal Table
extern sym_tab_entry *make_const_sym_tab_entry(int val);

//! Makes a symbol table entry for a Label in the Constant Literal Table
extern sym_tab_entry *make_lbl(string lbl_str);

//! Creates a constant literal for the parameter space
void 
sym_tab :: set_param_space()
{
	param_space = width;
   param_space_sym_entry = make_const_sym_tab_entry(param_space);
}

//! Updates a symbol table for endlbl,space,entry_lbl and so on
void
sym_tab :: update_fields(sym_tab_entry *endlbl)
{
	sym_tab_entry *sptr1;
   int space;
   string tmp;

   end_lbl = endlbl ;
   space = width - param_space ;

   /* Store entry label */
   tmp="_"+name;
   sptr1=make_lbl(tmp);
   entry_lbl = sptr1 ;
}

//! Calculates the size of all local variables put together
void
sym_tab :: calculate_local_space()
{
   int space;
	sym_tab_entry *sptr1;


   space = width - param_space ;

   /* Store the local space in the sym Table */
   sptr1 = make_const_sym_tab_entry(space);
   local_space = sptr1 ;
}
	
//! Gets the size of symbol table entry
int sym_tab_entry :: get_size()
{
   return(type->getsize() );
}


//! Makes a Type Expression from a given basic type
extern type_expr *make_type_expr(basic_types bt);

//! Make a symbol table entry for a temporary 
sym_tab_entry *
sym_tab :: make_temporary()
{
      char tmp_name[100];
      string my_tmp;
	   type_expr *t;
		sym_tab_entry *sptr;

      sprintf(tmp_name,"_t%d",no_of_temp);
      no_of_temp++;
      my_tmp += tmp_name;


	   t = make_type_expr(INTEGER);

		sptr = add(my_tmp,t);

      return(sptr);
}

//! Make a symbol table entry for a temporary , given its type
sym_tab_entry *
sym_tab :: make_temporary(type_expr *tmp_typ)
{
      char tmp_name[100];
      string my_tmp;
		sym_tab_entry *sptr;

      sprintf(tmp_name,"_t%d",no_of_temp);
      no_of_temp++;
      my_tmp += tmp_name;


		sptr = add(my_tmp,tmp_typ);

      return(sptr);
}
#endif

/* sym_tab.cc end */
