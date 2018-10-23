/* Definitions of target machine for GNU compiler.  Sun 68000/68020 version.
   Copyright (C) 1987, 1988 Free Software Foundation, Inc.

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


/* Note that some other tm.h files include this one and then override
   many of the definitions that relate to assembler syntax.  */


/* Names to predefine in the preprocessor for this target machine.  */

/* See sun3.h, sun2.h, isi.h for different CPP_PREDEFINES.  */

/* Print subsidiary information on the compiler version in use.  */
#define TARGET_VERSION fprintf (stderr, " (TMS34010)");


/* Run-time compilation parameters selecting different hardware subsets.  */
extern int target_flags;

/* Macros used in the machine description to test the flags.  */

#define TARGET_BITFIELD	(target_flags & 1)
#define TARGET_RTD		(target_flags & 2)
#define TARGET_SHORT	(target_flags & 4)
#ifdef AKPM_REGPARM
#define TARGET_REGPARM	(target_flags & 8)
#endif
#define TARGET_BREGS	(target_flags & 16)

/* Macro to define tables used to set the flags.
   This is a list in braces of pairs in braces,
   each pair being { "NAME", VALUE }
   where VALUE is the bits to set or minus the bits to clear.
   An empty string NAME is used to identify the default VALUE.  */

#define TARGET_SWITCHES			\
{								\
	{ "bitfield", 1},			\
	{ "nobitfield", -1},		\
    { "rtd", 2},				\
    { "nortd", -2},				\
    { "short", 4},				\
    { "noshort", -4},			\
	{ "regparm", 8},			\
	{ "noregparm", -8},			\
	{ "bregs", 16},				\
	{ "nobregs", -16},			\
    { "", TARGET_DEFAULT},		\
}
/* TARGET_DEFAULT is defined in tm.h */

#ifndef AKPM_REGPARM
#define OVERRIDE_OPTIONS							\
{													\
  if (TARGET_REGPARM)								\
    error("register parameters not supported\n");	\
}
#endif

/* target machine storage layout */

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields.
   This is true for 68020 insns such as bfins and bfexts.
   We make it true always by avoiding using the single-bit insns
   except in special cases with constant bit numbers.  */
#define BITS_BIG_ENDIAN 0

/* Define this if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN 0

/* Define this if most significant word of a multiword number is the lowest
   numbered.  */
#define WORDS_BIG_ENDIAN 0

/* number of bits in an addressable storage unit */
#define BITS_PER_UNIT 8

/* Width in bits of a "word", which is the contents of a machine register.
   Note that this is not necessarily the width of data type `int';
   if using 16-bit ints on a 68000, this would still be 32.
   But on a machine with 16-bit registers, this would be 16.  */
#define BITS_PER_WORD 32

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD 4

/* Width in bits of a pointer.
   See also the macro `Pmode' defined below.  */
#define POINTER_SIZE 32

#ifdef AKPM_REGPARM
#define PROMOTE_PROTOTYPES
#endif

/* Allocation boundary (in *bits*) for storing arguments in argument list.  */
#ifdef PROMOTE_PROTOTYPES
#define PARM_BOUNDARY	32
#else
#define PARM_BOUNDARY	16
#endif

/* Boundary (in *bits*) on which stack pointer should be aligned.  */
#define STACK_BOUNDARY 16

/* Allocation boundary (in *bits*) for the code of a function.  */
#define FUNCTION_BOUNDARY 16

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY 16

/* No data type wants to be aligned rounder than this.  */
#define BIGGEST_ALIGNMENT 16

/* Set this nonzero if move instructions will actually fail to work
   when given unaligned data.  */
#define STRICT_ALIGNMENT 0

#define SELECT_RTX_SECTION(MODE, X)					\
{									\
  if (!flag_pic)							\
    readonly_data_section();						\
  else if (LEGITIMATE_PIC_OPERAND_P (X))				\
    readonly_data_section();						\
  else									\
    data_section();							\
}

/* Define number of bits in most basic integer type.
   (If undefined, default is BITS_PER_WORD).  */

#define INT_TYPE_SIZE (TARGET_SHORT ? 16 : 32)

/* Define these to avoid dependence on meaning of `int'.
   Note that WCHAR_TYPE_SIZE is used in cexp.y,
   where TARGET_SHORT is not available.  */

#define WCHAR_TYPE "long int"
#define WCHAR_TYPE_SIZE 32

/* Standard register usage.  */

/* Number of actual hardware registers.
   The hardware registers are assigned numbers for the compiler
   from 0 to just below FIRST_PSEUDO_REGISTER.
   All registers that the compiler knows about must be given numbers,
   even those that are not normally considered general registers. */

#define FIRST_PSEUDO_REGISTER 32

/* This defines the register which is used to hold the offset table for PIC. */
#define PIC_OFFSET_TABLE_REGNUM 13

/* Used to output a (use pic_offset_table_rtx) so that we 
   always save/restore a5 in functions that use PIC relocation
   at *any* time during the compilation process. */
#define FINALIZE_PIC finalize_pic()

/* 1 for registers that have pervasive standard uses
   and are not available for the register allocator.
   On the 68000, only the stack pointer is such.  */

#define FIXED_REGISTERS			\
 {								\
  0, 0, 0, 0, 0, 0, 0, 0,		\
  0, 0, 0, 0, 0, 1, 0, 1,		\
  1, 1, 1, 1, 1, 1, 1, 1,		\
  1, 1, 1, 1, 1, 1, 1, 1,  }

/* 1 for registers not available across function calls.
   These must include the FIXED_REGISTERS and also any
   registers that can be used without being saved.
   The latter must include the registers where values are returned
   and the register where structure-value addresses are passed.
   Aside from that, you can include as many other registers as you like.  */
#define CALL_USED_REGISTERS		\
 {1, 1, 1, 1, 0, 0, 0, 0,		\
  0, 0, 0, 0, 0, 1, 0, 1,		\
  1, 1, 1, 1, 1, 1, 1, 1,		\
  1, 1, 1, 1, 1, 1, 1, 1, }

#define CONDITIONAL_REGISTER_USAGE \
{                                               \
  if (flag_pic)                                 \
    fixed_regs[PIC_OFFSET_TABLE_REGNUM] = 1;    \
  if (TARGET_BREGS)								\
  {												\
	int i;										\
												\
	for (i = 10; i <= 14; i++)					\
	{											\
		fixed_regs[i + 16] = 0;					\
		call_used_regs[i + 16] = 1;				\
	}											\
  }												\
}

/* Return number of consecutive hard regs needed starting at reg REGNO
   to hold something of mode MODE.
   This is ordinarily the length in words of a value of mode MODE
   but can be less for certain modes in special long registers.

   On the 68000, ordinary registers hold 32 bits worth;
   for the 68881 registers, a single register is always enough for
   anything that can be stored in them at all.  */
#define HARD_REGNO_NREGS(REGNO, MODE)   \
  ((REGNO) >= FIRST_PSEUDO_REGISTER ? GET_MODE_NUNITS (MODE)	\
   : ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))

/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.
   On the 68000, the cpu registers can hold any mode but the 68881 registers
   can hold only SFmode or DFmode.  The 68881 registers can't hold anything
   if 68881 use is disabled.  */

