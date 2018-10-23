# TABS4
# AKPM's makefile for gcc

CC34		=	gcc34
CPP34		=	g++34
OUTPUT		=	$(CPP34) $(CC34)

TMS34010	=	-DTMS34010 -DGSP8=gsp8\(\) -DGSP1=1
DEFINES		=	$(TMS34010) -DUSG -D_INCLUDE_POSIX_SOURCE -DAKPM_LOOP \
				-DIN_GCC -DHANDLE_SYSV_PRAGMA -DAKPM_SHORT \
				-DIO_BUFFER_SIZE=16384 -DNO_STAB_H -D__STDC__ -DAKPM_REGPARM \
				-DAKPM_RETS
STRING		=	#-DFAST_STRING
CPPFLAGS	=	$(STRING) $(DEFINES)
CPU			=	#-m68020
OPTFLAGS1	=	#-O
OPTFLAGS2	=	#-finline-functions #-fomit-frame-pointer
OPTFLAGS	=	$(OPTFLAGS1) $(OPTFLAGS2)
CFLAGS		=	$(CPPFLAGS) $(OPTFLAGS) $(XCFLAGS) $(CPU)

XLDFLAGS	=
LDFLAGS		=	#$(XLDFLAGS) -d -r -G
LIBDIR		=
GDBLIB		=	#$(LIBDIR)libgdb.a
LIBS		=	alloca.o -lm

CC			=	cc -g $(XCCFLAGS)

OFILES =	insn-peep.o insn-recog.o insn-extract.o insn-output.o \
			insn-emit.o insn-attrtab.o c-parse.o c-lang.o c-lex.o \
			c-decl.o c-typeck.o c-convert.o c-aux-info.o c-common.o \
			c-pragma.o toplev.o version.o tree.o print-tree.o insn-opinit.o \
			stor-layout.o fold-const.o function.o stmt.o expr.o aux-output.o \
			calls.o expmed.o explow.o optabs.o varasm.o rtl.o \
			print-rtl.o rtlanal.o emit-rtl.o dbxout.o sdbout.o \
			dwarfout.o xcoffout.o integrate.o jump.o cse.o loop.o \
			unroll.o flow.o stupid.o combine.o regclass.o real.o \
			local-alloc.o global.o reload.o reload1.o caller-save.o convert.o \
			bc-emit.o bc-optab.o c-iterate.o \
			reorg.o sched.o final.o recog.o reg-stack.o \
			obstack.o getpwd.o readwrite.o abort.o akpm.o akpmflags.o \
			shortlabels.o dm.o

CPOFILES=	cp-decl.o cp-decl2.o cp-typeck.o cp-type2.o cp-tree.o cp-ptree.o \
			cp-cvt.o cp-search.o cp-lex.o cp-gc.o cp-call.o cp-class.o \
			cp-init.o cp-method.o cp-except.o cp-expr.o cp-pt.o cp-edsel.o \
			cp-xref.o cp-spew.o cp-error.o cp-errfn.o cp-parse.o c-common.o \
			insn-opinit.o toplev.o version.o tree.o print-tree.o \
			stor-layout.o fold-const.o function.o stmt.o expr.o calls.o \
			expmed.o explow.o optabs.o varasm.o rtl.o print-rtl.o rtlanal.o \
			emit-rtl.o dbxout.o sdbout.o dwarfout.o xcoffout.o integrate.o \
			jump.o cse.o loop.o unroll.o flow.o stupid.o combine.o convert.o \
			regclass.o local-alloc.o global.o reload.o real.o reload1.o \
			caller-save.o bc-emit.o bc-optab.o insn-peep.o reorg.o sched.o \
			final.o recog.o reg-stack.o insn-recog.o insn-extract.o \
			insn-output.o insn-emit.o insn-attrtab.o aux-output.o getpwd.o \
			obstack.o readwrite.o abort.o akpm.o akpmflags.o shortlabels.o \
			c-pragma.o dm.o

GEN_HFILES=	insn-config.h insn-attr.h insn-flags.h insn-codes.h \
			bc-opcode.h bc-opname.h bc-arity.h

output:		$(OUTPUT)
			echo "\007\c"

$(CC34):	hofiles link
			echo "\007\c"

$(CPP34):	cpphofiles cpplink
			echo "\007\c"

hofiles: hfiles objects
cpphofiles: hfiles cppobjects

hfiles:	$(GEN_HFILES)

objects: $(OFILES)
cppobjects: $(CPOFILES)

link:
	$(CC) $(LDFLAGS) $(LFILES) $(OFILES) $(LIBS) -o $(CC34)

cpplink:
	$(CC) $(LDFLAGS) $(LFILES) $(CPOFILES) $(LIBS) -o $(CPP34)

clean:
	rm -f genattr genattrtab gencodes genconfig genemit genextract genflags \
		genoutput genpeep genrecog genopinit bi-opcode bi-opname bi-arity
	rm -f insn-attr.h insn-codes.h insn-config.h insn-flags.h bc-opcode.h \
		bc-opname.h bc-arity.h
	rm -f insn-attrtab.c insn-emit.c insn-extract.c insn-output.c insn-peep.c \
		insn-recog.c insn-opinit.c
	rm -f *.o $(CC34) $(CPP34)
	rm -f t.c.* t[0-9].c.*
	rm -f x y
	rm -f *.mix *.go *.g
	rm -f *.combine *.cse *.cse2 *.dbr *.flow *.greg *.jump *.jump2 *.loop \
		*.lreg *.rtl *.sched *.sched2

