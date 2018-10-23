/*
 * General header file for 1616/OS V4.2b system calls
 *
 * Andrew Morton, Applix pty limited
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <lang.h>

CPP_START

#ifndef STDIN
#define STDERR		0x100	/* Standard error file descriptor */
#define STDOUT		0x101	/* Standard output file descriptor */
#define STDIN		0x102	/* Standard input file descriptor */
#endif

/*
 * We macro all the system calls as calls to trap7().
 * The system calls are in upper case to distinguish them from
 * normal function calls, such as C library functions
 *
 * Note that the printf, sprintf and fprintf calls are missing here
 * because they use a variable number of arguments, which confuses C
 * preprocessors.
 */

extern long trap7(long, ...);

#define COLDBOOT()			trap7(0)
#define WARMBOOT()			trap7(1)
#define GETCHAR()			trap7(2)
#define SGETCHAR()			trap7(3)
#define PUTCHAR(CH)			trap7(4, CH)
#define SPUTCHAR()			trap7(5)
#define GETC(DVR)			trap7(6, DVR)
#define SGETC(DVR)			trap7(7, DVR)
#define PUTC(DVR, CH)			trap7(8, DVR, CH)
#define SPUTC(DVR)			trap7(9, DVR)
#define ADD_IPDVR(IO, STAT, NAME, PV)	trap7(10, IO, STAT, NAME, PV)
#define ADD_XIPDVR(io, stat, name,pv,m)	trap7(10, io, stat, name, pv, m)
#define LOADREL(IFD, ADDR)		trap7(11, IFD, ADDR)
#define ADD_OPDVR(IO, STAT, NAME, PV)	trap7(12, IO, STAT, NAME, PV)
#define EXIT(I)				trap7(13, I)
#define SET_SIP(DVR)			trap7(14, DVR)
#define SET_SOP(DVR)			trap7(15, DVR)
#define SET_VSVEC(VEC, RATE, CALLVAL)	trap7(16, VEC, RATE, CALLVAL)
#define CLR_VSVEC(VNUM)			trap7(17, VNUM)
#define GET_TICKS()			trap7(18)
#define GET_CPU()			trap7(19)
#define SET_SER(DVR)			trap7(20, DVR)
#define CASWRAW(START, LENGTH, LEADER)	trap7(21, START, LENGTH, LEADER)
#define CASRRAW(BUF, LEADER, MAXHUNK)	trap7(22, BUF, LEADER, MAXHUNK)
#define GETDATE(STR)			trap7(23, STR)
#define SETDATE(STR)			trap7(24, STR)
#define ABORTSTAT()			trap7(25)
#define ENT1INTS(VEC, PRELOAD)		trap7(26, VEC, PRELOAD)
#define DIST1INTS()			trap7(27)
#define SINE(I)				trap7(28, I)
#define DEF_FK(FKNUM, STR)		trap7(29, FKNUM, STR)
#define GETRAND()			trap7(30)
#define SET_640(MODE)			trap7(31, MODE)
#define SET_VDP(PAGE)			trap7(32, PAGE)
#define SET_VAP(PAGE)			trap7(33, PAGE)
#define SET_FGCOL(COL)			trap7(34, COL)
#define SET_BGCOL(COL)			trap7(35, COL)
#define SET_BDCOL(COL)			trap7(36, COL)
#define SET_PAL(PALPOS, COL)		trap7(37, PALPOS, COL)
#define RDCH_SHAPE(CHNO)		trap7(38, CHNO)
#define DEF_CHSHAPE(CHNO, DEFPTR)	trap7(39, CHNO, DEFPTR)
#define DEF_WIND(WIND)			trap7(40, WIND)
#define VID_ADDRESS(X, Y)		trap7(41, X, Y)
#define MOVE_WIND(BUF, MODE)		trap7(42, BUF, MODE)
#define RAWVID(ROW, COL, CH, FG, BG)	trap7(43, ROW, COL, CH, FG, BG)
#define FILL_WIND(COL)			trap7(44, COL)
#define SCURS_MODE(RATE, EN, MASK)	trap7(45, RATE, EN, MASK)
#define MOUSETRAP(N, V)			trap7(46, N, V)

/* Extra mousetrap modes */
#define SAVEVCONTEXT(p)			trap7(46, 4, p)
#define RESTVCONTEXT(p)			trap7(46, 5, p)
#define VCONTEXTSIZE()			trap7(46, 6)