#define HARD_REGNO_MODE_OK(REGNO, MODE) ((REGNO) < FIRST_PSEUDO_REGISTER)

/* Value is 1 if it is a good idea to tie two pseudo registers
   when one has mode MODE1 and one has mode MODE2.
   If HARD_REGNO_MODE_OK could produce different values for MODE1 and MODE2,
   for any hard reg, then this must be 0 for correct output.  */
#define MODES_TIEABLE_P(MODE1, MODE2) 1

/* Specify the registers used for certain standard purposes.
   The values of these macros are register numbers.  */

/* m68000 pc isn't overloaded on a register.  */
/* #define PC_REGNUM  */

/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM 15

/* Base register for access to local variables of the function.  */
#define FRAME_POINTER_REGNUM 14

/* Value should be nonzero if functions must have frame pointers.
   Zero means the frame pointer need not be set up (and parms
   may be accessed via the stack pointer) in functions that seem suitable.
   This is computed in `reload', in reload1.c.  */
#define FRAME_POINTER_REQUIRED 0

/* Base register for access to arguments of the function.  */
#define ARG_POINTER_REGNUM 14

/* Define the classes of registers for register constraints in the
   machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include all hard regs.
   If there is more than one class, another class must be named NO_REGS
   and contain no registers.

   The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.

   For any two classes, it is very desirable that there be another
   class that represents their union.  */

/* The 68000 has three kinds of registers, so eight classes would be
   a complete set.  One of them is not needed.  */

enum reg_class {
  NO_REGS,
  ODD_REGS,
  ODD_B_REGS,
  B_REGS,
  GENERAL_REGS,
  ALL_REGS,
  LIM_REG_CLASSES };

#define N_REG_CLASSES (int) LIM_REG_CLASSES

/* Give names of register classes as strings for dump file.   */

#define REG_CLASS_NAMES						\
 { "NO_REGS",								\
   "ODD_REGS",								\
   "ODD_B_REGS",							\
   "B_REGS",								\
   "GENERAL_REGS",							\
   "ALL_REGS" }

/* Define which registers fit in which classes.
   This is an initializer for a vector of HARD_REG_SET
   of length N_REG_CLASSES.  */

#define REG_CLASS_CONTENTS 				\
{										\
 0x00000000,   	/* NO_REGS */			\
 0x0000aaaa,	/* ODD_REGS */			\
 0xaaaa0000,	/* ODD_B_REGS */		\
 0xffff0000,	/* B_REGS */			\
 0x0000ffff,	/* GENERAL_REGS */		\
 0xffffffff,	/* ALL_REGS */			\
}

/* The same information, inverted:
   Return the class number of the smallest class containing
   reg number REGNO.  This could be a conditional expression
   or could index an array.  */

#define REGNO_REG_CLASS(REGNO)						\
(((1<<(REGNO)) & 0xaaaa) ? 1 : 						\
	(((1<<(REGNO)) & 0xaaaa0000) ? 2 : 				\
		(((1<<(REGNO)) & 0xffff0000) ? 3 :			\
			(((1<<(REGNO)) & 0xffffffff) ? 4 : 0))))

/* The class value for index registers, and the one for base regs.  */

#define INDEX_REG_CLASS GENERAL_REGS
#define BASE_REG_CLASS GENERAL_REGS

/* Get reg_class from a letter such as appears in the machine description.
   We do a trick here to modify the effective constraints on the
   machine description; we zorch the constraint letters that aren't
   appropriate for a specific target.  This allows us to guarantee
   that a specific kind of register will not be used for a given target
   without fiddling with the register classes above. */

#define REG_CLASS_FROM_LETTER(C)	\
    ((C) == '|' ? ODD_REGS :		\
     ((C) == 'b' ? B_REGS :			\
      ((C) == 'a' ? ODD_B_REGS :	\
     NO_REGS)))

/* The letters I, J, K, L and M in a register constraint string
   can be used to stand for particular ranges of immediate operands.
   This macro defines what the ranges are.
   C is the letter, and VALUE is a constant value.
   Return 1 if VALUE is in the range specified by C.

   For the 68000, `I' is used for the range 1 to 8
   allowed as immediate shift counts and in addq.
   `J' is used for the range of signed numbers that fit in 16 bits.
   `K' is for numbers that moveq can't handle.
   `L' is for range -8 to -1, range of values that can be added with subq.  */
#define CONST_OK_FOR_LETTER_P(VALUE, C)  \
  ((C) == 'I' ? (VALUE) > 0 && (VALUE) <= 32 :    \
   (C) == 'J' ? (VALUE) >= -0x8000 && (VALUE) <= 0x7FFF :	\
   (C) == 'K' ? (VALUE) < 1 || (VALUE) > 32 :	\
   (C) == 'L' ? (VALUE) < 0 && (VALUE) >= -8 : 0)

/*
 * A small bit of explanation:
 * "G" defines all of the floating constants that are *NOT* 68881
 * constants.  this is so 68881 constants get reloaded and the
 * fpmovecr is used.  "H" defines *only* the class of constants that
 * the fpa can use, because these can be gotten at in any fpa
 * instruction and there is no need to force reloads.
 */
#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C) ((C) == 'G' ? 1 : 0 )

/* Given an rtx X being reloaded into a reg required to be
   in class CLASS, return the class of reg to actually use.
   In general this is just CLASS; but on some machines
   in some cases it is preferable to use a more restrictive class.
   On the 68000 series, use a data reg if possible when the
   value is a constant in the range where moveq could be used
   and we ensure that QImodes are reloaded into data regs.
   Also, if a floating constant needs reloading, put it in memory
   if possible.  */

#ifdef AKPM
#define PREFERRED_RELOAD_CLASS(X,CLASS)  \
  ((GET_CODE (X) == CONST_INT			\
    && (unsigned) (INTVAL (X) + 0x80) < 0x100	\
    && (CLASS) != ADDR_REGS)			\
   ? DATA_REGS					\
   : (GET_MODE (X) == QImode && (CLASS) != ADDR_REGS) \
   ? DATA_REGS					\
   : (GET_CODE (X) == CONST_DOUBLE		\
      && GET_MODE_CLASS (GET_MODE (X)) == MODE_FLOAT) \
   ? NO_REGS					\
   : (CLASS))
#else
#define PREFERRED_RELOAD_CLASS(X,CLASS)  preferred_reload_class(X, CLASS)
#endif

/* Return the maximum number of consecutive registers
   needed to represent mode MODE in a register of class CLASS.  */
/* On the 68000, this is the size of MODE in words,
   except in the FP regs, where a single reg is always enough.  */

#define CLASS_MAX_NREGS(CLASS, MODE)	\
 ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

/* Moves between fp regs and other regs are two insns.  */
#define REGISTER_MOVE_COST(CLASS1, CLASS2) 2

/* Stack layout; function entry, exit and calling.  */

/* Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  */
#define STACK_GROWS_DOWNWARD

/* Nonzero if we need to generate stack-probe insns.
   On most systems they are not needed.
   When they are needed, define this as the stack offset to probe at.  */
#define NEED_PROBE 0

/* Define this if the nominal address of the stack frame
   is at the high-address end of the local variables;
   that is, each additional local variable allocated
   goes at a more negative offset in the frame.  */
