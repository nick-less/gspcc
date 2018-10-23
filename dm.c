#include "config.h"
#include "rtl.h"
#include "insn-config.h"
#include "recog.h"
#include "real.h"
#include "output.h"
#include "flags.h"
#include <stdio.h>

extern FILE *asm_out_file;
extern rtx a13_rtx;

static char *getmode(long mode, char *ret)
{
	static struct
	{
		long mode;
		char letter;
	} modes[] =
	{
		{ DM_BYTE,		'b' },
		{ DM_SHORT,		's' },
		{ DM_LONG,		'l' },
		{ DM_SEXT,		'S' },
		{ DM_ZEXT,		'Z' },
		{ DM_STRICT,	'R' },
		{ DM_TRUNC,		'T' },
	};
	int i;
	char *out = ret;

	for (i = 0; i < sizeof(modes)/sizeof(*modes); i++)
	{
		if (modes[i].mode & mode)
			*out++ = modes[i].letter;
	}
	*out++ = '\0';
	return ret;
}

_do_move(from, to, frommode, tomode, emitcode)
rtx from, to;
{
	rtx operands[2];
	char mode[2][32];
	char insn[256];

	operands[0] = to;
	operands[1] = from;
	getmode(frommode, &mode[0][0]);
	getmode(tomode, &mode[1][0]);
	sprintf(insn, "%sdomove %%1[%s],%%0[%s]",
					emitcode ? "" : "T", &mode[0][0], &mode[1][0]);
	output_asm_insn(insn, operands);
}

int
dmdebug()
{
	static char *p;
	static char gotit = 0;

	if (gotit == 0)
	{
		gotit = 1;
		p = (char *)getenv("DMDEBUG");
	}
	return p != 0;
}

#define DIE()															\
	do																	\
	{																	\
		fprintf(stderr, "problems in %s:%d\n", __FILE__, __LINE__);		\
		fprintf(stderr, "from[%d]:\n", GET_CODE(from)); debug_rtx(from);\
		fprintf(stderr, "to[%d]:\n", GET_CODE(to)); debug_rtx(to);		\
	} while (0);

#define OUTPUT_ASM_INSN(str, op)		\
	do {								\
		if (emitcode)					\
			output_asm_insn(str, op);	\
	} while (0)

do_move(from, to, frommode, tomode)
rtx from, to;
{
	return __do_move(from, to, frommode, tomode, 0, 1);
}

can_move(from, to, frommode, tomode)
rtx from, to;
{
	return 1;
}

_can_move(from, to, frommode, tomode)
rtx from, to;
{
	return __do_move(from, to, frommode, tomode, 0, 0);
}