#define FILL(X, Y, VAL)			trap7(47, X, Y, VAL)
#define RSET_PEL(X, Y, VAL)		trap7(50, X, Y, VAL)
#define SET_PEL(X, Y, VAL)		trap7(51, X, Y, VAL)
#define RLINE(X0, Y0, X1, Y1)		trap7(52, X0, Y0, X1, Y1)
#define DRAWLINE(X0, Y0, X1, Y1)	trap7(53, X0, Y0, X1, Y1)
#define RREAD_PEL(X, Y)			trap7(54, X, Y)
#define READ_PEL(X, Y)			trap7(55, X, Y)
#define SGFGCOL(VAL)			trap7(56, VAL)
#define RCIRCLE(X, Y, RADIUS)		trap7(59, X, Y, RADIUS)
#define CIRCLE(X, Y, RADIUS)		trap7(60, X, Y, RADIUS)
#define SDOTMODE(MODE)			trap7(61, MODE)
#define GETMEM(NBYTES, MODE)		trap7(62, NBYTES, MODE)
#define GETFMEM(FIRST, NBYTES, MODE)	trap7(63, FIRST, NBYTES, MODE)
#define FREEMEM(ADDR)			trap7(64, ADDR)
#define CHDIR(ADDR)			trap7(65, ADDR)
#define MKDIR(ADDR, N)			trap7(66, ADDR, N)
#define GETFULLPATH(PATH, MODE)		trap7(67, PATH, MODE)
#define PATHCMP(P1, P2)			trap7(68, P1, P2)
#define FLOADREL(PATH, GM)		trap7(69, PATH, GM)
#define ANIPSEL(IPNUM)			trap7(70, IPNUM)
#define ANOPSEL(OPNUM)			trap7(71, OPNUM)
#define ANOPDIS(V)			trap7(72, V)
#define ADC()				trap7(73)
#define DAC(VAL)			trap7(74, VAL)
#define SET_LED(VAL)			trap7(75, VAL)
#define FREETONE(TABLE, TABLEN, L, P)	trap7(76, TABLE, TABLEN, L, P)
#define FTTIME()			trap7(77)
#define	RDIPORT()			trap7(78)
#define RDBIPORT()			trap7(79)
#define SETSTVEC(VECNUM, WHERETO)	trap7(80, VECNUM, WHERETO)
#define NEW_CBUF(BNUM, ADDR, LEN)	trap7(81, BNUM, ADDR, LEN)
#define PROG_SIO(CHAN, SPPTR)		trap7(82, CHAN, SPPTR)
#define GETTDSTR(BUF)			trap7(83, BUF)
/* Extra (V4.2) modes for gettdstr */
#define CVTTDSTR(dateptr, mybuf)	trap7(83, 0, 0, dateptr, mybuf)
#define READTIMEINC()			trap7(83, 0, 1, 0)
#define SETTIMEINC(inc)			trap7(83, 0, 1, inc)

