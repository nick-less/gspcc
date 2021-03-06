/* Definitions of target machine for GNU compiler, for Hitachi Super-H.
   Copyright (C) 1993 Free Software Foundation, Inc.

   Contributed by Steve Chamberlain (sac@cygnus.com)

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


/* Run-time Target Specification.  */
#define TARGET_SH

#define TARGET_VERSION  \
  fputs (" (Hitachi SH)", stderr);

/* Generate SDB debugging information.  */

#define SDB_DEBUGGING_INFO  1

#define SDB_DELIM ";"

#define CPP_PREDEFINES "-D__sh__ -Acpu(sh) -Amachine(sh)"


/* Omitting the frame pointer is a very good idea on the SH */

#define OPTIMIZATION_OPTIONS(OPTIMIZE)  \
{					\
  if (OPTIMIZE)				\
    flag_omit_frame_pointer = 1;	\
   if (OPTIMIZE==0)OPTIMIZE=1;		\
}

/* Run-time compilation parameters selecting different hardware subsets. */

extern int target_flags;
#define ISIZE_BIT  1
#define FAST_BIT   2

#define MAC_BIT    8
#define RTL_BIT    16
#define DT_BIT     32
#define DALIGN_BIT 64
#define SH0_BIT	128
#define SH1_BIT	256
#define SH2_BIT	512
#define SH3_BIT	1024
#define C_BIT	2048
#define R_BIT     (1<<12)
#define SPACE_BIT (1<<13)

/* Nonzero if we should generate code using type 0 insns */
#define TARGET_SH0 (target_flags & SH0_BIT)

/* Nonzero if we should generate code using type 1 insns */
#define TARGET_SH1 (target_flags & SH1_BIT)

/* Nonzero if we should generate code using type 2 insns */
#define TARGET_SH2 (target_flags & SH2_BIT)

/* Nonzero if we should generate code using type 3 insns */
#define TARGET_SH3 (target_flags & SH3_BIT)

/* Nonzero if we should generate faster code rather than smaller code */
#define TARGET_FASTCODE   (target_flags & FAST_BIT)

/* Nonzero if we should generate faster code rather than smaller code */
#define TARGET_SMALLCODE   (target_flags & SPACE_BIT)

/* Nonzero if we should dump out instruction size info */
#define TARGET_DUMPISIZE  (target_flags & ISIZE_BIT)

/* Nonzero if we should try to generate mac instructions */
#define TARGET_MAC        (target_flags & MAC_BIT)

/* Nonzero if we should dump the rtl in the assembly file. */
#define TARGET_DUMP_RTL	  (target_flags & RTL_BIT)

/* Nonzero if we should dump the rtl somewher else. */
#define TARGET_DUMP_R	  (target_flags & R_BIT)

/* Nonzero to align doubles on 64 bit boundaries */
#define TARGET_ALIGN_DOUBLE (target_flags & DALIGN_BIT)


/* Nonzero if Combine dumping wanted */
#define TARGET_CDUMP (target_flags & C_BIT)

#define TARGET_SWITCHES  	\
{ {"isize", 	( ISIZE_BIT)  },\
  {"space", 	( SPACE_BIT)   },\
  {"0",	        ( SH0_BIT) },\
  {"1",	        ( SH1_BIT) },\
  {"2",	        ( SH2_BIT) },\
  {"3",	        ( SH3_BIT) },\
  {"ac",  	( MAC_BIT)    },\
  {"dalign",  	( DALIGN_BIT) },\
  {"c",  	( C_BIT) },\
  {"r",  	( RTL_BIT) },\
  {"R",  	( R_BIT) },\
  {"",   	TARGET_DEFAULT} \
}

#define TARGET_DEFAULT  FAST_BIT

#define OVERRIDE_OPTIONS override_options();


/* Target machine storage Layout.  */

/* Define to use software floating point emulator for REAL_ARITHMETIC and
   decimal <-> binary conversion. */
#define REAL_ARITHMETIC

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields.  */
#define BITS_BIG_ENDIAN  0

/* Define this if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN 1

/* Define this if most significant word of a multiword number is the lowest
   numbered.  */
#define WORDS_BIG_ENDIAN 1

/* Number of bits in an addressable storage unit */
#define BITS_PER_UNIT  8

/* Width in bits of a "word", which is the contents of a machine register.
   Note that this is not necessarily the width of data type `int';
   if using 16-bit ints on a 68000, this would still be 32.
   But on a machine with 16-bit registers, this would be 16.  */
#define BITS_PER_WORD  32
#define MAX_BITS_PER_WORD 32

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD	4

/* Width in bits of a pointer.
   See also the macro `Pmode' defined below.  */
#define POINTER_SIZE  32

/* Allocation boundary (in *bits*) for storing arguments in argument list.  */
#define PARM_BOUNDARY  	32

/* Boundary (in *bits*) on which stack pointer should be aligned.  */
#define STACK_BOUNDARY  32

/* Allocation boundary (in *bits*) for the code of a function.  */
#define FUNCTION_BOUNDARY  16

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY  32

/* No data type wants to be aligned rounder than this.  */
#define BIGGEST_ALIGNMENT  (TARGET_ALIGN_DOUBLE ? 64 : 32)

/* The best alignment to use in cases where we have a choice.  */
#define FASTEST_ALIGNMENT 32

/* Every structures size must be a multiple of 32 bits.  */
#define STRUCTURE_SIZE_BOUNDARY 32

/* Make strings word-aligned so strcpy from constants will be faster.  */
#define CONSTANT_ALIGNMENT(EXP, ALIGN)  \
  ((TREE_CODE (EXP) == STRING_CST	\
    && (ALIGN) < FASTEST_ALIGNMENT)	\
   ? FASTEST_ALIGNMENT : (ALIGN))

