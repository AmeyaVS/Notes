%token IDENTIFIER REAL_CONSTANT CONSTANT  IF ELSE WHILE STRUCT STRING_LITERAL
%token BREAK SWITCH CASE DEFAULT RET
%token INT CHAR FLOAT VOID
%token AND_OP OR_OP NEG
%token LT_OP GT_OP EQ_OP LE_OP GE_OP NE_OP PTR_OP
%token EXTERN


%left OR_OP
%left AND_OP
%left '='
%left '&'
%left '+' '-' 
%left '*' '/' 
%left '[' ']' PTR_OP DOT
%left LT_OP GT_OP EQ_OP LE_OP GE_OP NE_OP 
%left '(' ')' 


%nonassoc NEG

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%{
   #include <stdlib.h>
   #include "stl.h"

	 #include "semantic_analysis.h"
    #include "syntax_analysis.h"



   extern int yyerror (char *s);
   extern int yylex();

   extern int line_no;

    int sem_err=0;

   type_expr_list type_list;

   vector <sym_tab *> sym_tables;

   stack <sym_tab *> tbl_stk;

   sym_tab *curr_sym_tab_ptr,*global_sym_tab_ptr ;
	sym_tab *str_lit_sym_tab_ptr  ;

	void init_global_sym_tab()
   {
      global_sym_tab_ptr = curr_sym_tab_ptr = new sym_tab();
      curr_sym_tab_ptr ->name="GLOBALS";

      str_lit_sym_tab_ptr = new sym_tab();
      str_lit_sym_tab_ptr ->name="STRING LITERALS";

   }

   sym_tab_entry *lookup(string name)
   {
      sym_tab *sym_tab_ptr;
      sym_tab_entry *entry;

      sym_tab_ptr = curr_sym_tab_ptr ;

      while (sym_tab_ptr != NULL ){
         entry = sym_tab_ptr -> find(name);

         if(entry != NULL ){
            return(entry);
         }

           /* Check in the previous scope */
           sym_tab_ptr = sym_tab_ptr -> previous ;
      }

      return(NULL);
   }

    int
    check_operands(binary_expr_node *be1,binary_expr_node *be2,char sign)
    {
        if( (be1->expr_type->gettype() == INTEGER ) && 
             (be2->expr_type->gettype() == INTEGER ) ) {
				 return(1);
        }else {
            printf("Incompatible operands for the operator '%c' in line no =%d (Integer Operands allowed)\n",sign,line_no);
				 return(0);
        }
    }

	 void print_sym_tables()
	 {
	 	int len=sym_tables.size();
		int i;
		sym_tab *sptr;

		for(i=0;i<len;i++){
			sptr = sym_tables[i];
			sptr->print();
		}
	 }

%}

%start file


%%

file 
   : external_definition
   | file external_definition
   ; 

external_definition
   : function_definition
   | declaration
   ; 

function_definition
   : type_spec func_decl P function_body
     {   

       curr_sym_tab_ptr = tbl_stk.top();


       tbl_stk.pop();

     }
   ;

function_body 
   : '{' stmt_list '}'
   | '{' declaration_list stmt_list '}'
   ; 

declaration 
   : type_spec declarator_list ';'
   | type_spec func_decl ';'
   ;

type_spec 
          : INT
            {
                type_expr *t;

                t = new integer();

                $$ = new type_spec_node(t);
            }
          | CHAR
            {
                type_expr *t;

                t = new character();

                $$ = new type_spec_node(t);
            }
          | FLOAT
            {
                type_expr *t;

                t = new real();

                $$ = new type_spec_node(t);
            }
          | VOID
            {
                type_expr *t;

                t = new void_type();

                $$ = new type_spec_node(t);
            }
          | struct_specifier
            {
               $$=$1;
            }
          ;

