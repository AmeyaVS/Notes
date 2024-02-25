#include <stdio.h>
#include "stl.h"

#define INT_TYPE 1
#define CHAR_TYPE 2
#define FLOAT_TYPE 3

class node {

	public:
	string node_type; /* e.g identifier_list,IDENTIFIER,dtype */
	int data_type; /* e.g int,char,float*/
	string name;     /* e.g name of the identifier like i,j*/

	node(char *node_typ)
	{
		node_type += node_typ;
	}
	node(char *node_typ,int data_typ)
	{
		node_type += node_typ;
		data_type = data_typ;
	};
	node(char *node_typ,char *nam)
	{
		node_type += node_typ;
		name += nam;
	};
};

#define YYSTYPE node*
