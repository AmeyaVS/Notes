#include "semantic_analysis.h"
#include "ic_gen.h"
#include "target_code_gen.h"

/*!
	@file target_code_gen.cc
	@brief Target Code Generation Implementation
*/

//! Indicates if TAC is valid (defined in ic_gen.cc)
extern int tac_not_valid; /* defined in ic_gen */

//! Pointer to Global Symbol Table (defined in parser)
extern sym_tab *global_sym_tab_ptr; /* defined in parser */

//! Pointer to String Literal Table (defined in parser)
extern sym_tab *str_lit_sym_tab_ptr ; /* defined in parser */

//! The Final Quads pointer used for Target Code generation (defined in ic_gen.cc )
extern vector <quad *> *final_quad_arr_ptr; /* defined in ic_gen */



/* x86_as_quad.cc begin */

//! Running assembly instruction number
int as_next_quad;

//! Assembly instructions Vector
vector <as_quad *>as_quad_arr;

//! Emit an Assembly Instruction
void emit_as (x86_as_oprtr mne, struct op_type op_ty, x86_as_operand *op1_ptr, x86_as_operand *op2_ptr,string cmt="" )
{
	as_quad *as_quad_ptr;

	as_quad_ptr = new  as_quad(mne, op_ty, op1_ptr, op2_ptr,cmt );
	as_quad_arr.push_back(as_quad_ptr);
	as_next_quad ++ ;
	printf("%s \n",as_quad_ptr->as_string().c_str() );
}

//! x86 Assembly Operand Constructor, given the Identifier
x86_as_operand:: x86_as_operand(sym_tab_entry *sptr)
{
	sym_tab_entry_ptr = sptr;
	reg_ptr = NULL;
}

//! x86 Assembly Operand Constructor, given the Register
x86_as_operand:: x86_as_operand(reg_entry *eptr)
{
	reg_ptr = eptr;
	sym_tab_entry_ptr = NULL;
}

//! Get the Name of the x86 Assembly Operand 
string
x86_as_operand :: name()
{
	return(sym_tab_entry_ptr->get_name());
}

//! Make a String of the x86 Assembly Operand 
string
x86_as_operand :: make_operand(int sz)
{
	string ar;
	string dollar("$");
	string underscore("_");
	char tmp[50];
	string bp("(%ebp)");
	int my_type;
	int off;

	/* Register */
	if(reg_ptr){
		ar=reg_ptr->get_name(sz);
		return(ar);
	}

	if(sym_tab_entry_ptr == NULL ){
		printf("unrecovarable error \n");
		exit(0);
	}

	/* It is a Memory or Literal */
	my_type = sym_tab_entry_ptr->type->gettype() ;

	/* Deal with Constants and labels differently */
	if(my_type == CONSTAN ){ 
		ar= dollar + sym_tab_entry_ptr->get_name();
		return(ar);
   }else if (my_type == LABEL){
		ar = sym_tab_entry_ptr->get_name();
		return(ar);
   }
	
	
	/* Global */
	if(sym_tab_entry_ptr-> sym_tab_ptr -> name == "GLOBALS" ){
		ar = underscore+sym_tab_entry_ptr->get_name();
		return(ar);
   }

	/* Local */

	if(sym_tab_entry_ptr->offset >= 
   	sym_tab_entry_ptr->sym_tab_ptr -> param_space ){ 
		/* Local variable is a -ve offset */
		off = -(sym_tab_entry_ptr->offset - sym_tab_entry_ptr->sym_tab_ptr -> param_space)-sym_tab_entry_ptr->get_size();
	}else{
		/* Actual parameter */
		off = sym_tab_entry_ptr->offset + 8 ;
	}

	sprintf(tmp,"%d",off);
	ar = tmp+bp;
	return(ar);
}

