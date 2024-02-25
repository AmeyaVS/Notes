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
	#include "ic_gen.h"
	#include "syntax_analysis.h"


   extern int line_no; /* defined in lexical_analyzer */
	extern int next_quad; /* defined in ic_gen */


   extern int yyerror (char *s);
   extern int yylex();

	/* Globals used in other files too */
   int sem_err=0;
   sym_tab *global_sym_tab_ptr ;
	sym_tab *str_lit_sym_tab_ptr  ;

   static type_expr_list type_list;
   static vector <sym_tab *> sym_tables;

   static stack <sym_tab *> tbl_stk;

	static sym_tab *cons_lit_sym_tab_ptr  ;
   static sym_tab *curr_sym_tab_ptr;

	void init_global_sym_tab()
   {
      global_sym_tab_ptr = curr_sym_tab_ptr = new sym_tab();
      curr_sym_tab_ptr ->name="GLOBALS";

      str_lit_sym_tab_ptr = new sym_tab();
      str_lit_sym_tab_ptr ->name="STRING LITERALS";

      cons_lit_sym_tab_ptr = new sym_tab();
      cons_lit_sym_tab_ptr ->name="CONSTANT LITERALS";

   }

   static sym_tab_entry *lookup(string name)
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

    static int
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

	#ifdef ICGEN
   /* Ex4 */
	type_expr *make_type_expr(basic_types bt);
   sym_tab_entry *make_const_sym_tab_entry(int val)
   {
      sym_tab_entry *sptr;
      char str[50];
	   type_expr *t;
		string s;

	   t = make_type_expr(CONSTAN);

      sprintf(str,"%d",val);

		/* Lookup before creation */
		s +=str;

		sptr = cons_lit_sym_tab_ptr->find(s);

		if(sptr == NULL ){
		   sptr = cons_lit_sym_tab_ptr->add(s,t);
		}

      return(sptr);
   }

   static sym_tab_entry *make_real_const_sym_tab_entry(float val)
   {
      sym_tab_entry *sptr;
      char str[50];
	   type_expr *t;

	   t = make_type_expr(REAL);

      sprintf(str,"%f",val);
      sptr = new sym_tab_entry(str,t);

      return(sptr);
   }


	extern int line_no;

	type_expr *make_type_expr(basic_types bt)
	{
		type_expr *t;

		switch(bt)
		{
			case INTEGER:
                		t = new integer();
							break;
			case CHARACTER:
                		t = new character();
							break;
			case REAL:
                		t = new real();
							break;
			case VOID_TYPE:
                		t = new void_type();
							break;
			case LABEL:
	   					t = new label();
							break;
			case CONSTAN:
	   					t = new constant();
							break;
			case TYPE_ERROR:
                     t = new type_error();
							break;
			default :
						printf("Unknown Type \n");
						exit(0);
		}

		return(t);
	}


	static int str_lit_cnt=1;
   static sym_tab_entry *newtemp()
   {

		sym_tab_entry *sptr;

		sptr = curr_sym_tab_ptr->make_temporary();

      return(sptr);
   }

	static int no_of_lbl;
   sym_tab_entry *newlbl()
   {
      char tmp_name[100];
      string my_tmp;
		sym_tab_entry *sptr;
	   type_expr *t;

      sprintf(tmp_name,".L%d",no_of_lbl);
      no_of_lbl++;
      my_tmp += tmp_name;


	   t = make_type_expr(LABEL);

      sptr = new sym_tab_entry(my_tmp,t);

      return(sptr);
   }
	
   sym_tab_entry *make_lbl(string lbl_str)
	{
		sym_tab_entry *sptr;
	   type_expr *t;



	   t = make_type_expr(LABEL);

      sptr = new sym_tab_entry(lbl_str,t);

      return(sptr);
   }


   static sym_tab_entry *make_string_lit_sym_tab_entry(string str_lit)
   {
      char str[50];
	   type_expr *t;
      sym_tab_entry *sptr;

	   t = make_type_expr(CONSTAN);

      sprintf(str,".lc%d",str_lit_cnt);
		str_lit_cnt ++;
      sptr=str_lit_sym_tab_ptr->add(str,t);
		sptr->literal=str_lit;

      return(sptr);
   }


   /* Ex6 */
	static void fetch_content(sym_tab_entry *t,unary_expr_node *u)
	{
		if(u->offset == NULL ){
			emit(ASSIGN,u->place,NULL,t);
		}else{
			emit(R_INDEX_ASSIGN,u->place,u->offset,t);
		}
	}

	/* Ex7 */
   static void process_relational_op(enum oprtr op,binary_expr_node *be,
                                 binary_expr_node *be1,
                                 binary_expr_node *be2)
	{
         be->place = NULL;

         be->truelist = makelist(next_quad);
         be->falselist = makelist((next_quad+1));

         emit(op,be1->place,be2->place,NULL);
         emit (GOTO,NULL,NULL,NULL);
	}

	/* Ex8 */

    class brk_lbl
   {
      public:
      int label;
      brk_lbl(int qua)
      {
         label = qua;
      }
   };

   static stack <brk_lbl *> brk_lbl_stk;
   
   static void get_brk_list(quad_list *q)
   {
         brk_lbl *b;
   
         b = brk_lbl_stk.top();

         while( b ->label != -1){

            q->push_front(b->label);

            brk_lbl_stk.pop();
            b = brk_lbl_stk.top();
         }

         /* removing marker */
         brk_lbl_stk.pop();

   }

	/* Ex9 */

   class value_label
   {
      public:
      CONSTANT_node *value ;
		sym_tab_entry *label;
      char default_case;
      value_label(CONSTANT_node *c1,sym_tab_entry *qua)
      {
         label = qua;
         value = c1 ;
      }
   };

   static stack <value_label *> vl;

	static void emit_n_way_branch(sym_tab_entry *expr_place)
	{
			value_label *p,*p1;
			sym_tab_entry *cplace;
			char def_case=0;
			list <value_label *>tmp_list;
			list <value_label *> :: iterator iter;

			p = vl.top();

			while( p ->label != NULL){

				tmp_list.push_front(p);

				vl.pop();
				p = vl.top();
			}

			/* removing marker */
			vl.pop();

			iter = tmp_list.begin();

			while( iter != tmp_list.end()){

				p = (*iter);

				if(p->default_case){
					p1=p;
					def_case=1;
					iter ++;
					continue;
				}
				cplace = make_const_sym_tab_entry(p->value->val);
				emit (EQ,expr_place,cplace,p->label);

				iter++;
			}

			if(def_case){
				emit(GOTO,p1->label,NULL,NULL);
			}

	}

	/* Ex10 */
	extern vector <quad *>quad_arr;

	static sym_tab_entry *curr_proc_entry;
	static quad_list return_list;


	static sym_tab_entry *newtemp1(type_expr *tmp_typ)
	{
		sym_tab_entry *sptr;
		sptr = curr_sym_tab_ptr->make_temporary(tmp_typ);

      return(sptr);
   }
	#endif



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

			#ifdef ICGEN
			if(sem_err == 0 ){
				sym_tab_entry *sptr;
				sym_tab_entry *sptr1;
				func_decl_node *fptr;

				fptr = (func_decl_node *)$2;

				sptr = lookup(fptr->name);

				sptr1 = newlbl();
				emit(LBL,sptr1,NULL,NULL);
			   backpatch(&return_list,sptr1);

				/* emit PROC_END */
				emit(PROC_END,sptr,NULL,NULL);

				curr_sym_tab_ptr->update_fields(sptr1);
				return_list.clear();


			}
			#endif


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

			#ifdef ICGEN
			if(sem_err == 0 ){
          stmt_node *s;
          s = new stmt_node;
         $$=s;
			}
			#endif
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

			#ifdef ICGEN
			if(sem_err == 0 ){

			quad *q;

         /* Ex4 */
         if(u->offset == NULL ){
            emit(ASSIGN,e1->place,NULL,u->place);
         }else{
            q=emit(L_INDEX_ASSIGN,u->place,u->offset,e1->place);
            q->size_of_op= u->expr_type->getsize();
         }

         e->truelist=NULL;
         e->falselist=NULL;
         e->place = u->place;

			}
			#endif

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
            	sem_err ++;
            }
      
         $$=u;
     }      
   | REAL_CONSTANT
     {
         unary_expr_node *u;

         u = new unary_expr_node ;
         u->expr_type = new real();
			#ifdef ICGEN
			if(sem_err == 0 ){
				REAL_CONSTANT_node *cptr;
				sym_tab_entry *se_ptr;

				cptr = (REAL_CONSTANT_node *)$1;
				se_ptr = make_real_const_sym_tab_entry(cptr->val);
				u->place = se_ptr;

			}
			#endif

         $$ = u;
     }
   | CONSTANT 
     {
         unary_expr_node *u;

         u = new unary_expr_node ;

         u->expr_type = new integer();

			#ifdef ICGEN
			if(sem_err == 0 ){
				CONSTANT_node *cptr;
				sym_tab_entry *se_ptr;

				cptr = (CONSTANT_node *)$1;
				se_ptr = make_const_sym_tab_entry(cptr->val);
				u->place = se_ptr;
			}
			#endif

         $$ = u;
     }
   | STRING_LITERAL
     {
			#ifdef ICGEN
			if(sem_err == 0 ){
				unary_expr_node *u;
				STRING_LIT_node *sptr;
				sym_tab_entry *se_ptr;

				u = new unary_expr_node ;
				sptr = (STRING_LIT_node *)$1;

				se_ptr = make_string_lit_sym_tab_entry(sptr->str);

				u->place = se_ptr;
				u->expr_type = se_ptr->type;

				$$=u;
			}
			#endif
    }

   | '(' expr ')'
     {
         unary_expr_node *u;
         expr_node *e;

         u = new unary_expr_node ;
         e = (expr_node *)$2;

         u->expr_type = e->expr_type;

			#ifdef ICGEN
			if(sem_err == 0 ){
				u->place = e->place;
				/* Ex7 */
				u->truelist=e->truelist;
				u->falselist=e->falselist;
			}
			#endif

         $$=u;
     }
   | '-' unary_expr
	  {
         unary_expr_node *u,*u1;

         u1 = (unary_expr_node *)$2;
         u = new unary_expr_node;

			u->expr_type = u1->expr_type;

			#ifdef ICGEN
			if(sem_err == 0 ){
				u->place=newtemp();

				if(u1->offset == NULL ){
					emit(UMINUS,u1->place,NULL,u->place);
				}else{
					emit(R_INDEX_ASSIGN,u1->place,u1->offset,u->place);
					emit(UMINUS,u->place,NULL,u->place);
				}
			}
			#endif


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

				#ifdef ICGEN
			if(sem_err == 0 ){
					be->place=newtemp();
					emit(MUL,be1->place,be2->place,be->place);
			}
				#endif

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

				#ifdef ICGEN
			if(sem_err == 0 ){
					be->place=newtemp();
					emit(DIV,be1->place,be2->place,be->place);
			}
				#endif

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

				#ifdef ICGEN
			if(sem_err == 0 ){
					be->place=newtemp();
					emit(ADD,be1->place,be2->place,be->place);
			}
				#endif

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

				#ifdef ICGEN
			if(sem_err == 0 ){
					be->place=newtemp();
					emit(SUB,be1->place,be2->place,be->place);
			}
				#endif

				$$=be;
      }
   | unary_expr
     {
         unary_expr_node *u;
         binary_expr_node *be;

         be = new binary_expr_node();
         u = (unary_expr_node *)$1;

         be->expr_type=u->expr_type;

			#ifdef ICGEN
			if(sem_err == 0 ){
				 /* Ex4 */
				if(u->offset == NULL ){
					be->place=u->place;
				}else{
					be->place = newtemp();
					emit(R_INDEX_ASSIGN,u->place,u->offset,be->place);
				}

				/* Ex7 */
				be->truelist=u->truelist;
				be->falselist=u->falselist;
			}
			#endif

         $$=be;
      }
   ;