struct_specifier
   : STRUCT IDENTIFIER '{' T declaration_list '}'
     {
         type_expr *t1;
         IDENTIFIER_node *iptr;

         iptr = (IDENTIFIER_node *)$2;

         t1=type_list[iptr->name.c_str()];

         curr_sym_tab_ptr = tbl_stk.top();

         tbl_stk.pop();

         $$ = new type_spec_node(t1);
     }
   | STRUCT IDENTIFIER
     {
         type_expr *t1;
         IDENTIFIER_node *iptr;

         iptr = (IDENTIFIER_node *)$2;

         t1=type_list[iptr->name.c_str()];

         if(t1 == NULL){
                printf("Definition not found for struct %s in line no = %d \n",iptr->name.c_str(),line_no);
                t1 = new type_error();
         }

         $$ = new type_spec_node(t1);
     }
   ;

declarator
   : IDENTIFIER
     {
         declarator_node *dptr;
         IDENTIFIER_node *iptr;

         iptr = (IDENTIFIER_node *)$1;

         dptr = new declarator_node();
         dptr -> name = iptr ->name ; 

         delete(iptr);

         $$= (node *)dptr;
     }
   | declarator '[' CONSTANT ']'
     {
         declarator_node *dptr;
         array *aptr;
         CONSTANT_node *cptr;

         dptr = (declarator_node *)$1;
         cptr = (CONSTANT_node *)$3;

         aptr = new array(cptr->val);

         if(dptr -> data_type != NULL ){
            dptr -> data_type->chain(aptr);
         }else{
            dptr->data_type=aptr;
         }

           $$=(node *)dptr;
     }
   | '*' declarator
     {
        declarator_node *dptr;
        pointer *p;

        dptr = (declarator_node *)$2;

        p = new pointer();

        if( dptr -> data_type != NULL ){
            dptr -> data_type->chain(p);
         }else{
              dptr -> data_type = p;
         }
        $$= (node *)dptr;
     }
     ;
func_decl
   : declarator '('  ')'
     {
         declarator_node *dptr;
         func_decl_node *fptr;
         sym_tab_entry *sptr;
         type_spec_node *typtr ;
         type_expr *ret;
         type_expr *t;
			function *f;

         dptr = (declarator_node *)$1;
         typtr = (type_spec_node *)$0;


         ret = typtr->data_type ;

         /* prototype not present */
         sptr = lookup(dptr->name);
         if(sptr == NULL ){

            /* the type is a function */
            t = new function(ret);
            sptr=curr_sym_tab_ptr->add(dptr->name,t);
         }else{
				t= f = (function *)sptr->type;
				if(f->no_of_args != 0 ){
                printf("prototype does not match the definition on %d \n",line_no);
				}

			}

         fptr = new func_decl_node(dptr->name,t) ;

           $$=fptr;
     }
   | declarator '('  parameter_list ')'
     {
         parameter_list_node *plptr;
         declarator_node *dptr;
         func_decl_node *fptr;
         type_spec_node *typtr ;
         sym_tab_entry *sptr;
         sym_tab_entry *sptr1;
         type_expr *ret;
         int i,no_of_args;
         type_expr *t=NULL;
         function *f;
         type_expr *decl_arg_type,*formal_arg_type;
			int err_ctr=0;

         dptr = (declarator_node *)$1;
         plptr = (parameter_list_node *)$3;
         typtr = (type_spec_node *)$0;

         ret = typtr->data_type ;

            sptr = lookup(dptr->name);
         if(sptr == NULL ){
             /* prototype not present */
             t = new function(ret);
            sptr=curr_sym_tab_ptr->add(dptr->name,t);
             no_of_args = plptr->no_of_args;

             f = (function *)t;
             for(i=0;i<no_of_args;i++){
                sptr1 = plptr->get_arg(i);
                f -> add_arg(sptr1);
             }
         }else{
                /* prototype present */
                no_of_args = plptr->no_of_args;
                t = f = (function *)sptr->type;
					 if(f->no_of_args == no_of_args ){
                	for(i=0;i < no_of_args;i++){
                    decl_arg_type = f->get_arg_type(i);
                    formal_arg_type = plptr -> get_arg_type(i); 
                    if(!type_equal(decl_arg_type,formal_arg_type)){
						      err_ctr ++;
								break;
                    }
                	}
					 }else{
					 	err_ctr ++;
					 }
					 if(err_ctr){
                	printf("prototype does not match the definition on %d \n",line_no);
					 }
          }

         fptr = new func_decl_node(dptr->name,t) ;

           $$=fptr;
     }
   ;