__do_move(from, to, frommode, tomode, level, emitcode)
rtx from, to;
{
	rtx operands[2];
	int ret = 1;

	level++;
	if (level > 5)
	{
		fprintf(stderr, "oops\n");
		if (level > 10)
			exit(1);
	}

	operands[0] = to;
	operands[1] = from;

	if (dmdebug())
		_do_move(from, to, frommode, tomode, emitcode);
	if ((frommode & DM_LONG) && (tomode & DM_LONG))
	{	/* LL */
		if (GET_CODE(from) == CONST_INT && REG_P(to))
		{
			if (INTVAL(from) == 0)
				OUTPUT_ASM_INSN("clr %0", operands);
			else if ((INTVAL(from) >= 1) && (INTVAL(from) <= 32))
				OUTPUT_ASM_INSN("movk %1,%0", operands);
			else if ((INTVAL(from) >= -32768) && (INTVAL(from) <= 32767))
				OUTPUT_ASM_INSN("moviw %1,%0", operands);
			else
				OUTPUT_ASM_INSN("movi %1,%0", operands);
		}
		else if (CONSTANT_P(from) && REG_P(to))
			OUTPUT_ASM_INSN("movi %1,%0", operands);
		else if (REG_P(to) && REG_P(from))
		{
			if (REGNO(to) != REGNO(from))
				OUTPUT_ASM_INSN("move %1,%0", operands);
		}
		else if (REG_P(to) || REG_P(from))
			OUTPUT_ASM_INSN("move %1,%0,1", operands);
		else if ((GET_CODE(from) == MEM) && (GET_CODE(to) == MEM))
		{
			if (GET_CODE(XEXP(from, 0)) == REG && GET_CODE(XEXP(to, 0)) == REG)
			{	/* move *Rs,*Rd */
				goto win1;
			}
			else if (GET_CODE(XEXP(from, 0)) == PRE_DEC &&
						GET_CODE(XEXP(to, 0)) == PRE_DEC)
			{	/* move *-Rs,*-Rd */
				goto win1;
			}
			else if (GET_CODE(XEXP(from, 0)) == POST_INC &&
						GET_CODE(XEXP(to, 0)) == POST_INC)
			{	/* move *Rs+,*Rd+ */
				goto win1;
			}
			else if (GET_CODE(XEXP(from, 0)) == PLUS &&
						GET_CODE(XEXP(to, 0)) == POST_INC)
			{	/* move *Rs(offset),*Rd+ */
				goto win1;
			}
			else if (GET_CODE(XEXP(from, 0)) == PLUS &&
						GET_CODE(to) == MEM && REG_P(XEXP(to, 0)))
			{	/* Simulate move *Rs(offset),*Rd */
				OUTPUT_ASM_INSN("move %1,%0(0),1", operands);
			}
			else if (GET_CODE(from) == MEM && REG_P(XEXP(from, 0)) &&
					GET_CODE(XEXP(to, 0)) == PLUS)
			{	/* Simulate move *Rs,*Rd(offset) */
				OUTPUT_ASM_INSN("move %1(0),%0,1", operands);
			}
			else if (CONSTANT_P(XEXP(from, 0)) && CONSTANT_P(XEXP(to, 0)))
			{	/* addr1,addr2 */
				goto win1;
			}
			else if (GET_CODE(XEXP(from, 0)) == PLUS &&
						GET_CODE(XEXP(to, 0)) == PLUS)
			{	/* move *Rs(offset),*Rd(offset) */
win1:
				OUTPUT_ASM_INSN("move %1,%0,1", operands);
			}
			else
			{
lose1:
				if (!emitcode)
					return 0;
				__do_move(from, a13_rtx, frommode, DM_LONG, level, emitcode);
				__do_move(a13_rtx, to, DM_LONG, tomode, level, emitcode);
			}
		}
		else
			goto lose1;
	}
	else if ((frommode & DM_LONG) && (tomode & DM_SHORT))
	{	/* LS */
		if (GET_CODE(from) == CONST_INT && REG_P(to))
		{
			if (INTVAL(from) == 0)
				OUTPUT_ASM_INSN("clr %0", operands);
			else if ((INTVAL(from) >= 1) && (INTVAL(from) <= 32))
				OUTPUT_ASM_INSN("movk %1,%0", operands);
			else
				OUTPUT_ASM_INSN("moviw %1,%0", operands);
		}
		else if (CONSTANT_P(from) && REG_P(to))
			OUTPUT_ASM_INSN("moviw %1,%0", operands);
		else if (REG_P(to) && REG_P(from))
		{
			if (REGNO(to) != REGNO(from))
				OUTPUT_ASM_INSN("move %1,%0", operands);
			if (frommode & DM_TRUNC)
				OUTPUT_ASM_INSN("zext %0,0", operands);
		}
		else if (REG_P(to) || REG_P(from))
		{
			OUTPUT_ASM_INSN("move %1,%0,0", operands);
			if ((frommode & DM_TRUNC) && REG_P(to))
				OUTPUT_ASM_INSN("zext %0,0", operands);
		}
		else if ((GET_CODE(from) == MEM) && (GET_CODE(to) == MEM))
		{
			if (GET_CODE(XEXP(from, 0)) == REG && GET_CODE(XEXP(to, 0)) == REG)
			{	/* move *Rs,*Rd */
				goto win2;
			}
			else if (GET_CODE(XEXP(from, 0)) == PRE_DEC &&
						GET_CODE(XEXP(to, 0)) == PRE_DEC)
			{	/* move *-Rs,*-Rd */
				goto win2;
			}
			else if (GET_CODE(XEXP(from, 0)) == POST_INC &&
						GET_CODE(XEXP(to, 0)) == POST_INC)
			{	/* move *Rs+,*Rd+ */
				goto win2;
			}
			else if (GET_CODE(XEXP(from, 0)) == PLUS &&
						GET_CODE(XEXP(to, 0)) == POST_INC)
			{	/* move *Rs(offset),*Rd+ */
				goto win2;
			}
			else if (GET_CODE(XEXP(from, 0)) == PLUS &&
						GET_CODE(XEXP(to, 0)) == PLUS)
			{	/* move *Rs(offset),*Rd(offset) */
win2:
				OUTPUT_ASM_INSN("move %1,%0,0", operands);
			}
			else
			{
lose2:
				if (!emitcode)
					return 0;
				__do_move(from, a13_rtx, frommode, DM_SHORT, level, emitcode);
				__do_move(a13_rtx, to, DM_SHORT, tomode, level, emitcode);
			}
		}
		else
			goto lose2;
	}
	else if ((frommode & DM_LONG) && (tomode & DM_BYTE))
	{	/* LB */
		if (GET_CODE(from) == CONST_INT && REG_P(to))
		{
			if (INTVAL(from) == 0)
				OUTPUT_ASM_INSN("clr %0", operands);
			else if ((INTVAL(from) >= 1) && (INTVAL(from) <= 32))
				OUTPUT_ASM_INSN("movk %1,%0", operands);
			else
				OUTPUT_ASM_INSN("moviw %1,%0", operands);
		}
		else if (CONSTANT_P(from) && REG_P(to))
			OUTPUT_ASM_INSN("moviw %1,%0", operands);
		else if (REG_P(to) && REG_P(from))
		{
			if (REGNO(to) != REGNO(from))
				OUTPUT_ASM_INSN("move %1,%0", operands);
			if (frommode & DM_TRUNC)
				OUTPUT_ASM_INSN("andi 0xff,%0", operands);
		}
		else if (REG_P(from) && GET_CODE(to) == MEM &&
				GET_CODE(XEXP(to, 0)) == REG)
		{	/* movb Rs,*Rd */
			goto win3;
		}
		else if (REG_P(from) && GET_CODE(to) == MEM && CONSTANT_P(XEXP(to, 0)))
		{	/* movb Rs,label */
win3:
			OUTPUT_ASM_INSN("movb %1,%0", operands);
		}
		else if (GET_CODE(from) == MEM && REG_P(to))
		{	/* long mem -> reg */
			OUTPUT_ASM_INSN("move %1,%0,1", operands);
			if (frommode & DM_TRUNC)
				OUTPUT_ASM_INSN("andi 0xff,%0", operands);
		}
		else
		{
			if (!emitcode)
				return 0;
			__do_move(from, a13_rtx, frommode, DM_BYTE, level, emitcode);
			__do_move(a13_rtx, to, DM_BYTE, tomode, level, emitcode);
		}
	}
	else if ((frommode & DM_SHORT) && (tomode & DM_LONG))
	{	/* SL */
		if (GET_CODE(from) == CONST_INT && REG_P(to))
		{
			if (INTVAL(from) == 0)
				OUTPUT_ASM_INSN("clr %0", operands);
			else if ((INTVAL(from) >= 1) && (INTVAL(from) <= 32))
				OUTPUT_ASM_INSN("movk %1,%0", operands);
			else
				OUTPUT_ASM_INSN("moviw %1,%0", operands);
		}
		else if (CONSTANT_P(from) && REG_P(to))
			OUTPUT_ASM_INSN("moviw %1,%0", operands);
		else if (REG_P(to) && REG_P(from))
		{
			if ((REGNO(to) != REGNO(from)) || (frommode & (DM_SEXT|DM_ZEXT)))
				OUTPUT_ASM_INSN("move %1,%0", operands);
			if (frommode & DM_SEXT)
				OUTPUT_ASM_INSN("sext %0,0", operands);
			else if (frommode & DM_ZEXT)
				OUTPUT_ASM_INSN("zext %0,0", operands);
		}
		else if (REG_P(to) || REG_P(from))
		{
			OUTPUT_ASM_INSN("move %1,%0,0", operands);
			if (frommode & DM_ZEXT)
			{
				OUTPUT_ASM_INSN("zext %0,0", operands);
			}
		}
		else if (GET_CODE(from) == MEM && REG_P(to))
		{	/* Use magic extension of move mem,Rd,0 */
			ret = __do_move(from, to, DM_SHORT, DM_SHORT, level, emitcode);
			if (frommode & DM_ZEXT)
				OUTPUT_ASM_INSN("zext %0,0", operands);
		}
		else
		{
			if (!emitcode)
				return 0;
			__do_move(from, a13_rtx, frommode, DM_LONG, level, emitcode);
			__do_move(a13_rtx, to, DM_LONG, tomode, level, emitcode);
		}
	}
	else if ((frommode & DM_SHORT) && (tomode & DM_SHORT))
	{	/* SS */
		if (GET_CODE(from) == CONST_INT && REG_P(to))
		{
			if (INTVAL(from) == 0)
				OUTPUT_ASM_INSN("clr %0", operands);
			else if ((INTVAL(from) >= 1) && (INTVAL(from) <= 32))
				OUTPUT_ASM_INSN("movk %1,%0", operands);
			else
				OUTPUT_ASM_INSN("moviw %1,%0", operands);
		}
		else if (CONSTANT_P(from) && REG_P(to))
			OUTPUT_ASM_INSN("moviw %1,%0", operands);
		else if (REG_P(to) && REG_P(from))
		{
			if (REGNO(to) != REGNO(from))
				OUTPUT_ASM_INSN("move %1,%0", operands);
			if (frommode & (DM_TRUNC|DM_ZEXT))
				OUTPUT_ASM_INSN("zext %0,0", operands);
		}
		else if (REG_P(to) || REG_P(from))
			OUTPUT_ASM_INSN("move %1,%0,0", operands);
		else if ((GET_CODE(from) == MEM) && (GET_CODE(to) == MEM))
		{
			if (GET_CODE(XEXP(from, 0)) == REG && GET_CODE(XEXP(to, 0)) == REG)
			{	/* move *Rs,*Rd */
				goto win4;
			}
			else if (GET_CODE(XEXP(from, 0)) == PRE_DEC &&
						GET_CODE(XEXP(to, 0)) == PRE_DEC)
			{	/* move *-Rs,*-Rd */
				goto win4;
			}
			else if (GET_CODE(XEXP(from, 0)) == POST_INC &&
						GET_CODE(XEXP(to, 0)) == POST_INC)
			{	/* move *Rs+,*Rd+ */
				goto win4;
			}
			else if (GET_CODE(XEXP(from, 0)) == PLUS &&
						GET_CODE(XEXP(to, 0)) == POST_INC)
			{	/* move *Rs(offset),*Rd+ */
				goto win4;
			}
			else if (GET_CODE(XEXP(from, 0)) == PLUS &&
						GET_CODE(to) == MEM && REG_P(XEXP(to, 0)))
			{	/* Simulate move *Rs(offset),*Rd */
				OUTPUT_ASM_INSN("move %1,%0(0),0", operands);
			}
			else if (GET_CODE(from) == MEM && REG_P(XEXP(from, 0)) &&
					GET_CODE(XEXP(to, 0)) == PLUS)
			{	/* Simulate move *Rs,*Rd(offset) */
				OUTPUT_ASM_INSN("move %1(0),%0,0", operands);
			}
			else if (CONSTANT_P(XEXP(from, 0)) && CONSTANT_P(XEXP(to, 0)))
			{	/* addr1,addr2 */
				goto win4;
			}
			else if (GET_CODE(XEXP(from, 0)) == PLUS &&
						GET_CODE(XEXP(to, 0)) == PLUS)
			{	/* move *Rs(offset),*Rd(offset) */
win4:
				OUTPUT_ASM_INSN("move %1,%0,0", operands);
			}
			else
			{
lose4:
				if (!emitcode)
					return 0;
				__do_move(from, a13_rtx, frommode, DM_SHORT, level, emitcode);
				__do_move(a13_rtx, to, DM_SHORT, tomode, level, emitcode);
			}
		}
		else
			goto lose4;
	}
	else if ((frommode & DM_SHORT) && (tomode & DM_BYTE))
	{	/* SB */
		if (GET_CODE(from) == CONST_INT && REG_P(to))
		{
			if (INTVAL(from) == 0)
				OUTPUT_ASM_INSN("clr %0", operands);
			else if ((INTVAL(from) >= 1) && (INTVAL(from) <= 32))
				OUTPUT_ASM_INSN("movk %1,%0", operands);
			else
				OUTPUT_ASM_INSN("moviw %1,%0", operands);
		}
		else if (CONSTANT_P(from) && REG_P(to))
			OUTPUT_ASM_INSN("moviw %1,%0", operands);
		else if (REG_P(to) && REG_P(from))
		{
			if (REGNO(to) != REGNO(from))
				OUTPUT_ASM_INSN("move %1,%0", operands);
			if (frommode & DM_TRUNC)
				OUTPUT_ASM_INSN("andi 0xff,%0", operands);
		}
		else if (GET_CODE(from) == MEM && REG_P(to))
		{	/* short mem -> reg */
			OUTPUT_ASM_INSN("move %1,%0,0", operands);
			if (frommode & DM_TRUNC)
				OUTPUT_ASM_INSN("andi 0xff,%0", operands);
		}
		else if (REG_P(from) && GET_CODE(to) == MEM &&
				GET_CODE(XEXP(to, 0)) == REG)
		{	/* movb Rs,*Rd */
			goto win5;
		}
		else if (REG_P(from) && GET_CODE(to) == MEM && CONSTANT_P(XEXP(to, 0)))
		{	/* movb Rs,label */
win5:
			OUTPUT_ASM_INSN("movb %1,%0", operands);
		}
		else
		{
			if (!emitcode)
				return 0;
			__do_move(from, a13_rtx, frommode, DM_BYTE, level, emitcode);
			__do_move(a13_rtx, to, DM_BYTE, tomode, level, emitcode);
		}
	}
	else if ((frommode & DM_BYTE) && (tomode & DM_LONG))
	{	/* BL */
		if (GET_CODE(from) == CONST_INT && REG_P(to))
		{
			if (INTVAL(from) == 0)
				OUTPUT_ASM_INSN("clr %0", operands);
			else if ((INTVAL(from) >= 1) && (INTVAL(from) <= 32))
				OUTPUT_ASM_INSN("movk %1,%0", operands);
			else
				OUTPUT_ASM_INSN("moviw %1,%0", operands);
		}
		else if (CONSTANT_P(from) && REG_P(to))
			OUTPUT_ASM_INSN("movi %1,%0", operands);
		else if (REG_P(to) && REG_P(from))
		{
			if ((REGNO(to) != REGNO(from)) || (frommode & (DM_SEXT|DM_ZEXT)))
				OUTPUT_ASM_INSN("move %1,%0", operands);
			if (frommode & DM_ZEXT)
				OUTPUT_ASM_INSN("andi 0xff,%0", operands);
			else if (frommode & DM_SEXT)
				OUTPUT_ASM_INSN("setf 8,1,0\n\tsext %0,0\n\tsetf 16,1,0",
								operands);
		}
		else if (GET_CODE(from) == MEM &&
				GET_CODE(XEXP(from, 0)) == REG && REG_P(to))
		{	/* movb *Rs,Rd */
			goto win9;
		}
		else if (GET_CODE(from) == MEM &&
				GET_CODE(XEXP(from, 0)) == PLUS && REG_P(to))
		{	/* movb *Rs(offset),Rd */
			goto win9;
		}
		else if (GET_CODE(from) == MEM &&
				CONSTANT_P(XEXP(from, 0)) && REG_P(to))
		{	/* movb label,Rd */
win9:
			if (frommode & DM_ZEXT)
				OUTPUT_ASM_INSN("setf 8,0,0\n\tmove %1,%0,0\n\tsetf 16,1,0",
									operands);
			else
				OUTPUT_ASM_INSN("movb %1,%0", operands);
		}
		else if (GET_CODE(from) == MEM && REG_P(to))
		{	/* Use magic extension of movb mem,Rd */
			ret = __do_move(from, to, DM_BYTE, DM_BYTE, level, emitcode);
			if (frommode & DM_ZEXT)
				OUTPUT_ASM_INSN("andi 0xff,%0", operands);
		}
		else
		{
			if (!emitcode)
				return 0;
			__do_move(from, a13_rtx, frommode, DM_BYTE, level, emitcode);
			__do_move(a13_rtx, to, DM_BYTE, tomode, level, emitcode);
		}
	}
	else if ((frommode & DM_BYTE) && (tomode & DM_SHORT))
	{	/* BS */
		if (GET_CODE(from) == CONST_INT && REG_P(to))
		{
			if (INTVAL(from) == 0)
				OUTPUT_ASM_INSN("clr %0", operands);
			else if ((INTVAL(from) >= 1) && (INTVAL(from) <= 32))
				OUTPUT_ASM_INSN("movk %1,%0", operands);
			else
				OUTPUT_ASM_INSN("moviw %1,%0", operands);
		}
		else if (CONSTANT_P(from) && REG_P(to))
			OUTPUT_ASM_INSN("movi %1,%0", operands);
		else if (REG_P(to) && REG_P(from))
		{
			if ((REGNO(to) != REGNO(from)) || (frommode & (DM_SEXT|DM_ZEXT)))
				OUTPUT_ASM_INSN("move %1,%0", operands);
			if (frommode & DM_ZEXT)
				OUTPUT_ASM_INSN("andi 0xff,%0", operands);
			else if (frommode & DM_SEXT)
				OUTPUT_ASM_INSN("setf 8,1,0\n\tsext %0,0\n\tsetf 16,1,0",
								operands);
		}
		else if (GET_CODE(from) == MEM &&
				GET_CODE(XEXP(from, 0)) == REG && REG_P(to))
		{	/* movb *Rs,Rd */
			goto win11;
		}
		else if (GET_CODE(from) == MEM &&
				GET_CODE(XEXP(from, 0)) == PLUS && REG_P(to))
		{	/* movb *Rs(offset),Rd */
			goto win11;
		}
		else if (GET_CODE(from) == MEM &&
				CONSTANT_P(XEXP(from, 0)) && REG_P(to))
		{	/* movb label,Rd */
win11:
			if (frommode & DM_ZEXT)
				OUTPUT_ASM_INSN("setf 8,0,0\n\tmovb %1,%0\n\tsetf 16,1,0",
					operands);
			else
				OUTPUT_ASM_INSN("movb %1,%0", operands);
		}
		else if (GET_CODE(from) == MEM && REG_P(to))
		{	/* Use magic extension of movb mem,Rd */
			ret = __do_move(from, to, DM_BYTE, DM_BYTE, level, emitcode);
			if (frommode & DM_ZEXT)
				OUTPUT_ASM_INSN("andi 0xff,%0", operands);
		}
		else
		{
			if (!emitcode)
				return 0;
			__do_move(from, a13_rtx, frommode, DM_BYTE, level, emitcode);
			__do_move(a13_rtx, to, DM_BYTE, tomode, level, emitcode);
		}
	}
	else if ((frommode & DM_BYTE) && (tomode & DM_BYTE))
	{	/* BB */
		if (GET_CODE(from) == CONST_INT && REG_P(to))
		{
			if (INTVAL(from) == 0)
				OUTPUT_ASM_INSN("clr %0", operands);
			else if ((INTVAL(from) >= 1) && (INTVAL(from) <= 32))
				OUTPUT_ASM_INSN("movk %1,%0", operands);
			else
				OUTPUT_ASM_INSN("moviw %1,%0", operands);
		}
		else if (CONSTANT_P(from) && REG_P(to))
			OUTPUT_ASM_INSN("movi %1,%0", operands);
		else if (REG_P(to) && REG_P(from))
		{
			if (REGNO(to) != REGNO(from))
				OUTPUT_ASM_INSN("move %1,%0", operands);
			if (frommode & DM_ZEXT)
				OUTPUT_ASM_INSN("andi 0xff,%0", operands);
			else if (frommode & DM_SEXT)
				OUTPUT_ASM_INSN("setf 8,1,0\n\tsext %0,0\n\tsetf 16,1,0",
								operands);
		}
		else if (REG_P(from) && GET_CODE(to) == MEM && REG_P(XEXP(to, 0)))
		{	/* movb Rs,*Rd */
			goto win6;
		}
		else if (REG_P(from) && GET_CODE(to) == MEM &&
				GET_CODE(XEXP(to, 0)) == PLUS)
		{	/* movb Rs,*Rd(offset) */
			goto win6;
		}
		else if (GET_CODE(from) == MEM && REG_P(XEXP(from, 0)) && REG_P(to))
		{	/* movb *Rs,Rd */
			goto win6;
		}
		else if (GET_CODE(from) == MEM && REG_P(XEXP(from, 0)) &&
				GET_CODE(to) == MEM && REG_P(XEXP(to, 0)))
		{	/* movb *Rs,*Rd */
			goto win6;
		}
		else if (GET_CODE(from) == MEM &&
				GET_CODE(XEXP(from, 0)) == PLUS && REG_P(to))
		{	/* movb *Rs(offset),Rd */
			goto win6;
		}
		else if (GET_CODE(from) == MEM && GET_CODE(XEXP(from, 0)) == PLUS &&
					GET_CODE(to) == MEM && GET_CODE(XEXP(to, 0)) == PLUS)
		{	/* movb *Rs(offset),*Rd(offset) */
			goto win6;
		}
		else if (REG_P(from) && GET_CODE(to) == MEM && CONSTANT_P(XEXP(to, 0)))
		{	/* movb Rs,label */
			goto win6;
		}
		else if (GET_CODE(from) == MEM &&
				CONSTANT_P(XEXP(from, 0)) && REG_P(to))
		{	/* movb label,Rs */
			goto win6;
		}
		else if (GET_CODE(from) == MEM && CONSTANT_P(XEXP(from, 0)) &&
				GET_CODE(to) == MEM && CONSTANT_P(XEXP(to, 0)))
		{	/* movb label,label */
win6:
			OUTPUT_ASM_INSN("movb %1,%0", operands);
		}
		else if (GET_CODE(from) == MEM && GET_CODE(XEXP(from, 0)) == POST_INC)
		{	/* convert "movb *Rs+,xx" to "movb *Rs,xx;addk 8,Rs" */
			rtx modfrom;
			rtx xoperands[2];

			modfrom = copy_rtx(from);
			XEXP(modfrom, 0) = XEXP(XEXP(modfrom, 0), 0);
			ret = __do_move(modfrom, to, frommode, tomode, level, emitcode);
			xoperands[0] = XEXP(modfrom, 0);	/* The register */
			OUTPUT_ASM_INSN("addk 8,%0", xoperands);
		}
		else if (GET_CODE(to) == MEM && GET_CODE(XEXP(to, 0)) == POST_INC)
		{
			rtx modto;
			rtx xoperands[2];

			modto = copy_rtx(to);
			XEXP(modto, 0) = XEXP(XEXP(modto, 0), 0);
			ret = __do_move(from, modto, frommode, tomode, level, emitcode);
			xoperands[0] = XEXP(modto, 0);	/* The register */
			OUTPUT_ASM_INSN("addk 8,%0", xoperands);
		}
#if 0
		else if (GET_CODE(from) == MEM && GET_CODE(XEXP(from, 0)) == PRE_DEC)
		{	/* convert "movb *-Rs,xx" to "subk 8,R2;movb *Rs,xx" */
			rtx modfrom;
			rtx xoperands[2];

			modfrom = copy_rtx(from);
			XEXP(modfrom, 0) = XEXP(XEXP(modfrom, 0), 0);
			xoperands[0] = XEXP(modfrom, 0);	/* The register */
			OUTPUT_ASM_INSN("subk 8,%0", xoperands);
			ret = __do_move(modfrom, to, frommode, tomode, level, emitcode);
		}
#endif
#if 0
		else if (GET_CODE(to) == MEM && GET_CODE(XEXP(to, 0)) == PRE_DEC)
		{
			rtx modto;
			rtx xoperands[2];

			modto = copy_rtx(to);
			XEXP(modto, 0) = XEXP(XEXP(modto, 0), 0);
			xoperands[0] = XEXP(modto, 0);	/* The register */
			OUTPUT_ASM_INSN("subk 8,%0", xoperands);
			ret = __do_move(from, modto, frommode, tomode, level, emitcode);
		}
#endif
		else if (GET_CODE(from) == MEM && GET_CODE(XEXP(from, 0)) == PLUS &&
				GET_CODE(to) == MEM && REG_P(XEXP(to, 0)))
		{	/*
			 * We don't have "movb *Rs(offset),*Rd", so emit
			 * "movb *Rs(offset),*Rd(0)"
			 */
			OUTPUT_ASM_INSN("movb %1,%0(0)", operands);
		}
		else if (GET_CODE(from) == MEM && REG_P(XEXP(from, 0)) &&
				GET_CODE(to) == MEM && GET_CODE(XEXP(to, 0)) == PLUS)
		{	/*
			 * We don't have "movb *Rs,*Rd(offset)", so emit
			 * "movb *Rs(0),*Rd(offset)"
			 */
			OUTPUT_ASM_INSN("movb %1(0),%0", operands);
		}
#if 0
		else if (REG_P(from) || REG_P(to))
#endif
		else if (_can_move(from, to, DM_SHORT, DM_SHORT))
		{	/* Handle missing modes with 8 bit field size */
			OUTPUT_ASM_INSN("setf 8,1,0\n\tmove %1,%0,0\n\tsetf 16,1,0",
						operands);
		}
		else
		{
			if (!emitcode)
				return 0;
			__do_move(from, a13_rtx, frommode, DM_BYTE, level, emitcode);
			__do_move(a13_rtx, to, DM_BYTE, tomode, level, emitcode);
		}
	}
	else
		DIE();
	if (dmdebug())
		OUTPUT_ASM_INSN("--------", operands);
	return ret;
}

