/* Subroutines for insn-output.c for Motorola 68000 family.
   Copyright (C) 1987 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */


/* Some output-actions in m68k.md need these.  */
#include <stdio.h>
#include "config.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-flags.h"
#include "output.h"
#include "insn-attr.h"
#include "flags.h"

/* This flag is used to communicate between movhi and ASM_OUTPUT_CASE_END,
   if SGS_SWITCH_TABLE.  */
int switch_table_difference_label_flag;

static rtx find_addr_reg();
rtx legitimize_pic_address();

#ifndef AKPM_INTERRUPT
int AKPMleaf_function;
char AKPMisr;
#endif

extern int cfpas;

/* Emit a (use pic_offset_table_rtx) if we used PIC relocation in the
   function at any time during the compilation process.  In the future
   we should try and eliminate the USE if we can easily determine that
   all PIC references were deleted from the current function.  That would
   save an address register */

finalize_pic()
{
	if (flag_pic && current_function_uses_pic_offset_table)
		emit_insn(gen_rtx(USE, VOIDmode, pic_offset_table_rtx));
}

#define NOT32	0

/* This function generates the assembly code for function entry.
   STREAM is a stdio stream to output the code to.
   SIZE is an int: how many units of temporary storage to allocate.
   Refer to the array `regs_ever_live' to determine which registers
   to save; `regs_ever_live[I]' is nonzero if register number I
   is ever used in the function.  This function is responsible for
   knowing which registers should not be saved even if used.  */


void
output_function_prologue(stream, size)
FILE *stream;
int size;
{
	register int regno;
	register int mask = 0;
	int num_saved_regs = 0;
	extern char call_used_regs[];
	int fsize = (size + 3) & -4;

	fsize *= GSP8;

	if (cfpas)
	{
		asm_fprintf(stream, "\tmove *sp+,a13,1\n");
		asm_fprintf(stream, "\tmove a3,-*sp,1\n");
		if (cfpas > 4)
			asm_fprintf(stream, "\tmove a2,-*sp,1\n");
		if (cfpas > 8)
			asm_fprintf(stream, "\tmove a1,-*sp,1\n");
		if (cfpas > 12)
			asm_fprintf(stream, "\tmove a0,-*sp,1\n");
		asm_fprintf(stream, "\tmove a13,*-sp,1\n");
	}

	if (frame_pointer_needed)
	{
		asm_fprintf(stream, "\tmove fp,-*sp,1\n\tmove sp,fp\n");
		if (fsize)
		{
			do_addsubreg(stack_pointer_rtx, fsize, 0);
		}
	}
	else if (fsize)
	{
		do_addsubreg(stack_pointer_rtx, fsize + NOT32, 0);
	}
	mask = 0;
	for (regno = 0; regno < 16; regno++)
	{
		if (regs_ever_live[regno] && !call_used_regs[regno])
		{
			mask |= 1 << (15 - regno);
			num_saved_regs++;
		}
	}
#ifndef AKPM_INTERRUPT
	{
		extern char AKPMisr;

		if (AKPMisr)
		{
			AKPMleaf_function = leaf_function_p();
			for (regno = 0; regno < 15; regno++)
			{
				if ((regno == 13) ||
						((!AKPMleaf_function || regs_ever_live[regno]) &&
							call_used_regs[regno]))
				{
					mask |= 1 << (15 - regno);
					num_saved_regs++;
				}
			}
		}
	}
#endif
	if (frame_pointer_needed)
	{
		mask &= ~(1 << (15 - FRAME_POINTER_REGNUM));
		num_saved_regs--;
	}

#if NEED_PROBE
	fprintf(stream, "\ttstl sp@(%d)\n", NEED_PROBE - num_saved_regs * 4);
#endif

	if (mask)
	{
		if (num_saved_regs <= 2)
		{
			int i;

			/* Undo the work from above. */
			for (i = 0; i < 16; i++)
			{
				if (mask & (1 << i))
					asm_fprintf(stream, "\tmove %s,-*sp,1\n",
							reg_names[15 - i]);
			}
		}
		else
		{
			int i;
			asm_fprintf(stream, "\tmmtm sp");
			for (i = 0; i < 16; i++)
			{
				if (mask & (1 << i))
					asm_fprintf(stream, ",a%d", 15 - i);
			}
			asm_fprintf(stream, "\n");
		}
	}
}

/* This function generates the assembly code for function exit,
   on machines that need it.  Args are same as for FUNCTION_PROLOGUE.

   The function epilogue should not depend on the current stack pointer!
   It should use the frame pointer only, if there is a frame pointer.
   This is mandatory because of alloca; we also take advantage of it to
   omit stack adjustments before returning.  */