//! Get the Assembly Instruction as a String for printing
string
as_quad::as_string()
{
	string mne;
	string src;
	string dst;
	string to_print;
	string cmt;


	switch(mnemonic)
	{
		case AS_MOV :
						  src = op1 -> make_operand(as_oprtr_type.sz);
						  dst = op2 -> make_operand(as_oprtr_type.sz);
		              mne = "mov" + get_suffix();
						  if(as_oprtr_type.op2_mode == INDIRECT ){
						  	dst = op2 -> make_operand(4);
						  	to_print = "\t" + mne + " " +  src + "," + 
							           "(" + dst + ")";
						  }else if(as_oprtr_type.op1_mode == INDIRECT ){
						  	src = op1 -> make_operand(4);
						  	to_print = "\t" + mne + " " +  "(" + src + ")" + "," + dst ;
						  }else{
						  	to_print = "\t" + mne + " " +  src + "," + dst;
						  }
						  break;
		case AS_ADD :
						  src = op1 -> make_operand(as_oprtr_type.sz);
						  dst = op2 -> make_operand(as_oprtr_type.sz);
		              mne = "add" + get_suffix();
						  to_print = "\t" + mne + " " +  src + "," + dst;
						  break;
		case AS_MUL :
						  src = op1 -> make_operand(as_oprtr_type.sz);
						  dst = op2 -> make_operand(as_oprtr_type.sz);
		              mne = "imul" + get_suffix();
						  to_print = "\t" + mne + " " +  src  + "," + dst;
						  break;
		case AS_DIV :
						  src = op1 -> make_operand(as_oprtr_type.sz);
						  dst = op2 -> make_operand(as_oprtr_type.sz);
		              mne = "idiv" + get_suffix();
						  to_print = "\t" + mne + " " +  src  + "," + dst;
						  break;
		case AS_SUB :
						  src = op1 -> make_operand(as_oprtr_type.sz);
						  dst = op2 -> make_operand(as_oprtr_type.sz);
		              mne = "sub" + get_suffix();
						  to_print = "\t" + mne + " " +  src + "," + dst;
						  break;
		case AS_NEG :
						  src = op1 -> make_operand(as_oprtr_type.sz);
		              mne = "neg" + get_suffix();
						  to_print = "\t" + mne + " " +  src ;
						  break;
		case AS_JMP :
						  dst = op1 -> make_operand(as_oprtr_type.sz);
		              mne = "jmp" ;
						  to_print = "\t" + mne + " " + dst ;
						  break;
		case AS_ALIGN :
						  to_print = "\t.align 4" ;
						  break;
		case AS_PUSH :
						  src = op1 -> make_operand(as_oprtr_type.sz);
		              mne = "pushl" ;
						  to_print = "\t" + mne + " " + src ;
						  break;
		case AS_GLOBAL :
						  to_print = ".globl _" +  op1->name() ;
						  break;
		case AS_LABEL :
						  to_print = op1->name() + ":" ;
						  break;
		case AS_POP :
						  mne = "popl";
						  src = op1 -> make_operand(as_oprtr_type.sz);
						  to_print = "\t" + mne + " " + src ;
						  break;
		case AS_RET :
						  mne = "ret";
						  to_print = "\t" + mne ;
						  break;
		case AS_CMP :
						  src = op1 -> make_operand(as_oprtr_type.sz);
						  dst = op2 -> make_operand(as_oprtr_type.sz);
		              mne = "cmp" ;
						  to_print = "\t" + mne + get_suffix() + " " +  src + "," + dst;
						  break;
		case AS_JL :
						  mne = "jl";
						  dst = op1->make_operand(as_oprtr_type.sz);
						  to_print = "\t" + mne + " " + dst ;
						  break;
		case AS_JG :
						  mne = "jg";
						  dst = op1->make_operand(as_oprtr_type.sz);
						  to_print = "\t" + mne + " " + dst ;
						  break;
		case AS_JLE :
						  mne = "jle";
						  dst = op1->make_operand(as_oprtr_type.sz);
						  to_print = "\t" + mne + " " + dst ;
						  break;
		case AS_JGE :
						  mne = "jge";
						  dst = op1->make_operand(as_oprtr_type.sz);
						  to_print = "\t" + mne + " " + dst ;
						  break;
		case AS_JE :
						  mne = "je";
						  dst = op1->make_operand(as_oprtr_type.sz);
						  to_print = "\t" + mne + " " + dst ;
						  break;
		case AS_JNE :
						  mne = "jne";
						  dst = op1->make_operand(as_oprtr_type.sz);
						  to_print = "\t" + mne + " " + dst ;
						  break;
		case AS_LEA :
						  mne = "leal";
						  src = op1->make_operand(as_oprtr_type.sz);
						  dst = op2->make_operand(as_oprtr_type.sz);
						  to_print = "\t" + mne + " " + src + "," + dst ;
						  break;
		case AS_CALL :
						  mne = "call";
						  src = op1->name();
						  to_print = "\t" + mne + " _" + src ;
						  break;
		case AS_REP :
						  mne = "rep";
						  to_print = "\t" + mne ;
						  break;
		case AS_CLD :
						  mne = "cld";
						  to_print = "\t" + mne ;
						  break;
		case AS_CLTD :
						  mne = "cltd";
						  to_print = "\t" + mne ;
						  break;
		case AS_MOVS :
						  	mne = "movs" + get_suffix();
						  	to_print = "\t" + mne ;
							break;
		case AS_XOR :
						  	mne = "xorl" ;
							src = op1->make_operand(as_oprtr_type.sz);
							dst = op2->make_operand(as_oprtr_type.sz);
						  	to_print = "\t" + mne + " " + src + "," + dst ;
							break;
		case AS_MOVSBL :
						  	mne = "movsbl" ;
							src = op1->make_operand(as_oprtr_type.sz);
							dst = op2->make_operand(as_oprtr_type.sz);
						  	to_print = "\t" + mne + " " + src + "," + dst ;
							break;
		default:
					break;
	}

	if(comment.size() >0 ){
		cmt="/*"+comment+"*/\n";
		to_print=cmt+to_print;
	}
		


	return(to_print);
}