noabort()
{
	fprintf(stderr, "noabort()\n");
}

do_addsub(operands, isadd)
rtx *operands;
int isadd;
{
	if (REG_P(operands[2]))
	{
		output_asm_insn(isadd ? "add %2,%0" : "sub %2,%0", operands);
	}
	else if (GET_CODE(operands[2]) == CONST_INT)
	{
		if (INTVAL(operands[2]) >= 1 && INTVAL(operands[2]) <= 32)
		{
			output_asm_insn(isadd ? "addk %2,%0" : "subk %2,%0", operands);
		}
		else if (INTVAL(operands[2]) >= -32 && INTVAL(operands[2]) <= -1)
		{
			rtx xoperands[2];

			xoperands[0] = operands[0];
			xoperands[1] = GEN_INT(-INTVAL(operands[2]));

			output_asm_insn(isadd ? "subk %1,%0" : "addk %1,%0", xoperands);
		}
		else if (INTVAL(operands[2]) >= -32768 && INTVAL(operands[2]) <= 32767)
		{
			output_asm_insn(isadd ? "addiw %2,%0" : "subiw %2,%0", operands);
		}
		else
		{
			output_asm_insn(isadd ? "addi %2,%0" : "subi %2,%0", operands);
		}
	}
	else if (CONSTANT_P(operands[2]))
	{
		output_asm_insn(isadd ? "addi %2,%0" : "subi %2,%0", operands);
	}
	else
		abort();

	return;
}