#define NLEDIT(STR, LEN)		trap7(84, STR, LEN)
#define LEDIT(STR)			trap7(86, STR)
#define IEXEC(PROMPT)			trap7(87, PROMPT)
#define EXEC(LB)			trap7(88, LB)
#define CALLMRD(N, CMD, ARG)		trap7(89, N, CMD, ARG)
#define SET_KVEC(VEC)			trap7(90, VEC)
#define CLPARSE(A, T, V)		trap7(91, A, T, V)
#define QSORT(B, N, W, C)		trap7(92, B, N, W, C)
#define SLICEARGS(S, A, C)		trap7(93, S, A, C)
#define CPUSPEED()			trap7(94)
#define FIND_DRIVER(IORO, NAME)		trap7(95, IORO, NAME)
#define GET_DVRLIST(IORO)		trap7(96, IORO)
#define EXECA(ARGV)			trap7(97, ARGV)
#define EXECV(PATH, ARGV)		trap7(98, PATH, ARGV)
#define OPTION(OPNUM, SET)		trap7(99, OPNUM, SET)
#define INST_BDVR(BR, BW, BS, NAME, B)	trap7(100, BR, BW, BS, NAME, B)
#define FIND_BDVR(NAME)			trap7(102, NAME)
#define BLKREAD(BLK, BUF, DEV)		trap7(103, BLK, BUF, DEV)
#define BLKWRITE(BLK, BUF, DEV)		trap7(104, BLK, BUF, DEV)
#define OPEN(NAME, MODE)		trap7(105, NAME, MODE)
#define READ(FD, BUF, NBYTES)		trap7(106, FD, BUF, NBYTES)
#define CLOSE(FD)			trap7(107, FD)
#define CREAT(NAME, BITS, ADDR)		trap7(108, NAME, BITS, ADDR)
#define WRITE(FD, BUF, NBYTES)		trap7(109, FD, BUF, NBYTES)
#define UNLINK(NAME)			trap7(110, NAME)
#define RENAME(OLD, NEW)		trap7(111, OLD, NEW)
#define FILESTAT(NAME, BUF)		trap7(112, NAME, BUF)
#define READDIR(DEV, BUF, DP, POS, PAR)	trap7(113, DEV, BUF, DP, POS, PAR)
#define INTERPBEC(EC, BUF)		trap7(114, EC, BUF)
#define SEEK(FD, OFFSET, MODE)		trap7(115, FD, OFFSET, MODE)
#define TELL(FD)			trap7(116, FD)
#define BDMISC(BDNUM, CODE, ARG1)	trap7(117, BDNUM, CODE, ARG1)
#define PROCESSDIR(PN, BUF, MODE)	trap7(118, PN, BUF, MODE)
#define MULTIBLKIO(dvr,cmd,ptr,blk,nblks) trap7(119, dvr,cmd,ptr,blk,nblks)
#define FPUTS(FD, BUF)			trap7(121, FD, BUF)
#define ERRMES(EC)			trap7(122, EC)
#define FGETS(FD, BUF)			trap7(123, FD, BUF)
#define RDALLDIR(p, mm, sm, ps)		trap7(124, p, mm, sm, ps)
#define OSCONTROL(a1, a2)		trap7(125, a1, a2)
#define OSC1(a1)			trap7(125, a1)
#define GETROMVER()			trap7(126)
#define	LOCKZ80(mode, arg)		trap7(127, mode, arg)
#define AEXECA(argv, isasync)		trap7(128, argv, isasync)
#define PROCCNTL(mode, arg)		trap7(129, mode, arg)
#define NEWCHSET(PTR)			trap7(130, PTR)
#define SCHEDPREP(addr, av, flags, ss)	trap7(131, addr, av, flags, ss)
#define PIPE(ptr)			trap7(132, ptr)
#define CDMISC(dvr, cmd, p1, p2, p3)	trap7(133, dvr, cmd, p1, p2, p3)
#define SLINK(one, two, three)		trap7(134, one, two, three)
#define ALIAS(cmd, arg)			trap7(135, cmd, arg)
#define CRTC_INIT(mode, p)		trap7(140, mode, p)
#define CHKPERM(pdirent, mask, path)	trap7(141, pdirent, mask, path)
#define FNLEDIT(buf, len, in, out)	trap7(142, buf, len, in, out)

/* Various modes for proccntl */
#define GETPID()			trap7(129, 0)
#define GETPPID(PID)			trap7(129, 1, PID)
#define KILL(PID)			trap7(129, 3, PID)
#define SLEEP(TICKS)			trap7(129, 4, TICKS)
#define GETPROCTAB(PID)			trap7(129, 5, PID)
#define CWD(PID, PATH)			trap7(129, 6, PID, PATH)
#define LOCKIN(MODE)			trap7(129, 7, MODE)
#define RUNSTATS(mode)			trap7(129, 8, mode)
#define WAIT(PID)			trap7(129, 9, PID)
#define NICE(PID, V)			trap7(129, 10, PID, V)
#define SIGSEND(PID, V1, V2)		trap7(129, 11, PID, V1, V2)
#define SIGCATCH(V)			trap7(129, 12, V)
#define SENDINTERRUPT(RP)		trap7(129, 13, RP)
#define PROCTRACE(RP)			trap7(129, 14, RP)
#define GETPROCFLAGS(PID)		trap7(129, 15, PID)
#define ISINTERACTIVE(PID)		trap7(129, 16, PID)
#define NOSPCHECK(pid, mode)		trap7(129, 17, pid, mode)
#define CSVEC(v)			trap7(129, 18, v)
#define GETPCURPID()			trap7(129, 19)
#define READSIGVEC(pid)			trap7(129, 20, pid)
#define FSBPTR()			trap7(129, 21)
#define FSPPTR()			trap7(129, 22)
#define SSPTR()				trap7(129, 23)
#define	KILLUSER(hs)			trap7(129, 24, hs)
#define SIGBLOCK(pid, blk)		trap7(129, 25, pid, blk)
#define ALARM(n)			trap7(129, 26, n)
#define SIGBLOCKER(rootpid, sig, arg)	trap7(129, 27, rootpid, sig, arg)
#define SNOOZE(vec, p1, p2)		trap7(129, 28, vec, p1, p2)
#define SIGUSER(pid, sig, arg)		trap7(129, 29, pid, sig, arg)
#define FINDHOMESHELL(pid)		trap7(129, 30, pid)
#define SETSHNICE(pid, nice)		trap7(129, 31, pid, nice)
#define LASTCHILD(pid)			trap7(129, 32, pid)
#define SWPPTR()			trap7(129, 33)
#define KILLDOWN(STARTPID)		trap7(129, 34, STARTPID)
#define SIGDOWN(STARTPID, VAL1, VAL2)	trap7(129, 35, STARTPID, VAL1, VAL2)
#define KILLUID(uid)			trap7(129, 36, uid)
#define SIGUID(uid, p1, p2)		trap7(129, 37, uid, p1, p2)
#define SETSIGEXIT(pid, mode)		trap7(129, 38, pid, mode)
#define SETPG(pg)			trap7(129, 39, pg)
#define SIGPG(pg, sig, arg)		trap7(129, 40, pg, sig, arg)
#define KILLPG(pg)			trap7(129, 41, pg)
#define SETPROCUMASK(pid, mask)		trap7(129, 42, pid, mask)
#define SETENVBITS(pid, mask, set)	trap7(129, 43, pid, mask, set)
#define GETENVBITS(pid)			trap7(129, 44, pid)
#define NAMETOPID(pid)			trap7(129, 45, pid)
#define BLOCKTX(pid, addr, len, sig)	trap7(129, 46, pid, addr, len, sig)
#define BLOCKRX(mode)			trap7(129, 47, mode)
#define SETENV(pid, name, set, mode)	trap7(129, 48, pid, name, set, mode)
#define GETENV(pid, name, mode)		trap7(129, 49, pid, name, mode)

