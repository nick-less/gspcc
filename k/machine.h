#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#if defined(TMS34010)

typedef long			statusRegister_t;
typedef void *			returnAddress_t;
typedef long			stackArgument_t;
typedef unsigned char	byteRead_t;
typedef unsigned short	byteWrite_t;

const statusRegister_t initialStatusRegister =
	((1<<21) | (1<<11) | (0<<6) | (1<<5) | (16<<0));

static inline void clearHardwareTimer(void)
{}

#elif defined(M68K)

typedef unsigned short	statusRegister_t;
typedef void *			returnAddress_t;
typedef long			stackArgument_t;
typedef unsigned char	byteRead_t;
typedef unsigned char	byteWrite_t;

const statusRegister_t initialStatusRegister = 0x2000;

static inline void clearHardwareTimer(void)
{}

#endif

#endif		/* MACHINE_H_INCLUDED */