/* Make arrays of chars word-aligned for the same reasons.  */
#define DATA_ALIGNMENT(TYPE, ALIGN)		\
  (TREE_CODE (TYPE) == ARRAY_TYPE		\
   && TYPE_MODE (TREE_TYPE (TYPE)) == QImode	\
   && (ALIGN) < FASTEST_ALIGNMENT ? FASTEST_ALIGNMENT : (ALIGN))

/* Set this nonzero if move instructions will actually fail to work
   when given unaligned data.  */
#define STRICT_ALIGNMENT 1


/* Standard register usage.  */

/* Register allocation for our first guess 

	r0-r3		scratch
	r4-r7		args in and out
	r8-r12		call saved
	r13		assembler temp
	r14		frame pointer
	r15		stack pointer
	ap		arg pointer (doesn't really exist, always eliminated)
	pr		subroutine return address
	t               t bit
	mach		multiply/accumulate result
	macl
*/

/* Number of actual hardware registers.
   The hardware registers are assigned numbers for the compiler
   from 0 to just below FIRST_PSEUDO_REGISTER.
   All registers that the compiler knows about must be given numbers,
   even those that are not normally considered general registers.

   SH has 16 integer registers and 4 control registers + the arg
   pointer */

#define FIRST_PSEUDO_REGISTER 22

#define PR_REG   17
#define T_REG    18
#define GBR_REG  19
#define MACH_REG 20
#define MACL_REG 21


/* 1 for registers that have pervasive standard uses
   and are not available for the register allocator.  */
 /*  r0  r1  r2  r3  r4  r5  r6  r7  r8  r9  r10 r11 r12 r13 r14 r15 ap  pr  t   gbr mh   ml */
#define FIXED_REGISTERS  \
   { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  1,  0,  1,  1, 1,   1}

/* 1 for registers not available across function calls.
   These must include the FIXED_REGISTERS and also any
   registers that can be used without being saved.
   The latter must include the registers where values are returned
   and the register where structure-value addresses are passed.
   Aside from that, you can include as many other registers as you like.  */

 /*  r0  r1  r2  r3  r4  r5  r6  r7  r8 r9  r10 r11 r12 r13 r14 r15 ap  pr  t   gbr mh  ml */
#define CALL_USED_REGISTERS \
   { 1,  1,  1,  1,  1,  1,  1,  1,  0, 0,  0,  0,  0,  1,  0,  1,  1,  0,  1,  1, 1, 1}

/* Return number of consecutive hard regs needed starting at reg REGNO
   to hold something of mode MODE.
   This is ordinarily the length in words of a value of mode MODE
   but can be less for certain modes in special long registers.

   On the SH regs are UNITS_PER_WORD bits wide; */