#define FRAME_GROWS_DOWNWARD

/* Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  */
#define STARTING_FRAME_OFFSET 0

/* If we generate an insn to push BYTES bytes,
   this says how many the stack pointer really advances by.
   On the 68000, sp@- in a byte insn really pushes a word.  */
#if 0
#define PUSH_ROUNDING(BYTES) (((BYTES) + 1) & ~1)
#endif
#define PUSH_ROUNDING(BYTES) (BYTES)

/* Offset of first parameter from the argument pointer register value.  */
#define FIRST_PARM_OFFSET(FNDECL) 64

/* Define how to find the value returned by a function.
   VALTYPE is the data type of the value (as a tree).
   If the precise function being called is known, FUNC is its FUNCTION_DECL;
   otherwise, FUNC is 0.  */

/* On the 68000 the return value is in D0 regardless.  */

#define FUNCTION_VALUE(VALTYPE, FUNC)  \
  gen_rtx (REG, TYPE_MODE (VALTYPE), 0)

/* Define how to find the value returned by a library function
   assuming the value has mode MODE.  */

/* On the 68000 the return value is in D0 regardless.  */

#define LIBCALL_VALUE(MODE)  gen_rtx (REG, MODE, 0)

/* 1 if N is a possible register number for a function value.
   On the 68000, d0 is the only register thus used.  */

#define FUNCTION_VALUE_REGNO_P(N) ((N) == 0)

/* Define this if PCC uses the nonreentrant convention for returning
   structure and union values.  */

/* #define PCC_STATIC_STRUCT_RETURN */
#define DEFAULT_PCC_STRUCT_RETURN 0

/* Register in which address to store a structure value
   is passed to a function.  */
#ifdef AKPM_REGPARM
#define STRUCT_VALUE_REGNUM		4
#else
#define STRUCT_VALUE_REGNUM		3
#endif

/* Register in which static-chain is passed to a function.  */
#ifdef AKPM_REGPARM
#define STATIC_CHAIN_REGNUM		5
#else
#define STATIC_CHAIN_REGNUM		4
#endif

/* 1 if N is a possible register number for function argument passing.
   On the 68000, no registers are used in this way.  */

#ifdef AKPM_REGPARM
#define FUNCTION_ARG_REGNO_P(N) (TARGET_REGPARM ? ((N) < 4) : 0)
#else
#define FUNCTION_ARG_REGNO_P(N) 0
#endif

/* Define a data type for recording info about an argument list
   during the scan of that argument list.  This data type should
   hold all necessary information about the function itself
   and about the args processed so far, enough to enable macros
   such as FUNCTION_ARG to determine where the next arg should go.

   On the m68k, this is a single integer, which is a number of bytes
   of arguments scanned so far.  */

#define CUMULATIVE_ARGS int

/* Initialize a variable CUM of type CUMULATIVE_ARGS
   for a call to a function whose data type is FNTYPE.
   For a library call, FNTYPE is 0.

   On the m68k, the offset starts at 0.  */

#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME)	\
 ((CUM) = 0)

/* Update the data in CUM to advance over an argument
   of mode MODE and data type TYPE.
   (TYPE is null for libcalls where that information may not be available.)  */

#ifdef AKPM_T
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)	\
 ((CUM) += ((MODE) != BLKmode			\
	    ? (GET_MODE_SIZE (MODE) + 3) & ~3	\
	    : (int_size_in_bytes (TYPE) + 3) & ~3))
#else
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)	\
do {							\
	if (0)				\
	{						\
	 ((CUM) += ((MODE) != BLKmode			\
	    ? (GET_MODE_SIZE (MODE) + 3) & ~3		\
	    : (int_size_in_bytes (TYPE) + 3) & ~3));	\
	}						\
	else						\
	{						\
	 ((CUM) += ((MODE) != BLKmode			\
	    ? (GET_MODE_SIZE (MODE) + 1) & ~1		\
	    : (int_size_in_bytes (TYPE) + 1) & ~1));	\
	}						\
   } while (0)
#endif

/* Define where to put the arguments to a function.
   Value is zero to push the argument on the stack,
   or a hard register in which to store the argument.

   MODE is the argument's machine mode.
   TYPE is the data type of the argument (as a tree).
    This is null for libcalls where that information may
    not be available.
   CUM is a variable of type CUMULATIVE_ARGS which gives info about
    the preceding args and about the function being called.
   NAMED is nonzero if this argument is a named parameter
    (otherwise it is an extra parameter matching an ellipsis).  */

/* On the 68000 all args are pushed, except if -mregparm is specified
   then the first two words of arguments are passed in d0, d1.
   *NOTE* -mregparm does not work.
   It exists only to test register calling conventions.  */

#ifdef AKPM_REGPARM
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) \
	(((TARGET_REGPARM && (CUM) < 16)) ? gen_rtx (REG, (MODE), (CUM) / 4) : 0)
#else
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) 0
#endif

/* For an arg passed partly in registers and partly in memory,
   this is the number of registers used.
   For args passed entirely in registers or entirely in memory, zero.  */

#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED)		\
((	TARGET_REGPARM												\
	&& ((CUM) < 16)												\
	&& 16 < ((CUM) + ((MODE) == BLKmode							\
		      ? int_size_in_bytes (TYPE)						\
		      : GET_MODE_SIZE (MODE))))  						\
 ? 4 - (CUM) / 4 : 0)

/* Generate the assembly code for function entry. */
#define FUNCTION_PROLOGUE(FILE, SIZE) output_function_prologue(FILE, SIZE)

/* Output assembler code to FILE to increment profiler label # LABELNO
   for profiling a function entry.  */

/* AKPM_34010 */
#define FUNCTION_PROFILER(FILE, LABELNO)  \
  asm_fprintf (FILE, "\tlea %LLP%d,%Ra0\n\tjsr mcount\n", (LABELNO))

/* Output assembler code to FILE to initialize this source file's
   basic block profiling info, if that has not already been done.  */

/* AKPM_34010 */
#define FUNCTION_BLOCK_PROFILER(FILE, LABELNO)  \
  asm_fprintf (FILE, "\ttstl %LLPBX0\n\tbne %LLPI%d\n\tpea %LLPBX0\n\tjsr %U__bb_init_func\n\taddql %I4,%Rsp\n%LLPI%d:\n",  \
	   LABELNO, LABELNO);

/* Output assembler code to FILE to increment the entry-count for
   the BLOCKNO'th basic block in this source file.  */

/* AKPM_34010 */
#define BLOCK_PROFILER(FILE, BLOCKNO)	\
  asm_fprintf (FILE, "\taddql %I1,%LLPBX2+%d\n", 4 * BLOCKNO)

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in
   functions that have frame pointers.
   No definition is equivalent to always zero.  */

#define EXIT_IGNORE_STACK 1

/* Generate the assembly code for function exit. */
#define FUNCTION_EPILOGUE(FILE, SIZE) output_function_epilogue (FILE, SIZE)
  
/* This is a hook for other tm files to change.  */
/* #define FUNCTION_EXTRA_EPILOGUE(FILE, SIZE) */

