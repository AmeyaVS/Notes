#include <stdio.h>
#include "stl.h"

class parse_tree_node {
	public:
	vector <parse_tree_node *> children;
	virtual ~parse_tree_node()
	{
		;
	}
};

#define declaration_list_rule1  1    /* declaration_list -> declaration_list declaration */
#define declaration_list_rule2 2    /* declaration_list -> declaration */
#define declaration_rule  3    /* declaration -> identifier_list ':' type_spec ';' */
#define type_spec_rule1 4    /* type_spec -> INT */
#define type_spec_rule2 5    /* type_spec -> CHAR */
#define type_spec_rule3 6    /* type_spec -> FLOAT */
#define identifier_list_rule1 7    /* identifier_list -> identifier_list ',' IDENTIFIER */
#define identifier_list_rule2 8    /* identifier_list -> IDENTIFIER */


#define INT_TYPE 1
#define CHAR_TYPE 2
#define REAL_TYPE 3

class declaration_list_node:public parse_tree_node
{
	public:
   int rule_no;
	declaration_list_node(int rule )
	{
		rule_no = rule ;
	}
	void  eval_semantic_rules();
};

class declaration_node:public parse_tree_node
{
	public:
   int rule_no;
	string data_type;

	declaration_node(int rule )
	{
		rule_no = rule ;
	}

	void  eval_semantic_rules();
};
class type_spec_node:public parse_tree_node
{
	public:
   int rule_no;
	int data_type;

	type_spec_node(int rule )
	{
		rule_no = rule ;
	}

	void eval_semantic_rules( int &data_typ);
};

class identifier_list_node:public parse_tree_node
{
	public:
   int rule_no;
	int type;

	identifier_list_node(int rule )
	{
		rule_no = rule ;
	}

	void eval_semantic_rules(int data_typ);
};

class IDENTIFIER_node:public parse_tree_node
{
	public:
	string place;
	IDENTIFIER_node(char *lexem)
	{
		place +=lexem;
	};
	void eval_semantic_rules(string &identifier_place);
};

#define YYSTYPE parse_tree_node*


extern parse_tree_node *parse_root;