/* Ex4 */

unary_expr
   : unary_expr '[' expr ']'
     {
         unary_expr_node *u1;
         expr_node *e;
			int u1_type;

         u1 = (unary_expr_node *)$1;
         e = (expr_node *)$3;

			u1_type = u1->expr_type->gettype();

         if( (u1_type != ARRAY) && (u1_type != POINTER) ) {
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

			#ifdef ICGEN
			if(sem_err == 0 ){

				sym_tab_entry *se_ptr;
				int sz_of_unit;
				sym_tab_entry *t,*t1;


				sz_of_unit=u1->expr_type->getsize();
				se_ptr = make_const_sym_tab_entry(sz_of_unit);

				t = newtemp();
				emit(MUL,e->place,se_ptr,t);

				if(u1->offset == NULL ){

					if(u1_type == ARRAY ){
						t1 = newtemp();
						emit(ADDR_OF,u1->place,NULL,t1);
						u1->place=t1;
					}

					u1->offset=t;
				}else{
					emit(ADD,t,u1->offset,u1->offset);
				}

			}
         #endif

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

         #ifdef ICGEN
			if(sem_err == 0 ){
				sym_tab_entry *s;

//				s = newtemp();
				s = newtemp1(u->expr_type);

				if(u->offset == NULL ){
					emit(ADDR_OF,u->place,NULL,s);
				}else{
					emit(ADD,u->place,u->offset,s);
				}

				u->place=s;
				u->offset = NULL;

			}
         #endif




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

         #ifdef ICGEN
			if(sem_err == 0 ){
				sym_tab_entry *t1;

				if(u->offset != NULL ){
					t1=newtemp();
					emit(R_INDEX_ASSIGN,u->place,u->offset,t1);
					u->place=t1;
				}

				u->offset=make_const_sym_tab_entry(0);

			}
         #endif

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

         #ifdef ICGEN
			if(sem_err == 0 ){
				sym_tab_entry *t1,*se_ptr;
				 /* No error */
				se_ptr = make_const_sym_tab_entry(fptr->offset);
				u1->expr_type=fptr->type;

				if(u1->offset == NULL){
					t1 = newtemp();
					emit(ADDR_OF,u1->place,NULL,t1);
					u1->place = t1;
					u1->offset = newtemp();
					emit(ASSIGN,se_ptr,NULL,u1->offset);
				}else{
					/* Add to the existing offset */
					emit(ADD,u1->offset,se_ptr,u1->offset);
				}
			}
         #endif

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

         #ifdef ICGEN
			if(sem_err == 0 ){
				sym_tab_entry *se_ptr=NULL;
				sym_tab_entry *t;

				se_ptr = make_const_sym_tab_entry(fptr->offset);
				u1->expr_type=fptr->type;

				/* Fetch content */
				t = newtemp();
				fetch_content(t,u1);

				u1->place=t;
				u1->offset=newtemp();
				emit(ASSIGN,se_ptr,NULL,u1->offset);
			}
         #endif


         $$=u1;
      }
   ;

/* Ex7 */
stmt 
   : IF '(' test_expr ')' M stmt     %prec LOWER_THAN_ELSE
     {
			#ifdef ICGEN
			if(sem_err == 0 ){
				expr_node *e;
				M_node *m;
				stmt_node *s, *s1;

				e = (expr_node *)$3;
				m = (M_node *) $5 ;
				s1 = (stmt_node *)$6;

				s = new stmt_node;

				backpatch(e ->truelist,m->lbl);
				quadlist_merge(s ->nextlist, e->falselist,s1->nextlist);

				$$ = s;
			}
         #endif
      }
   | IF '(' test_expr ')' M stmt ELSE N M stmt
     {
            #ifdef ICGEN
			if(sem_err == 0 ){
				  expr_node *e;
					 M_node *m1, *m2;
					 N_node *n;
					 quad_list *tmp;
					 stmt_node *s, *s1 , *s2;

					 e = (expr_node *)$3;
					 m1 = (M_node *)$5;
					 s1 = (stmt_node *)$6;
					 n  = (N_node *)$8;
					 m2 = (M_node *)$9;
					 s2 = (stmt_node *)$10;

					 tmp = new quad_list;
					 s = new stmt_node;

				  backpatch(e->truelist,m1->lbl);
				  backpatch(e->falselist,m2->lbl);

					quadlist_merge(tmp, n->nextlist,s2->nextlist);
					quadlist_merge(s -> nextlist, s1 -> nextlist,tmp);

					$$ = s;
			}
            #endif

       }
   ;

stmt
   : '{' stmt_list '}'
     {
           #ifdef ICGEN
			if(sem_err == 0 ){
				  stmt_list_node *sl;
				  stmt_node *s;

				  s = new stmt_node;
				  sl= (stmt_list_node *)$2;


				  $$ = s;
			}
           #endif

      }
   ;

stmt_list 
   : stmt_list stmt
     {
           #ifdef ICGEN
			if(sem_err == 0 ){
				stmt_list_node *sl,*sl1;
				stmt_node *s;
				sym_tab_entry *sptr;

				sl1 = (stmt_list_node *)$1;
				s = (stmt_node *)$2;

				sl = new stmt_list_node;


				if(s->nextlist->size() ){
					sptr = newlbl();
					emit(LBL,sptr,NULL,NULL);
					backpatch(s->nextlist,sptr);
				}


				$$ = sl;
			}
           #endif
     }
   | stmt
     {
           #ifdef ICGEN
			if(sem_err == 0 ){
				stmt_list_node *sl;
				stmt_node *s;
				sym_tab_entry *sptr;

				s = (stmt_node *)$1;
				sl = new stmt_list_node;

				if(s->nextlist->size() ){
					sptr = newlbl();
					emit(LBL,sptr,NULL,NULL);
					backpatch(s->nextlist,sptr);
				}

				$$ = sl;
			}
           #endif

     }
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

        #ifdef ICGEN
			if(sem_err == 0 ){
			  e->place = be->place;
			  e->truelist = be->truelist;
			  e->falselist = be -> falselist;
			}
        #endif

        $$=$1;
     }

   ;