/* Determine if the epilogue should be output as RTL.
   You should override this if you define FUNCTION_EXTRA_EPILOGUE.  */
#define USE_RETURN_INSN 0

/* Store in the variable DEPTH the initial difference between the
   frame pointer reg contents and the stack pointer reg contents,
   as of the start of the function body.  This depends on the layout
   of the fixed parts of the stack frame and on how registers are saved.

   On the 68k, if we have a frame, we must add one word to its length
   to allow for the place that a6 is stored when we do have a frame pointer.
   Otherwise, we would need to compute the offset from the frame pointer
   of a local variable as a function of frame_pointer_needed, which
   is hard.  */

#define INITIAL_FRAME_POINTER_OFFSET(DEPTH)					\
{ int regno;												\
  int offset = -4;											\
  for (regno = 0; regno < 16; regno++)						\
    if (regs_ever_live[regno] && ! call_used_regs[regno])	\
      offset += 4;											\
  (DEPTH) = (offset + ((get_frame_size () + 3) & -4)		\
	     + (get_frame_size () == 0 ? 0 : 4));				\
  (DEPTH) *= GSP8;\
}

/* Output assembler code for a block containing the constant parts
   of a trampoline, leaving space for the variable parts.  */

/* On the 68k, the trampoline looks like this:
     mov  @#.,a0
     jsr  @#___trampoline
     jsr  @#___trampoline
     .long STATIC
     .long FUNCTION
The reason for having three jsr insns is so that an entire line
of the instruction cache is filled in a predictable way
that will always be the same.

We always use the assembler label ___trampoline
regardless of whether the system adds underscores.  */

#define TRAMPOLINE_TEMPLATE(FILE)					\
{									\
  ASM_OUTPUT_SHORT (FILE, gen_rtx (CONST_INT, VOIDmode, 0x207c));	\
  ASM_OUTPUT_SHORT (FILE, const0_rtx);					\
  ASM_OUTPUT_SHORT (FILE, const0_rtx);					\
  ASM_OUTPUT_SHORT (FILE, gen_rtx (CONST_INT, VOIDmode, 0x4eb9));	\
  ASM_OUTPUT_INT (FILE, gen_rtx (SYMBOL_REF, SImode, "*___trampoline"));\
  ASM_OUTPUT_SHORT (FILE, gen_rtx (CONST_INT, VOIDmode, 0x4eb9));	\
  ASM_OUTPUT_INT (FILE, gen_rtx (SYMBOL_REF, SImode, "*___trampoline"));\
  ASM_OUTPUT_SHORT (FILE, const0_rtx);					\
  ASM_OUTPUT_SHORT (FILE, const0_rtx);					\
  ASM_OUTPUT_SHORT (FILE, const0_rtx);					\
  ASM_OUTPUT_SHORT (FILE, const0_rtx);					\
}

/* Length in units of the trampoline for entering a nested function.  */

#define TRAMPOLINE_SIZE 26

/* Alignment required for a trampoline.  16 is used to find the
   beginning of a line in the instruction cache.  */

#define TRAMPOLINE_ALIGN 16

/* Emit RTL insns to initialize the variable parts of a trampoline.
   FNADDR is an RTX for the address of the function's pure code.
   CXT is an RTX for the static chain value for the function.  */

#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT)			\
{									\
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant (TRAMP, 2)), TRAMP); \
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant (TRAMP, 18)), CXT); \
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant (TRAMP, 22)), FNADDR); \
}

/* This is the library routine that is used
   to transfer control from the trampoline
   to the actual nested function.  */

/* A colon is used with no explicit operands
   to cause the template string to be scanned for %-constructs.  */
/* The function name __transfer_from_trampoline is not actually used.
   The function definition just permits use of "asm with operands"
   (though the operand list is empty).  */
#define TRANSFER_FROM_TRAMPOLINE								\
void															\
__transfer_from_trampoline ()									\
{																\
  register char *a0 asm ("%a0");								\
  asm (GLOBAL_ASM_OP " ___trampoline");							\
  asm ("___trampoline:");										\
  asm volatile ("move%.l %0,%@" : : "m" (a0[22]));				\
  asm volatile ("move%.l %1,%0" : "=a" (a0) : "m" (a0[18]));	\
  asm ("rts":);													\
}

/* Addressing modes, and classification of registers for them.  */

#define HAVE_POST_INCREMENT
/* #define HAVE_POST_DECREMENT */

#define HAVE_PRE_DECREMENT
/* #define HAVE_PRE_INCREMENT */

/* Macros to check register numbers against specific register classes.  */

/* These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.  */

#define REGNO_OK_FOR_INDEX_P(REGNO) \
((REGNO) < 16 || (unsigned) reg_renumber[REGNO] < 16)
#define REGNO_OK_FOR_BASE_P(REGNO) \
((REGNO) < 16 || (unsigned) reg_renumber[REGNO] < 16)
#define REGNO_OK_FOR_DATA_P(REGNO) \
((REGNO) < 16 || (unsigned) reg_renumber[REGNO] < 16)
#define REGNO_OK_FOR_FP_P(REGNO) 0

/* Now macros that check whether X is a register and also,
   strictly, whether it is in a specified class.

   These macros are specific to the 68000, and may be used only
   in code for printing assembler insns and in conditions for
   define_optimization.  */

/* Maximum number of registers that can appear in a valid memory address.  */

#define MAX_REGS_PER_ADDRESS 1

/* Recognize any constant value that is a valid address.  */

#define CONSTANT_ADDRESS_P(X)  CONSTANT_P (X)

/* Nonzero if the constant value X is a legitimate general operand.
   It is given that X satisfies CONSTANT_P or is a CONST_DOUBLE.  */

#define LEGITIMATE_CONSTANT_P(X) 1

/* Nonzero if the constant value X is a legitimate general operand
   when generating PIC code.  It is given that flag_pic is on and 
   that X satisfies CONSTANT_P or is a CONST_DOUBLE.  */

#define LEGITIMATE_PIC_OPERAND_P(X)	\
  (! symbolic_operand (X, VOIDmode))

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx
   and check its validity for a certain class.
   We have two alternate definitions for each of them.
   The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.
   The symbol REG_OK_STRICT causes the latter definition to be used.

   Most source files want to accept pseudo regs in the hope that
   they will get allocated to the class that the insn wants them to be in.
   Source files for reload pass need to be strict.
   After reload, it makes no difference, since pseudo regs have
   been eliminated by then.  */

#ifndef REG_OK_STRICT

/* Nonzero if X is a hard reg that can be used as an index
   or if it is a pseudo reg.  */
#define REG_OK_FOR_INDEX_P(X) \
	((REGNO (X) < 16) || (REGNO(X) >= FIRST_PSEUDO_REGISTER))
/* Nonzero if X is a hard reg that can be used as a base reg
   or if it is a pseudo reg.  */
#define REG_OK_FOR_BASE_P(X) \
	((REGNO (X) < 16) || (REGNO(X) >= FIRST_PSEUDO_REGISTER))

#else

/* Nonzero if X is a hard reg that can be used as an index.  */
#define REG_OK_FOR_INDEX_P(X) REGNO_OK_FOR_INDEX_P (REGNO (X))
/* Nonzero if X is a hard reg that can be used as a base reg.  */
#define REG_OK_FOR_BASE_P(X) REGNO_OK_FOR_BASE_P (REGNO (X))