void
output_function_epilogue(stream, size)
FILE *stream;
int size;
{
	register int regno;
	register int mask;
	register int nregs;
	int offset;
	extern char call_used_regs[];
	int fsize = (size + 3) & -4;
	int big = 0;
	rtx insn = get_last_insn();
	extern char AKPMisr;
	char cantUseA13 = 0;

	fsize *= GSP8;

	/* If the last insn was a BARRIER, we don't have to write any code.  */
	if (GET_CODE(insn) == NOTE)
		insn = prev_nonnote_insn(insn);
	if (insn && GET_CODE(insn) == BARRIER)
		return;

#ifdef FUNCTION_EXTRA_EPILOGUE
	FUNCTION_EXTRA_EPILOGUE(stream, size);
#endif
	nregs = 0;
	mask = 0;
	if (frame_pointer_needed)
		regs_ever_live[FRAME_POINTER_REGNUM] = 0;
	for (regno = 0; regno < 16; regno++)
	{
		if (regs_ever_live[regno] && !call_used_regs[regno])
		{
			nregs++;
			mask |= 1 << regno;
		}
	}

#ifndef AKPM_INTERRUPT
	if (AKPMisr)
	{
		for (regno = 0; regno < 15; regno++)
		{
			if ((regno == 13) ||
				((!AKPMleaf_function || regs_ever_live[regno]) &&
						call_used_regs[regno]))
			{
				nregs++;
				mask |= 1 << regno;
			}
		}
	}
#endif

	offset = nregs * 4 * GSP8;
	if (frame_pointer_needed && mask &&
			(nregs >= 2 || (offset + fsize > 0x8000)))
	{
		if (AKPMisr)
		{
			cantUseA13 = 1;

			do_addsubreg(frame_pointer_rtx, offset + fsize, 0);
			big = 1;
			/* fp now points at the regs to be restored */
		}
		else
		{
			asm_fprintf(stream, "\tmove fp,a13\n");
			do_addsubreg(a13_rtx, offset + fsize, 0);
			big = 1;
			/* a13 now points at the regs to be restored */
		}
	}

	if (mask)
	{
		int i;

		if (!frame_pointer_needed)
		{
			if (nregs <= 2)
			{
				for (i = 15; i >= 0; i--)
				{
					if (mask & (1 << i))
						asm_fprintf(stream, "\tmove *sp+,a%d,1\n", i);
				}
			}
			else
			{
				asm_fprintf(stream, "\tmmfm sp");
				for (i = 0; i < 16; i++)
				{
					if (mask & (1 << i))
						asm_fprintf(stream, ",a%d", i);
				}
				asm_fprintf(stream, "\n");
			}
		}
		else
		{
			if (big)
			{
				if (nregs <= 2)
				{
					for (i = 15; i >= 0; i--)
					{
						if (mask & (1 << i))
						{
							if (cantUseA13)
								asm_fprintf(stream, "\tmove *fp+,a%d,1\n", i);
							else
								asm_fprintf(stream, "\tmove *a13+,a%d,1\n", i);
						}
					}
				}
				else
				{
					if (cantUseA13)
						asm_fprintf(stream, "\tmmfm fp");
					else
						asm_fprintf(stream, "\tmmfm a13");
					for (i = 0; i < 16; i++)
					{
						if (mask & (1 << i))
							asm_fprintf(stream, ",a%d", i);
					}
					asm_fprintf(stream, "\n");
				}
				if (cantUseA13 && fsize)
				{	/*
					 * fp has been used to restore the registers
					 * (an interrupt function).  Now restore the fp's
					 * original value to restore the sp from
					 */
					do_addsubreg(frame_pointer_rtx, fsize, 1);
				}
			}
			else
			{
				int offs = 0;

				for (i = 0; i < 16; i++)
				{
					if (mask & (1 << i))
					{
						asm_fprintf(stream, "\tmove *fp(%d),a%d,1\n",
							-(offset + fsize) + (offs * 32), i);
						offs++;
					}
				}
			}
		}
	}

	if (frame_pointer_needed)
		fprintf(stream, "\tmove fp,sp\n\tmove *sp+,fp,1\n");
	else if (fsize)
	{
		do_addsubreg(stack_pointer_rtx, fsize + NOT32, 1);
	}
	{
		extern char AKPMisr;

		if (AKPMisr)
		{
			if (cfpas || current_function_pops_args)
				error("Interrupt functions can't take arguments");
			fprintf(stream, "\treti\n");
			AKPMisr = 0;
			return;
		}
	}
	{
		int deallbytes = current_function_pops_args / 8 + cfpas;
		if (deallbytes > 62)
		{
			fprintf(stream, "\tmove *sp+,a13,1\n");
			do_addsubreg(stack_pointer_rtx, cfpas, 1);
			fprintf(stream, "\tjump a13\n");
		}
		else
		{
			fprintf(stream, "\trets %d\n", deallbytes / 2);
		}
	}
}

/* Similar to general_operand, but exclude stack_pointer_rtx.  */

int
not_sp_operand(op, mode)
register rtx op;
enum machine_mode mode;
{
	return op != stack_pointer_rtx && general_operand(op, mode);
}

char *
output_btst(operands, countop, dataop, insn, signpos)
rtx *operands;
rtx countop, dataop;
rtx insn;
int signpos;
{
	operands[0] = countop;
	operands[1] = dataop;

	return "btst %0,%1";
}


/* Returns 1 if OP is either a symbol reference or a sum of a symbol
   reference and a constant.  */