or_expr
   : or_expr OR_OP M and_expr
     {
      #ifdef ICGEN
			if(sem_err == 0 ){
			binary_expr_node *be1,*be2;
			binary_expr_node *be;
			M_node *m;

			be1 = (binary_expr_node *)$1;
			be2 = (binary_expr_node *)$4;

			m = (M_node *)$3;

			be = new binary_expr_node();
			be->place=NULL;

			backpatch(be1->falselist, m->lbl);

			quadlist_merge(be->truelist,be1->truelist,be2->truelist);
			be->falselist = be2 ->falselist ;

			$$ = (node *)be ;
			}
      #endif
     }
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
     {
      #ifdef ICGEN
			if(sem_err == 0 ){
			binary_expr_node *be1,*be2;
			binary_expr_node *be;
			M_node *m;

			be1 = (binary_expr_node *)$1;
			be2 = (binary_expr_node *)$4;
			m = (M_node *)$3;

			backpatch(be1->truelist, m->lbl);

			be = new binary_expr_node();
			be->place=NULL;
			be ->truelist = be2->truelist ;
			quadlist_merge(be->falselist , be1->falselist,be2->falselist);

			$$ = (node *)be ;
			}
      #endif
     }

   ;

binary_expr
   : binary_expr LT_OP binary_expr
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				binary_expr_node *be1,*be2;
				binary_expr_node *be;

				be = new binary_expr_node ;
				be1 = (binary_expr_node *)$1;
				be2 = (binary_expr_node *)$3;

				process_relational_op(LT,be,be1,be2);

				$$=be;
			}
         #endif

     }
   | binary_expr GT_OP binary_expr
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				binary_expr_node *be1,*be2;
				binary_expr_node *be;

				be = new binary_expr_node ;
				be1 = (binary_expr_node *)$1;
				be2 = (binary_expr_node *)$3;

				process_relational_op(GT,be,be1,be2);

				$$=be;
			}
			#endif

     }
   | binary_expr GE_OP binary_expr
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				binary_expr_node *be1,*be2;
				binary_expr_node *be;

				be = new binary_expr_node ;
				be1 = (binary_expr_node *)$1;
				be2 = (binary_expr_node *)$3;

				process_relational_op(GE,be,be1,be2);

				$$=be;
			}
         #endif

     }
   | binary_expr LE_OP binary_expr
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				binary_expr_node *be1,*be2;
				binary_expr_node *be;

				be = new binary_expr_node ;
				be1 = (binary_expr_node *)$1;
				be2 = (binary_expr_node *)$3;

				process_relational_op(LE,be,be1,be2);

				$$=be;
			}
         #endif

     }
   | binary_expr EQ_OP binary_expr
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				binary_expr_node *be1,*be2;
				binary_expr_node *be;

				be = new binary_expr_node ;
				be1 = (binary_expr_node *)$1;
				be2 = (binary_expr_node *)$3;

				process_relational_op(EQ,be,be1,be2);

				$$=be;
			}
         #endif
      }
   | binary_expr NE_OP binary_expr
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				binary_expr_node *be1,*be2;
				binary_expr_node *be;

				be = new binary_expr_node ;
				be1 = (binary_expr_node *)$1;
				be2 = (binary_expr_node *)$3;

				process_relational_op(NE,be,be1,be2);

				$$=be;
			}
         #endif
      }

   ;