#endif

/* GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression
   that is a valid memory address for an instruction.
   The MODE argument is the machine mode for the MEM expression
   that wants to use this address.

   When generating PIC, an address involving a SYMBOL_REF is legitimate
   if and only if it is the sum of pic_offset_table_rtx and the SYMBOL_REF.
   We use LEGITIMATE_PIC_OPERAND_P to throw out the illegitimate addresses,
   and we explicitly check for the sum of pic_offset_table_rtx and a SYMBOL_REF.

   Likewise for a LABEL_REF when generating PIC.

   The other macros defined here are used only in GO_IF_LEGITIMATE_ADDRESS.  */

#define INDIRECTABLE_1_ADDRESS_P(X)  \
  ((CONSTANT_ADDRESS_P (X) && (!flag_pic || LEGITIMATE_PIC_OPERAND_P (X))) \
   || (GET_CODE (X) == REG && REG_OK_FOR_BASE_P (X))			\
   || ((GET_CODE (X) == PRE_DEC || GET_CODE (X) == POST_INC)		\
       && REG_P (XEXP (X, 0))						\
       && REG_OK_FOR_BASE_P (XEXP (X, 0)))				\
   || (GET_CODE (X) == PLUS						\
       && REG_P (XEXP (X, 0)) && REG_OK_FOR_BASE_P (XEXP (X, 0))	\
       && GET_CODE (XEXP (X, 1)) == CONST_INT				\
       && ((unsigned) INTVAL (XEXP (X, 1)) + 0x8000) < 0x10000)		\
   || (GET_CODE (X) == PLUS && XEXP (X, 0) == pic_offset_table_rtx 	\
       && flag_pic && GET_CODE (XEXP (X, 1)) == SYMBOL_REF)		\
   || (GET_CODE (X) == PLUS && XEXP (X, 0) == pic_offset_table_rtx 	\
       && flag_pic && GET_CODE (XEXP (X, 1)) == LABEL_REF))		\

#define GO_IF_NONINDEXED_ADDRESS(X, ADDR)  \
{ if (INDIRECTABLE_1_ADDRESS_P (X)) goto ADDR; }

/* Only labels on dispatch tables are valid for indexing from.  */
#define GO_IF_INDEXABLE_BASE(X, ADDR)				\
{ rtx temp;							\
  if (GET_CODE (X) == LABEL_REF					\
      && (temp = next_nonnote_insn (XEXP (X, 0))) != 0		\
      && GET_CODE (temp) == JUMP_INSN				\
      && (GET_CODE (PATTERN (temp)) == ADDR_VEC			\
	  || GET_CODE (PATTERN (temp)) == ADDR_DIFF_VEC))	\
    goto ADDR;							\
  if (GET_CODE (X) == REG && REG_OK_FOR_BASE_P (X)) goto ADDR; }

#define GO_IF_INDEXING(X, ADDR)	\
{ /* if (GET_CODE (X) == PLUS && LEGITIMATE_INDEX_P (XEXP (X, 0)))		\
    { GO_IF_INDEXABLE_BASE (XEXP (X, 1), ADDR); }			\
  if (GET_CODE (X) == PLUS && LEGITIMATE_INDEX_P (XEXP (X, 1)))		\
    { GO_IF_INDEXABLE_BASE (XEXP (X, 0), ADDR); } */ }

#define GO_IF_INDEXED_ADDRESS(X, ADDR)	 \
{ /* GO_IF_INDEXING (X, ADDR);						\
  if (GET_CODE (X) == PLUS)						\
    { if (GET_CODE (XEXP (X, 1)) == CONST_INT				\
	  && (unsigned) INTVAL (XEXP (X, 1)) + 0x8000 < 0x10000)		\
	{ rtx go_temp = XEXP (X, 0); GO_IF_INDEXING (go_temp, ADDR); }	\
      if (GET_CODE (XEXP (X, 0)) == CONST_INT				\
	  && (unsigned) INTVAL (XEXP (X, 0)) + 0x8000 < 0x10000)		\
	{ rtx go_temp = XEXP (X, 1); GO_IF_INDEXING (go_temp, ADDR); } } */ }

#define LEGITIMATE_INDEX_REG_P(X)   0

#define LEGITIMATE_INDEX_P(X) 0

#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)  \
{ GO_IF_NONINDEXED_ADDRESS (X, ADDR);			\
  /* GO_IF_INDEXED_ADDRESS (X, ADDR); */ }

/* Try machine-dependent ways of modifying an illegitimate address
   to be legitimate.  If we find one, return the new, valid address.
   This macro is used in only one place: `memory_address' in explow.c.

   OLDX is the address as it was before break_out_memory_refs was called.
   In some cases it is useful to look at this to decide what needs to be done.

   MODE and WIN are passed so that this macro can use
   GO_IF_LEGITIMATE_ADDRESS.

   It is always safe for this macro to do nothing.  It exists to recognize
   opportunities to optimize the output.

   For the 68000, we handle X+REG by loading X into a register R and
   using R+REG.  R will go in an address reg and indexing will be used.
   However, if REG is a broken-out memory address or multiplication,
   nothing needs to be done because REG can certainly go in an address reg.  */

#if 0
#define COPY_ONCE(Y) if (!copied) { Y = copy_rtx (Y); copied = ch = 1; }
#define LEGITIMIZE_ADDRESS(X,OLDX,MODE,WIN)   \
{ register int ch = (X) != (OLDX);					\
  if (GET_CODE (X) == PLUS)						\
    { int copied = 0;							\
      if (GET_CODE (XEXP (X, 0)) == MULT)				\
	{ COPY_ONCE (X); XEXP (X, 0) = force_operand (XEXP (X, 0), 0);}	\
      if (GET_CODE (XEXP (X, 1)) == MULT)				\
	{ COPY_ONCE (X); XEXP (X, 1) = force_operand (XEXP (X, 1), 0);}	\
      if (ch) { GO_IF_LEGITIMATE_ADDRESS (MODE, X, WIN); }		\
      if (GET_CODE (XEXP (X, 0)) == REG					\
	       || (GET_CODE (XEXP (X, 0)) == SIGN_EXTEND		\
		   && GET_CODE (XEXP (XEXP (X, 0), 0)) == REG		\
		   && GET_MODE (XEXP (XEXP (X, 0), 0)) == HImode))	\
	{ register rtx temp = gen_reg_rtx (Pmode);			\
	  register rtx val = force_operand (XEXP (X, 1), 0);		\
	  emit_move_insn (temp, val);					\
	  COPY_ONCE (X);						\
	  XEXP (X, 1) = temp;						\
	  goto WIN; }							\
      else if (GET_CODE (XEXP (X, 1)) == REG				\
	       || (GET_CODE (XEXP (X, 1)) == SIGN_EXTEND		\
		   && GET_CODE (XEXP (XEXP (X, 1), 0)) == REG		\
		   && GET_MODE (XEXP (XEXP (X, 1), 0)) == HImode))	\
	{ register rtx temp = gen_reg_rtx (Pmode);			\
	  register rtx val = force_operand (XEXP (X, 0), 0);		\
	  emit_move_insn (temp, val);					\
	  COPY_ONCE (X);						\
	  XEXP (X, 0) = temp;						\
	  goto WIN; }}}