do_addsubreg(reg, value, isadd)
rtx reg;
int value;
int isadd;
{
	rtx operands[3];

	operands[0] = reg;
	operands[1] = reg;
	operands[2] = GEN_INT(value);
	do_addsub(operands, isadd);
}

gsp8()
{
	return 8;
}

asm_output_ascii(file, string, length)
FILE *file;
char *string;
int length;
{
	fprintf(file, "\t.string \"");
	while (length--)
	{
		char ch = *string++;
		if (ch >= ' ' && ch < 0x7f && ch != '"')
			putc(ch, file);
		else
		{
			switch (ch)
			{
			case '\r':	fprintf(file, "\\r"); break;
			case '\n':	fprintf(file, "\\n"); break;
			case '\t':	fprintf(file, "\\t"); break;
			case '\f':	fprintf(file, "\\f"); break;
			case '\b':	fprintf(file, "\\b"); break;
			default:
				if (length && *string >= '0' && *string <= '7')
					fprintf(file, "\\%03o", (unsigned)ch);
				else
					fprintf(file, "\\%o", (unsigned)ch);
			}
		}
	}
	fprintf(file, "\"\n");
}

/* Nonzero if an A file reg appears somewhere within IN. */

int
areg_mentioned_p (in)
     register rtx in;
{
  register char *fmt;
  register int i;
  register enum rtx_code code;

  if (in == 0)
    return 0;

  if (REG_P(in) && (REGNO(in) < 15))
    return 1;

  if (GET_CODE (in) == LABEL_REF)
    return areg_mentioned_p(XEXP (in, 0));

  code = GET_CODE (in);

  switch (code)
    {
      /* Compare registers by number.  */
    case REG:
      return REGNO (in) < 15;

      /* These codes have no constituent expressions
	 and are unique.  */
    case SCRATCH:
    case CC0:
    case PC:
      return 0;

    case CONST_INT:
      
    case CONST_DOUBLE:
      /* These are kept unique for a given value.  */
      return 0;
    }

  fmt = GET_RTX_FORMAT (code);

  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'E')
	{
	  register int j;
	  for (j = XVECLEN (in, i) - 1; j >= 0; j--)
	    if (areg_mentioned_p (XVECEXP (in, i, j)))
	      return 1;
	}
      else if (fmt[i] == 'e'
	       && areg_mentioned_p (XEXP (in, i)))
	return 1;
    }
  return 0;
}