#define HARD_REGNO_NREGS(REGNO, MODE)  \
   (((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))

/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.
   We may keep double values in even registers */

#define HARD_REGNO_MODE_OK(REGNO, MODE)  \
  ((TARGET_ALIGN_DOUBLE && GET_MODE_SIZE(MODE) > 4) ? (((REGNO)&1)==0) : 1)

/* Value is 1 if it is a good idea to tie two pseudo registers
   when one has mode MODE1 and one has mode MODE2.
   If HARD_REGNO_MODE_OK could produce different values for MODE1 and MODE2,
   for any hard reg, then this must be 0 for correct output.  */

#define MODES_TIEABLE_P(MODE1, MODE2) \
  ((MODE1) == (MODE2) || GET_MODE_CLASS (MODE1) == GET_MODE_CLASS (MODE2))

/* Specify the registers used for certain standard purposes.
   The values of these macros are register numbers.  */

/* Define this if the program counter is overloaded on a register.  */
/* #define PC_REGNUM		15*/

/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM	15

/* Base register for access to local variables of the function.  */
#define FRAME_POINTER_REGNUM	14

/* Value should be nonzero if functions must have frame pointers.
   Zero means the frame pointer need not be set up (and parms may be accessed
   via the stack pointer) in functions that seem suitable.  */


#define FRAME_POINTER_REQUIRED	(get_frame_size() > 1000)

/* Definitions for register eliminations.

   We have two registers that can be eliminated on the m88k.  First, the
   frame pointer register can often be eliminated in favor of the stack
   pointer register.  Secondly, the argument pointer register can always be
   eliminated; it is replaced with either the stack or frame pointer.  */

/* This is an array of structures.  Each structure initializes one pair
   of eliminable registers.  The "from" register number is given first,
   followed by "to".  Eliminations of the same "from" register are listed
   in order of preference.  */

#define ELIMINABLE_REGS				\
{{ FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},	\
 { ARG_POINTER_REGNUM,   STACK_POINTER_REGNUM},	\
 { ARG_POINTER_REGNUM,   FRAME_POINTER_REGNUM},}

/* Given FROM and TO register numbers, say whether this elimination
   is allowed.  */
#define CAN_ELIMINATE(FROM, TO) \
  (!((FROM) == FRAME_POINTER_REGNUM && FRAME_POINTER_REQUIRED))

/* Define the offset between two registers, one to be eliminated, and the other
   its replacement, at the start of a routine.  */

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
  OFFSET = initial_elimination_offset (FROM, TO)

/* Base register for access to arguments of the function.  */
#define ARG_POINTER_REGNUM	16

/* Register in which the static-chain is passed to a function.  */
#define STATIC_CHAIN_REGNUM	13

/* The register in which a struct value address is passed */

#define STRUCT_VALUE_REGNUM 3



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

/* The SH has two sorts of general registers, R0 and the rest.  R0 can 
   be used as the destination of some of the arithmetic ops. There are
   also some special purpose registers; the T bit register, the
   Procedure Return Register and the Multipy Accumulate Registers */

enum reg_class
{
  NO_REGS,
  R0_REGS,
  GENERAL_REGS,
  PR_REGS,
  T_REGS,
  MAC_REGS,
  ALL_REGS,
  LIM_REG_CLASSES
};

#define N_REG_CLASSES  (int) LIM_REG_CLASSES

/* Give names of register classes as strings for dump file.   */
#define REG_CLASS_NAMES  \
{			\
  "NO_REGS",		\
  "R0_REGS",		\
  "GENERAL_REGS",	\
  "PR_REGS",		\
  "T_REGS",		\
  "MAC_REGS",		\
  "ALL_REGS",		\
}

/* Define which registers fit in which classes.
   This is an initializer for a vector of HARD_REG_SET
   of length N_REG_CLASSES.  */

#define REG_CLASS_CONTENTS      \
{				\
  0x000000,  /* NO_REGS      */	\
  0x000001,  /* R0_REGS      */	\
  0x01FFFF,  /* GENERAL_REGS */	\
  0x020000,  /* PR_REGS      */	\
  0x040000,  /* T_REGS       */	\
  0x300000,  /* MAC_REGS     */	\
  0x37FFFF   /* ALL_REGS     */	\
}

/* The same information, inverted:
   Return the class number of the smallest class containing
   reg number REGNO.  This could be a conditional expression
   or could index an array.  */

extern int regno_reg_class[];
#define REGNO_REG_CLASS(REGNO) regno_reg_class[REGNO]

/* The order in which register should be allocated.  */
#define REG_ALLOC_ORDER  \
  { 1,2,3,7,4,5,6,0,8,9,10,11,12,13,14,15,16,17,18,19,20,21}

/* The class value for index registers, and the one for base regs.  */
#define INDEX_REG_CLASS  R0_REGS
#define BASE_REG_CLASS	 GENERAL_REGS

/* Get reg_class from a letter such as appears in the machine 
   description. */
extern enum reg_class reg_class_from_letter[];

#define REG_CLASS_FROM_LETTER(C) \
   ( (C) >= 'a' && (C) <= 'z' ? reg_class_from_letter[(C)-'a'] : NO_REGS )


/* The letters I, J, K, L and M in a register constraint string
   can be used to stand for particular ranges of immediate operands.
   This macro defines what the ranges are.
   C is the letter, and VALUE is a constant value.
   Return 1 if VALUE is in the range specified by C.
	I: arithmetic operand -127..128, as used in add, sub, etc
	L: logical operand 0..255, as used in and, or, etc.
	M: constant 1
	K: shift operand 1,2,8 or 16 */


#define CONST_OK_FOR_I(VALUE) (((int)(VALUE))>= -128 && ((int)(VALUE)) <= 127)
#define CONST_OK_FOR_L(VALUE) (((int)(VALUE))>=    0 && ((int)(VALUE)) <= 255)
#define CONST_OK_FOR_M(VALUE) ((VALUE)==1)
#define CONST_OK_FOR_K(VALUE) ((VALUE)==1||(VALUE)==2||(VALUE)==8||(VALUE)==16)

#define CONST_OK_FOR_LETTER_P(VALUE, C)     \
     ((C) == 'I' ? CONST_OK_FOR_I (VALUE)   \
    : (C) == 'L' ? CONST_OK_FOR_L (VALUE)   \
    : (C) == 'M' ? CONST_OK_FOR_M (VALUE)   \
    : (C) == 'K' ? CONST_OK_FOR_K (VALUE)   \
    : 0)

/* Similar, but for floating constants, and defining letters G and H.
   Here VALUE is the CONST_DOUBLE rtx itself.  */

#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C) \
   ((C) == 'G' ? CONST_OK_FOR_I (CONST_DOUBLE_HIGH (VALUE)) \
	      && CONST_OK_FOR_I (CONST_DOUBLE_LOW (VALUE))  \
    : 0)

/* Given an rtx X being reloaded into a reg required to be
   in class CLASS, return the class of reg to actually use.
   In general this is just CLASS; but on some machines
   in some cases it is preferable to use a more restrictive class.  */

#define PREFERRED_RELOAD_CLASS(X, CLASS)  CLASS

/* Return the register class of a scratch register needed to copy IN into
   or out of a register in CLASS in MODE.  If it can be done directly,
   NO_REGS is returned.  */

#define SECONDARY_RELOAD_CLASS(CLASS, MODE, X) NO_REGS

/* Return the maximum number of consecutive registers
   needed to represent mode MODE in a register of class CLASS. 

   On SH this is the size of MODE in words */
#define CLASS_MAX_NREGS(CLASS, MODE)  \
     ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)


/* Stack layout; function entry, exit and calling.  */

/* Define the number of register that can hold parameters.
   These two macros are used only in other macro definitions below.  */
#define NPARM_REGS 4
#define FIRST_PARM_REG 4
#define FIRST_RET_REG 4

/* Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  */
#define STACK_GROWS_DOWNWARD  

/* Define this if the nominal address of the stack frame
   is at the high-address end of the local variables;
   that is, each additional local variable allocated
   goes at a more negative offset in the frame.  */
#define FRAME_GROWS_DOWNWARD 

/* Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  */
#define STARTING_FRAME_OFFSET  0

/* If we generate an insn to push BYTES bytes,
   this says how many the stack pointer really advances by.  */
#define PUSH_ROUNDING(NPUSHED)  (((NPUSHED) + 3) & ~3)

/* Offset of first parameter from the argument pointer register value.  */
#define FIRST_PARM_OFFSET(FNDECL)  0

/* Value is the number of byte of arguments automatically
   popped when returning from a subroutine call.
   FUNTYPE is the data type of the function (as a tree),
   or for a library call it is an identifier node for the subroutine name.
   SIZE is the number of bytes of arguments passed on the stack.

   On the SH, the caller does not pop any of its arguments that were passed
   on the stack.  */
#define RETURN_POPS_ARGS(FUNTYPE, SIZE)  0

