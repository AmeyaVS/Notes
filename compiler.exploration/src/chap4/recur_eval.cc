#include <stdio.h>
#include "stl.h"
#include "recur_eval.h"


    #define declaration_list_rule1  1    /* declaration_list -> declaration_list declaration */
    #define declaration_list_rule2 2    /* declaration_list -> declaration */
    #define declaration_rule  3    /* declaration -> identifier_list ':' type_spec ';' */
    #define type_spec_rule1 4    /* type_spec -> INT */
    #define type_spec_rule2 5    /* type_spec -> CHAR */
    #define type_spec_rule3 6    /* type_spec -> FLOAT */
    #define identifier_list_rule1 7    /* identifier_list -> identifier_list ',' IDENTIFIER */
    #define identifier_list_rule2 8    /* identifier_list -> IDENTIFIER */

void		add_to_sym_table(string name, int type)
{
	/* 
	   Ideally, this would add the symbol 'name' of 'type' 
	   into the symbol table. But for now, we just print it out 
	*/
	string str;
	switch(type)
	{
		case INT_TYPE : str="INTEGER";break;
		case REAL_TYPE : str="REAL";break;
		case CHAR_TYPE : str="CHAR";break;
	}
	printf("Identifier name=%s type=%s\n",name.c_str(),str.c_str());
}

void  declaration_list_node::eval_semantic_rules()
{
	declaration_list_node *declaration_list_node_ptr;
	declaration_node *declaration_node_ptr;

	if(rule_no == declaration_list_rule1 ){
		declaration_list_node_ptr = (declaration_list_node *)children[0];
		declaration_node_ptr = (declaration_node *)children[1];

		declaration_list_node_ptr -> eval_semantic_rules();
		declaration_node_ptr -> eval_semantic_rules();
	} else if(rule_no == declaration_list_rule2 ){
		declaration_node_ptr = (declaration_node *)children[0];
		declaration_node_ptr -> eval_semantic_rules();
	}

}

void  declaration_node::eval_semantic_rules()
{
		int type_spec_data_type;
		identifier_list_node *identifier_list_ptr; 
		type_spec_node *type_spec_node_ptr;

		if(rule_no == declaration_rule ){
			
         /* declaration -> identifier_list ':' type_spec ';' */

			identifier_list_ptr = (identifier_list_node *)children[0];
			type_spec_node_ptr = (type_spec_node *)children[1];

			/* Getting the 'data_type' synthesised from type_spec_node 
			   - passing by reference 
			*/
			type_spec_node_ptr -> eval_semantic_rules(type_spec_data_type);

			/* Passing the 'data_type' to the children of identifier_list */
			identifier_list_ptr -> eval_semantic_rules(type_spec_data_type);
		}
}

void type_spec_node::eval_semantic_rules( int &data_typ)
{
	if( (rule_no == type_spec_rule1) || (rule_no == type_spec_rule2 ) || (rule_no == type_spec_rule3 ) ){
		data_typ = data_type ;
	}
}

void identifier_list_node:: eval_semantic_rules(int data_typ)
{

	 /*
	 	7 identifier_list : identifier_list , IDENTIFIER
    	8 identifier_list : IDENTIFIER
	 */
	 string identifier_place;

	 type = data_typ ;

	 if(rule_no == identifier_list_rule1 ){

	 	identifier_list_node *identifier_list_node_ptr;
		IDENTIFIER_node *IDENTIFIER_node_ptr;

		identifier_list_node_ptr = (identifier_list_node *)children[0];
		IDENTIFIER_node_ptr = (IDENTIFIER_node *)children[1];

		identifier_list_node_ptr -> eval_semantic_rules(type);
		IDENTIFIER_node_ptr -> eval_semantic_rules(identifier_place);

		add_to_sym_table(identifier_place,type);

	 }else if ( rule_no == identifier_list_rule2 ){
		IDENTIFIER_node *IDENTIFIER_node_ptr;

		IDENTIFIER_node_ptr = (IDENTIFIER_node *)children[0];

	 	IDENTIFIER_node_ptr -> eval_semantic_rules(identifier_place);

		add_to_sym_table(identifier_place,type);

	 }
}

void IDENTIFIER_node :: eval_semantic_rules(string &identifier_place)
{
	identifier_place = place;
}
	