/* The modes for oscontrol */
#define SETIBCVEC(vec)			OSCONTROL(0, vec)
#define OBRAMSTART()			OSC1(1)
/* No 2! */
#define READIBCVEC()			OSC1(3)
#define FORCELEVEL0()			OSC1(4)
#define READXPATH(n)			OSCONTROL(5, n)
#define READASSIGN(m)			OSCONTROL(6, n)
#define SETUMASK(n)			OSCONTROL(7, n)
#define READUMASK()			OSC1(8)
#define SETUID(n)			OSCONTROL(9, n)
#define READUID()			OSC1(10)
#define GET_BDEV(path)			OSCONTROL(11, path)
#define DUMPLASTLINES(cdev)		OSCONTROL(12, cdev)
#define SETWILDVEC(vec)			OSCONTROL(13, vec)
#define READWILDVEC()			OSC1(14)
#define VIDEO_INIT(level)		OSCONTROL(17, level)
#define KB_INIT(level)			OSCONTROL(18, level)
#define SETBEEPVOL(vol)			OSCONTROL(19, vol)
#define READBEEPVOL()			OSC1(20)
#define SETBEEPVEC(vec)			OSCONTROL(21, vec)
#define READBEEPVEC()			OSC1(22)
#define NOUSEFFBS(n)			OSCONTROL(23, n)
#define READFFBS()			OSC1(24)
#define SETMEMFAULT(n)			OSCONTROL(25, n)
#define RXTXPTR(n)			OSCONTROL(26, n)
#define SETBDLOCKIN(n)			OSCONTROL(27, n)
#define READBDLOCKIN()			OSC1(28)
#define STARTOFDAY()			OSC1(29)
#define PFASTCHAR()			OSC1(30)
#define SRBRCLOCK(n)			OSCONTROL(31, n)
#define TIMER1USED()			OSC1(32)
#define TRASHASSIGN(uid)		OSCONTROL(33, uid)
#define TRASHENVSTRINGS(pid)		OSCONTROL(34, pid)
#define ENVSUB(s, dollaronly, mmode)	trap7(125, 35, s, dollaronly, mmode)
#define DOASSIGN(argc, argv)		trap7(125, 36, argc, argv)

/*
 * Read entry points from ROM to use more arguments
 */

#ifndef __GNUC__
#define PRINTF		(*(int(*)(char *, ...))SETSTVEC(48, -1))
#define SPRINTF		(*(int(*)(char *, char *, ...))SETSTVEC(49, -1))
#define FPRINTF		(*(int(*)(int, char *, ...))SETSTVEC(120, -1))
#endif

#ifdef __GNUC__
extern PRINTF(char *, ...);
extern SPRINTF(char *, char *, ...);
extern FPRINTF(int, char *, ...);
#endif

CPP_END

#endif SYSCALLS_H