/* Define how to find the value returned by a function.
   VALTYPE is the data type of the value (as a tree).
   If the precise function being called is known, FUNC is its FUNCTION_DECL;
   otherwise, FUNC is 0.  */
#define FUNCTION_VALUE(VALTYPE, FUNC)  \
    gen_rtx (REG, TYPE_MODE (VALTYPE), FIRST_RET_REG)

/* Define how to find the value returned by a library function
   assuming the value has mode MODE.  */
#define LIBCALL_VALUE(MODE)  \
    gen_rtx (REG, MODE, FIRST_RET_REG)

/* 1 if N is a possible register number for a function value.
   On the SH, only r4 can return results.  */
#define FUNCTION_VALUE_REGNO_P(REGNO)  \
	  ((REGNO) == FIRST_RET_REG)

/* 1 if N is a possible register number for function argument passing.*/

#define FUNCTION_ARG_REGNO_P(REGNO)  \
  ((REGNO) >= FIRST_PARM_REG && (REGNO) < (NPARM_REGS + FIRST_PARM_REG))



/* Define a data type for recording info about an argument list
   during the scan of that argument list.  This data type should
   hold all necessary information about the function itself
   and about the args processed so far, enough to enable macros
   such as FUNCTION_ARG to determine where the next arg should go.

   On SH, this is a single integer, which is a number of words
   of arguments scanned so far (including the invisible argument,
   if any, which holds the structure-value-address).
   Thus NARGREGS or more means all following args should go on the stack.  */

#define CUMULATIVE_ARGS  int

#define ROUND_ADVANCE(SIZE)	\
  ((SIZE + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

/* Round a register number up to a proper boundary for an arg of mode 
   MODE. 
   
   We round to an even reg for things larger than a word */

#define ROUND_REG(X, MODE) 					\
  ((TARGET_ALIGN_DOUBLE 					\
   && GET_MODE_UNIT_SIZE ((MODE)) > UNITS_PER_WORD) 		\
   ? ((X) + ((X) & 1)) : (X))


/* Initialize a variable CUM of type CUMULATIVE_ARGS
   for a call to a function whose data type is FNTYPE.
   For a library call, FNTYPE is 0.

   On SH, the offset always starts at 0: the first parm reg is always
   the same reg.  */

#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME)  \
  ((CUM) = 0)

/* Update the data in CUM to advance over an argument
   of mode MODE and data type TYPE.
   (TYPE is null for libcalls where that information may not be
   available.)  */

#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)	\
 ((CUM) = (ROUND_REG ((CUM), (MODE))			\
	   + ((MODE) != BLKmode				\
	      ? ROUND_ADVANCE (GET_MODE_SIZE (MODE))	\
	      : ROUND_ADVANCE (int_size_in_bytes (TYPE)))))

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
    (otherwise it is an extra parameter matching an ellipsis).

   On SH the first args are normally in registers
   and the rest are pushed.  Any arg that starts within the first
   NPARM_REGS words is at least partially passed in a register unless
   its data type forbids.  */

#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED)			\
  (NAMED && ROUND_REG ((CUM), (MODE)) < NPARM_REGS		\
   && ((TYPE)==0 || ! TREE_ADDRESSABLE ((tree)(TYPE)))		\
   && ((TYPE)==0 || (MODE) != BLKmode				\
       || (TYPE_ALIGN ((TYPE)) % PARM_BOUNDARY == 0))		\
   ? gen_rtx (REG, (MODE),					\
  	    (FIRST_PARM_REG + ROUND_REG ((CUM), (MODE))))	\
   : 0)

/* For an arg passed partly in registers and partly in memory,
   this is the number of registers used.
   For args passed entirely in registers or entirely in memory, zero.
   Any arg that starts in the first NPARM_REGS regs but won't entirely
   fit in them needs partial registers on the SH.  */

#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED) 		\
  ((ROUND_REG ((CUM), (MODE)) < NPARM_REGS				\
    && ((TYPE)==0 || ! TREE_ADDRESSABLE ((tree)(TYPE)))			\
    && ((TYPE)==0 || (MODE) != BLKmode					\
	|| (TYPE_ALIGN ((TYPE)) % PARM_BOUNDARY == 0))			\
    && (ROUND_REG ((CUM), (MODE))					\
	+ ((MODE) == BLKmode						\
	   ? ROUND_ADVANCE (int_size_in_bytes (TYPE))			\
	   : ROUND_ADVANCE (GET_MODE_SIZE (MODE)))) - NPARM_REGS > 0)	\
   ? (NPARM_REGS - ROUND_REG ((CUM), (MODE)))				\
   : 0)

extern int current_function_anonymous_args;

/* Perform any needed actions needed for a function that is receiving a
   variable number of arguments. */

#define SETUP_INCOMING_VARARGS(ASF, MODE, TYPE, PAS, ST) \
  current_function_anonymous_args = 1;


/* Call the function profiler with a given profile label. */

#define FUNCTION_PROFILER(STREAM,LABELNO)			\
{								\
	fprintf(STREAM, "	trapa	#5\n");			\
 	fprintf(STREAM, "	.align	2\n");			\
	fprintf(STREAM, "	.long	LP%d\n", (LABELNO));	\
}


/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in
   functions that have frame pointers.
   No definition is equivalent to always zero.  */

#define EXIT_IGNORE_STACK 0

/* Generate the assembly code for function exit 
   Just dump out any accumulated constant table.*/

#define FUNCTION_EPILOGUE(STREAM, SIZE)  \
  dump_constants(0);  

/* Output assembler code for a block containing the constant parts
   of a trampoline, leaving space for the variable parts.

   On the SH, the trapoline looks like
   1 0000 D301     		mov.l	l1,r3
   2 0002 DD02     	   	mov.l	l2,r13
   3 0004 4D2B     		jmp	@r13
   4 0006 200B     		or	r0,r0
   5 0008 00000000 	l1:  	.long   function
   6 000c 00000000 	l2:	.long   area	
*/
#define TRAMPOLINE_TEMPLATE(FILE)  		\
{						\
  fprintf ((FILE), "	.word	0xd301\n");	\
  fprintf ((FILE), "	.word	0xdd02\n");	\
  fprintf ((FILE), "	.word	0x4d2b\n");        \
  fprintf ((FILE), "	.word	0x200b\n");	\
  fprintf ((FILE), "	.long	0\n");		\
  fprintf ((FILE), "	.long	0\n");		\
}

/* Length in units of the trampoline for entering a nested function.  */
#define TRAMPOLINE_SIZE  16

/* Alignment required for a trampoline in units.  */
#define TRAMPOLINE_ALIGN  4

/* Emit RTL insns to initialize the variable parts of a trampoline.
   FNADDR is an RTX for the address of the function's pure code.
   CXT is an RTX for the static chain value for the function.  */

#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT)  \
{									\
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant ((TRAMP), 8)),	\
		  (CXT));						\
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant ((TRAMP), 12)),	\
		  (FNADDR));						\
}