M 
   :
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				M_node *m;
				sym_tab_entry *sptr;

				m = new M_node ;
				m->quad = next_quad ;


				sptr = newlbl();
				emit(LBL,sptr,NULL,NULL);

				m->lbl = sptr;

				$$ = (node *)m ;
			}
         #endif
     }

   ;
N 
   :
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				N_node *n;

				n = new N_node ;
				n->nextlist = makelist(next_quad) ;
				emit (GOTO,NULL,NULL,NULL);
				$$ = (node *)n ;
			}
         #endif
      }
   ;

/* Ex8 */

stmt
   : WHILE R '(' test_expr ')' M stmt
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				 stmt_node *s1, *s;
				 expr_node *e;
				 M_node *m1,*m2;
				 quad_list *brk_list;


				 m1 = (M_node *)$2;
				 e  = (expr_node *)$4;
				 m2 = (M_node *)$6;
				 s1 = (stmt_node *)$7;

				 s = new stmt_node ;

				 brk_list = new quad_list;

				 get_brk_list(brk_list);

				 backpatch(s1->nextlist,m1->lbl);
				 backpatch(e->truelist,m2->lbl);

				 quadlist_merge(s -> nextlist, e -> falselist,brk_list) ;
				 emit (GOTO,m1->lbl,NULL,NULL);

				 $$= s;
			}
         #endif
     }
   | BREAK ';'
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				stmt_node *s;
				brk_lbl *b;

				if(brk_lbl_stk.size() == 0 ){
					printf("break not in while/switch case \n");
					exit(1);
				}

				s = new stmt_node;

				b = new brk_lbl(next_quad);

				brk_lbl_stk.push(b);

				emit (GOTO,NULL,NULL,NULL);
				$$ = s ;
			}
         #endif
     }

   ;

