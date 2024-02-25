#ifndef TARGET_CODE_GEN_H
#define TARGET_CODE_GEN_H

#include "semantic_analysis.h"

/*!
	@file target_code_gen.h
	@brief Target Code Generation Header
*/


//! An x86 register
enum x86_regs
{
	REG_A,
	REG_B,
	REG_C,
	REG_D,
	REG_BP,
	REG_SP,
	REG_SI,
	REG_DI
};

class x86_as_operand;

//! Represents a Register
class reg_entry
{
	public:
	//! The Register name
	x86_regs reg;
	//! Width of the Register
	int width;
	//! TBD
	int used;
	//! Constructor, given the register name and its width
	reg_entry(x86_regs re,int wid)
	{
		reg = re;
		width = wid;
		used = 0;
	}
	string get_name(int wid);
	//! The register in a ready to use Assembly Operand Form
	x86_as_operand *as_operand_ptr;
};
	
//! An x86 Operator
enum x86_as_oprtr
{
	AS_ADD,
	AS_MOV,
	AS_MUL,
	AS_DIV,
	AS_SUB,
	AS_NEG,
	AS_JMP,
	AS_ALIGN,
	AS_GLOBAL,
	AS_LABEL,
	AS_PUSH,
	AS_POP,
	AS_RET,
	AS_CMP,
	AS_JL,
	AS_JG,
	AS_JLE,
	AS_JGE,
	AS_JE,
	AS_JNE,
	AS_LEA,
	AS_CALL,
	AS_MOVS,
	AS_REP,
	AS_CLD,
	AS_CLTD,
	AS_XOR,
	AS_MOVSBL,
	AS_UNKNOWN_OP
};


//! Represents an Operand in a Assembly Instruction
class x86_as_operand
{
	public:
	//! Operand as Identifier 
	sym_tab_entry *sym_tab_entry_ptr;
	//! Operand as Register 
	reg_entry *reg_ptr ;
	string name();
	string make_operand(int sz);
	x86_as_operand(sym_tab_entry *sptr);
	x86_as_operand(reg_entry *eptr);
	private:
	int get_type();
};

//! Direct mode operand
#define DIRECT 1
//! Indirect mode operand
#define INDIRECT 2

//! Operands Type
struct op_type
{
	//! Size of the Operands
	int sz;
	//! Operand 1 Mode (Direct/Indirect)
	int op1_mode;
	//! Operand 2 Mode (Direct/Indirect)
	int op2_mode;
};



//! Represents an a Assembly Instruction
class as_quad
{
	public:
	//! Mnemonic ( e.g. ADD,SUB etc)
	x86_as_oprtr mnemonic;
	//! Operator Type
	struct op_type as_oprtr_type ; /* Byte, Word, or Long */
	//! Operand 1
	x86_as_operand *op1;
	//! Operand 2
	x86_as_operand *op2;
	//! Comment for the Instruction
	string comment;

	//! Constructor of an Assembly Instruction, given the mnemonic,Operator Type and the Operands
	as_quad(x86_as_oprtr mne, struct op_type op_typ, x86_as_operand *op1_ptr, x86_as_operand *op2_ptr,string cmt="" )
	{
		mnemonic=mne;
		as_oprtr_type = op_typ ;
		op1 = op1_ptr ;
		op2 = op2_ptr ;

		if(cmt != "" ){
			comment=cmt;
		}
	};
	string as_string();
	private:

	string get_suffix();
	string func_entry(sym_tab_entry *res); /* used in PROC_BEGIN */

};
//void emit_as (x86_as_oprtr mne, struct op_type op_typ, x86_as_operand *op1_ptr, x86_as_operand *op2_ptr );


/***
x86 specific starts
**/

//! Specializes the quad to have x86 specific Target Code Generation Facilities
class x86_quad : public quad
{
	public:
	//! Constructor, given the Operator and the arguments
	 x86_quad(oprtr opr, sym_tab_entry *argmt1,sym_tab_entry *argmt2,sym_tab_entry *result): quad(opr, argmt1,argmt2,result)
	{
	};
	void translate();
	private:
	void comp_gen(x86_as_operand *as1, x86_as_operand *as2 , struct op_type op_typ,int sz1,int sz2);
	x86_as_operand *fetch_x86_operand(sym_tab_entry *arg2);
};

//! Specializes the Symbol Table to have x86 specific Target Code Generation Facilities
class x86_sym_tab : public sym_tab
{
	public:
	void gen_decl();
   void gen_str_lits(); 
	void gen_text_seg();
};
	
/* extension to a sym tab entry */

//! x86 specific extension to the Symbol Table entry
class x86_extension
{
	public:
	//! x86 operand associated with the symbol table entry
	x86_as_operand *as_operand_ptr;
	//! Constructor 
	x86_extension(x86_as_operand *p)
	{
		as_operand_ptr=p;
	};

};


/**
x86 specific end
**/


	
#endif