/* Addressing modes, and classification of registers for them.  */

/*#define HAVE_POST_INCREMENT  1*/
/*#define HAVE_PRE_INCREMENT   1*/
/*#define HAVE_POST_DECREMENT  1*/
/*#define HAVE_PRE_DECREMENT   1*/

/* Macros to check register numbers against specific register classes.  */

/* These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.

*/
#define REGNO_OK_FOR_BASE_P(REGNO)  \
  ((REGNO) < PR_REG || (unsigned) reg_renumber[(REGNO)] < PR_REG)

#define REGNO_OK_FOR_INDEX_P(REGNO)   ((REGNO)==0)

/* Maximum number of registers that can appear in a valid memory 
   address. */

#define MAX_REGS_PER_ADDRESS 4

/* Recognize any constant value that is a valid address.  */

#define CONSTANT_ADDRESS_P(X) 	\
  (GET_CODE (X) == LABEL_REF)

/* Nonzero if the constant value X is a legitimate general operand.
   It is given that X satisfies CONSTANT_P or is a CONST_DOUBLE.

   On the SH, allow anything but a double */

#define LEGITIMATE_CONSTANT_P(X)  (GET_CODE(X) != CONST_DOUBLE)

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx
   and check its validity for a certain class.
   We have two alternate definitions for each of them.
   The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.
   The symbol REG_OK_STRICT causes the latter definition to be used.  */

#ifndef REG_OK_STRICT

/* Nonzero if X is a hard reg that can be used as a base reg
   or if it is a pseudo reg.  */
#define REG_OK_FOR_BASE_P(X) \
    	(REGNO (X) <= 16 || REGNO (X) >= FIRST_PSEUDO_REGISTER)

/* Nonzero if X is a hard reg that can be used as an index
   or if it is a pseudo reg.  */
#define REG_OK_FOR_INDEX_P(X) \
    	(REGNO (X) == 0 || REGNO (X) >= FIRST_PSEUDO_REGISTER)

#define REG_OK_FOR_PRE_POST_P(X) \
  	(REGNO (X) <= 16)

#else

/* Nonzero if X is a hard reg that can be used as a base reg.  */
#define REG_OK_FOR_BASE_P(X)	\
	REGNO_OK_FOR_BASE_P (REGNO (X))

/* Nonzero if X is a hard reg that can be used as an index.  */
#define REG_OK_FOR_INDEX_P(X) 	\
  	REGNO_OK_FOR_INDEX_P (REGNO (X))

#define REG_OK_FOR_PRE_POST_P(X)  \
	(REGNO (X) <= 16 || (unsigned) reg_renumber[REGNO (X)] <=16)
#endif

/* GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression
   that is a valid memory address for an instruction.
   The MODE argument is the machine mode for the MEM expression
   that wants to use this address.

   The other macros defined here are used only in GO_IF_LEGITIMATE_ADDRESS.  */

#define BASE_REGISTER_RTX_P(X)  \
  (GET_CODE (X) == REG && REG_OK_FOR_BASE_P (X))

#define INDEX_REGISTER_RTX_P(X)  \
  (GET_CODE (X) == REG && REG_OK_FOR_INDEX_P (X))


/* Jump to LABEL if X is a valid address RTX.  This must also take
   REG_OK_STRICT into account when deciding about valid registers, but it uses
   the above macros so we are in luck.  
 
   Allow  REG
	  REG+disp
	  REG+r0
	  REG++
	  --REG
*/

/* The SH allows a displacement in a QI or HI amode, but only when the 
   other operand is R0. GCC doesn't handle this very well, so we forgo
   all of that.

   A legitimate index for a QI or HI is 0, SI and above can be any 
   number 0..63 */

#define GO_IF_LEGITIMATE_INDEX(MODE, REGNO, OP, LABEL)  \
  do {							\
    if (GET_CODE (OP) == CONST_INT) 			\
      {							\
	if (0&&GET_MODE_SIZE (MODE) == 2 && ((unsigned)INTVAL(OP)) <=30)\
	  goto LABEL;					\
	if (0&&GET_MODE_SIZE (MODE) == 1 && ((unsigned)INTVAL(OP)) <=15)\
	  goto LABEL;					\
	if (GET_MODE_SIZE (MODE) >=4 			\
	    && ((unsigned)INTVAL(OP)) < 64)		\
	  goto LABEL;					\
      }							\
  } while(0)