int
symbolic_operand(op, mode)
register rtx op;
enum machine_mode mode;
{
	switch (GET_CODE(op))
	{
	case SYMBOL_REF:
	case LABEL_REF:
		return 1;

	case CONST:
		op = XEXP(op, 0);
		return ((GET_CODE(XEXP(op, 0)) == SYMBOL_REF
			 || GET_CODE(XEXP(op, 0)) == LABEL_REF)
			&& GET_CODE(XEXP(op, 1)) == CONST_INT);

#if 0				/* Deleted, with corresponding change in
				 * m68k.h, so as to fit the specs.  No
				 * CONST_DOUBLE is ever symbolic.  */
	case CONST_DOUBLE:
		return GET_MODE(op) == mode;
#endif

	default:
		return 0;
	}
}



/* Legitimize PIC addresses.  If the address is already
   position-independent, we return ORIG.  Newly generated
   position-independent addresses go to REG.  If we need more
   than one register, we lose.

   An address is legitimized by making an indirect reference
   through the Global Offset Table with the name of the symbol
   used as an offset.

   The assembler and linker are responsible for placing the
   address of the symbol in the GOT.  The function prologue
   is responsible for initializing a5 to the starting address
   of the GOT.

   The assembler is also responsible for translating a symbol name
   into a constant displacement from the start of the GOT.

   A quick example may make things a little clearer:

   When not generating PIC code to store the value 12345 into _foo
   we would generate the following code:

	movel #12345, _foo

   When generating PIC two transformations are made.  First, the compiler
   loads the address of foo into a register.  So the first transformation makes:

	lea	_foo, a0
	movel   #12345, a0@

   The code in movsi will intercept the lea instruction and call this
   routine which will transform the instructions into:

	movel   a5@(_foo:w), a0
	movel   #12345, a0@


   That (in a nutshell) is how *all* symbol and label references are
   handled.  */

rtx
legitimize_pic_address(orig, mode, reg)
rtx orig, reg;
enum machine_mode mode;
{
	rtx pic_ref = orig;

	/* First handle a simple SYMBOL_REF or LABEL_REF */
	if (GET_CODE(orig) == SYMBOL_REF || GET_CODE(orig) == LABEL_REF)
	{
		if (reg == 0)
			abort();

		pic_ref = gen_rtx(MEM, Pmode,
				  gen_rtx(PLUS, Pmode,
					  pic_offset_table_rtx, orig));
		current_function_uses_pic_offset_table = 1;
		RTX_UNCHANGING_P(pic_ref) = 1;
		emit_move_insn(reg, pic_ref);
		return reg;
	}
	else if (GET_CODE(orig) == CONST)
	{
		rtx base, offset;

		/* Make sure this is CONST has not already been legitimized */
		if (GET_CODE(XEXP(orig, 0)) == PLUS
		    && XEXP(XEXP(orig, 0), 0) == pic_offset_table_rtx)
			return orig;

		if (reg == 0)
			abort();

		/* legitimize both operands of the PLUS */
		if (GET_CODE(XEXP(orig, 0)) == PLUS)
		{
			base = legitimize_pic_address(XEXP(XEXP(orig, 0), 0), Pmode, reg);
			orig = legitimize_pic_address(XEXP(XEXP(orig, 0), 1), Pmode,
						      base == reg ? 0 : reg);
		}
		else
			abort();

		if (GET_CODE(orig) == CONST_INT)
			return plus_constant_for_output(base, INTVAL(orig));
		pic_ref = gen_rtx(PLUS, Pmode, base, orig);
		/* Likewise, should we set special REG_NOTEs here?  */
	}
	return pic_ref;
}



/* Return the best assembler insn template
   for moving operands[1] into operands[0] as a fullword.  */

static char *
singlemove_string(operands)
rtx *operands;
{
	if (REG_P(operands[0])
	    && GET_CODE(operands[1]) == CONST_INT
	    && INTVAL(operands[1]) <= 32
	    && INTVAL(operands[1]) >= 1)
	{
		return "movk %1,%0";
	}
	if (operands[1] != const0_rtx)
		return "move %1,%0";
	return "clr %0";
}

/* Output assembler code to perform a doubleword move insn
   with operands OPERANDS.  */