R
   :
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				M_node *m;
				brk_lbl *b;
				sym_tab_entry *sptr;

				m = new M_node ;
				m->quad = next_quad ;


				sptr = newlbl();
				emit(LBL,sptr,NULL,NULL);

				m->lbl = sptr;

				b = new brk_lbl(-1);
				brk_lbl_stk.push(b);

				$$ = (node *)m ;
			}
         #endif
      }
   ;

/* Ex9 */

stmt
   : SWITCH Q '(' expr ')' N '{' case_stmt_list '}'
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				stmt_node *s;
				case_stmt_list_node *c;
				N_node *n;
				expr_node *e;
				sym_tab_entry *sptr;
				quad_list *tmp;
				quad_list *q;
				quad_list *q1;

				e = (expr_node *)$4;
				n = (N_node *)$6;
				c = (case_stmt_list_node *)$8;

				s = new stmt_node;

				q = makelist(next_quad) ;

				emit (GOTO,NULL,NULL,NULL);

				sptr = newlbl();
				emit(LBL,sptr,NULL,NULL);

				backpatch(n->nextlist,sptr);

				emit_n_way_branch(e->place);

				q1 = new quad_list;
				tmp = new quad_list;

				get_brk_list(q1);
				quadlist_merge(s->nextlist , q1,q) ;


				$$ = s;
			}
         #endif
      }
   ;