#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, LABEL)                  \
{ 								  \
  if (BASE_REGISTER_RTX_P (X))					  \
    goto LABEL;							  \
  else if ((GET_CODE (X) == POST_INC || GET_CODE (X) == PRE_DEC)  \
	   && GET_CODE (XEXP (X, 0)) == REG			  \
	   && REG_OK_FOR_PRE_POST_P (XEXP (X, 0)))		  \
    goto LABEL;							  \
  else if (GET_CODE (X) == PLUS || GET_CODE(X) == LO_SUM) 	  \
    {								  \
      rtx xop0 = XEXP(X,0);					  \
      rtx xop1 = XEXP(X,1);					  \
      if (GET_MODE_SIZE(MODE) >= 4 && BASE_REGISTER_RTX_P (xop0)) \
	GO_IF_LEGITIMATE_INDEX (MODE, REGNO (xop0), xop1, LABEL); \
      if (GET_MODE_SIZE(MODE) >= 4 && BASE_REGISTER_RTX_P (xop1)) \
	GO_IF_LEGITIMATE_INDEX (MODE, REGNO (xop1), xop0, LABEL); \
      if (GET_MODE_SIZE(MODE)<=4) {				  \
	if(BASE_REGISTER_RTX_P(xop1) &&			 	  \
	   INDEX_REGISTER_RTX_P(xop0)) goto LABEL;		  \
	if(INDEX_REGISTER_RTX_P(xop1) &&			  \
	   BASE_REGISTER_RTX_P(xop0)) goto LABEL;		  \
      }							 	  \
    }								  \
  else if ((GET_CODE (X) == PRE_INC || GET_CODE (X) == POST_DEC)  \
	   && GET_CODE (XEXP (X, 0)) == REG			  \
	   && REG_OK_FOR_PRE_POST_P (XEXP (X, 0)))		  \
    goto LABEL;                                                   \
}								   
								   
								   
/* Try machine-dependent ways of modifying an illegitimate address 
   to be legitimate.  If we find one, return the new, valid address.
   This macro is used in only one place: `memory_address' in explow.c.

   OLDX is the address as it was before break_out_memory_refs was called.
   In some cases it is useful to look at this to decide what needs to be done.

   MODE and WIN are passed so that this macro can use
   GO_IF_LEGITIMATE_ADDRESS.

   It is always safe for this macro to do nothing.  It exists to recognize
   opportunities to optimize the output.

   On the SH we don't try anything */

#define LEGITIMIZE_ADDRESS(X, OLDX, MODE, WIN)  ;

/* Go to LABEL if ADDR (a legitimate address expression)
   has an effect that depends on the machine mode it is used for.  */
#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL)  \
{									\
  if (GET_CODE(ADDR) == PRE_DEC || GET_CODE(ADDR) == POST_DEC		\
      || GET_CODE(ADDR) == PRE_INC || GET_CODE(ADDR) == POST_INC)	\
    goto LABEL;								\
}

/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.  */
#define CASE_VECTOR_MODE SImode

/* Define this if the tablejump instruction expects the table
   to contain offsets from the address of the table.
   Do not define this if the table should contain absolute addresses.  */
/* #define CASE_VECTOR_PC_RELATIVE */

/* Specify the tree operation to be used to convert reals to integers.  */
#define IMPLICIT_FIX_EXPR  FIX_ROUND_EXPR

/* This is the kind of divide that is easiest to do in the general case.  */
#define EASY_DIV_EXPR  TRUNC_DIV_EXPR

/* 'char' is signed by default */
#define DEFAULT_SIGNED_CHAR  1

/* The type of size_t unsigned int.  */
#define SIZE_TYPE "unsigned int"

/* Don't cse the address of the function being compiled.  */
#define NO_RECURSIVE_FUNCTION_CSE 1

/* Max number of bytes we can move from memory to memory
   in one reasonably fast instruction.  */
#define MOVE_MAX 4

/* Define if operations between registers always perform the operation
   on the full register even if a narrower mode is specified.  */
#define WORD_REGISTER_OPERATIONS

/* Define if loading in MODE, an integral mode narrower than BITS_PER_WORD
   will either zero-extend or sign-extend.  The value of this macro should
   be the code that says which one of the two operations is implicitly
   done, NIL if none.  */
#define LOAD_EXTEND_OP(MODE) SIGN_EXTEND

/* Define this if zero-extension is slow (more than one real instruction).
   On the SH, it's only one instruction */
/* #define SLOW_ZERO_EXTEND */

/* Nonzero if access to memory by bytes is slow and undesirable.  */
#define SLOW_BYTE_ACCESS 0

/* We assume that the store-condition-codes instructions store 0 for false
   and some other value for true.  This is the value stored for true.  */

#define STORE_FLAG_VALUE 1

/* Immediate shift counts are truncated by the output routines (or was it
   the assembler?).  Shift counts in a register are truncated by ARM.  Note
   that the native compiler puts too large (> 32) immediate shift counts
   into a register and shifts by the register, letting the ARM decide what
   to do instead of doing that itself.  */
#define SHIFT_COUNT_TRUNCATED 1

/* All integers have the same format so truncation is easy.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC,INPREC)  1

/* Define this if addresses of constant functions
   shouldn't be put through pseudo regs where they can be cse'd.
   Desirable on machines where ordinary constants are expensive
   but a CALL with constant address is cheap.  */
/*#define NO_FUNCTION_CSE 1*/

/* Chars and shorts should be passed as ints.  */
#define PROMOTE_PROTOTYPES 1

/* The machine modes of pointers and functions */
#define Pmode  SImode
#define FUNCTION_MODE  Pmode

/* The structure type of the machine dependent info field of insns
   No uses for this yet.  */
/* #define INSN_MACHINE_INFO  struct machine_info  */

/* The relative costs of various types of constants.  Note that cse.c defines
   REG = 1, SUBREG = 2, any node = (2 + sum of subnodes).  */

#define CONST_COSTS(RTX, CODE, OUTER_CODE)      \
  case CONST_INT:				\
    if (CONST_OK_FOR_I (INTVAL(RTX)))           \
      return 1;					\
    else					\
      return 8;					\
  case CONST: 					\
  case LABEL_REF:				\
  case SYMBOL_REF:				\
    return 5;					\
  case CONST_DOUBLE:				\
      return 10;

