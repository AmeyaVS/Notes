#include <stdio.h>
#include <assert.h>
#include "stl.h"
#include "ptree_method.h"

/* dec_parse_tree_node.cc dep_graph.cc eval_sem_rules.cc */

#define	ADD	1
#define	SUB	2
#define	MUL	3
#define	DIV	4

extern graph dep_graph;
extern void	top_sort();
extern graph top_sorted_dep_graph;

parse_tree_node *parse_root;

int parse_tree_node::display_dec_parse_tree;
int parse_tree_node::no_of_nodes;

void expr_node::create_dep_graph_nodes()
{
	dep_graph_node *p;

	place_node_ptr = p = new dep_graph_node ( PLACE_ATTRIB,this) ;
	dep_graph.push_back(p);

}


void expr_node::setup_semantic_rules()
{
	if((rule_no == expr1_rule) || (rule_no == expr2_rule ) ){
		expr_node *expr1_node_ptr;
		term_node *term_node_ptr;

		expr1_node_ptr=(expr_node *)children[0];
		term_node_ptr = (term_node *)children[2];

		/* Updating attributes */
		/* expr.place = newtemp() */
		/* expr.code = expr1.code || term.code || 
								gen(expr.place,expr1.place,"+",term.place) */

		place_node_ptr ->setup_dependency(expr1_node_ptr->place_node_ptr);
		place_node_ptr ->setup_dependency(term_node_ptr->place_node_ptr);

	} else if(rule_no == expr3_rule ){
		term_node *term_node_ptr;

		term_node_ptr = (term_node *)children[0];

		/* Dependency of  attributes */
		/* expr.code = fn(term.code ) */
		/* expr.place = fn(term.place) */
		place_node_ptr -> setup_dependency ( term_node_ptr->place_node_ptr );
	}else{
		assert(0);
	}
}

void expr_node::eval_semantic_rules(int attrib_no)
{
	int op;

	if((rule_no == expr1_rule) || (rule_no == expr2_rule ) ){
		if(rule_no == expr1_rule){
			op=ADD;
		}else{
			op=SUB;
		}
		expr_node *expr1_node_ptr;
		term_node *term_node_ptr;

		expr1_node_ptr=(expr_node *)children[0];
		term_node_ptr = (term_node *)children[2];

		/* Updating attributes */
		/* expr.place = newtemp() */
		/* expr.code = expr1.code || term.code || 
							gen(expr.place,expr1.place,"+",term.place) */
		if(op == ADD){
			place = expr1_node_ptr->place + term_node_ptr->place;
		}else{
			place = expr1_node_ptr->place - term_node_ptr->place;
		}

	} else if(rule_no == expr3_rule ){
		term_node *term_node_ptr;

		term_node_ptr = (term_node *)children[0];

		/* Updating attributes */
		/* expr.place = term.place */
		place = term_node_ptr->place;
	}
}



void stmt_node::create_dep_graph_nodes()
{
	dep_graph_node *p;

	place_node_ptr = p = new dep_graph_node ( PLACE_ATTRIB,this) ;
	dep_graph.push_back(p);
}

void stmt_node::setup_semantic_rules()
{
	expr_node *expr_node_ptr;

	if(rule_no == stmt_rule){
		expr_node_ptr = (expr_node *)children[0];

		/* Updating attributes */
		/* stmt.code = expr.code || 
										  gen(stmt.place,expr.place,"",""); 
		*/
		place_node_ptr -> setup_dependency( expr_node_ptr->place_node_ptr );
	}else{
		assert(0);
	}
}

void stmt_node::eval_semantic_rules(int attrib_no)
{
	expr_node *expr_node_ptr;

	expr_node_ptr = (expr_node *)children[0];

	/* Updating attributes */
	/* stmt.code = expr.code || 
									  gen(stmt.place,expr.place,"",""); 
	*/
	if(rule_no == stmt_rule ){
		if (attrib_no == PLACE_ATTRIB ){
			place = expr_node_ptr->place;
		}
	}
}