/* Nonzero if an B file reg appears somewhere within IN. */

int
breg_mentioned_p (in)
     register rtx in;
{
  register char *fmt;
  register int i;
  register enum rtx_code code;

  if (in == 0)
    return 0;

  if (REG_P(in) && (REGNO(in) >= 16) && (REGNO(in) < 32))
    return 1;

  if (GET_CODE (in) == LABEL_REF)
    return areg_mentioned_p(XEXP (in, 0));

  code = GET_CODE (in);

  switch (code)
    {
      /* Compare registers by number.  */
    case REG:
      return (REGNO(in) >= 16) && (REGNO(in) < 32);

      /* These codes have no constituent expressions
	 and are unique.  */
    case SCRATCH:
    case CC0:
    case PC:
      return 0;

    case CONST_INT:
      
    case CONST_DOUBLE:
      /* These are kept unique for a given value.  */
      return 0;
    }

  fmt = GET_RTX_FORMAT (code);

  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'E')
	{
	  register int j;
	  for (j = XVECLEN (in, i) - 1; j >= 0; j--)
	    if (areg_mentioned_p (XVECEXP (in, i, j)))
	      return 1;
	}
      else if (fmt[i] == 'e'
	       && areg_mentioned_p (XEXP (in, i)))
	return 1;
    }
  return 0;
}

enum reg_class
secondary_reload_class(enum reg_class class, enum machine_mode mode, void *_x)
{
	rtx x = _x;

	if (class == B_REGS || class == ODD_B_REGS)
	{
		return NO_REGS;
		if (REG_P(x) || CONSTANT_P(x))
			return NO_REGS;
		if (areg_mentioned_p(x))
			return GENERAL_REGS;
	}
	return NO_REGS;
}

int extra_constraint(value, c)
rtx value;
{
	if (c != 'Q')
		return 0;
	if (areg_mentioned_p(value))
		return 0;
	return 1;
}

enum reg_class
preferred_reload_class(x, class)
rtx x;
enum reg_class class;
{
	if (GET_CODE(x) == CONST_DOUBLE &&
		GET_MODE_CLASS(GET_MODE (x)) == MODE_FLOAT)
	return NO_REGS;
#if 0
	if (class == B_REGS || class == ODD_B_REGS)
	{
		if (!breg_mentioned_p(x))
			return GENERAL_REGS;
	}
#endif
	return class;
}