#endif

#define LEGITIMIZE_ADDRESS(X,OLDX,MODE,WIN)   {}

/* Go to LABEL if ADDR (a legitimate address expression)
   has an effect that depends on the machine mode it is used for.
   On the 68000, only predecrement and postincrement address depend thus
   (the amount of decrement or increment being the length of the operand).  */

#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL)	\
 if (GET_CODE (ADDR) == POST_INC || GET_CODE (ADDR) == PRE_DEC) goto LABEL

/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.  */
#define CASE_VECTOR_MODE SImode

/* Define this if the tablejump instruction expects the table
   to contain offsets from the address of the table.
   Do not define this if the table should contain absolute addresses.  */
#ifndef TMS34010
#define CASE_VECTOR_PC_RELATIVE
#endif

/* Specify the tree operation to be used to convert reals to integers.  */
#define IMPLICIT_FIX_EXPR FIX_ROUND_EXPR

/* This is the kind of divide that is easiest to do in the general case.  */
#define EASY_DIV_EXPR TRUNC_DIV_EXPR

/* Define this as 1 if `char' should by default be signed; else as 0.  */
#define DEFAULT_SIGNED_CHAR 1

/* Don't cse the address of the function being compiled.  */
#define NO_RECURSIVE_FUNCTION_CSE

/* Max number of bytes we can move from memory to memory
   in one reasonably fast instruction.  */
#define MOVE_MAX 4

/* Define this if zero-extension is slow (more than one real instruction).  */
/* #define SLOW_ZERO_EXTEND */

/* Nonzero if access to memory by bytes is slow and undesirable.  */
#define SLOW_BYTE_ACCESS 0

/* Value is 1 if truncating an integer of INPREC bits to OUTPREC bits
   is done just by pretending it is already truncated.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 0	/* AKPM_NOTE */

/* We assume that the store-condition-codes instructions store 0 for false
   and some other value for true.  This is the value stored for true.  */

#define STORE_FLAG_VALUE 1

/* When a prototype says `char' or `short', really pass an `int'.  */
#ifdef AKPM_T
#define PROMOTE_PROTOTYPES
#endif

/* Specify the machine mode that pointers have.
   After generation of rtl, the compiler makes no further distinction
   between pointers and any other objects of this machine mode.  */
#define Pmode SImode

/* A function address in a call instruction
   is a byte address (for indexing purposes)
   so give the MEM rtx a byte's mode.  */
#define FUNCTION_MODE QImode

/* Compute the cost of computing a constant rtl expression RTX
   whose rtx-code is CODE.  The body of this macro is a portion
   of a switch statement.  If the code is computed here,
   return it with a return statement.  Otherwise, break from the switch.  */

#define CONST_COSTS(RTX,CODE,OUTER_CODE) \
  case CONST_INT:						\
    /* Constant zero is super cheap due to clr instruction.  */	\
    if (RTX == const0_rtx) return 0;				\
    /* Constants between -128 and 127 are cheap due to moveq */ \
    if (INTVAL (RTX) >= 1 && INTVAL (RTX) <= 32) return 1;	\
    if (INTVAL (RTX) >= -32 && INTVAL (RTX) <= -1) return 2;	\
    if (INTVAL (RTX) >= -32768 && INTVAL (RTX) <= 32767) return flags('c')?2:3;	\
  case CONST:							\
  case LABEL_REF:						\
  case SYMBOL_REF:						\
    return 4;							\
  case CONST_DOUBLE:					\
    return 5;

/* Compute the cost of various arithmetic operations.
   These are vaguely right for a 34010.  */
#define MULL_COST 8		/* Experimentally determined... */

#define RTX_COSTS(X,CODE,OUTER_CODE)				\
  case PLUS:							\
    break;							\
  case ASHIFT:							\
  case ASHIFTRT:						\
  case LSHIFT:							\
  case LSHIFTRT:						\
    if (GET_CODE (XEXP (X, 1)) == CONST_INT)			\
      return COSTS_N_INSNS (1);	 /* sra k,dn */		\
    break;							\
  case MULT:							\
    if (GET_CODE (XEXP (x, 1)) == CONST_INT			\
	&& exact_log2 (INTVAL (XEXP (x, 1))) >= 0)		\
      {								\
	  return COSTS_N_INSNS (2);	 /* sla k,dn */	\
	break;							\
      }								\
    else							\
      return COSTS_N_INSNS (MULL_COST);				\
    break;							\
  case DIV:							\
  case UDIV:							\
  case MOD:							\
  case UMOD:							\
    return COSTS_N_INSNS (37);	 /* div.l */

/* Tell final.c how to eliminate redundant test instructions.  */

/* Here we define machine-dependent flags and fields in cc_status
   (see `conditions.h').  */

/* Store in cc_status the expressions that the condition codes will
   describe after execution of an instruction whose pattern is EXP.
   Do not alter them if the instruction would not alter the cc's.  */

/* On the 68000, all the insns to store in an address register fail to
   set the cc's.  However, in some cases these instructions can make it
   possibly invalid to use the saved cc's.  In those cases we clear out
   some or all of the saved cc's so they won't be used.  */

#define NOTICE_UPDATE_CC(EXP,INSN) notice_update_cc (EXP, INSN)

#define OUTPUT_JUMP(NORMAL, FLOAT, NO_OV)  \
{ if (cc_prev_status.flags & CC_NO_OVERFLOW)			\
    return NO_OV;						\
  return NORMAL; }

/* Control the assembler format that we output.  */

/* Output at beginning of assembler file.  */

#define ASM_FILE_START(FILE)	;

/* Output to assembler file text saying following lines
   may contain character constants, extra white space, comments, etc.  */

#define ASM_APP_ON ""

/* Output to assembler file text saying following lines
   no longer contain unusual constructs.  */

#define ASM_APP_OFF ""

/* Output before read-only data.  */

#ifdef AKPM
#define TEXT_SECTION_ASM_OP "\t.text"
#else
extern char *AKPMtext;
#define TEXT_SECTION_ASM_OP AKPMtext
#endif

/* Output before writable data.  */

#define DATA_SECTION_ASM_OP "\t.data"

/* Here are four prefixes that are used by asm_fprintf to
   facilitate customization for alternate assembler syntaxes.
   Machines with no likelihood of an alternate syntax need not
   define these and need not use asm_fprintf.  */

/* The prefix for register names.  Note that REGISTER_NAMES
   is supposed to include this prefix.  */

#define REGISTER_PREFIX ""

/* The prefix for local labels.  You should be able to define this as
   an empty string, or any arbitrary string (such as ".", ".L%", etc)
   without having to make any other changes to account for the specific
   definition.  Note it is a string literal, not interpreted by printf
   and friends. */

#define LOCAL_LABEL_PREFIX ""

/* The prefix to add to user-visible assembler symbols.  */

#define USER_LABEL_PREFIX "_"

/* The prefix for immediate operands.  */

#define IMMEDIATE_PREFIX ""

/* How to refer to registers in assembler output.
   This sequence is indexed by compiler's hard-register-number (see above).  */

