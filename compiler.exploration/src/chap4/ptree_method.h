#ifndef PTREE_METHOD_H
#define PTREE_METHOD_H
#include <stdio.h>
#include "stl.h"


class parse_tree_node;


class dep_graph_node {
public:
	vector <dep_graph_node *>outgoing;
	parse_tree_node *parse_tree_node_ptr;
	dep_graph_node(int att_no,parse_tree_node *p )
	{
		parse_tree_node_ptr=p;
		attrib_no=att_no;
		incntr=0;

	}
	int attrib_no;
		
	int incntr;
	string name;
	void setup_dependency(dep_graph_node *p );
	void adjust_dependency_incntr();
};

typedef vector <dep_graph_node *> graph;


#define	CODE_ATTRIB 1
#define	PLACE_ATTRIB 2


class parse_tree_node {
	public:
	vector <parse_tree_node *> children;
	static int display_dec_parse_tree;
	static int no_of_nodes;
	int node_num;
	void set_display_to_dec()
	{
		display_dec_parse_tree=1;
	}
	virtual void print ()=0;
	virtual void eval_semantic_rules(int attrib_no)=0;
	virtual void create_dep_graph_nodes()=0;
	virtual void setup_semantic_rules()=0;
	virtual ~parse_tree_node()
	{
		;
	}
};


class expr_node:public parse_tree_node
{
	public:
	int place;
	dep_graph_node *place_node_ptr;
	int rule_no;

	expr_node(int rule_number)
	{
		rule_no = rule_number;
		node_num = no_of_nodes ;
		no_of_nodes ++;
	}
	void print ()
	{
		printf("expr_node[NON-TERMINAL,node_num=%d]\n",node_num);
	}
	void create_dep_graph_nodes() ;
	void setup_semantic_rules();
	void eval_semantic_rules(int attrib_no) ;


};

class stmt_node:public parse_tree_node
{
	public:
	int place;
	dep_graph_node *place_node_ptr;
	int rule_no;

	stmt_node(int rule_number)
	{
		rule_no = rule_number;
		node_num = no_of_nodes ;
		no_of_nodes ++;
	}
	void print ()
	{
		printf("stmt_node[NON-TERMINAL,node_num=%d place=%d]\n",node_num,place);
	}
	void create_dep_graph_nodes() ;
	void setup_semantic_rules();
	void eval_semantic_rules(int attrib_no) ;
};


class term_node:public parse_tree_node
{
	public:
	int place;

	dep_graph_node *place_node_ptr;
	int rule_no;

	term_node(int rule_number)
	{
		rule_no = rule_number;
		node_num = no_of_nodes ;
		no_of_nodes ++;
	}
	void print ()
	{
			printf("term_node[NON-TERMINAL,node_num=%d]\n",node_num);
	}
	void create_dep_graph_nodes() ;
	void setup_semantic_rules();
	void eval_semantic_rules(int attrib_no) ;
};

class factor_node:public parse_tree_node
{
	public:
	int place;
	dep_graph_node *place_node_ptr;
	int rule_no;

	factor_node(int rule_number)
	{
		rule_no = rule_number;
		node_num = no_of_nodes ;
		no_of_nodes ++;
	}
	void print ()
	{
			printf("factor_node[NON-TERMINAL,node_num=%d]\n",node_num);
	}

	void create_dep_graph_nodes() ;
	void setup_semantic_rules();
	void eval_semantic_rules(int attrib_no) ;
};



class CONSTANT_node:public parse_tree_node
{
	public:
	string lexeme;
	int value;
	dep_graph_node *value_node_ptr;

	CONSTANT_node(char *lexem)
	{
		lexeme +=lexem;
		node_num = no_of_nodes ;
		no_of_nodes ++;
		value = atoi(lexem);
	};
	void print ()
	{
			printf("CONSTANT_node[TERMINAL - lexeme is '%s' node_num=%d]\n",lexeme.c_str(),node_num);
	}
	void setup_semantic_rules() { ; }
	void create_dep_graph_nodes() ;
	void eval_semantic_rules(int attrib_no) 
	{ 
	}
};


class operator_node:public parse_tree_node
{
	public:
	string lexeme;
	operator_node(char *lexem)
	{
		lexeme +=lexem;
		node_num = no_of_nodes ;
		no_of_nodes ++;
	};
	void print ()
	{
			printf("operator_node[TERMINAL - lexeme is '%s',node_num=%d]\n",lexeme.c_str(),node_num);
	}
	void create_dep_graph_nodes() { ; }
	void setup_semantic_rules() { ; }
	void eval_semantic_rules(int attrib_no) { ; }
};

class misc_terminal_node:public parse_tree_node
{
	public:
	string lexeme;
	misc_terminal_node(char *lexem)
	{
		lexeme +=lexem;
		node_num = no_of_nodes ;
		no_of_nodes ++;
	};
	void print ()
	{
			printf("'%s' node [TERMINAL,node_num=%d]\n",lexeme.c_str(),node_num);
	}
	void eval_semantic_rules(int rule_no) { ; }
	void create_dep_graph_nodes() { ; }
	void setup_semantic_rules() { ; }
};



	#define stmt_rule 1  /*   stmt -> expr ';' */ 
	#define expr1_rule 2 /*    expr -> expr '+' term */
	#define expr2_rule 3 /*    expr -> expr '-' term */
	#define expr3_rule 4 /*    expr -> term */
	#define term1_rule 5 /*    term -> term '*' factor */
	#define term2_rule 6 /*   term -> term '/' factor */
	#define term3_rule 7 /*   term -> factor */
	#define factor1_rule 8 /*   factor -> '(' expr ')' */
	#define factor2_rule 9 /*   factor -> IDENTIFIER */
	#define factor3_rule 10 /*   factor -> CONSTANT */



#define YYSTYPE parse_tree_node*


#endif