parameter
   : type_spec declarator
     {
         declarator_node *dptr;
         type_spec_node *typtr ;
         parameter_node *pptr;
         type_expr *t1;

         dptr = (declarator_node *)$2;
         typtr = (type_spec_node *)$1;

         t1 = typtr->data_type ;

        if( dptr -> data_type != NULL ){
            dptr -> data_type->chain(t1);
         }else{
              dptr -> data_type = t1;
         }

         pptr = new parameter_node(dptr->name,dptr->data_type);

         $$ = (node *)pptr;

     }
   ;

parameter_list
   : parameter
     {
          parameter_node *pptr;
          parameter_list_node *plptr;

          pptr = (parameter_node *)$1;

          plptr = new parameter_list_node(pptr->name,pptr -> expr_type );

          $$ = (node *)plptr; 

     }
   | parameter_list ',' parameter
     {
          parameter_node *pptr;
          parameter_list_node *plptr;

          pptr = (parameter_node *)$3;
          plptr = (parameter_list_node *)$1;

          plptr ->add_arg(pptr->name,pptr -> expr_type);

          $$ = (node *)plptr; 

     }
   ;

declarator_list 
   : declarator
     {
         declarator_node *dptr;
         type_spec_node *typtr ;
         type_expr *t1;
			sym_tab_entry *tmp;

         dptr = (declarator_node *)$1;
         typtr = (type_spec_node *)$0;

         t1 = typtr->data_type ;

        if( dptr -> data_type != NULL ){
            dptr -> data_type->chain(t1);
         }else{
              dptr -> data_type = t1;
         }

			tmp = curr_sym_tab_ptr->find(dptr->name);

			if(tmp != NULL){
            printf("Re-declaration of the Variable '%s' in the same scope line=%d \n",dptr->name.c_str(),line_no);
            sem_err ++;
			}else{
         	tmp=curr_sym_tab_ptr->add(dptr->name,dptr->data_type);
			}


     }
   | declarator_list ',' declarator
     {
         declarator_node *dptr;
         type_spec_node *tptr;
         type_expr *t1;
			sym_tab_entry *tmp;

         dptr = (declarator_node *)$3;
         tptr = (type_spec_node *)$0;

         t1=tptr->data_type;

         /* chain it to the end */
        if( dptr -> data_type != NULL ){
            dptr -> data_type->chain(t1);
         }else{
              dptr -> data_type = t1;
         }

			tmp = curr_sym_tab_ptr->find(dptr->name);

			if(tmp != NULL){
            printf("Re-declaration of the Variable '%s' in the same scope line=%d \n",dptr->name.c_str(),line_no);
            sem_err ++;
			}else{
         	tmp=curr_sym_tab_ptr->add(dptr->name,dptr->data_type);
			}

         
         //curr_sym_tab_ptr->add(dptr->name,dptr->data_type);
     }
   ;


declaration_list 
   : declaration
   | declaration_list declaration
   ; 
T
   : {
         IDENTIFIER_node *id;
         type_expr *t1;

          id = (IDENTIFIER_node *)($-1);
        tbl_stk.push(curr_sym_tab_ptr);

        curr_sym_tab_ptr = new sym_tab();

		  sym_tables.push_back(curr_sym_tab_ptr);

          curr_sym_tab_ptr -> name = id ->name ;
        curr_sym_tab_ptr -> previous = tbl_stk.top();

         t1=type_list[id->name.c_str()];

         if(t1 == NULL){
             t1 = new record(curr_sym_tab_ptr,id->name);
            type_list[id->name.c_str()]= t1;
         }else{
            /* Already present */
                printf("struct %s defined multiple times \n",id->name.c_str());
                exit(0);
         }

     }   ;

/* Ex3 */

stmt 
   : expr ';'
     {
           expr_node *e;

         e = (expr_node *)$1;

     }
   ;

expr
   : unary_expr '=' expr
     {
          unary_expr_node *u;
            expr_node *e,*e1;

          u = (unary_expr_node *)$1;
          e1 = (expr_node *)$3;

          e = new expr_node();

         if(type_equal(u->expr_type,e1->expr_type)){
            e->expr_type = new void_type();
         }else{
            printf("Semantic error in line %d (LHS and RHS of '=' are not the same type)\n",line_no);
            sem_err ++;
            e->expr_type = new type_error();
         }
         $$ = (node *)e;
     }
   ;