#define RTX_COSTS(X, CODE, OUTER_CODE)			\
  case MULT:						\
    return COSTS_N_INSNS (multcosts (X));		\
  case ASHIFT:						\
  case ASHIFTRT:					\
  case LSHIFT:						\
    return COSTS_N_INSNS (shiftcosts (X)) ;		\
  case DIV:						\
  case UDIV:						\
  case MOD:						\
  case UMOD:						\
    return COSTS_N_INSNS (100);				\
  case FLOAT:						\
  case FIX:						\
    return 100;

/* Compute extra cost of moving data between one register class
   and another.  

   On the SH it is hard to move into the T reg, but simple to load
   from it.
*/

#define REGISTER_MOVE_COST(SRCCLASS, DSTCLASS)  \
	((DSTCLASS == T_REGS) ? 10 : 1)

/* Assembler output control */

/* The text to go at the start of the assembler file */
#define ASM_FILE_START(STREAM) 							\
  output_file_start (STREAM, f_options, sizeof f_options / sizeof f_options[0], \
		     W_options, sizeof W_options / sizeof W_options[0]); 	


#define ASM_FILE_END(STREAM) \
  dump_constants(0);  

#define ASM_APP_ON  ""
#define ASM_APP_OFF  ""

#define FILE_ASM_OP "\t.file\n"
#define IDENT_ASM_OP "\t.ident\n"


/* Switch to the text or data segment.  */
#define TEXT_SECTION_ASM_OP  "\t.text"
#define DATA_SECTION_ASM_OP  "\t.data"
#define CTORS_SECTION_ASM_OP "\t.section\t.ctors\n"
#define DTORS_SECTION_ASM_OP "\t.section\t.dtors\n"

#define EXTRA_SECTIONS in_ctors, in_dtors
#define EXTRA_SECTION_FUNCTIONS                              \
void							     \
ctors_section() 					     \
{							     \
  if (in_section != in_ctors)				     \
    {							     \
      fprintf (asm_out_file, "%s\n", CTORS_SECTION_ASM_OP);  \
      in_section = in_ctors;				     \
    }							     \
}							     \
void							     \
dtors_section() 					     \
{							     \
  if (in_section != in_dtors)				     \
    {							     \
      fprintf (asm_out_file, "%s\n", DTORS_SECTION_ASM_OP);  \
      in_section = in_dtors;				     \
    }							     \
}							      

#define ASM_OUTPUT_SECTION(file, nam) \
   do { fprintf (file, "\t.section\t%s\n", nam); } while(0) 

#define ASM_OUTPUT_CONSTRUCTOR(FILE,NAME)	\
   do { ctors_section();  fprintf(FILE,"\t.long\t_%s\n", NAME); } while (0)

#define ASM_OUTPUT_DESTRUCTOR(FILE,NAME)	\
   do {  dtors_section();  fprintf(FILE,"\t.long\t_%s\n", NAME); } while (0)


#undef DO_GLOBAL_CTORS_BODY                     
#define DO_GLOBAL_CTORS_BODY			\
{						\
  typedef (*pfunc)();				\
  extern pfunc __ctors[];			\
  extern pfunc __ctors_end[];			\
  pfunc *p;					\
  for (p = __ctors; p < __ctors_end; p++)	\
    {						\
      (*p)();					\
    }						\
}						

#undef DO_GLOBAL_DTORS_BODY			 
#define DO_GLOBAL_DTORS_BODY                    \
{						\
  typedef (*pfunc)();				\
  extern pfunc __dtors[];			\
  extern pfunc __dtors_end[];			\
  pfunc *p;					\
  for (p = __dtors; p < __dtors_end; p++)	\
    {						\
      (*p)();					\
    }						\
}						 



#define ASM_OUTPUT_REG_PUSH(file, v) \
  fprintf (file, "\tmov.l	r%s,-@r15\n", v);

#define ASM_OUTPUT_REG_POP(file, v) \
  fprintf (file, "\tmov.l	@r15+,r%s\n", v);


  
/* The assembler's names for the registers.  RFP need not always be used as
   the Real framepointer; it can also be used as a normal general register.
   Note that the name `fp' is horribly misleading since `fp' is in fact only
   the argument-and-return-context pointer.  */
#define REGISTER_NAMES  				\
{				                   	\
  "r0", "r1", "r2",  "r3",  "r4",  "r5",  "r6",  "r7", 	\
  "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",	\
  "ap", "pr", "t",  "gbr", "mach","macl" 		\
}

/* DBX register number for a given compiler register number */
#define DBX_REGISTER_NUMBER(REGNO)  (REGNO)

/* Output a label definition.  */
#define ASM_OUTPUT_LABEL(FILE,NAME)  \
  do { assemble_name (FILE, NAME); fputs (":\n", FILE); } while (0)


/* This is how to output an assembler line
   that says to advance the location counter
   to a multiple of 2**LOG bytes.  */

#define ASM_OUTPUT_ALIGN(FILE,LOG)	\
  if ((LOG) != 0)			\
    fprintf (FILE, "\t.align %d\n", LOG)

/* Output a function label definition.  */
#define ASM_DECLARE_FUNCTION_NAME(STREAM,NAME,DECL) \
    ASM_OUTPUT_LABEL(STREAM, NAME)

/* Output a globalising directive for a label.  */
#define ASM_GLOBALIZE_LABEL(STREAM,NAME)  \
  (fprintf (STREAM, "\t.global\t"),	  \
   assemble_name (STREAM, NAME),	  \
   fputc ('\n',STREAM))                   \

/* Output a reference to a label.  */
#define ASM_OUTPUT_LABELREF(STREAM,NAME)  \
  fprintf (STREAM, "_%s", NAME)