case_stmt 
   : CASE CONSTANT ':' M stmt_list
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
			  stmt_list_node *sl;
			  M_node *m;
			  CONSTANT_node *c;
			  case_stmt_node *cs ;
			  value_label *p;

			  c = (CONSTANT_node *)$2;
			  m = (M_node *)$4;
			  sl = (stmt_list_node *)$5;

			  p = new value_label(c,m->lbl);
			  vl.push(p);

			  cs = new case_stmt_node ;

			  $$ = (node *)cs ;
			}
         #endif
      }
   | DEFAULT ':' M stmt_list
       {
         #ifdef ICGEN
			if(sem_err == 0 ){
				  stmt_list_node *sl;
					M_node *m;
					CONSTANT_node *c;
					case_stmt_node *cs ;
					value_label *p;

					c = new CONSTANT_node("0");;
					m = (M_node *)$3;
					sl = (stmt_list_node *)$4;

					p = new value_label(c,m->lbl);
					p ->default_case = 1;
					vl.push(p);

					cs = new case_stmt_node ;


					$$ = (node *)cs ;
			}
         #endif
         }
   ;

case_stmt_list
   : case_stmt_list case_stmt
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
				case_stmt_list_node *cl,*cl1;
				case_stmt_node *c;

				cl1 = (case_stmt_list_node *)$1;
				c = (case_stmt_node *)$2;


				cl = new case_stmt_list_node;

				$$ = cl;
			}
         #endif
     }

   | case_stmt
     {
         #ifdef ICGEN
			if(sem_err == 0 ){
			  case_stmt_list_node *cl;
			  case_stmt_node *cs;

			  cs = (case_stmt_node *)$1;

			  cl = new case_stmt_list_node;
			  $$ = cl;
			}
         #endif
     }

   ;