char *
output_move_double(operands)
rtx *operands;
{
	enum
	{
		REGOP, OFFSOP, MEMOP, PUSHOP, POPOP, CNSTOP, RNDOP
	} optype0, optype1;
	rtx latehalf[2];
	rtx addreg0 = 0, addreg1 = 0;

	/* First classify both operands.  */

	if (REG_P(operands[0]))
		optype0 = REGOP;
	else if (offsettable_memref_p(operands[0]))
		optype0 = OFFSOP;
	else if (GET_CODE(XEXP(operands[0], 0)) == POST_INC)
		optype0 = POPOP;
	else if (GET_CODE(XEXP(operands[0], 0)) == PRE_DEC)
		optype0 = PUSHOP;
	else if (GET_CODE(operands[0]) == MEM)
		optype0 = MEMOP;
	else
		optype0 = RNDOP;

	if (REG_P(operands[1]))
		optype1 = REGOP;
	else if (CONSTANT_P(operands[1]))
		optype1 = CNSTOP;
	else if (offsettable_memref_p(operands[1]))
		optype1 = OFFSOP;
	else if (GET_CODE(XEXP(operands[1], 0)) == POST_INC)
		optype1 = POPOP;
	else if (GET_CODE(XEXP(operands[1], 0)) == PRE_DEC)
		optype1 = PUSHOP;
	else if (GET_CODE(operands[1]) == MEM)
		optype1 = MEMOP;
	else
		optype1 = RNDOP;

	/*
	 * Check for the cases that the operand constraints are not supposed
	 * to allow to happen.  Abort if we get one, because generating code
	 * for these cases is painful.
	 */

	if (optype0 == RNDOP || optype1 == RNDOP)
		abort();

	/*
	 * If one operand is decrementing and one is incrementing decrement
	 * the former register explicitly and change that operand into
	 * ordinary indexing.
	 */

	if (optype0 == PUSHOP && optype1 == POPOP)
	{
		operands[0] = XEXP(XEXP(operands[0], 0), 0);
		output_asm_insn("subi 64,%0", operands);
		operands[0] = gen_rtx(MEM, DImode, operands[0]);
		optype0 = OFFSOP;
	}
	if (optype0 == POPOP && optype1 == PUSHOP)
	{
		operands[1] = XEXP(XEXP(operands[1], 0), 0);
		output_asm_insn("subi 64,%1", operands);
		operands[1] = gen_rtx(MEM, DImode, operands[1]);
		optype1 = OFFSOP;
	}

	/*
	 * If an operand is an unoffsettable memory ref, find a register we
	 * can increment temporarily to make it refer to the second word.
	 */

	if (optype0 == MEMOP)
		addreg0 = find_addr_reg(XEXP(operands[0], 0));

	if (optype1 == MEMOP)
		addreg1 = find_addr_reg(XEXP(operands[1], 0));

	/*
	 * Ok, we can do one word at a time. Normally we do the low-numbered
	 * word first, but if either operand is autodecrementing then we do
	 * the high-numbered word first.
	 * 
	 * In either case, set up in LATEHALF the operands to use for the
	 * high-numbered word and in some cases alter the operands in
	 * OPERANDS to be suitable for the low-numbered word.
	 */

	if (optype0 == REGOP)
		latehalf[0] = gen_rtx(REG, SImode, REGNO(operands[0]) + 1);
	else if (optype0 == OFFSOP)
		latehalf[0] = adj_offsettable_operand(operands[0], 32);
	else
		latehalf[0] = operands[0];

	if (optype1 == REGOP)
		latehalf[1] = gen_rtx(REG, SImode, REGNO(operands[1]) + 1);
	else if (optype1 == OFFSOP)
		latehalf[1] = adj_offsettable_operand(operands[1], 32);
	else if (optype1 == CNSTOP)
		split_double(operands[1], &operands[1], &latehalf[1]);
	else
		latehalf[1] = operands[1];

	/*
	 * If insn is effectively movd N(sp),-(sp) then we will do the high
	 * word first.  We should use the adjusted operand 1 (which is
	 * N+4(sp)) for the low word as well, to compensate for the first
	 * decrement of sp.
	 */
	if (optype0 == PUSHOP
	    && REGNO(XEXP(XEXP(operands[0], 0), 0)) == STACK_POINTER_REGNUM
	    && reg_overlap_mentioned_p(stack_pointer_rtx, operands[1]))
		operands[1] = latehalf[1];

	/*
	 * If one or both operands autodecrementing, do the two words,
	 * high-numbered first.
	 */

	/*
	 * Likewise,  the first move would clobber the source of the second
	 * one, do them in the other order.  This happens only for registers;
	 * such overlap can't happen in memory unless the user explicitly
	 * sets it up, and that is an undefined circumstance.
	 */

	if (optype0 == PUSHOP || optype1 == PUSHOP
	    || (optype0 == REGOP && optype1 == REGOP
		&& REGNO(operands[0]) == REGNO(latehalf[1])))
	{

		/*
		 * Make any unoffsettable addresses point at high-numbered
		 * word.
		 */
		if (addreg0)
			output_asm_insn("addk 32,%0", &addreg0);
		if (addreg1)
			output_asm_insn("addk 32,%0", &addreg1);

		/* Do that word.  */
#if 0
		output_asm_insn(singlemove_string(latehalf), latehalf);
#else
		do_move(latehalf[1], latehalf[0], DM_LONG, DM_LONG);
#endif
		/* Undo the adds we just did.  */
		if (addreg0)
			output_asm_insn("subk 32,%0", &addreg0);
		if (addreg1)
			output_asm_insn("subk 32,%0", &addreg1);

		/* Do low-numbered word.  */
#if 0
		return singlemove_string(operands);
#else
		do_move(operands[1], operands[0], DM_LONG, DM_LONG);
		return "";
#endif
	}

	/* Normal case: do the two words, low-numbered first.  */

#if 0
	output_asm_insn(singlemove_string(operands), operands);
#else
	do_move(operands[1], operands[0], DM_LONG, DM_LONG);
#endif

	/* Make any unoffsettable addresses point at high-numbered word.  */
	if (addreg0)
		output_asm_insn("addk 32,%0", &addreg0);
	if (addreg1)
		output_asm_insn("addk 32,%0", &addreg1);

	/* Do that word.  */
#if 0
	output_asm_insn(singlemove_string(latehalf), latehalf);
#else
	do_move(latehalf[1], latehalf[0], DM_LONG, DM_LONG);
#endif

	/* Undo the adds we just did.  */
	if (addreg0)
		output_asm_insn("subk 32,%0", &addreg0);
	if (addreg1)
		output_asm_insn("subk 32,%0", &addreg1);

	return "";
}