//! Get the Suffix of the x86 Assembly instruction (e.g. addw/addl/addb )
string
as_quad::get_suffix()
{
	string ret;

	if(as_oprtr_type.sz == 1){
		ret="b";
	}else if(as_oprtr_type.sz == 2){
		ret = "w";
	}else if(as_oprtr_type.sz == 4){
		ret = "l";
	}else{
		ret = "q" ;
	}
	return(ret);
}

/* x86_as_quad.cc begin */


/* x86_code_gen.cc begin */

//! Makes an entry to literal table (defined in parser)
extern sym_tab_entry *make_const_sym_tab_entry(int val);


//! The assembly operand for register ax
x86_as_operand *as_reg_a;
//! The assembly operand for register bx
x86_as_operand *as_reg_b;
//! The assembly operand for register cx
x86_as_operand *as_reg_c;
//! The assembly operand for register dx
x86_as_operand *as_reg_d;
//! The assembly operand for register bp
x86_as_operand *as_reg_bp;
//! The assembly operand for register sp
x86_as_operand *as_reg_sp;
//! The assembly operand for register si
x86_as_operand *as_reg_si;
//! The assembly operand for register di
x86_as_operand *as_reg_di;



//! The register entry for ax
reg_entry *reg_a;
//! The register entry for bx
reg_entry *reg_b;
//! The register entry for cx
reg_entry *reg_c;
//! The register entry for dx
reg_entry *reg_d;
//! The register entry for bp
reg_entry *reg_bp;
//! The register entry for sp
reg_entry *reg_sp;
//! The register entry for si
reg_entry *reg_si;
//! The register entry for di
reg_entry *reg_di;



static void init_regs();
//static void print_as_bound(int st,int end);

int init_before_tc_gen();

//! Entry point for Target Code Generation
void generate_target_code()
{
	int n,i;
	int start,end;
	int ret;
	string quad_str;
	x86_quad *quad_ptr;

	ret=init_before_tc_gen();
	if(ret == 0 ){
		return;
	}

	n = (*final_quad_arr_ptr).size();

	for(i=0;i<n;i++) {
		start = as_quad_arr.size();
		quad_str = (*final_quad_arr_ptr)[i] -> as_string();
		quad_ptr = (x86_quad *)(*final_quad_arr_ptr)[i] ;

		/* calculate the local space */
		if(quad_ptr->op == PROC_BEGIN ){
			quad_ptr->arg1->sym_tab_ptr->calculate_local_space();
		}

		printf("/* %s   */ \n",quad_str.c_str() );
		quad_ptr -> translate();
		end = as_quad_arr.size();
	//	print_as_bound(start,end);
	}

}