void term_node::create_dep_graph_nodes()
{
	dep_graph_node *p;

	place_node_ptr = p = new dep_graph_node ( PLACE_ATTRIB,this) ;
	dep_graph.push_back(p);
}
void term_node::setup_semantic_rules()
{

	if((rule_no == term1_rule) || (rule_no == term2_rule) ){

		term_node *term_node1_ptr;
		factor_node *factor_node_ptr;

		term_node1_ptr = (term_node *)children[0];
		factor_node_ptr = (factor_node *)children[2];

		/* Updating attributes */
		/* term.place = newtemp() */
		/* term.code = term1.code || factor.code || gen(term.place,term1.place,"*",factor.place) */
		place_node_ptr ->setup_dependency(term_node1_ptr->place_node_ptr );
		place_node_ptr ->setup_dependency(factor_node_ptr->place_node_ptr);
	}else if (rule_no == term3_rule){
		factor_node *factor_node_ptr;

		factor_node_ptr = (factor_node *)children[0] ;

		/* Updating attributes */
		/* term.place = factor.place */
		/* term.code = factor.code */
		place_node_ptr -> setup_dependency(factor_node_ptr->place_node_ptr);
	}else{
		assert(0);
	}
}
void term_node::eval_semantic_rules(int attrib_no)
{
	int op;

		if((rule_no == term1_rule) || (rule_no == term2_rule) ){

			if(rule_no == term1_rule ){
				op=MUL;
			}else {
				op=DIV;
			}
			term_node *term_node1_ptr;
			factor_node *factor_node_ptr;

			term_node1_ptr = (term_node *)children[0];
			factor_node_ptr = (factor_node *)children[2];

			/* Updating attributes */
			/* term.place = newtemp() */
			/* term.code = term1.code || factor.code || gen(term.place,term1.place,"*",factor.place) */
			if(op == MUL ){
				place=term_node1_ptr->place * factor_node_ptr->place;
			}else{
				place=term_node1_ptr->place / factor_node_ptr->place;
			}
		}else if (rule_no == term3_rule){
			factor_node *factor_node_ptr;

			factor_node_ptr = (factor_node *)children[0] ;

			/* Updating attributes */
			/* term.place = factor.place */
			/* term.code = factor.code */
			place = factor_node_ptr->place;
		}
}

void factor_node::create_dep_graph_nodes()
{
		dep_graph_node *p;

		place_node_ptr = p = new dep_graph_node ( PLACE_ATTRIB,this) ;
		dep_graph.push_back(p);
}


void factor_node::setup_semantic_rules()
{

	if(rule_no == factor1_rule ){
		expr_node *expr_node_ptr;

		expr_node_ptr = (expr_node *)children[1];

		/* Updating attributes */
		/* factor.place = expr.place */
		/* factor.code = expr.code */
		place_node_ptr -> setup_dependency(expr_node_ptr ->place_node_ptr);

	} else if (rule_no == factor3_rule ){
		CONSTANT_node *constant_node_ptr;

		constant_node_ptr = (CONSTANT_node *)children[0];

		/* Updating attributes */
		/* factor.place = newtemp()*/
		/* factor.code = gen(factor.place,CONSTANT.value,NULL,NULL); */

		 place_node_ptr -> setup_dependency(constant_node_ptr->value_node_ptr);

	}else{
		assert(0);
	}
}
void factor_node::eval_semantic_rules(int attrib_no)
{
		if(rule_no == factor1_rule ){
			expr_node *expr_node_ptr;

			expr_node_ptr = (expr_node *)children[1];

			/* Updating attributes */
			/* factor.place = expr.place */
			/* factor.code = expr.code */
			place = expr_node_ptr ->place;

		}else if(rule_no == factor2_rule ){

		} else if (rule_no == factor3_rule ){
			CONSTANT_node *constant_node_ptr;

			constant_node_ptr = (CONSTANT_node *)children[0];

			/* Updating attributes */
			/* factor.place = newtemp()*/
			/* factor.code = gen(factor.place,CONSTANT.value,NULL,NULL); */
			place = constant_node_ptr->value ;

		}
}

void 
CONSTANT_node::create_dep_graph_nodes() 
{ 
	value_node_ptr = new dep_graph_node(PLACE_ATTRIB,this); 
	dep_graph.push_back(value_node_ptr);
}

