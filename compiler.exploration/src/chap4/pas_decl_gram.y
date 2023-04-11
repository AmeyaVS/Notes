%token IDENTIFIER INTEGER CHAR REAL

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "recur_eval.h"
	#include "pas_decl_gram.hh"

	parse_tree_node *parse_tree_node_ptr;
   extern parse_tree_node *parse_root;
	extern int yyerror (char *s);
   extern int yylex();

   /*
		1 declaration_list : declaration_list declaration
		2 declaration_list : declaration
		1 declaration : identifier_list : type_spec ';'
		2 type_spec : INT
		3 type_spec : CHAR
		4 type_spec : FLOAT
		5 identifier_list : identifier_list , IDENTIFIER
		6 identifier_list : IDENTIFIER
   */
%}

%start declaration_list


%%

declaration_list : declaration_list declaration
                   {
		                parse_tree_node_ptr = new declaration_list_node(declaration_list_rule1) ;
                      parse_tree_node_ptr->children.push_back($1);
                      parse_tree_node_ptr->children.push_back($2);
						    parse_root = parse_tree_node_ptr;
                      $$=parse_tree_node_ptr;
						 }
                 | declaration
					    {
		                parse_tree_node_ptr = new declaration_list_node(declaration_list_rule2) ;
                      parse_tree_node_ptr->children.push_back($1);
						    parse_root = parse_tree_node_ptr;
                      $$=parse_tree_node_ptr;
						 }

declaration :	identifier_list ':' type_spec ';'
               {
		            parse_tree_node_ptr = new declaration_node(declaration_rule ) ;
                  parse_tree_node_ptr->children.push_back($1);
                  parse_tree_node_ptr->children.push_back($3);
                  $$=parse_tree_node_ptr;
					}

type_spec:	INTEGER  { 
	               type_spec_node *p;
		            p = new type_spec_node(type_spec_rule1) ;
						p -> data_type = INT_TYPE ;
						parse_tree_node_ptr = p;
                  $$=p;
				  		
              }
      |CHAR   { 
	               type_spec_node *p;
		            p = new type_spec_node(type_spec_rule2) ;
						p -> data_type = CHAR_TYPE ;
						parse_tree_node_ptr = p;
                  $$=p;
				  		
              }
		|REAL  { 
	               type_spec_node *p;
		            p = new type_spec_node(type_spec_rule3) ;
						p -> data_type = REAL_TYPE ;
						parse_tree_node_ptr = p;
                  $$=p;
				  		
              }
		;

identifier_list: identifier_list ',' IDENTIFIER   
					  { 
	                 parse_tree_node_ptr = new identifier_list_node(identifier_list_rule1) ;
                    parse_tree_node_ptr->children.push_back($1);
                    parse_tree_node_ptr->children.push_back($3);
                    $$=parse_tree_node_ptr;
					  } 
					|IDENTIFIER 
					  { 
	                 parse_tree_node_ptr = new identifier_list_node(identifier_list_rule2) ;
                    parse_tree_node_ptr->children.push_back($1);
                    $$=parse_tree_node_ptr;
					  }
					;
%%