unary_expr
   : IDENTIFIER
     {      
         unary_expr_node *u;
         IDENTIFIER_node *id;

         u = new unary_expr_node ;
         id = (IDENTIFIER_node *)$1;

         u->place = lookup(id->name) ;
   
         if(u->place != NULL ){
            u->expr_type = u->place->type;
         }else{
                u->expr_type = new type_error();
                printf("Variable '%s' not defined, but used in line %d \n",id->name.c_str(),line_no);
            }
      
         $$=u;
     }      
   | REAL_CONSTANT
     {
         unary_expr_node *u;

         u = new unary_expr_node ;
         u->expr_type = new real();

         $$ = u;
     }
   | CONSTANT 
     {
         unary_expr_node *u;

         u = new unary_expr_node ;
         u->expr_type = new integer();

         $$ = u;
     }
   | STRING_LITERAL
   | '(' expr ')'
	  {
	      unary_expr_node *u;
         expr_node *e;
 
		   u = new unary_expr_node ;
         e = (expr_node *)$2;

			u->expr_type = e->expr_type;
         $$=u;
	  }
   | '-' unary_expr
   {
         unary_expr_node *u,*u1;

         u1 = (unary_expr_node *)$2;
         u = new unary_expr_node;

			u->expr_type = u1->expr_type;

         $$=u;
     }
	  ;

binary_expr
   : binary_expr '*' binary_expr
      {
            binary_expr_node *be;
            binary_expr_node *be1,*be2;

            be1= (binary_expr_node *)$1;
            be2= (binary_expr_node *)$3;

            be = new binary_expr_node();

            if(check_operands(be1,be2,'*') == 1){
               be->expr_type = be1->expr_type;
            }else{
               be->expr_type = new type_error();
            }

				$$=be;
     }
   | binary_expr '/' binary_expr
      {
            binary_expr_node *be;
            binary_expr_node *be1,*be2;

            be1= (binary_expr_node *)$1;
            be2= (binary_expr_node *)$3;

            be = new binary_expr_node();

            if(check_operands(be1,be2,'*') == 1){
               be->expr_type = be1->expr_type;
            }else{
               be->expr_type = new type_error();
            }

				$$=be;
     }
   | binary_expr '+' binary_expr
      {
            binary_expr_node *be;
            binary_expr_node *be1,*be2;

            be1= (binary_expr_node *)$1;
            be2= (binary_expr_node *)$3;

            be = new binary_expr_node();

            if(check_operands(be1,be2,'*') == 1){
               be->expr_type = be1->expr_type;
            }else{
               be->expr_type = new type_error();
            }

				$$=be;
     }
   | binary_expr '-' binary_expr
     {
            binary_expr_node *be;
            binary_expr_node *be1,*be2;

            be1= (binary_expr_node *)$1;
            be2= (binary_expr_node *)$3;

            be = new binary_expr_node();

            if(check_operands(be1,be2,'*') == 1){
               be->expr_type = be1->expr_type;
            }else{
               be->expr_type = new type_error();
            }

				$$=be;
     }
   | unary_expr
     {
         unary_expr_node *u;
         binary_expr_node *be;

         be = new binary_expr_node();
         u = (unary_expr_node *)$1;

         be->expr_type=u->expr_type;

         $$=be;
      }
   ;

/* Ex4 */

unary_expr
   : unary_expr '[' expr ']'
     {
         unary_expr_node *u1;
         expr_node *e;

         u1 = (unary_expr_node *)$1;
         e = (expr_node *)$3;

         if( u1->expr_type->gettype() != ARRAY ) {
            printf("Incompatible operator '[' used in line %d \n",line_no);
            u1->expr_type = new type_error();
         } else {

            if(e->expr_type->gettype() != INTEGER ){
               printf("Index in array is not an integer in line %d \n",line_no);
               u1->expr_type = new type_error();
            }else{
               u1->expr_type=u1->expr_type->next;
            }

         }


         $$=u1;
    }
   ;