/* Return a REG that occurs in ADDR with coefficient 1.
   ADDR can be effectively incremented by incrementing REG.  */

static rtx
find_addr_reg(addr)
rtx addr;
{
	while (GET_CODE(addr) == PLUS)
	{
		if (GET_CODE(XEXP(addr, 0)) == REG)
			addr = XEXP(addr, 0);
		else if (GET_CODE(XEXP(addr, 1)) == REG)
			addr = XEXP(addr, 1);
		else if (CONSTANT_P(XEXP(addr, 0)))
			addr = XEXP(addr, 1);
		else if (CONSTANT_P(XEXP(addr, 1)))
			addr = XEXP(addr, 0);
		else
			abort();
	}
	if (GET_CODE(addr) == REG)
		return addr;
	abort();
}


/* Store in cc_status the expressions that the condition codes will
   describe after execution of an instruction whose pattern is EXP.
   Do not alter them if the instruction would not alter the cc's.  */

/* On the 68000, all the insns to store in an address register fail to
   set the cc's.  However, in some cases these instructions can make it
   possibly invalid to use the saved cc's.  In those cases we clear out
   some or all of the saved cc's so they won't be used.  */

cc_s_i()
{}

notice_update_cc(exp, insn)
rtx exp;
rtx insn;
{
	if (GET_CODE(exp) == SET)
	{
		rtx src = SET_SRC(exp);
		rtx dest = SET_DEST(exp);

		if (REG_P(dest))
		{
			if (
				(GET_CODE(src) == MEM &&
					!(	GET_CODE(XEXP(src, 0)) == POST_INC &&
						GET_MODE(src) == QImode)) ||
				(GET_CODE(src) == SIGN_EXTEND &&
					GET_CODE(XEXP(src, 0)) == MEM &&
					!(	GET_CODE(XEXP(XEXP(src, 0), 0)) == POST_INC &&
						GET_MODE(XEXP(src, 0)) == QImode))
				)
			{
				cc_status.flags = 0;
				cc_status.value1 = dest;
				cc_status.value2 = src;
				if (GET_MODE(cc_status.value1) == QImode ||
					GET_MODE(cc_status.value1) == HImode)
				{
					cc_status.value1 = copy_rtx(cc_status.value1);
					PUT_MODE(cc_status.value1, SImode);
				}
			}
			else if ( 0
				|| (GET_CODE(SET_SRC(exp)) == MULT)
				|| (GET_CODE(SET_SRC(exp)) == DIV)
				|| (GET_CODE(SET_SRC(exp)) == UDIV)
				|| (GET_CODE(SET_SRC(exp)) == PLUS)
				|| (GET_CODE(SET_SRC(exp)) == MINUS)
				|| (GET_CODE(SET_SRC(exp)) == NEG)
				|| (GET_CODE(SET_SRC(exp)) == ASHIFT)
				|| (GET_CODE(SET_SRC(exp)) == ASHIFTRT)
				|| (GET_CODE(SET_SRC(exp)) == SIGN_EXTEND)
				|| (GET_CODE(SET_SRC(exp)) == ZERO_EXTEND)
				)
			{
				cc_status.flags = 0;
				cc_status.value1 = dest;
				cc_status.value2 = 0;
			}
			else if (GET_CODE(src) == CALL)
				CC_STATUS_INIT;
			else if (REG_P(src) && GET_MODE(src) == SImode)
			{
				cc_status.flags = 0;
				cc_status.value1 = dest;
				cc_status.value2 = src;
				return;		/* Setting a register sets flags */
			}
			else
			{
				CC_STATUS_INIT;
				return;
			}
		}
		else if (GET_CODE(dest) == MEM &&
				GET_MODE(src) == SImode && GET_MODE(dest) == SImode &&
				_can_move(src, dest, DM_LONG, DM_LONG))
		{
			return;
		}
		else if (GET_CODE(dest) == MEM &&
				GET_MODE(src) == HImode && GET_MODE(dest) == HImode &&
				_can_move(src, dest, DM_SHORT, DM_SHORT))
		{
			return;
		}
		else if (GET_CODE(dest) == MEM &&
				GET_MODE(src) == QImode && GET_MODE(dest) == QImode &&
				_can_move(src, dest, DM_BYTE, DM_BYTE))
		{
			return;
		}
		else if (XEXP(exp, 0) == pc_rtx)
			return;
		else if (GET_CODE(dest) == CC0)
			return;
		else
			CC_STATUS_INIT;
	}
	else
		CC_STATUS_INIT;
	return;
}

char *
output_move_const_double(operands)
rtx *operands;
{
	abort();
}

char *
output_move_const_single(operands)
rtx *operands;
{
	abort();
}

/* If X is a floating-point constant, return the logarithm of X base 2,
   or 0 if X is not a power of 2.  */

int
floating_exact_log2(x)
rtx x;
{
	register double d, d1;
	int i;

#if HOST_FLOAT_FORMAT != TARGET_FLOAT_FORMAT
	if (!flag_pretend_float)
		return 0;
#endif

	REAL_VALUE_FROM_CONST_DOUBLE(d, x);

	if (!(d > 0))
		return 0;

	for (d1 = 1.0, i = 0; d1 < d; d1 *= 2.0, i++)
		;

	if (d == d1)
		return i;

	return 0;
}