//! Function to initialize Data Sturtcures before Target Code Generation
int init_before_tc_gen()
{
	x86_sym_tab *g_sym_ptr;
	x86_sym_tab *str_lit_ptr;

	if(tac_not_valid){
		return(0);
	}
	g_sym_ptr = (x86_sym_tab *)global_sym_tab_ptr;
	str_lit_ptr = (x86_sym_tab *)str_lit_sym_tab_ptr;

	/* Handle declarations */
	g_sym_ptr->gen_decl();

	/* It is all text Now */
	g_sym_ptr->gen_text_seg();

	/* Handle String literals */
	str_lit_ptr->gen_str_lits();

	/* Initialize register entries to be used in code generation */
	init_regs();

	return(1);


}

/*
static void print_as_bound(int st,int end)
{
	int i;
	string as;

	for(i=st;i<end;i++) {
		as = as_quad_arr[i] ->as_string();
		printf("%s \n",as.c_str() );
	}
}
*/


//! Function to Print assembly instructions
void print_as_quads()
{
	int i;
	string as;
	int end;

	end = as_quad_arr.size();

	for(i=0;i<end;i++) {
		as = as_quad_arr[i] ->as_string();
		printf("%s \n",as.c_str() );
	}
}

//! Fetch x86 operand given the symbol table entry
x86_as_operand *
x86_quad :: fetch_x86_operand(sym_tab_entry *arg2)
{
	x86_as_operand *z=NULL;
	x86_extension *e;

	z=NULL;
	e=(x86_extension *)arg2->extension;

	if( e != NULL ){
		z=e->as_operand_ptr;
	}

	if( z == NULL ){
		z = new x86_as_operand(arg2);
		e = new x86_extension(z);
		arg2->extension=(void *)e;
	}
	return(z);
}