#define REGISTER_NAMES									\
{"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",		\
 "a8", "a9", "a10", "a11", "a12", "a13", "fp", "sp",	\
 "b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7",		\
 "b8", "b9", "b10", "b11", "b12", "b13", "b14", "b15" }

/* How to renumber registers for dbx and gdb. */

#define DBX_REGISTER_NUMBER(REGNO) (REGNO)

/* This is how to output the definition of a user-level label named NAME,
   such as the label on a static function or variable NAME.  */

#define ASM_OUTPUT_LABEL(FILE,NAME)	\
  do { assemble_name (FILE, NAME); fputs (":\n", FILE); } while (0)

/* This is how to output a command to make the user-level label named NAME
   defined for reference from other files.  */

#define GLOBAL_ASM_OP "\t.globl"
#define ASM_GLOBALIZE_LABEL(FILE,NAME)	\
  do { fprintf (FILE, "%s ", GLOBAL_ASM_OP);		\
       assemble_name (FILE, NAME);			\
       fputs ("\n", FILE);} while (0)

/* This is how to output a reference to a user-level label named NAME.
   `assemble_name' uses this.  */

#define ASM_OUTPUT_LABELREF(FILE,NAME)	\
  asm_fprintf (FILE, "%0U%s", NAME)

/* This is how to output an internal numbered label where
   PREFIX is the class of label and NUM is the number within the class.  */

#define ASM_OUTPUT_INTERNAL_LABEL(FILE,PREFIX,NUM)	\
  asm_fprintf (FILE, "%0L%s%d:\n", PREFIX, NUM)

/* This is how to store into the string LABEL
   the symbol_ref name of an internal numbered label where
   PREFIX is the class of label and NUM is the number within the class.
   This is suitable for output with `assemble_name'.  */

#define ASM_GENERATE_INTERNAL_LABEL(LABEL,PREFIX,NUM)	\
  sprintf (LABEL, "*%s%s%d", LOCAL_LABEL_PREFIX, PREFIX, NUM)

/* This is how to output an assembler line defining a `double' constant.  */

#define ASM_OUTPUT_DOUBLE(FILE,VALUE)  \
  fprintf (FILE, "\t.double 0r%.20g\n", (VALUE))

/* This is how to output an assembler line defining a `float' constant.  */

/* Sun's assembler can't handle floating constants written as floating.
   However, when cross-compiling, always use that in case format differs.  */

#ifdef CROSS_COMPILE

#define ASM_OUTPUT_FLOAT(FILE,VALUE)  \
  fprintf (FILE, "\t.float 0r%.10g\n", (VALUE))

#else

#define ASM_OUTPUT_FLOAT(FILE,VALUE)  \
do { union { float f; long l;} tem;			\
     tem.f = (VALUE);					\
     fprintf (FILE, "\t.long 0x%x\n", tem.l);	\
   } while (0)

#endif /* not CROSS_COMPILE */

#ifdef AKPM
/* This is how to output an assembler line defining an `int' constant.  */