genattr:	genattr.o rtl.o obstack.o readwrite.o alloca.o
			$(CC) $(LDFLAGS) $(LFILES) genattr.o rtl.o obstack.o readwrite.o \
				$(LIBS) $(COFFEE) $(COFFEEFLAGS) -o genattr

insn-attr.h:	md genattr
				genattr md > insn-attr.h

genflags:	genflags.o rtl.o obstack.o readwrite.o alloca.o
			$(CC) $(LDFLAGS) $(LFILES) genflags.o rtl.o obstack.o readwrite.o \
				$(LIBS) $(COFFEE) $(COFFEEFLAGS) -o genflags

insn-flags.h:	md genflags
				genflags md > insn-flags.h

gencodes:	gencodes.o rtl.o obstack.o readwrite.o alloca.o
			$(CC) $(LDFLAGS) $(LFILES) gencodes.o rtl.o obstack.o readwrite.o \
				$(LIBS) $(COFFEE) $(COFFEEFLAGS) -o gencodes

insn-codes.h:	md gencodes
				gencodes md > insn-codes.h

genconfig:		genconfig.o rtl.o obstack.o readwrite.o alloca.o
				$(CC) $(LDFLAGS) $(LFILES) genconfig.o rtl.o obstack.o \
					readwrite.o $(LIBS) $(COFFEE) $(COFFEEFLAGS) -o genconfig

insn-config.h:	md genconfig
				genconfig md > insn-config.h

bi-opcode:		bi-opcode.o bi-parser.o bi-lexer.o bi-reverse.o obstack.o
				$(CC) $(LDFLAGS) $(LFILES) bi-opcode.o bi-parser.o bi-lexer.o \
					bi-reverse.o obstack.o \
					$(LIBS) $(COFFEE) $(COFFEEFLAGS) -o bi-opcode

bc-opcode.h:	bi-opcode bytecode.def
				./bi-opcode < ./bytecode.def > bc-opcode.h

bi-opname:		bi-opname.o bi-parser.o bi-lexer.o bi-reverse.o obstack.o
				$(CC) $(LDFLAGS) $(LFILES) bi-opname.o bi-parser.o bi-lexer.o \
					bi-reverse.o obstack.o \
					$(LIBS) $(COFFEE) $(COFFEEFLAGS) -o bi-opname
				
bc-opname.h:	bi-opname
				./bi-opname < ./bytecode.def > bc-opname.h

bi-arity:		bi-arity.o bi-parser.o bi-lexer.o bi-reverse.o obstack.o
				$(CC) $(LDFLAGS) $(LFILES) bi-arity.o bi-parser.o bi-lexer.o \
					bi-reverse.o obstack.o \
					$(LIBS) $(COFFEE) $(COFFEEFLAGS) -o bi-arity

bc-arity.h:		bi-arity
				./bi-arity < bytecode.def > bc-arity.h

genpeep:		genpeep.o rtl.o obstack.o readwrite.o alloca.o
				$(CC) $(LDFLAGS) $(LFILES) genpeep.o rtl.o obstack.o \
					readwrite.o $(LIBS) $(COFFEE) $(COFFEEFLAGS) -o genpeep

insn-peep.c:	md genpeep
				genpeep md > insn-peep.c

genrecog:		genrecog.o rtl.o obstack.o readwrite.o alloca.o
				$(CC) $(LDFLAGS) $(LFILES) genrecog.o rtl.o obstack.o \
					readwrite.o $(LIBS) -o genrecog

insn-recog.c:	md genrecog
				genrecog md > insn-recog.c

genextract: 	genextract.o rtl.o obstack.o readwrite.o alloca.o
				$(CC) $(LDFLAGS) $(LFILES) genextract.o rtl.o obstack.o \
					readwrite.o $(LIBS) -o genextract

insn-extract.c: 	md genextract
				genextract md > insn-extract.c

genoutput: 	genoutput.o rtl.o obstack.o readwrite.o alloca.o
				$(CC) $(LDFLAGS) $(LFILES) genoutput.o rtl.o obstack.o \
					readwrite.o $(LIBS) -o genoutput

insn-output.c:	md genoutput
				genoutput md > insn-output.c

genemit:		genemit.o rtl.o obstack.o readwrite.o alloca.o
				$(CC) $(LDFLAGS) $(LFILES) genemit.o rtl.o obstack.o \
					readwrite.o $(LIBS) -o genemit

insn-emit.c:	md genemit
				genemit md > insn-emit.c

genattrtab:		genattrtab.o rtl.o print-rtl.o rtlanal.o obstack.o \
					readwrite.o alloca.o
				$(CC) $(LDFLAGS) $(LFILES) genattrtab.o rtl.o print-rtl.o \
					rtlanal.o obstack.o readwrite.o $(LIBS) -o genattrtab

insn-attrtab.c:	md genattrtab
				genattrtab md > insn-attrtab.c

genopinit:		genopinit.o obstack.o rtl.o readwrite.o alloca.o
				$(CC) $(LDFLAGS) $(LFILES) genopinit.o obstack.o rtl.o \
					readwrite.o $(LIBS) -o genopinit

insn-opinit.c:	md genopinit
				genopinit md > insn-opinit.c

insn-peep.o:	insn-peep.c

insn-recog.o:	insn-recog.c

insn-extract.o:	insn-extract.c

insn-output.o:	insn-output.c

insn-emit.o:	insn-emit.c

insn-attrtab.o:	insn-attrtab.c

insn-opinit.o:	insn-opinit.c

optabs.o:		optabs.c md

cp-parse.o:		cp-parse.c

c-parse.c:		c-parse.y
				bison -d c-parse.y
				mv c-parse.tab.c c-parse.c
				mv c-parse.tab.h c-parse.h