//! Translate a quad into a series of x86 Assembly instrutions
void 
x86_quad ::translate()
{
	x86_as_operand *as1,*as2,*as3;
	int sz=4;
	int sz1,sz2,sz3;
	struct op_type op_typ;

	op_typ.sz = sz;
	op_typ.op1_mode = DIRECT ;
	op_typ.op2_mode = DIRECT ;

	as1=as2=as3=NULL;

	if(arg1){
		as1 = fetch_x86_operand(arg1);
		sz1=arg1->type->getsize();
	}

	if(arg2){
		as2 = fetch_x86_operand(arg2);
		sz2=arg2->type->getsize();
	}

	if(res){
		as3 = fetch_x86_operand(res);
		sz3=res->type->getsize();
	}


	switch(op)
	{
		case ASSIGN : 
					/* 
						if arg1 is literal
							Generate  mov arg1,res

						else
							Generate mov arg1,eax
							Generate mov eax, res

					*/

					op_typ.sz = sz3;

					if(arg1->type->gettype() == CONSTAN ){
						emit_as(AS_MOV,op_typ,as1,as3);
					}else{
						emit_as(AS_MOV,op_typ,as1,as_reg_a);
						emit_as(AS_MOV,op_typ,as_reg_a,as3);
					}
					break;
		case ADD:
						/*
						MOV arg2,eax
						ADD arg1,eax
						MOV eax,res
						*/

					if(sz2 < 4 ){ /* Size Conversion */
						op_typ.sz = 4;
						emit_as(AS_MOVSBL,op_typ,as1,as_reg_a);
					}else{
						op_typ.sz = sz1;
						emit_as(AS_MOV,op_typ,as1,as_reg_a);
					}

					op_typ.sz = sz2;
					emit_as(AS_ADD,op_typ,as2,as_reg_a);

					op_typ.sz = sz3;
					emit_as(AS_MOV,op_typ,as_reg_a,as3);
					break;
		case MUL:
					/*
						MOV arg1,eax
						MOV arg2,ecx
						IMULL ecx
						MOV eax,res
					*/

					if(sz1 < 4 ){ /* Size Conversion */
						op_typ.sz = 4;
						emit_as(AS_MOVSBL,op_typ,as1,as_reg_a);
					}else{
						op_typ.sz = sz1;
						emit_as(AS_MOV,op_typ,as1,as_reg_a);
					}

					if(sz2 < 4 ){ /* Size Conversion */
						op_typ.sz = 4;
						emit_as(AS_MOVSBL,op_typ,as2,as_reg_c);
					}else{
						op_typ.sz = sz2;
						emit_as(AS_MOV,op_typ,as2,as_reg_c);
					}

					op_typ.sz = 4;
					emit_as(AS_MUL,op_typ,as_reg_c,as_reg_a);

					op_typ.sz = sz3;
					emit_as(AS_MOV,op_typ,as_reg_a,as3);

					break;
		case DIV :
					/*
						movl arg1,%eax
						cltd
						idivl arg2
						movl %eax,res
					*/

					if(sz1 < 4 ){ /* Size Conversion */
						op_typ.sz = 4;
						emit_as(AS_MOVSBL,op_typ,as1,as_reg_a);
					}else{
						op_typ.sz = sz1;
						emit_as(AS_MOV,op_typ,as1,as_reg_a);
					}

					emit_as(AS_CLTD,op_typ,NULL,NULL);

					op_typ.sz = sz2;
					emit_as(AS_DIV,op_typ,as2,as_reg_a);

					op_typ.sz = sz3;
					emit_as(AS_MOV,op_typ,as_reg_a,as3);

					break;
		case SUB:
						/*
						MOV arg1,eax
						SUB arg2,eax
						MOV eax,res
						*/

					if(sz1 < 4 ){ /* Size Conversion */
						op_typ.sz = 4;
						emit_as(AS_MOVSBL,op_typ,as1,as_reg_a);
					}else{
						op_typ.sz = sz1;
						emit_as(AS_MOV,op_typ,as1,as_reg_a);
					}

					op_typ.sz = sz2;
					emit_as(AS_SUB,op_typ,as2,as_reg_a);

					op_typ.sz = sz3;
					emit_as(AS_MOV,op_typ,as_reg_a,as3);

					break;
		case UMINUS :
						/*
						MOV arg1,eax
						NEG eax
						MOV eax,res
						*/
					op_typ.sz = sz1;
					emit_as(AS_MOV,op_typ,as1,as_reg_a);
					emit_as(AS_NEG,op_typ,as_reg_a,NULL);

					op_typ.sz = sz3;
					emit_as(AS_MOV,op_typ,as_reg_a,as3);

					break;
		case GOTO:
					/* JMP label */
					emit_as(AS_JMP,op_typ,as1,NULL);
					break;
		case PROC_BEGIN:
					/* Function entry */
					emit_as(AS_ALIGN,op_typ,NULL,NULL);
					emit_as(AS_GLOBAL,op_typ,as1,NULL);
					as2=new x86_as_operand(arg1->sym_tab_ptr->entry_lbl);
					emit_as(AS_LABEL,op_typ,as2,NULL);
					emit_as(AS_PUSH,op_typ,as_reg_bp,NULL);
					emit_as(AS_MOV,op_typ,as_reg_sp,as_reg_bp);
					as3 = new x86_as_operand(arg1->sym_tab_ptr->local_space);
					emit_as(AS_SUB,op_typ,as3,as_reg_sp);
					break;
		case PROC_END :
					emit_as(AS_MOV,op_typ,as_reg_bp,as_reg_sp);
					emit_as(AS_POP,op_typ,as_reg_bp,NULL);
					emit_as(AS_RET,op_typ,NULL,NULL);
					break;
		case RETURN :
					op_typ.sz = sz1;
					emit_as(AS_MOV,op_typ,as1,as_reg_a);
					break;
		case LBL :
					emit_as(AS_ALIGN,op_typ,NULL,NULL);
					emit_as(AS_LABEL,op_typ,as1,NULL);
					break;
		case LT :
					comp_gen(as1,as2,op_typ,sz1,sz2);
					emit_as(AS_JL,op_typ,as3,NULL);
					break;
    	case GT :
					comp_gen(as1,as2,op_typ,sz1,sz2);
					emit_as(AS_JG,op_typ,as3,NULL);
					break;
    	case LE :
					comp_gen(as1,as2,op_typ,sz1,sz2);
					emit_as(AS_JLE,op_typ,as3,NULL);
					break;
    	case GE :
					comp_gen(as1,as2,op_typ,sz1,sz2);
					emit_as(AS_JGE,op_typ,as3,NULL);
					break;
    	case EQ :
					comp_gen(as1,as2,op_typ,sz1,sz2);
					emit_as(AS_JE,op_typ,as3,NULL);
					break;
    	case NE :
					comp_gen(as1,as2,op_typ,sz1,sz2);
					emit_as(AS_JNE,op_typ,as3,NULL);
					break;
		
		case L_INDEX_ASSIGN :
					/* 
						(1) mov arg1,eax
						(2) add arg2,%eax
						(3) mov res,%ecx
						(4) mov %ecx,(%eax)
					*/

					op_typ.sz = sz3;
					emit_as(AS_MOV,op_typ,as1,as_reg_a);

					op_typ.sz = sz1;
					emit_as(AS_ADD,op_typ,as2,as_reg_a);

					op_typ.sz = sz2;
					emit_as(AS_MOV,op_typ,as3,as_reg_c);

					op_typ.op2_mode = INDIRECT;
					op_typ.sz = sz2; // =4; /* rags -hack */
					emit_as(AS_MOV,op_typ,as_reg_c,as_reg_a);

					break;
		case R_INDEX_ASSIGN :
					/* 
						(1) mov arg1,eax
						(2) add arg2,eax
						(3) mov (%eax),%ecx
						(4) mov %ecx,res
					*/

					op_typ.sz = sz1;
					emit_as(AS_MOV,op_typ,as1,as_reg_a);
					op_typ.sz = sz2;
					emit_as(AS_ADD,op_typ,as2,as_reg_a);

					op_typ.sz = sz;
					op_typ.op1_mode = INDIRECT;
					emit_as(AS_MOV,op_typ,as_reg_a,as_reg_c);

					op_typ.sz = sz3;
					op_typ.op1_mode = DIRECT;
					emit_as(AS_MOV,op_typ,as_reg_c,as3);
					break;
		case ADDR_OF :
					 /* 
						leal arg1,eax
						mov  eax,res
					*/
					emit_as(AS_LEA,op_typ,as1,as_reg_a);
					op_typ.sz = sz3;
					emit_as(AS_MOV,op_typ,as_reg_a,as3);
					break;

		case PARAM  :
					{
						int my_sz;
						sym_tab_entry *sptr;
						/*
							mov arg1,eax
							pushl eax
						*/
						my_sz=arg1->get_size();
						if( my_sz <=4 ){
							if(my_sz == 4){
								emit_as(AS_MOV,op_typ,as1,as_reg_a);
							}else{
								op_typ.sz=4;
								emit_as(AS_MOVSBL,op_typ,as1,as_reg_a);
							}
							emit_as(AS_PUSH,op_typ,as_reg_a,NULL);
						}else{
							/* We should loop and copy the entire object */
							/*
								mov esp,edi
								lea res,esi
								mov sz,ecx
								cld
								rep 
								movsb
								addl sz,esp
							*/
							if(my_sz%4){
								my_sz= ((my_sz/4)+1)*4;
							}
							sptr=make_const_sym_tab_entry(my_sz);
							as2= new x86_as_operand(sptr);
							emit_as(AS_SUB,op_typ,as2,as_reg_sp);

							emit_as(AS_MOV,op_typ,as_reg_sp,as_reg_di);
							emit_as(AS_LEA,op_typ,as1,as_reg_si);
							emit_as(AS_MOV,op_typ,as2,as_reg_c);
							emit_as(AS_CLD,op_typ,NULL,NULL);
							emit_as(AS_REP,op_typ,NULL,NULL);
							op_typ.sz=1;
							emit_as(AS_MOVS,op_typ,NULL,NULL);
						}
					}
					break;
		case CALL :
					/*
						call arg1
						addl param_space,esp (for poping the push of args)
					*/
					emit_as(AS_CALL,op_typ,as1,NULL);
					emit_as(AS_ADD,op_typ,as2,as_reg_sp);
					break;
		case RETRIEVE :
					/*
						mov eax,arg1
					*/
					op_typ.sz = sz1;
					emit_as(AS_MOV,op_typ,as_reg_a,as1);
					break;
		default:
					break;
	}
}
	