/* Ex5 */

unary_expr
   : '&' unary_expr
     {
         unary_expr_node *u;
         type_expr *t;
         int curr_typ;

         u = (unary_expr_node *)$2;

         curr_typ = u->expr_type->gettype();

         if((curr_typ == CONSTANT) || (curr_typ == REAL_CONSTANT) ){
            printf("Incompatible operand to & used in line %d \n",line_no);
            u->expr_type = new type_error();
         }else{
            t = new pointer();
            t -> chain(u->expr_type);
            u->expr_type = t;
         }
         $$ = (node *)u;

     }
   | '*' unary_expr
     {
         unary_expr_node *u;
         type_expr *t;
         int curr_typ;

         u = (unary_expr_node *)$2;

         curr_typ = u->expr_type->gettype();

         if(curr_typ != POINTER ){
            printf("Incompatible operator * used in line %d \n",line_no);
            u->expr_type = new type_error();
         }else {
            t = u->expr_type->next;
            u->expr_type = t;
         }
         $$ = (node *)u;
     }
   ;


/* Ex6 */

unary_expr
   : unary_expr DOT IDENTIFIER
     {
         unary_expr_node *u1;
         IDENTIFIER_node *iptr;
         sym_tab *sym_tab_ptr;
         sym_tab_entry *fptr;
         type_expr *typ;
         record *rptr;

         u1 = (unary_expr_node *)$1;
         iptr = (IDENTIFIER_node *)$3;

         typ = u1->expr_type;

         /* The LHS is a record Type */
         if(typ->gettype() != RECORD){
            printf("Incompatible operator (.) used in line %d \n",line_no);
            u1->expr_type = new type_error();
         } else {

            /* It is a Record Type */
            rptr=(record *)typ;
            sym_tab_ptr=rptr->fields;

            /* Check for the Field */
            if((fptr=sym_tab_ptr->find(iptr->name)) == NULL ){
               printf("Field '%s' used in line %d not declared \n",iptr->name.c_str(),line_no);
               u1->expr_type = new type_error();
            }else{
               /* No error */
               u1->expr_type=fptr->type;

            }

         }

         $$=u1;
     }
   | unary_expr PTR_OP IDENTIFIER
     {
         unary_expr_node *u1;
         IDENTIFIER_node *iptr;
         sym_tab_entry *fptr;;
         sym_tab *sym_tab_ptr;
         record *rptr;

         u1 = (unary_expr_node *)$1;
         iptr = (IDENTIFIER_node *)$3;

         /* Check the type */
         if( u1->expr_type->gettype() != POINTER) {
             printf("Incompatible operator '->' used in line %d \n",line_no);
             u1->expr_type = new type_error();
         } else {

            if((u1->expr_type->next->gettype()) != RECORD ) {
                printf("Incompatible operator '->' used in line %d \n",line_no);
                u1->expr_type = new type_error();

            } else {

               rptr=(record *)u1->expr_type->next ;
               sym_tab_ptr=rptr->fields;

               /* Get the offset of the Field */
               if((fptr=sym_tab_ptr->find(iptr->name)) == NULL ){
                  printf("Field '%s' used in line %d not declared \n",
                                       iptr->name.c_str(),line_no);
                   u1->expr_type = new type_error();
               } else {

                  u1->expr_type=fptr->type;
               }

            }
         }

         $$=u1;
      }
   ;

/* Ex7 */
stmt 
   : IF '(' test_expr ')' M stmt     %prec LOWER_THAN_ELSE
   | IF '(' test_expr ')' M stmt ELSE N M stmt
   ;

stmt
   : '{' stmt_list '}'
   ;

stmt_list 
   : stmt_list stmt
   | stmt
   ;

test_expr 
   : expr
   ;

expr
   : or_expr
     {

        expr_node *e;
        binary_expr_node *be;

        e = new expr_node;
        be = (binary_expr_node *)$1;

        e->expr_type = be -> expr_type;

        $$=$1;
     }

   ;

or_expr
   : or_expr OR_OP M and_expr
   | and_expr
     {
        $$=$1;
     }

   ;