/* Make an internal label into a string.  */
#define ASM_GENERATE_INTERNAL_LABEL(STRING, PREFIX, NUM)  \
  sprintf (STRING, "*%s%d", PREFIX, NUM)

/* Output an internal label definition.  */
#define ASM_OUTPUT_INTERNAL_LABEL(FILE,PREFIX,NUM)	\
  fprintf (FILE, "%s%d:\n", PREFIX, NUM)

/* #define ASM_OUTPUT_CASE_END(STREAM,NUM,TABLE)	    */

/* Construct a private name.  */
#define ASM_FORMAT_PRIVATE_NAME(OUTVAR,NAME,NUMBER)  \
  ((OUTVAR) = (char *) alloca (strlen (NAME) + 10),  \
   sprintf ((OUTVAR), "%s.%d", (NAME), (NUMBER)))

/* Jump tables must be 32 bit aligned. */
#define ASM_OUTPUT_CASE_LABEL(STREAM,PREFIX,NUM,TABLE) \
  fprintf (STREAM, "\t.align 2\n%s%d:\n", PREFIX, NUM);

/* Output a relative address. Not needed since jump tables are absolute
   but we must define it anyway.  */
#define ASM_OUTPUT_ADDR_DIFF_ELT(STREAM,VALUE,REL)  \
  fputs ("- - - ASM_OUTPUT_ADDR_DIFF_ELT called!\n", STREAM)

/* Output an element of a dispatch table.  */
#define ASM_OUTPUT_ADDR_VEC_ELT(STREAM,VALUE)  \
    fprintf (STREAM, "\t.long\tL%d\n", VALUE)

/* Output various types of constants.  */


/* This is how to output an assembler line defining a `double' */

#define ASM_OUTPUT_DOUBLE(FILE,VALUE)			\
do { char dstr[30];					\
     REAL_VALUE_TO_DECIMAL ((VALUE), "%.20e", dstr);	\
     fprintf (FILE, "\t.double %s\n", dstr);		\
   } while (0)


/* This is how to output an assembler line defining a `float' constant.  */
#define ASM_OUTPUT_FLOAT(FILE,VALUE)		\
do { char dstr[30];					\
     REAL_VALUE_TO_DECIMAL ((VALUE), "%.20e", dstr);	\
     fprintf (FILE, "\t.float %s\n", dstr);		\
   } while (0)


#define ASM_OUTPUT_INT(STREAM, EXP)  	\
  (fprintf (STREAM, "\t.long\t"),      	\
   output_addr_const (STREAM, (EXP)),  	\
   fputc ('\n', STREAM))		

#define ASM_OUTPUT_SHORT(STREAM, EXP)  \
  (fprintf (STREAM, "\t.short\t"),     \
   output_addr_const (STREAM, (EXP)),  \
   fputc ('\n', STREAM))		

#define ASM_OUTPUT_CHAR(STREAM, EXP)  	\
  (fprintf (STREAM, "\t.byte\t"),      	\
   output_addr_const (STREAM, (EXP)),  	\
   fputc ('\n', STREAM))

#define ASM_OUTPUT_BYTE(STREAM, VALUE)  	\
  fprintf (STREAM, "\t.byte\t%d\n", VALUE)  	\

/* This is how to output an assembler line
   that says to advance the location counter by SIZE bytes.  */

#define ASM_OUTPUT_SKIP(FILE,SIZE)  \
  fprintf (FILE, "\t.space %d\n", (SIZE))

/* This says how to output an assembler line
   to define a global common symbol.  */

#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
( fputs ("\t.comm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ",%d\n", (SIZE)))

/* This says how to output an assembler line
   to define a local common symbol.  */

#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE,ROUNDED)	\
( fputs ("\t.lcomm ", (FILE)),				\
  assemble_name ((FILE), (NAME)),			\
  fprintf ((FILE), ",%d\n", (SIZE)))


/* The assembler's parentheses characters.  */
#define ASM_OPEN_PAREN "("
#define ASM_CLOSE_PAREN ")"

/* Target characters.  */
#define TARGET_BELL	007
#define TARGET_BS	010
#define TARGET_TAB	011
#define TARGET_NEWLINE	012
#define TARGET_VT	013
#define TARGET_FF	014
#define TARGET_CR	015


/* Only perform branch elimination (by making instructions conditional) if
   we're optimising.  Otherwise it's of no use anyway.  */
#define FINAL_PRESCAN_INSN(INSN, OPVEC, NOPERANDS)  \
     final_prescan_insn (INSN, OPVEC, NOPERANDS)

/* Print operand X (an rtx) in assembler syntax to file FILE.
   CODE is a letter or dot (`z' in `%z0') or 0 if no letter was specified.
   For `%' followed by punctuation, CODE is the punctuation and X is null.  */

#define PRINT_OPERAND(STREAM, X, CODE)  print_operand (STREAM, X, CODE)

/* Print a memory address as an operand to reference that memory location.  */

#define PRINT_OPERAND_ADDRESS(STREAM,X)  print_operand_address (STREAM, X)

#define PRINT_OPERAND_PUNCT_VALID_P(CHAR) \
  ((CHAR)=='.' || (CHAR) == '#' || (CHAR) == '*' || (CHAR) == '^' || (CHAR) == '!')


/* Define the information needed to generate branch insns.  This is stored
   from the compare operation.  Note that we can't use "rtx" here since it
   hasn't been defined!  */

extern struct rtx_def *sh_compare_op0;
extern struct rtx_def *sh_compare_op1;
extern struct rtx_def *prepare_scc_operands();

extern enum attr_cpu sh_cpu;	/* target cpu */

/* Declare functions defined in sh.c and used in templates. */

extern char *output_branch();
extern char *output_shift();
extern char *output_movedouble();
extern char *output_movepcrel();


#define ADJUST_INSN_LENGTH(insn, length) \
  adjust_insn_length (insn, insn_lengths)