//! Utility to generate comparison assembly statement
void
x86_quad::comp_gen(x86_as_operand *as1, x86_as_operand *as2 , struct op_type op_typ,int sz1,int sz2)
{
	int tmp;
	/*
		MOV arg2,eax
		CMP eax,arg1
	*/
	op_typ.sz = sz2;
	emit_as(AS_MOV,op_typ,as2,as_reg_a);
	tmp = (sz1 <= sz2)?sz1:sz2;
	op_typ.sz = tmp;
	emit_as(AS_CMP,op_typ,as_reg_a,as1);
}


//! Generate Directive for Text Segment
void 
x86_sym_tab::gen_text_seg()
{
	printf(".text \n");
}

//! Generate a Series of String Literals from the string Literal Table
void 
x86_sym_tab::gen_str_lits()
{  
   sym_tab_entry *sptr;
   stab :: iterator iter;
   pair <const char *,sym_tab_entry *> res;
	string directive(".ascii");
	string terminator("\\0");
	int i;

   iter = sym_table.begin();

   while( iter != sym_table.end() )
   {
      res = (*iter);
      sptr = res.second;
		printf("%s :\n",sptr->name.c_str());

		i=sptr->literal.rfind("\"");
		sptr->literal.insert(i,terminator);

		printf("%s %s \n",directive.c_str(),sptr->literal.c_str());
      iter ++ ;
   }
   printf("\n");
   return;
}