and_expr 
   : binary_expr
     {
         $$=$1;
     }
   | and_expr AND_OP M binary_expr
   ;

binary_expr
   : binary_expr LT_OP binary_expr
   | binary_expr GT_OP binary_expr
   | binary_expr GE_OP binary_expr
   | binary_expr LE_OP binary_expr
   | binary_expr EQ_OP binary_expr
   | binary_expr NE_OP binary_expr
   ;

M 
   :
   ;
N 
   :
   ;

/* Ex8 */

stmt
   : WHILE R '(' test_expr ')' M stmt
   | BREAK ';'
   ;

R
   :
   ;

/* Ex9 */

stmt
   : SWITCH Q '(' expr ')' N '{' case_stmt_list '}'
   ;

case_stmt 
   : CASE CONSTANT ':' M stmt_list
   | DEFAULT ':' M stmt_list
   ;

case_stmt_list
   : case_stmt_list case_stmt
   | case_stmt
   ;

Q 
   :
   ;

/* Ex10 */

unary_expr
   : unary_expr '(' ')'
     {
            unary_expr_node *u,*u1;
          function *f;

          u1 = (unary_expr_node *)$1;

          u = new unary_expr_node();

           if(u1->expr_type->gettype() == FUNCTION ){
               f = (function *)u1->expr_type;
               u->expr_type = f->ret_type;
           } else {
               printf("Trying to invoke a non Function in line %d \n",line_no);
               u->expr_type = new type_error();
           }
           $$=u;
     }
   | unary_expr '(' args_list ')'
     {
            unary_expr_node *u,*u1;
          args_list_node *a1;
          function *f;
          int no_of_args;
          int i;
          int err_counter=0;
          type_expr *decl_arg_type,*formal_arg_type;

          u1 = (unary_expr_node *)$1;
          a1 = (args_list_node *)$3;

          u = new unary_expr_node();

          if(u1->expr_type->gettype() == FUNCTION ){
                 f = (function *)u1->expr_type;
               no_of_args = f -> no_of_args;

               /* Verify arguments type */
               if(f->no_of_args != a1->no_of_args){
                  err_counter ++;
               }else{
                  for(i=0;i < no_of_args;i++){
                     decl_arg_type = f->get_arg_type(i);
                     formal_arg_type = a1 -> get_arg_type(i); 
                     if(!type_equal(decl_arg_type,formal_arg_type)){
                        err_counter ++;
                        break;
                     }
                  }
               }
               if(err_counter == 0){
                    u->expr_type = f->ret_type;
               }else{
                  printf("Function Invocation used in line %d does not match with its Definition/Declaration \n",line_no);
                  u->expr_type = new type_error();
               }
          } else {
               printf("Trying to invoke a non Function in line %d \n",line_no);
               u->expr_type = new type_error();
          }
          $$=u;
     }
   ;

args_list 
   : args_list ',' expr
     {
          expr_node *e;
          args_list_node *a;

          a = (args_list_node *)$1;
          e = (expr_node *)$3;

          a->add_arg(e -> expr_type );

          $$ = (node *)a; 

     }
   | expr 
     {
          expr_node *e;
          args_list_node *a;

          e = (expr_node *)$1;

          a = new args_list_node(e -> expr_type );

          $$ = (node *)a; 

     }
   ;
          
stmt 
   : RET ';'
   | RET expr ';'
   ;

P
   :
     {   
         M_node *m;
            func_decl_node  *fptr;
				function *f;
				int no_of_args;
				sym_tab_entry *sptr1;
				int i;
 
           tbl_stk.push(curr_sym_tab_ptr);

             fptr = (func_decl_node *)$0;

          curr_sym_tab_ptr = new sym_tab(fptr->name);

		    sym_tables.push_back(curr_sym_tab_ptr);

           curr_sym_tab_ptr -> previous = tbl_stk.top();

			  /* Adding the parameters into the Symbol table*/
			  f = (function *)fptr->expr_type;
			  no_of_args=f->no_of_args;
           for(i=0;i<no_of_args;i++){
              sptr1 = f->get_arg(i);
				  curr_sym_tab_ptr->add(sptr1->name,sptr1->type);
           }



         $$ = (node *)m ;
     } 
   ;

%%