#define ASM_OUTPUT_INT(FILE,VALUE)  \
( fprintf (FILE, "\t.long "),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

/* Likewise for `char' and `short' constants.  */

#define ASM_OUTPUT_SHORT(FILE,VALUE)  \
( fprintf (FILE, "\t.word "),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

#define ASM_OUTPUT_CHAR(FILE,VALUE)  \
( fprintf (FILE, "\t.byte "),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

#else
/* This is how to output an assembler line defining an `int' constant.  */

#define ASM_OUTPUT_INT(FILE,VALUE)  \
( fprintf (FILE, "\t.long "),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

/* Likewise for `char' and `short' constants.  */

#define ASM_OUTPUT_SHORT(FILE,VALUE)  \
( fprintf (FILE, "\t.word "),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

#define ASM_OUTPUT_CHAR(FILE,VALUE)  \
( fprintf (FILE, "\t.byte "),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

#endif		/* AKPM */

/* This is how to output an assembler line for a numeric constant byte.  */

#define ASM_OUTPUT_BYTE(FILE,VALUE)  \
  fprintf (FILE, "\t.byte 0x%x\n", (VALUE))

/* This is how to output an insn to push a register on the stack.
   It need not be very fast code.  */

#define ASM_OUTPUT_REG_PUSH(FILE,REGNO)  \
  asm_fprintf (FILE, "\tmovel %s,%Rsp@-\n", reg_names[REGNO])

/* This is how to output an insn to pop a register from the stack.
   It need not be very fast code.  */

#define ASM_OUTPUT_REG_POP(FILE,REGNO)  \
  asm_fprintf (FILE, "\tmovel %Rsp@+,%s\n", reg_names[REGNO])

/* This is how to output an element of a case-vector that is absolute.
   (The 68000 does not use such vectors,
   but we must define this macro anyway.)  */

#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE)  \
  asm_fprintf (FILE, "\t.long %LL%d\n", VALUE)

/* This is how to output an element of a case-vector that is relative.  */

#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, VALUE, REL)  \
  asm_fprintf (FILE, "\t.word %LL%d-%LL%d\n", VALUE, REL)

/* This is how to output an assembler line
   that says to advance the location counter
   to a multiple of 2**LOG bytes.  */

/* We don't have a way to align to more than a two-byte boundary, so do the
   best we can and don't complain.  */
#define ASM_OUTPUT_ALIGN(FILE,LOG)	\
  if ((LOG) >= 1)			\
    fprintf (FILE, "\t.even\n");

#define ASM_OUTPUT_SKIP(FILE,SIZE)  \
  fprintf (FILE, "\t.space %u\n", (SIZE))

/* This says how to output an assembler line
   to define a global common symbol.  */

#define EXTRA_SECTIONS	in_bss		/* varasm.c */

#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)	\
do {													\
	fprintf((FILE), "\t.bss\n\t.even\n\t.globl ");		\
	assemble_name((FILE), (NAME));						\
	fprintf((FILE), "\n");								\
	assemble_name((FILE), (NAME));						\
	fprintf((FILE), ":\t.space %u\n", (ROUNDED));		\
	if (in_section == in_text)							\
	{													\
		in_section = in_bss;							\
		text_section();									\
	}													\
	else if (in_section == in_data)						\
	{													\
		in_section = in_bss;							\
		data_section();									\
	}													\
} while (0)

/* This says how to output an assembler line
   to define a local common symbol.  */

#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)	\
do {													\
	fprintf((FILE), "\t.bss\n\t.even\n");				\
	assemble_name((FILE), (NAME));						\
	fprintf((FILE), ":\t.space %u\n", (ROUNDED));		\
	if (in_section == in_text)							\
	{													\
		in_section = in_bss;							\
		text_section();									\
	}													\
	else if (in_section == in_data)						\
	{													\
		in_section = in_bss;							\
		data_section();									\
	}													\
} while (0)

/* Store in OUTPUT a string (made with alloca) containing
   an assembler-name for a local static variable named NAME.
   LABELNO is an integer which is different for each call.  */

#define ASM_FORMAT_PRIVATE_NAME(OUTPUT, NAME, LABELNO)	\
( (OUTPUT) = (char *) alloca (strlen ((NAME)) + 10),	\
  sprintf ((OUTPUT), "%s.%d", (NAME), (LABELNO)))

/* Define the parentheses used to group arithmetic operations
   in assembler code.  */

#define ASM_OPEN_PAREN "("
#define ASM_CLOSE_PAREN ")"

/* Define results of standard character escape sequences.  */
#define TARGET_BELL 007
#define TARGET_BS 010
#define TARGET_TAB 011
#define TARGET_NEWLINE 012
#define TARGET_VT 013
#define TARGET_FF 014
#define TARGET_CR 015

/* Output a float value (represented as a C double) as an immediate operand.
   This macro is a 68k-specific macro.  */
#define ASM_OUTPUT_FLOAT_OPERAND(FILE,VALUE)				\
  asm_fprintf (FILE, "%I0r%.9g", (VALUE))

/* Output a double value (represented as a C double) as an immediate operand.
   This macro is a 68k-specific macro.  */
#define ASM_OUTPUT_DOUBLE_OPERAND(FILE,VALUE)				\
  asm_fprintf (FILE, "%I0r%.20g", (VALUE))

/* Print operand X (an rtx) in assembler syntax to file FILE.
   CODE is a letter or dot (`z' in `%z0') or 0 if no letter was specified.
   For `%' followed by punctuation, CODE is the punctuation and X is null.

   On the 68000, we use several CODE characters:
   '.' for dot needed in Motorola-style opcode names.
   '-' for an operand pushing on the stack:
       sp@-, -(sp) or -(%sp) depending on the style of syntax.
   '+' for an operand pushing on the stack:
       sp@+, (sp)+ or (%sp)+ depending on the style of syntax.
   '@' for a reference to the top word on the stack:
       sp@, (sp) or (%sp) depending on the style of syntax.
   '#' for an immediate operand prefix (# in MIT and Motorola syntax
       but & in SGS syntax).
   '!' for the fpcr register (used in some float-to-fixed conversions).
   '$' for the letter `s' in an op code, but only on the 68040.
   '&' for the letter `d' in an op code, but only on the 68040.

   'b' for byte insn (no effect, on the Sun; this is for the ISI).
   'd' to force memory addressing to be absolute, not relative.
   'f' for float insn (print a CONST_DOUBLE as a float rather than in hex) */

#define PRINT_OPERAND_PUNCT_VALID_P(CODE)				\
  ((CODE) == '.' || (CODE) == '#' || (CODE) == '-'			\
   || (CODE) == '+' || (CODE) == '@' || (CODE) == '!'			\
   || (CODE) == '$' || (CODE) == '&')

#ifdef HOST_WORDS_BIG_ENDIAN
#define PRINT_OPERAND_EXTRACT_FLOAT(X)					\
      u.i[0] = CONST_DOUBLE_LOW (X); u.i[1] = CONST_DOUBLE_HIGH (X);
#else
#define PRINT_OPERAND_EXTRACT_FLOAT(X)					\
      u.i[0] = CONST_DOUBLE_HIGH (X); u.i[1] = CONST_DOUBLE_LOW (X);
#endif

#ifdef CROSS_COMPILE
#define PRINT_OPERAND_PRINT_FLOAT(CODE, FILE)   \
  ASM_OUTPUT_FLOAT_OPERAND (FILE, u1.f);
#else
#define PRINT_OPERAND_PRINT_FLOAT(CODE, FILE)   \
{ if (CODE == 'f')							\
    ASM_OUTPUT_FLOAT_OPERAND (FILE, u1.f);				\
  else									\
    asm_fprintf (FILE, "%I0x%x", u1.i); }
#endif

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

   See m68k.c for the m68k specific codes.  */

#define PRINT_OPERAND(FILE, X, CODE) print_operand (FILE, X, CODE)

/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand that is a memory
   reference whose address is ADDR.  ADDR is an RTL expression.

   On some machines, the syntax for a symbolic address depends on
   the section that the address refers to.  On these machines,
   define the macro `ENCODE_SECTION_INFO' to store the information
   into the `symbol_ref', and then check for it here.  */

#define PRINT_OPERAND_ADDRESS(FILE, ADDR) print_operand_address (FILE, ADDR)


/* Define functions defined in aux-output.c and used in templates.  */

extern char *output_move_double ();
extern char *output_move_const_single ();
extern char *output_move_const_double ();
extern char *output_btst ();

/*
Local variables:
version-control: t
End:
*/

#define BYTE_LOADS_SIGN_EXTEND
#define LOAD_EXTEND_OP(MODE)	SIGN_EXTEND
#define SHORT_IMMEDIATES_SIGN_EXTEND

#define ASM_OUTPUT_ASCII(MYFILE, MYSTRING, MYLENGTH)			\
	asm_output_ascii(MYFILE, MYSTRING, MYLENGTH)

#define DM_BYTE		1
#define DM_SHORT	2
#define DM_LONG		4
#define DM_ANY		(DM_BYTE|DM_SHORT|DM_LONG)
#define DM_SEXT		8
#define DM_ZEXT		16
#define DM_STRICT	32
#define DM_TRUNC	64

#define PANIC()	do { fprintf(stderr, "PANIC in line %d, file %s\n", __LINE__, __FILE__); abort(); } while (0)
char *do_seq();

/* If this is zero, no structs are put in regs */
#define MAX_FIXED_MODE_SIZE	(flags('M') ? 32:0)

/* Use memcpy(), etc rather than bcopy() */
#define TARGET_MEM_FUNCTIONS

/* Value is the number of byte of arguments automatically
   popped when returning from a subroutine call.
   FUNTYPE is the data type of the function (as a tree),
   or for a library call it is an identifier node for the subroutine name.
   SIZE is the number of bytes of arguments passed on the stack.

   On the 68000, the RTS insn cannot pop anything.
   On the 68010, the RTD insn may be used to pop them if the number
     of args is fixed, but if the number is variable then the caller
     must pop them all.  RTD can't be used for library calls now
     because the library is compiled with the Unix compiler.
   Use of RTD is a selectable option, since it is incompatible with
   standard Unix calling sequences.  If the option is not selected,
   the caller must always pop the args.  */

#ifdef AKPM_RETS
#define RETURN_POPS_ARGS(FUNTYPE,SIZE)						\
  ((TARGET_RTD												\
	&& TREE_CODE (FUNTYPE) != IDENTIFIER_NODE				\
    && (TYPE_ARG_TYPES (FUNTYPE) == 0						\
	|| (TREE_VALUE (tree_last (TYPE_ARG_TYPES (FUNTYPE)))	\
	    == void_type_node)))								\
   ? (SIZE) : 0)
#else
#define RETURN_POPS_ARGS(FUNTYPE, SIZE) 0
#endif

#define cfpas current_function_pretend_args_size
#define SETUP_INCOMING_VARARGS(args_so_far, passed_mode, passed_type, Cfpas, second_time)		\
{\
	Cfpas = 12 - args_so_far;\
}
#if 0
extern enum reg_class secondary_reload_class(enum reg_class, enum machine_mode, void *);

#define SECONDARY_RELOAD_CLASS(CLASS, MODE, X) \
		secondary_reload_class(CLASS, MODE, X)
#endif
#define EXTRA_CONSTRAINT(VALUE, C)	extra_constraint(VALUE, C)