Q 
   :
   {
         #ifdef ICGEN
			if(sem_err == 0 ){
			value_label *marker;
			brk_lbl *b;

			marker = new value_label(NULL,NULL);
			vl.push(marker);

			b = new brk_lbl(-1);
			brk_lbl_stk.push(b);
			}
         #endif

   }

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
         #ifdef ICGEN
			if(sem_err == 0 ){
				sym_tab_entry *t,*s;

				s = make_const_sym_tab_entry(0);
				emit(CALL,u1->place,s,NULL);

				if(f->ret_type->gettype() != VOID_TYPE ){
					t = newtemp1(f->ret_type);
					 emit(RETRIEVE,t,NULL,NULL);
					u->place = t ;
				}

			}
         #endif
           $$=u;
     }
   | unary_expr '(' args_list ')'
     {
          unary_expr_node *u,*u1;
          args_list_node *a1;
          function *f;
          int no_of_args;
          int i;

          u1 = (unary_expr_node *)$1;
          a1 = (args_list_node *)$3;

          u = new unary_expr_node();

          if(u1->expr_type->gettype() == FUNCTION ){
                 f = (function *)u1->expr_type;
               no_of_args = f -> no_of_args;

               /* Diluting the checks for the args of function */
					#if 0 
          		int err_counter=0;
          		type_expr *decl_arg_type,*formal_arg_type;
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
					#endif
               u->expr_type = f->ret_type;
          } else {
               printf("Trying to invoke a non Function in line %d \n",line_no);
               u->expr_type = new type_error();
          }

			 #ifdef ICGEN
			if(sem_err == 0 ){
				 sym_tab_entry *s;
				 sym_tab_entry *t,*t1;
				 int sz,total_sz;


				no_of_args = a1 -> no_of_args;

				total_sz=0;
				for(i=(no_of_args-1);i>=0;i--){
					s=a1->get_arg_sym_tab_entry(i);
					emit(PARAM,s,NULL,NULL);
					sz=s->type->getsize();
					/* Make it rounded by 4 */
					if((sz%4) != 0 ){
						sz=((sz/4)+1)*4;
					}
					total_sz +=sz;
				}
				t1=make_const_sym_tab_entry(total_sz);
				emit(CALL,u1->place,t1,NULL);

				f = (function *)u1->place->type;

				if(f->ret_type->gettype() != VOID_TYPE ){
					t = newtemp1(f->ret_type);
					emit(RETRIEVE,t,NULL,NULL);
					u->place = t ;
				}

			}
			 #endif
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

			 #ifdef ICGEN
			if(sem_err == 0 ){
				 a->add_arg_sym_tab_entry(e->place);
			}
			 #endif

          $$ = (node *)a; 

     }
   | expr 
     {
          expr_node *e;
          args_list_node *a;

          e = (expr_node *)$1;

          a = new args_list_node(e -> expr_type );

			 #ifdef ICGEN
			if(sem_err == 0 ){
				 a->add_arg_sym_tab_entry(e->place);
			}
			 #endif

          $$ = (node *)a; 

     }
   ;
          
