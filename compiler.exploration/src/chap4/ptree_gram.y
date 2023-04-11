%token IDENTIFIER CONSTANT 

%{
	#include <stdlib.h>
	//#include "dec_parse_tree_node.h"
	#include "ptree_method.h"
	#include "ptree_gram.hh"

	parse_tree_node *parse_tree_node_ptr;
	extern parse_tree_node *parse_root;
	extern int yyerror (char *s);
   extern int yylex();

%}

%start stmt


%%

stmt : expr
	               { 
						    parse_tree_node_ptr = new stmt_node(stmt_rule) ; 
							 parse_tree_node_ptr->children.push_back($1);
				          $$=parse_tree_node_ptr;
							 parse_root=$$;
						}
				;

expr : expr '+' term  
	    {
				parse_tree_node_ptr = new expr_node(expr1_rule) ; 
				parse_tree_node_ptr->children.push_back($1);
				parse_tree_node_ptr->children.push_back($2);
				parse_tree_node_ptr->children.push_back($3);
				$$=parse_tree_node_ptr;
		 } 
       | expr '-' term 
		   {
				parse_tree_node_ptr = new expr_node(expr2_rule) ; 
				parse_tree_node_ptr->children.push_back($1);
				parse_tree_node_ptr->children.push_back($2);
				parse_tree_node_ptr->children.push_back($3);
				$$=parse_tree_node_ptr;
			}
       | term 
		   {
				parse_tree_node_ptr = new expr_node(expr3_rule) ; 
				parse_tree_node_ptr->children.push_back($1);
				$$=parse_tree_node_ptr;
			}
		 ;

term : term '*' factor 
         {
				parse_tree_node_ptr = new term_node(term1_rule) ; 
				parse_tree_node_ptr->children.push_back($1);
				parse_tree_node_ptr->children.push_back($2);
				parse_tree_node_ptr->children.push_back($3);
				$$=parse_tree_node_ptr;
			}
       | term '/' factor 
		 	{
				parse_tree_node_ptr = new term_node(term2_rule) ; 
				parse_tree_node_ptr->children.push_back($1);
				parse_tree_node_ptr->children.push_back($2);
				parse_tree_node_ptr->children.push_back($3);
				$$=parse_tree_node_ptr;
			}
       | factor  
		   {
				parse_tree_node_ptr = new term_node(term3_rule) ; 
				parse_tree_node_ptr->children.push_back($1);
				$$=parse_tree_node_ptr;
			}
		 ;
factor : '(' expr ')'
		   { 
				parse_tree_node_ptr = new factor_node(factor1_rule) ; 
				parse_tree_node_ptr->children.push_back($1);
				parse_tree_node_ptr->children.push_back($2);
				parse_tree_node_ptr->children.push_back($3);
				$$=parse_tree_node_ptr;
			}
       | IDENTIFIER 
		 	{ 
				parse_tree_node_ptr = new factor_node(factor2_rule) ; 
				parse_tree_node_ptr->children.push_back($1);
				$$=parse_tree_node_ptr;
			}
       | CONSTANT  
		   { 
				parse_tree_node_ptr = new factor_node(factor3_rule) ; 
				parse_tree_node_ptr->children.push_back($1);
				$$=parse_tree_node_ptr;
			}
		 ;
%%