vector <parse_tree_node *> parse_tree_node_list;

void visit(parse_tree_node *p)
{
	int n,i;


	parse_tree_node_list.push_back(p);

	n=p->children.size();

	for(i=0;i<n;i++){
		visit(p->children[i]);
	}
}


void create_parse_tree_node_list(parse_tree_node *p)
{
	visit(p);
}

void create_dep_graph()
{

	int i,n;
	parse_tree_node *p;

	create_parse_tree_node_list(parse_root);

	n = parse_tree_node_list.size();

	/*
	for (i=0;i<n;i++){
		p = parse_tree_node_list[i];
		p->print();
	}
	*/

	for (i=0;i<n;i++){
		p = parse_tree_node_list[i];
		p->create_dep_graph_nodes();
	}

	for (i=0;i<n;i++){
		p = parse_tree_node_list[i];
		p->setup_semantic_rules();
	}

	/*
	dep_graph_node *d;
	n=dep_graph.size();
	for (i=0;i<n;i++){
		d = dep_graph[i];
		d->parse_tree_node_ptr->print();
		printf("inc=%d outgoing=%d \n",d->incntr,d->outgoing.size());
	}
	*/

	top_sort();
}

void print_result()
{
	stmt_node *s;

	s = (stmt_node *)parse_root;
	printf("%d\n",s->place);
}


/* dep_graph.cc */
graph dep_graph;
graph top_sorted_dep_graph;

void display(graph *ptr);


graph :: iterator find_dep_graph_node_with_no_incoming();
int print_inc();

void
dep_graph_node::setup_dependency(dep_graph_node *p )
{
	p->outgoing.push_back(this);
	incntr++;
}

void
dep_graph_node::adjust_dependency_incntr()
{
	int j,m;
	string s;

	m=outgoing.size();
	for(j=0;j<m;j++){
		outgoing[j]->incntr --;
	}
}

void top_sort();

void top_sort()
{
	dep_graph_node *v;
	int i,n;
	graph::iterator result ;


	n=dep_graph.size();

	for(i=0;i<n;i++){
		result = find_dep_graph_node_with_no_incoming();
		if(result == dep_graph.end() ){
			print_inc();
			printf("Graph has a cycle. Exiting \n");
			exit(1);
		}
		v= (*result);

		v->adjust_dependency_incntr();

		top_sorted_dep_graph.push_back(v);
		dep_graph.erase(result);


	}
}
	
	
graph :: iterator 
find_dep_graph_node_with_no_incoming()
{
	graph :: iterator iter,end;


	iter = dep_graph.begin();
	end = dep_graph.end();

	while(iter != end ){

		if((*iter) ->incntr ==  0 ){
			return(iter);
		}
		iter ++;
	}
	return(iter);
}

int 
print_inc()
{
	graph :: iterator iter,end;


	iter = dep_graph.begin();
	end = dep_graph.end();

	while(iter != end ){
		printf(" inc cntr=%d \n",(*iter) ->incntr );
		iter ++;
	}
	return(0);
}

void display_top_sorted_dep_graph()
{
	int n,i;
	dep_graph_node *ptr;

	printf("\n==============================\n");
	printf("\nTopographically sorted dependency Graph is \n\n");
	n=top_sorted_dep_graph.size();

	for(i=0;i<n;i++){
		ptr = top_sorted_dep_graph[i];
		printf("%d. %s attribute of ",i,(ptr->attrib_no==CODE_ATTRIB)?"code":"place");
		ptr->parse_tree_node_ptr->print();
	}
}

/* eval_sem_rules.cc */
extern graph top_sorted_dep_graph;

void evaluate_semantic_rules()
{
	int i,n;
	dep_graph_node *d;
	parse_tree_node *p;
	int attrib_no=d->attrib_no;

	n = top_sorted_dep_graph.size();

	for(i=0;i<n;i++){
		d = top_sorted_dep_graph[i];
		p=d->parse_tree_node_ptr;
		attrib_no=d->attrib_no;
		p->eval_semantic_rules(attrib_no);
	}
}