//! Generate a Series of Declarations
void 
x86_sym_tab::gen_decl()
{  
   sym_tab_entry *sptr;
   stab :: iterator iter;
   pair <const char *,sym_tab_entry *> res;
	string directive(".comm  ");
	int sz;

   iter = sym_table.begin();

   while( iter != sym_table.end() )
   {
      res = (*iter);
      sptr = res.second;
		if(sptr ->type != NULL ){
			sz=sptr->type->getsize();
			if(sptr->type->gettype() != FUNCTION ){
				printf("%s _%s,%d\n",directive.c_str(),sptr->name.c_str(),sz);
   		}
		}
      iter ++ ;
   }
   printf("\n");
   return;
}


//! Get Register name in the Proper Format, given the width (e.g. eax/ax/al/ah)
string
reg_entry::get_name(int wid)
{
	string name,suffix,prefix,reg_name;
	if(wid == 1){
		suffix="l";
	}else if(wid == 2){
		suffix = "x";
	}else if(wid == 4 ){
		prefix = "e" ;
		suffix = "x" ;
	}

	switch(reg){
		case REG_A :
						reg_name="a";
						break;
		case REG_B :
						reg_name="b";
						break;
		case REG_C :
						reg_name="c";
						break;
		case REG_D :
						reg_name="d";
						break;
		case REG_BP :
						prefix="e";
						suffix="";
						reg_name="bp";
						break;
		case REG_SP :
						prefix="e";
						suffix="";
						reg_name="sp";
						break;
		case REG_SI :
						prefix="e";
						suffix="";
						reg_name="si";
						break;
		case REG_DI :
						prefix="e";
						suffix="";
						reg_name="di";
						break;
		default :
					break;
	}

	name = "%" + prefix + reg_name + suffix ;
	return(name);
}

//! Initialize the Register related Data Structures
static void init_regs()
{
	reg_a = new reg_entry(REG_A,4);
	as_reg_a = new x86_as_operand(reg_a);
	reg_a->as_operand_ptr=as_reg_a;

	reg_b = new reg_entry(REG_B,4);
	as_reg_b = new x86_as_operand(reg_b);
	reg_b->as_operand_ptr=as_reg_b;

	reg_c = new reg_entry(REG_C,4);
	as_reg_c = new x86_as_operand(reg_c);
	reg_c->as_operand_ptr=as_reg_c;

	reg_d = new reg_entry(REG_D,4);
	as_reg_d = new x86_as_operand(reg_d);
	reg_d->as_operand_ptr=as_reg_d;

	reg_bp = new reg_entry(REG_BP,4);
	as_reg_bp = new x86_as_operand(reg_bp);
	reg_bp->as_operand_ptr=as_reg_bp;

	reg_sp = new reg_entry(REG_SP,4);
	as_reg_sp = new x86_as_operand(reg_sp);
	reg_sp->as_operand_ptr=as_reg_sp;

	reg_si = new reg_entry(REG_SI,4);
	as_reg_si = new x86_as_operand(reg_si);
	reg_si->as_operand_ptr=as_reg_si;

	reg_di = new reg_entry(REG_DI,4);
	as_reg_di = new x86_as_operand(reg_di);
	reg_di->as_operand_ptr=as_reg_di;

}

/* x86_code_gen.cc begin */