stmt 
   : RET ';'
     {
			#ifdef ICGEN
			if(sem_err == 0 ){
				stmt_node *s;

				s = new stmt_node;
         	return_list.push_back(next_quad);
         	emit (GOTO,NULL,NULL,NULL);

				$$=s;
			}
			#endif
     }
   | RET expr ';'
     {
			#ifdef ICGEN
			if(sem_err == 0 ){
				stmt_node *s;
				expr_node *e;

				e = (expr_node *)$2;

				s = new stmt_node;
				emit(RETURN,e->place,NULL,NULL);
         	return_list.push_back(next_quad);
         	emit (GOTO,NULL,NULL,NULL);

				$$=s;
			}
			#endif
     }
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

			  #ifdef ICGEN
			if(sem_err == 0 ){
				  sym_tab_entry *sptr;
				  curr_sym_tab_ptr->set_param_space();

				  m = new M_node ;
				  m->quad = next_quad ;

				 if((sptr=lookup(fptr->name)) == NULL ){
						printf("Suspicious \n");
						exit(0);
				  }

				  sptr->sym_tab_ptr = curr_sym_tab_ptr ;


				  emit(PROC_BEGIN,sptr,NULL,NULL);
				  curr_proc_entry = sptr ;
			}
			  #endif


         $$ = (node *)m ;
     } 
   ;

%%