#ifdef SUPPORT_SUN_FPA
/* Return nonzero if X, a CONST_DOUBLE, has a value that we can get
   from the Sun FPA's constant RAM.
   The value returned, anded with 0x1ff, gives the code to use in fpmove
   to get the desired constant. */
#define S_E (2.718281745910644531)
#define D_E (2.718281828459045091)
#define S_PI (3.141592741012573242)
#define D_PI (3.141592653589793116)
#define S_SQRT2 (1.414213538169860840)
#define D_SQRT2 (1.414213562373095145)
#define S_LOG2ofE (1.442695021629333496)
#define D_LOG2ofE (1.442695040888963387)
#define S_LOG2of10 (3.321928024291992188)
#define D_LOG2of10 (3.321928024887362182)
#define S_LOGEof2 (0.6931471824645996094)
#define D_LOGEof2 (0.6931471805599452862)
#define S_LOGEof10 (2.302585124969482442)
#define D_LOGEof10 (2.302585092994045901)
#define S_LOG10of2 (0.3010300099849700928)
#define D_LOG10of2 (0.3010299956639811980)
#define S_LOG10ofE (0.4342944920063018799)
#define D_LOG10ofE (0.4342944819032518167)

/* This code should be fixed for cross-compilation. */

int
standard_sun_fpa_constant_p(x)
rtx x;
{
	register double d;

#if HOST_FLOAT_FORMAT != TARGET_FLOAT_FORMAT
	if (!flag_pretend_float)
		return 0;
#endif

	REAL_VALUE_FROM_CONST_DOUBLE(d, x);

	if (d == 0.0)
		return 0x200;	/* 0 once 0x1ff is anded with it */
	if (d == 1.0)
		return 0xe;
	if (d == 0.5)
		return 0xf;
	if (d == -1.0)
		return 0x10;
	if (d == 2.0)
		return 0x11;
	if (d == 3.0)
		return 0xB1;
	if (d == 4.0)
		return 0x12;
	if (d == 8.0)
		return 0x13;
	if (d == 0.25)
		return 0x15;
	if (d == 0.125)
		return 0x16;
	if (d == 10.0)
		return 0x17;
	if (d == -(1.0 / 2.0))
		return 0x2E;

/*
 * Stuff that looks different if it's single or double
 */
	if (GET_MODE(x) == SFmode)
	{
		if (d == S_E)
			return 0x8;
		if (d == (2 * S_PI))
			return 0x9;
		if (d == S_PI)
			return 0xA;
		if (d == (S_PI / 2.0))
			return 0xB;
		if (d == S_SQRT2)
			return 0xC;
		if (d == (1.0 / S_SQRT2))
			return 0xD;

		/*
		 * Large powers of 10 in the constant ram are not used
		 * because they are not equal to a C double constant
		 */
		if (d == -(S_PI / 2.0))
			return 0x27;
		if (d == S_LOG2ofE)
			return 0x28;
		if (d == S_LOG2of10)
			return 0x29;
		if (d == S_LOGEof2)
			return 0x2A;
		if (d == S_LOGEof10)
			return 0x2B;
		if (d == S_LOG10of2)
			return 0x2C;
		if (d == S_LOG10ofE)
			return 0x2D;
	}
	else
	{
		if (d == D_E)
			return 0x8;
		if (d == (2 * D_PI))
			return 0x9;
		if (d == D_PI)
			return 0xA;
		if (d == (D_PI / 2.0))
			return 0xB;
		if (d == D_SQRT2)
			return 0xC;
		if (d == (1.0 / D_SQRT2))
			return 0xD;

		/*
		 * Large powers of 10 in the constant ram are not used
		 * because they are not equal to a C double constant
		 */
		if (d == -(D_PI / 2.0))
			return 0x27;
		if (d == D_LOG2ofE)
			return 0x28;
		if (d == D_LOG2of10)
			return 0x29;
		if (d == D_LOGEof2)
			return 0x2A;
		if (d == D_LOGEof10)
			return 0x2B;
		if (d == D_LOG10of2)
			return 0x2C;
		if (d == D_LOG10ofE)
			return 0x2D;
	}
	return 0x0;
}

#endif				/* define SUPPORT_SUN_FPA */


/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand X.  X is an RTL
   expression.

   CODE is a value that can be used to specify one of several ways
   of printing the operand.  It is used when identical operands
   must be printed differently depending on the context.  CODE
   comes from the `%' specification that was used to request
   printing of the operand.  If the specification was just `%DIGIT'
   then CODE is 0; if the specification was `%LTR DIGIT' then CODE
   is the ASCII code for LTR.

   If X is a register, this macro should print the register's name.
   The names can be found in an array `reg_names' whose type is
   `char *[]'.  `reg_names' is initialized from `REGISTER_NAMES'.

   When the machine description has a specification `%PUNCT' (a `%'
   followed by a punctuation character), this macro is called with
   a null pointer for X and the punctuation character for CODE.

   The m68k specific codes are:

   '.' for dot needed in Motorola-style opcode names.
   '-' for an operand pushing on the stack:
       sp@-, -(sp) or -(%sp) depending on the style of syntax.
   '+' for an operand pushing on the stack:
       sp@+, (sp)+ or (%sp)+ depending on the style of syntax.
   '@' for a reference to the top word on the stack:
       sp@, (sp) or (%sp) depending on the style of syntax.
   '#' for an immediate operand prefix (# in MIT and Motorola syntax
       but & in SGS syntax).
   '!' for the cc register (used in an `and to cc' insn).
   '$' for the letter `s' in an op code, but only on the 68040.
   '&' for the letter `d' in an op code, but only on the 68040.

   'b' for byte insn (no effect, on the Sun; this is for the ISI).
   'd' to force memory addressing to be absolute, not relative.
   'f' for float insn (print a CONST_DOUBLE as a float rather than in hex)
   'w' for FPA insn (print a CONST_DOUBLE as a SunFPA constant rather
       than directly).  Second part of 'y' below.
   'x' for float insn (print a CONST_DOUBLE as a float rather than in hex),
       or print pair of registers as rx:ry.
   'y' for a FPA insn (print pair of registers as rx:ry).  This also outputs
       CONST_DOUBLE's as SunFPA constant RAM registers if
       possible, so it should not be used except for the SunFPA.

   */

void
print_operand(file, op, letter)
FILE *file;			/* file to write to */
rtx op;				/* operand to print */
int letter;			/* %<letter> or 0 */
{
	int i;

	if (letter == '.')
	{
#ifdef MOTOROLA
		asm_fprintf(file, ".");
#endif
	}
	else if (letter == '#')
	{
		asm_fprintf(file, "%0I");
	}
	else if (letter == '-')
	{
		asm_fprintf(file, "*-%Rsp");
	}
	else if (letter == '+')
	{
		asm_fprintf(file, "*%Rsp+");
	}
	else if (letter == '@')
	{
		asm_fprintf(file, "*%Rsp");
	}
	else if (letter == '!')
	{
		asm_fprintf(file, "%Rfpcr");
	}
	else if (letter == '$')
	{
	}
	else if (letter == '&')
	{
	}
	else if (GET_CODE(op) == REG)
	{
		if (REGNO(op) < 16
		    && (letter == 'y' || letter == 'x')
		    && GET_MODE(op) == DFmode)
		{
			fprintf(file, "%s:%s", reg_names[REGNO(op)],
				reg_names[REGNO(op) + 1]);
		}
		else
		{
			fprintf(file, "%s", reg_names[REGNO(op)]);
		}
	}
	else if (GET_CODE(op) == MEM)
	{
		output_address(XEXP(op, 0));
	}
	else if (GET_CODE(op) == CONST_DOUBLE && GET_MODE(op) == SFmode)
	{
		double d;
		union
		{
			float f;
			int i;
		} u1;

		REAL_VALUE_FROM_CONST_DOUBLE(d, op);
		u1.f = d;
		PRINT_OPERAND_PRINT_FLOAT(letter, file);
	}
	else if (GET_CODE(op) == CONST_DOUBLE && GET_MODE(op) != DImode)
	{
		double d;

		REAL_VALUE_FROM_CONST_DOUBLE(d, op);
		ASM_OUTPUT_DOUBLE_OPERAND(file, d);
	}
	else
	{
		asm_fprintf(file, "%0I");
		output_addr_const(file, op);
	}
}



/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand that is a memory
   reference whose address is ADDR.  ADDR is an RTL expression.

   Note that this contains a kludge that knows that the only reason
   we have an address (plus (label_ref...) (reg...)) when not generating
   PIC code is in the insn before a tablejump, and we know that m68k.md
   generates a label LInnn: on such an insn.

   It is possible for PIC to generate a (plus (label_ref...) (reg...))
   and we handle that just like we would a (plus (symbol_ref...) (reg...)).

   Some SGS assemblers have a bug such that "Lnnn-LInnn-2.b(pc,d0.l*2)"
   fails to assemble.  Luckily "Lnnn(pc,d0.l*2)" produces the results
   we want.  This difference can be accommodated by using an assembler
   define such "LDnnn" to be either "Lnnn-LInnn-2.b", "Lnnn", or any other
   string, as necessary.  This is accomplished via the ASM_OUTPUT_CASE_END
   macro.  See m68ksgs.h for an example; for versions without the bug.

   They also do not like things like "pea 1.w", so we simple leave off
   the .w on small constants.

   This routine is responsible for distinguishing between -fpic and -fPIC
   style relocations in an address.  When generating -fpic code the
   offset is output in word mode (eg movel a5@(_foo:w), a0).  When generating
   -fPIC code the offset is output in long mode (eg movel a5@(_foo:l), a0) */

void
print_operand_address(file, addr)
FILE *file;
rtx addr;
{
	register rtx reg1, reg2, breg, ireg;
	rtx offset;

	switch (GET_CODE(addr))
	{
	case REG:
		fprintf(file, "*%s", reg_names[REGNO(addr)]);
		break;
	case PRE_DEC:
		fprintf(file, "-*%s", reg_names[REGNO(XEXP(addr, 0))]);
		break;
	case POST_INC:
		fprintf(file, "*%s+", reg_names[REGNO(XEXP(addr, 0))]);
		break;
	case PLUS:
		reg1 = reg2 = ireg = breg = offset = 0;
		if (CONSTANT_ADDRESS_P(XEXP(addr, 0)))
		{
			offset = XEXP(addr, 0);
			addr = XEXP(addr, 1);
		}
		else if (CONSTANT_ADDRESS_P(XEXP(addr, 1)))
		{
			offset = XEXP(addr, 1);
			addr = XEXP(addr, 0);
		}
		if (GET_CODE(addr) != PLUS)
		{
			;
		}
		else if (GET_CODE(XEXP(addr, 0)) == SIGN_EXTEND)
		{
			reg1 = XEXP(addr, 0);
			addr = XEXP(addr, 1);
		}
		else if (GET_CODE(XEXP(addr, 1)) == SIGN_EXTEND)
		{
			reg1 = XEXP(addr, 1);
			addr = XEXP(addr, 0);
		}
		else if (GET_CODE(XEXP(addr, 0)) == MULT)
		{
			reg1 = XEXP(addr, 0);
			addr = XEXP(addr, 1);
		}
		else if (GET_CODE(XEXP(addr, 1)) == MULT)
		{
			reg1 = XEXP(addr, 1);
			addr = XEXP(addr, 0);
		}
		else if (GET_CODE(XEXP(addr, 0)) == REG)
		{
			reg1 = XEXP(addr, 0);
			addr = XEXP(addr, 1);
		}
		else if (GET_CODE(XEXP(addr, 1)) == REG)
		{
			reg1 = XEXP(addr, 1);
			addr = XEXP(addr, 0);
		}
		if (GET_CODE(addr) == REG || GET_CODE(addr) == MULT
		    || GET_CODE(addr) == SIGN_EXTEND)
		{
			if (reg1 == 0)
			{
				reg1 = addr;
			}
			else
			{
				reg2 = addr;
			}
			addr = 0;
		}
		if (offset != 0)
		{
			if (addr != 0)
			{
				abort();
			}
			addr = offset;
		}
		if ((reg1 && (GET_CODE(reg1) == SIGN_EXTEND
			      || GET_CODE(reg1) == MULT))
		    || (reg2 != 0 && REGNO_OK_FOR_BASE_P(REGNO(reg2))))
		{
			breg = reg2;
			ireg = reg1;
		}
		else if (reg1 != 0 && REGNO_OK_FOR_BASE_P(REGNO(reg1)))
		{
			breg = reg1;
			ireg = reg2;
		}
		if (ireg != 0 && breg == 0 && GET_CODE(addr) == LABEL_REF
		    && !(flag_pic && ireg == pic_offset_table_rtx))
		{
			int scale = 1;

			if (GET_CODE(ireg) == MULT)
			{
				scale = INTVAL(XEXP(ireg, 1));
				ireg = XEXP(ireg, 0);
			}
			if (GET_CODE(ireg) == SIGN_EXTEND)
			{
				asm_fprintf(file, "%LL%d-%LLI%d.b(%Rpc,%s.w",
					    CODE_LABEL_NUMBER(XEXP(addr, 0)),
					    CODE_LABEL_NUMBER(XEXP(addr, 0)),
					    reg_names[REGNO(XEXP(ireg, 0))]);
			}
			else
			{
				asm_fprintf(file, "%LL%d-%LLI%d.b(%Rpc,%s.l",
					    CODE_LABEL_NUMBER(XEXP(addr, 0)),
					    CODE_LABEL_NUMBER(XEXP(addr, 0)),
					    reg_names[REGNO(ireg)]);
			}
			if (scale != 1)
			{
				fprintf(file, "*%d", scale);
			}
			putc(')', file);
			break;
		}
		if (breg != 0 && ireg == 0 && GET_CODE(addr) == LABEL_REF
		    && !(flag_pic && breg == pic_offset_table_rtx))
		{
			asm_fprintf(file, "%LL%d-%LLI%d.b(%Rpc,%s.l",
				    CODE_LABEL_NUMBER(XEXP(addr, 0)),
				    CODE_LABEL_NUMBER(XEXP(addr, 0)),
				    reg_names[REGNO(breg)]);
			putc(')', file);
			break;
		}
		if (ireg != 0 || breg != 0)
		{
			int scale = 1;

			if (breg == 0)
			{
				abort();
			}
			if (!flag_pic && addr && GET_CODE(addr) == LABEL_REF)
			{
				abort();
			}
			fprintf(file, "*%s", reg_names[REGNO(breg)]);
			if (addr != 0)
			{
				putc('(', file);
				output_addr_const(file, addr);
				putc(')', file);
			}
			break;
		}
		else if (reg1 != 0 && GET_CODE(addr) == LABEL_REF
			 && !(flag_pic && reg1 == pic_offset_table_rtx))
		{
			asm_fprintf(file, "%LL%d-%LLI%d.b(%Rpc,%s.l)",
				    CODE_LABEL_NUMBER(XEXP(addr, 0)),
				    CODE_LABEL_NUMBER(XEXP(addr, 0)),
				    reg_names[REGNO(reg1)]);
			break;
		}
		/* FALL-THROUGH (is this really what we want? */
	default:
		if (GET_CODE(addr) == CONST_INT
		    && INTVAL(addr) < 0x8000
		    && INTVAL(addr) >= -0x8000)
		{
			fprintf(file, "%d", INTVAL(addr));
		}
		else
		{
			output_addr_const(file, addr);
		}
		break;
	}
}

