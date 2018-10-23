#line 1 "CCBFIT.c"


































































































































#line 1 ":::inc:stdio.h"















#line 1 ":::inc:DBInterface.h"





































enum DBOpenModes { DBO_TERMINAL = 0, DBO_CREAT = 1, DBO_APPEND = 2, DBO_RDONLY = 3 };


enum DBModeModes { DBMODE_RAW = 1, DBMODE_COOKED = 2, DBMODE_ECHO = 3, DBMODE_NOECHO = 4 };





















short DBMode(short handle, enum DBModeModes mode);

short DBPrintf(short handle, char *format, ...)

           __attribute__ ((format (printf, 2, 3)))

    ;








extern char DBSuppressed[          8 ];         

#line 116 ":::inc:DBInterface.h"

















void DBDisable(short handle);
void DBEnable(short handle);
short DBIsEnabled(short handle);
short DBNoSessionName(short handle, short noSessionName);
short DBRead(short handle, unsigned char *data, unsigned long length);
short DBWrite(short handle, unsigned char *odata, unsigned long olength);
short DBClose(short handle);
short DBOpen(char *sessionName, enum DBOpenModes mode);
short DBReset(void);
short DBInit(void);
short DBTimeStamp(short handle, short turnon);


typedef long (*DBInitIOFunc)(unsigned char channel, unsigned char *buf, unsigned long nbytes);


DBInitIOFunc	DBSetAuxWriteFunc(DBInitIOFunc val);
short 			DBELDInit(DBInitIOFunc readFunc, DBInitIOFunc writeFunc,
                	unsigned char readChannel, unsigned char writeChannel);
short			DBELDWrite(unsigned char *data, unsigned long nBytes);
short			DBELDRead(unsigned char *data, unsigned long dataLen, long timeoutMilliSec);


char *dgets(    unsigned char (*getfn)(void),
                void (*putfn)(unsigned char),
                unsigned char *buf,
                unsigned long buflen);


extern volatile unsigned long BC_Timer;


#line 16 ":::inc:stdio.h"







#line 1 ":::inc:libc.h"



































#line 1 ":::inc:stdarg.h"
















typedef char *va_list;














#line 36 ":::inc:libc.h"



long		atoi(const char *);
long		htoi(const char *);
void		itoa(long val, char *str, int base);

int			isupper(int);
int			islower(int);
int			toupper(int);
int			tolower(int);
int			isdigit(int);

void		movmem(const char *from, char *to, long nbytes);
void		bzero(char *addr, long nbytes);
void		bsetmem(char *addr, long val, long nbytes);
void		wsetmem(char *addr, long val, long nwords);
void		lsetmem(char *addr, long val, long nlongs);
void		swapmem(char *s1, char *s2, long nbytes);
void		delay100(long n100usecs);
void		reset(void);


typedef void (*writefunc_t)(void *arg, char *buf, unsigned long nbytes);

void		uprintf(void *arg, writefunc_t writefunc, char **fmt);
int			memprintf(char *buf, unsigned long bufsize, const char *fmt, ...);
int			sprintf(char *buf, const char *fmt, ...);
int			vmemprintf(char *buf, unsigned long bufsize, const char *fmt, va_list arg);
int			vsprintf(char *buf, const char *fmt, va_list arg);


#line 23 ":::inc:stdio.h"


#line 131 "CCBFIT.c"

#line 1 ":::inc:string.h"













extern void *	memcpy (void *dest, const void *src, unsigned		 n);
extern void *	memmove (void *dest, const void *src, unsigned		 n);
extern char *	strcpy(char *destStr, const char *srcStr);
extern char *	strncpy(char *destStr, const char *srcStr, unsigned		 n);
extern char *	strcat(char *destStr, const char *srcStr);
extern char *	strncat(char *destStr, const char *srcStr, unsigned		 n);
extern int	memcmp(const void *s1, const void *s2, unsigned		 n);
extern int	strcmp(const char *s1, const char *s2);
extern int	stricmp(const char *s1, const char *s2);
extern int	strncmp(const char *s1, const char *s2, unsigned		 n);
extern int	strnicmp(const char *s1, const char *s2, unsigned		 n);
extern void *	memchr(const void *s, int c, unsigned		 n);
extern unsigned			strcspn(const char *srcStr, const char *s2);
extern char *	strpbrk(const char *s1, const char *s2);
extern unsigned			strspn(const char *spanChrs, const char *srcStr);
extern char *	strstr(const char *s1, const char *s2);
extern char *	strtok(char *destStr, const char *tokenStr);
extern void *	memset(void *dest, int c, unsigned		 n);
extern unsigned			strlen(const char *);
extern char *	strchr(const char *s, int c);
extern char *	strrchr(const char *s, int c);
extern char *	strupr(char *s);
extern char *	strlwr(char *s);
extern char *   strdup(const char *s);


#line 132 "CCBFIT.c"

#line 1 "::inc:flags.h"



















































































































































































































































#line 297 "::inc:flags.h"



#line 133 "CCBFIT.c"

#line 1 "::inc:gendef.h"





























































































































































#line 1 ":::inc:local.h"







































typedef unsigned char BYTE;              
typedef unsigned char BCD;               
typedef int BOOLEAN, BOOL;               
typedef short int INT16;                 
typedef unsigned short int UINT16;       
typedef long INT32;                      
typedef unsigned long UINT32;            







































#line 157 "::inc:gendef.h"


#line 169 "::inc:gendef.h"















#line 203 "::inc:gendef.h"































#line 134 "CCBFIT.c"

#line 1 "::inc:tune.h"











































































































































































































































#line 135 "CCBFIT.c"

#line 1 "::inc:exec.h"



































































































































typedef unsigned long MWORD;                                 













typedef unsigned int U_CLOCK;                    
typedef int S_CLOCK;                               



typedef int   SEQNO;                          
typedef int   PRIORITY;                   
typedef int   TIMER_COUNT;              
















typedef struct tcb
{
   int         Status;                                 
   struct tcb  *Next;                         
   struct tcb  **Queue;                             
   int         Priority;                             
   MWORD       Contxt[      1 ];                   
   void        (*Task)     (void) ;         

   MWORD       *Stack;                       
   int         StackSize;                       

}
TCB;
typedef struct tcb *TASK_ID;





typedef struct semaphore
{
   int         Value;                                
   TCB         *Head;         
}
SEM;
typedef struct semaphore *SEM_ID;






typedef struct ext_memory_addr
{
   BYTE        *Addr;                                
}
EXT_MEM;











typedef struct mail_descriptor
{
   struct mail_descriptor *Next;          
   EXT_MEM                ExtMem;                   
   SEQNO                  Sn;         
   int                    Priority;                 
}
MAIL_DESCRIPTOR;











typedef struct mail_queue
{
   int               Limit;                  
   int               Avail;            
   MAIL_DESCRIPTOR   *Head;                     
}
MAIL_Q;
typedef struct mail_queue *MAIL_Q_ID;










typedef struct mailbox
{
   SEM               Sem;                       
   MAIL_Q            Up;                               
   MAIL_Q            Down;                           
   MAIL_Q            *LastRead;         
}
MAILBOX;
typedef struct mailbox *MAILBOX_ID;













typedef struct timer_block
{
   struct timer_block   *Next;             
   struct timer_block   *Prev;               
   MAILBOX              *MailboxID;    
   EXT_MEM              ExtMem;                     
   U_CLOCK              ExpireTime;               
   SEQNO                Sn;           
   BYTE                 Avail;        
   int                  Priority;             
}
TIMER_BLOCK;
typedef struct timer_block *TIMER_ID;

































#line 1 ":::DTAinc:Interrupts.h"

















































































































#line 126 ":::DTAinc:Interrupts.h"



#line 142 ":::DTAinc:Interrupts.h"



#line 166 ":::DTAinc:Interrupts.h"



#line 186 ":::DTAinc:Interrupts.h"
















static inline unsigned short ReadSR(void)
{
	unsigned short sr;

	asm volatile ("
		movew	sr,%0
		andw	#0x0700,%0
	" : "=d" (sr) : );
	return sr;
}












static inline unsigned short ReadAndRaiseSR4(void)
{
	unsigned short sr;

	asm volatile (
		"movew	sr,%0
		andw	#0x0700,%0
"






		"orw		#0x0400,sr
" : "=d" (sr) : );
	return sr;
}

static inline unsigned short ReadAndRaiseSR6(void)
{
	unsigned short sr;

	asm volatile ("
		movew	sr,%0
		andw	#0x0700,%0
		orw		#0x0600,sr
	" : "=d" (sr) : );
	return sr;
}

static inline unsigned short ReadAndRaiseSR7(void)
{
	unsigned short sr;

	asm volatile ("
		movew	sr,%0
		andw	#0x0700,%0
		orw		#0x0700,sr
	" : "=d" (sr) : );
	return sr;
}


static inline unsigned long ReadPC(void)
{
	unsigned long ret;

	asm volatile ("
		pea		pc@(0)
		movel	sp@+,%0
	" : "=&d" (ret) : );
	return ret;
}

#line 303 ":::DTAinc:Interrupts.h"








static inline void ResetSR(unsigned short InterruptMask)

{
	unsigned short tmp;
#line 330 ":::DTAinc:Interrupts.h"

	asm volatile ("
		movew	sr,%0
		andw	#0xF8FF,%0
		orw		%1,%0
		movew	%0,sr
	" : "=&d" (tmp) : "d" (InterruptMask));
}


#line 373 ":::DTAinc:Interrupts.h"





#line 324 "::inc:exec.h"








extern unsigned short gIntLevel;














































#line 391 "::inc:exec.h"

























































extern U_CLOCK MasterClock;                      
extern SEM MainSem;                           

extern INT16 CSLevel;                         
extern INT16 NPSLevel;                     
extern int ExecErr;                                  
extern int TSMinPri;                    
extern int TSMaxPri;                    


extern int MS_UsedMailDesc;          
extern int MS_MaxMailDesc;         
extern int MS_UsedTimers;                      
extern int MS_MaxTimers;             








                       void ClearTimer           (TIMER_BLOCK *, SEQNO, MAILBOX *) ;
                       void ExecExit             (void) ;
                       void _ExitCS              (void) ;
                       void _ExitICS             (void) ;
                       void _ExitNPS             (void) ;
                       void _ExitTCS             (BOOLEAN) ;
                       void InitMail             (MAIL_DESCRIPTOR *, int) ;
                       void InitTCB              (TCB *, MWORD *, int, void (*)    (void) ) ;
                       void InitTimer            (TIMER_BLOCK *, int) ;
                       void MailBoxInit          (MAILBOX *, int) ;
                       void MemSet               (BYTE *, BYTE, unsigned) ;
                       void ReadMail             (MAILBOX *, EXT_MEM *) ;
                       void SemaphoreSignal      (SEM *) ;
                       void SemaphoreWait        (SEM *) ;
                       void SemInit              (SEM *, int) ;
                       int SendTMail             (MAILBOX *, int, EXT_MEM *, int, SEQNO) ;
                       void SetPriority          (TCB *, int) ;
                       int SetTimer              (MAILBOX *, EXT_MEM *, int, TIMER_BLOCK **, SEQNO *, int) ;

                       void ShowQueue            (SEM *) ;
                       void ShowReadyList        (void) ;
                       void ShowTimers           (void) ;

                       int Test                  (SEM *) ;
                       void TickHandler          (void) ;

                       int StackUsed             (TCB *) ;





#line 136 "CCBFIT.c"

#line 1 "::inc:execuser.h"
































































































































































































typedef void (*INTVEC)     (void) ;












#line 222 "::inc:execuser.h"


















                       void        FinUsr              (void) ;
                       void        InitUsr             (void) ;

                       void        MemStats            (void) ;


                       void        StkStats            (void) ;


                       void        TimerOff            (void) ;
                       void        TimerOn             (void) ;











                      void noteL3Up				    (BYTE) ;
                      void noteL1Up				    (BYTE) ;



































#line 311 "::inc:execuser.h"


#line 321 "::inc:execuser.h"


#line 331 "::inc:execuser.h"


#line 341 "::inc:execuser.h"



  extern TCB TCB_TRACER ;
    extern MWORD                          STACK_TRACER[        1024  ] ;
                      void Tracer     (void) ;
  extern MAILBOX TRACER_Mb ;
  extern MAILBOX TRACER_PVT_Mb ;





  extern TCB TCB_ME ;
    extern MWORD                          STACK_ME[     512 ] ;
                      void MEIF_StartME     (void) ;
  extern MAILBOX ME_Mb ;




#line 371 "::inc:execuser.h"


#line 382 "::inc:execuser.h"


#line 413 "::inc:execuser.h"


#line 444 "::inc:execuser.h"


#line 463 "::inc:execuser.h"










#line 481 "::inc:execuser.h"



  extern TCB TCB_CC ;
    extern MWORD                          STACK_CC[     1024 ] ;
                      void CC_Task     (void) ;
  extern MAILBOX CC_Mb ;



























#line 527 "::inc:execuser.h"




















  extern TCB TCB_L3 ;
    extern MWORD                          STACK_L3[     1024 ] ;
                      void L3IF_StartL3     (void) ;
  extern MAILBOX L3_Mb ;




  extern TCB TCB_L2 ;
    extern MWORD                          STACK_L2[     512 ] ;
                      void L2IF_StartL2     (void) ;
  extern MAILBOX L2_Mb ;




  extern TCB TCB_SET_IF ;
    extern MWORD                          STACK_SET_IF[     512 ] ;
                      void SET_IF_Task     (void) ;
  extern MAILBOX SET_IF_Mb ;           




  extern TCB TCB_BC_MGR ;
    extern MWORD                          STACK_BC_MGR[     1024 ] ;
                      void BC_Mgr_Task     (void) ;
  extern MAILBOX BC_MGR_Mb ;




  extern TCB TCB_HOST_IF ;
    extern MWORD                          STACK_HOST_IF[     1024 ] ;
                      void HOST_IF_Task     (void) ;
  extern MAILBOX HOST_IF_Mb ;



#line 594 "::inc:execuser.h"












#line 622 "::inc:execuser.h"



  extern TCB TCB_HDLC_XRQ_1 ;
    extern MWORD                          STACK_HDLC_XRQ_1[     1024 ] ;
                      void M302DRV_HDLC_XRQ_1_Task     (void) ;
  extern MAILBOX HDLC_XRQ_1_Mb ;

  extern MAILBOX HDLC_ISR_1_Mb ;



  extern TCB TCB_HDLC_RCV_1 ;
    extern MWORD                          STACK_HDLC_RCV_1[     1024 ] ;
                      void M302DRV_HDLC_RCV_1_Task     (void) ;
extern SEM HDLC_RCV_1_Sem ;



#line 659 "::inc:execuser.h"


#line 679 "::inc:execuser.h"






#line 693 "::inc:execuser.h"


#line 703 "::inc:execuser.h"


#line 713 "::inc:execuser.h"


#line 723 "::inc:execuser.h"



  extern TCB TCB_L1 ;
    extern MWORD                          STACK_L1[     512 ] ;
                      void L1_Srq_Task     (void) ;
  extern MAILBOX L1_Mb ;













































#line 783 "::inc:execuser.h"




#line 137 "CCBFIT.c"

#line 1 "::inc:lif.h"



















































































































#line 1 "::inc:pools.h"



































































































                      int  GetBlkSize         (int) ;
                      int  GetBlock           (int, EXT_MEM *) ;
                      void InitMemMgr         (void) ;
                      int  MakePool           (int *, int, int, int) ;



                      void ReturnBlock        (int, EXT_MEM *) ;
                      int  TestBlkAvail       (int) ;
                      int  TestBlkLimit       (int) ;

                      int  TestMaxUsed        (int) ;



#line 116 "::inc:lif.h"




#line 1 ":::inc:stdlib.h"






extern void exit(int code);
#line 120 "::inc:lif.h"


#line 1 ":::inc:stdio.h"
























#line 122 "::inc:lif.h"

#line 1 ":::inc:ctype.h"








#line 123 "::inc:lif.h"


















#line 152 "::inc:lif.h"




















































































#line 244 "::inc:lif.h"
























































































































































































































































































































































































































































































#line 736 "::inc:lif.h"






























#line 780 "::inc:lif.h"








































































































































typedef enum
{
   BRI_NET3_BT_STYPE,
   PRI_4ESS_STYPE,
   PRI_5ESS_STYPE,
   PRI_5ESS_SYM_STYPE,
   BRI_5ESS_STYPE,
   BRI_5ESS_SYM_STYPE,
   PRI_DMS100_STYPE,
   BRI_DMS100_STYPE,
   VN2_STYPE,
   KDD_STYPE,
   NTT_STYPE,
   PRI_1TR6_STYPE,
   BRI_1TR6_STYPE,
   BRI_NI1_STYPE,
   BRI_DMS100_S_STYPE,
   TPH_1856_STYPE,
   PRI_NET5_STYPE,
   BRI_TPH1962_STYPE,
   PRI_DASS2_STYPE,
   CCITT_STYPE
}
SWITCH_TYPE;




typedef int BUFFER_TYPE;










typedef struct Packet
{
   INT16 HeadLen;                                      
   BYTE *Header;                                       
   INT16 InfoLen;                                        
   BUFFER_TYPE InfoType;                                   
   BYTE *Info;                                           
   INT16 Owners;              
}
PKT, *PKT_ID;



typedef struct Primitive
{
   BUFFER_TYPE BufType;                 
   INT16       L2_Protocol;           
   INT16       Source_ID;                                  
   BYTE        SourceState;                             
   INT16       Dest_ID;                               
   INT16       Prim_ID;                              
   BYTE        Sapi;                                            
   BYTE        Ces;                                       
   INT16       LLI;                           
   UINT16      Call_ID;                    
   BYTE        Dsl_ID;                       
   INT16       DSL_Chan_ID;       
   BOOLEAN     FreePkt;           
   PKT_ID      Pkt;                                          
}
PRIM, *PRIM_ID;






typedef struct
{
   BUFFER_TYPE     BufType;                     
   INT16           SourceID;                               
   INT16           DestID;                
   INT16           InfoLen;                                   
   BUFFER_TYPE     InfoBufType;     
   BYTE            MsgType;         
   BYTE *          PkgInfo;                             
}
PKG, * PKG_ID;





typedef struct SingleByteCommand
{
   BUFFER_TYPE BufType;                 
   INT16 Source_ID;                                        
   INT16 Dest_ID;                                        
   BYTE Cmd;                                                 
}
SBC, * SBC_ID;






































































extern BOOLEAN TimersOn;



extern SWITCH_TYPE SwitchType;



extern int Trashcan;




extern BUFFER_TYPE NO_INFO_BTYPE;
extern BUFFER_TYPE PRIM_BTYPE;
extern BUFFER_TYPE PKT_BTYPE;
extern BUFFER_TYPE HEADER_BTYPE;
extern BUFFER_TYPE SML_INFO_BTYPE;
extern BUFFER_TYPE LRG_INFO_BTYPE;

extern BUFFER_TYPE PKG_BTYPE;


extern BUFFER_TYPE SBC_BTYPE;




extern BYTE layersUp[              2 ];         








                       INT16     LIF_AddTimerP       (INT16, INT16, INT16, BYTE,
                                        BYTE, BYTE, INT16, INT16,
                                        UINT32, PKT_ID) ;
                       void      LIF_Block           (int) ;
                       void      LIF_CpyPkt          (PKT_ID, PKT_ID) ;
                       void      LIF_CpyPktLogical     (PKT_ID) ;
                       INT16     LIF_CpyPrim         (PRIM_ID, PRIM_ID) ;
                       void      LIF_Dispose         (void *) ;


                       void      LIF_DispPkg         (PKG_ID) ;

                       void      LIF_DispPkt         (PKT_ID) ;
                       void      LIF_DispPrim        (PRIM_ID) ;

                       PKT_ID    LIF_DupPkt          (PKT_ID src) ;


                      void NTWLIF_Report	    (char *, char *, char *, char) ;


#line 1 ":::inc:EventLog.h"



































































typedef struct {
	unsigned char bf[8];
} tLogEvent;











































#line 225 ":::inc:EventLog.h"

















                      void		eventLogInit		    (long apx_reason) ;
                      void		eventLog			    (unsigned char sev) ;
                      void		eventLogDB			    (unsigned char sev, char *file, long line, char *expl) ;
                      char	*	fmt					    (char *format, ...) ;
                      void 	eventLogNVUpdate	    (void)  ;





extern unsigned int MasterClock;




#line 1143 "::inc:lif.h"
























                       void      LIF_FreeBuffer      (BUFFER_TYPE, BYTE *) ;

                       void      LIF_FreePkg         (PKG_ID) ;

                       void      LIF_FreePkt         (PKT_ID) ;
                       void      LIF_FreePrim        (PRIM_ID) ;

                       void      LIF_FreeSbc         (SBC_ID) ;

                       BYTE      *LIF_GetBuffer      (BUFFER_TYPE, BOOLEAN) ;
                       INT16     LIF_GetBufSize      (BUFFER_TYPE) ;

                       int       LIF_GetDriverID     (int, int, int) ;


                       PKG_ID    LIF_GetPkg          (void) ;
                       PKG_ID    LIF_GetPkgI         (BUFFER_TYPE) ;

                       PKT_ID    LIF_GetPkt          (BUFFER_TYPE, INT16, BOOLEAN) ;
                       PRIM_ID   LIF_GetPrim         (BOOLEAN) ;

                       SBC_ID    LIF_GetSbc          (BOOLEAN) ;

                       void      LIF_Init            (void) ;

                       INT16     LIF_MailPkg         (PKG_ID, UINT32, BYTE, unsigned) ;

                       INT16     LIF_MailPrim        (PRIM_ID, INT16, INT16, UINT32,
                                        BYTE, unsigned ) ;

                       INT16     LIF_MailSbc         (SBC_ID, BYTE, unsigned ) ;

                       void      LIF_PutStr          (char *) ;
                       BYTE      *LIF_ReadMail       (INT16) ;

                       BYTE      *LIF_ReadWait       (INT16, int) ;

                       void      LIF_RemoveTimer     (INT16) ;

                       INT16     LIF_SendBuf         (INT16, INT16, INT16, BUFFER_TYPE,
                                        BYTE *, BYTE, UINT32, BYTE) ;

                       INT16     LIF_SendPkt         (INT16, INT16, BYTE, INT16,
                                        INT16, BYTE, BYTE, BYTE, INT16,
                                        INT16, INT16, BOOLEAN, PKT_ID,
                                        UINT32, BYTE) ;

                       INT16     LIF_SendSbc         (INT16, INT16, BYTE, BYTE) ;

                       INT16     LIF_Sleep           (INT16) ;
                       void      LIF_StartTimer      (INT16) ;
                       void      LIF_StopTimer       (INT16) ;
                       BOOLEAN   LIF_TestMail         (INT16) ;
                       void      LIF_Term             (void) ;
                       BOOLEAN   LIF_TimerRunning     (INT16) ;
                       INT16     LIF_UpdateTimerP     (INT16, INT16, INT16, INT16,
                                         BYTE, BYTE, BYTE, INT16, INT16,
                                         UINT32, PKT_ID) ;

                      MAILBOX_ID lif_IdToMb        (int) ;




                      void LIF_SetTracing      (BOOLEAN) ;



#line 138 "CCBFIT.c"

#line 1 "::inc:ccie.h"














































































































































































































































































































#line 321 "::inc:ccie.h"


















































































































































































































































































































typedef struct
    {
    BYTE Ident;                                   
    BYTE Len;           
    } IE_HEAD;



typedef struct
    {
    IE_HEAD     IE_Header;          
    BYTE        Call_State;       
    } *CALL_STATE_ELEM;























                      int     CCIE_BuildCallAppear_IE        (BYTE *, BYTE) ;



                      int     CCIE_BuildCallState_IE         (BYTE *, BYTE) ;
                      int     CCIE_BuildCallingNumb_IE       (BYTE *, int, int,                                                 int, int, BYTE *, int,                                                 BOOLEAN) ;


                      int     CCIE_BuildCause_IE             (BYTE *, BYTE, BOOLEAN,                                                 BYTE *) ;





                      int     CCIE_BuildChanId_IE            (BYTE *, int, int, int,                                                 BOOLEAN, BOOLEAN, int,                                                 BYTE *) ;













                      int     CCIE_BuildHookState_IE         (BYTE *, BYTE) ;
                      int     CCIE_BuildKeypad_IE            (BYTE *, BYTE *, int, BOOLEAN ) ;



                      int     CCIE_BuildPartyNumb_IE         (BYTE *, int, int, int,                                                 BYTE *, int, BOOLEAN) ;









                      BOOLEAN CCIE_CauseValid                (int) ;
                      void    CCIE_Change_Msg_Type           (BYTE *, int) ;
                      BYTE   *CCIE_ExpandBearCap_IE          (BYTE *, BOOLEAN *) ;
                      BYTE   *CCIE_ExpandCause_IE            (BYTE *, BOOLEAN *) ;
                      BYTE   *CCIE_ExpandChanId_IE           (BYTE *, BOOLEAN *) ;





                      BYTE   *CCIE_ExpandNotifyInd_IE        (BYTE *, BOOLEAN *) ;
                      BYTE   *CCIE_ExpandPartyNumb_IE        (BYTE *, BOOLEAN *) ;
                      BYTE   *CCIE_ExpandProgressInd_IE      (BYTE *, BOOLEAN *) ;
                      BYTE   *CCIE_ExpandSignal_IE           (BYTE *, BOOLEAN *) ;



                      BYTE   *CCIE_Get_IE                    (BYTE *, int, BYTE, int, int) ;
                      BYTE   *CCIE_GetAnyIE                  (BYTE *, int, BYTE, int, int) ;



                      int     CCIE_GetChanSpan               ( int ) ;
                      BYTE    CCIE_LenOf_L3_Msg_Header       (BYTE *) ;




                      BOOLEAN CCIE_Single_Byte_IE            (BYTE, BYTE *, BYTE *) ;








#line 139 "CCBFIT.c"

#line 1 "::inc:ccmim.h"











































































































typedef struct
   {
   BOOLEAN  valid;          
   BOOLEAN  epsf;                              
   int      usid;                                     
   int      tid;                                         
   }
EPID, *EP_ID;













#line 140 "CCBFIT.c"

#line 1 "::inc:ccbri.h"






















































































































































































































#line 227 "::inc:ccbri.h"


#line 242 "::inc:ccbri.h"




enum
   {
   CHANNEL_STATUS_REQUEST,
   CHANNEL_FREE,
   CHANNEL_VOICE,

   CHANNEL_DATA,
   CHANNEL_3_1_AUDIO,			 






   };



















enum
   {
   CCBRI_CALL_CONNECTED,
   CCBRI_CALL_CONNECTING,
   CCBRI_CALL_IDLE,
   CCBRI_CALL_DIALING,
   CCBRI_CALL_ALERTING,
   CCBRI_CALL_LOCKOUT,
   CCBRI_CALL_OFFERED,
   CCBRI_CALL_ORIGINATE,
   CCBRI_CALL_RELEASING,
   CCBRI_CALL_ONHOOK_RELEASING,
   CCBRI_CALL_OFFHOOK_RELEASING,
	CCBRI_CALL_PASSIVE,
   CCBRI_CALL_REGISTER,
   CCBRI_CALL_ROUTING,
   CCBRI_CALL_ROUTINGIN,
   CCBRI_CALL_ROUTINGOUT,
   CCBRI_CALL_CONFERENCING,
   CCBRI_DECODE_CALL_STATE,
   CCBRI_DIGIT_COLLECTION,
   CCBRI_DISCONNECTING_HOST,
   CCBRI_GET_OUT_PKT,

   CCBRI_GET_OUT_INFO,



   CCBRI_GO,
   CCBRI_SEND_TRACE,
   CCBRI_GET_HCB_INDEX,
   CCBRI_GET_L3_EVENT,
   CCBRI_GET_NEW_CCB,
   CCBRI_VOICE_CONNECTED,
   CCBRI_VOICE_CONFERENCING,
   CCBRI_VALID_HW_EVENT,
   CCBRI_INIT,
   CCBRI_MAIL_TO_L3,
   CCBRI_MAIL_TO_HOST,
   CCBRI_REMOVE_CCB,
   CCBRI_RELEASE_CHAN,
   CCBRI_RELEASE_CALL,
   CCBRI_RESIZE_INFO,
   CCBRI_SET_NEW_CHAN,
   CCBRI_REG_MAINTENANCE,



   };



enum
   {
   NO_ERROR,                               
   SEND_SBC_FAILED,
   SEND_PKT_FAILED,

   SEND_BUF_FAILED,



   SEND_PRIM_FAILED,
   UNKNOWN_EVENT,
   NO_CCB,
   INVALID_HW_EVENT,
   UNKNOWN_CALL,
   UNKNOWN_MSG_SOURCE,
   NO_CALL_SLOT,
   INVALID_STATE,
   INVALID_PAS_CCB,
   INVALID_SWITCH_TYPE,
   OUT_OF_PKTS,
   INITIALIZATION_FAILED,
   CCB_CORRUPTED,
   OUT_OF_CHANNELS,

   OUT_OF_INFO_BUFFERS,



   CCB_OR_CHAN_TABLE_CORRUPTED,
   CCB_LIST_CORRUPTED,
   OUT_OF_CCBS,
   EVENT_PENDING,
   INOPPORTUNE_MSG,
   HCB_CORRUPTED
   };




















#line 399 "::inc:ccbri.h"


struct CallControlBlock
    {
    int     State;                                

    int     SS_State;            

    int     CallType;                               
    int     B_Chan;                                 
    int     Call_ID;                             
    int     CallRef;                                  
    BYTE    Ces;                          
    BYTE    Sapi;                     
    int     CallAppearance;             
    BOOLEAN MakePrivateMsg;            
    BOOLEAN CopyPublicMsg;       
    BOOLEAN FirstDigit;       
    int     HcbIndex;                       
    int     CallRefLen;                        
    int     Cause;                         
    int     Host_Specifier;                         
    INT16   Host_ID;                                






    BOOLEAN CollectAddress;                 
    int     Service;                                
    struct  CallControlBlock *Next;                       
    };
typedef struct CallControlBlock *CCB_ID;






typedef struct
   {
   char   DN [             24 ] [          (20+1) ] ;                            
   BOOLEAN DN_Routing;                   
   int     Bearer_Type;                


   BOOLEAN Bearer_Routing;     
   INT16   Host_ID;                          
   int     Ces;                                                  

   BYTE   SPID [        (20+1) ];                                 
   BOOLEAN Initializing_Terminal;          
   EPID     epid [              24 ];                             
   int     Terminal_State;             


   int     Timer_T415;                       
   BOOLEAN First_T415;

   void    *SS;                
   BYTE    *HostMsg;                         
   int     Msglen;                            
   } HOST_CB, *H_ID;
extern HOST_CB Host_CB [                 1 ] [               2  ];















extern  int     Btype_NI1_CCB;           



extern CCB_ID   CCB_Head[                  1  ], CCB_Tail[                  1  ], CCB;
extern BYTE     Dsl_ID;              
extern unsigned int Call_Id_Source;   
extern int      Source;                        
extern int      L3_State;                                
extern int      SmlBufSize;     
extern int      ActiveCCBs[                  1  ];          
extern struct   CallControlBlock Dummy_Ccb;            
extern BYTE *   IE;                     
extern BYTE *   IE_Fields;      
extern int      TheTrash;             
extern PKT_ID   InPkt;                      
extern PKT_ID   OutPkt;                     
extern BYTE *   InPkgInfo;                   
extern int      InPkgInfoLen;                    
extern BYTE *   OutPkgInfo;              
extern int      OutPkgInfoLen;                   
extern BUFFER_TYPE OutPkgInfoType;                 
extern INT16    Terminal_Status;        


extern BOOLEAN	setIsAct;						 
extern BOOLEAN	hostIsAct;						 
extern BOOLEAN	waitForTwait;					 





































                          void       CCBRI_Go                       ( int, int, int, PKT_ID, int, BYTE, BYTE, BYTE,
                                                        INT16 ) ;
                          void       CCBRI_Init                     ( int ) ;

                          int        CCBRI_AcceptBearerCap          ( void ) ;
                          int        CCBRI_AcceptCalledNumb         ( int * , BYTE ) ;
                          int        CCBRI_AcceptCallingNumb        ( BYTE *, int * ) ;
                          int        CCBRI_AcceptCause              ( BYTE * ) ;
                          int        CCBRI_AcceptChanId             ( BYTE *, BYTE * ) ;
                          int        CCBRI_AcceptEndpointId         ( int *, int *, int * ) ;






                          int        CCBRI_AcceptNotify             ( BYTE * ) ;


                          int        CCBRI_AcceptProgress           ( BYTE * ) ;
                          int        CCBRI_AcceptSendingComplete      ( BYTE * ) ;
                          int        CCBRI_AcceptSignal             ( BYTE * ) ;
                          void       CCBRI_Error                    ( int, int, int, int, BOOLEAN ) ;









                          int        CCBRI_TerminalInit             ( int,  int, int ) ;
                          void       CCBRI_TerminalParms            ( int, int, BYTE * ) ;
                          INT16      CCBRI_HostRouter               ( void ) ;
                          void       CCBRI_DisconnectHostCall       ( BYTE *, int ) ;
                          void       CCBRI_FreeCallBufs             ( void ) ;
                          BYTE *     CCBRI_GetLdn                   ( void ) ;
                          void       CCBRI_GetL3Event               ( int * ) ;
                          int        CCBRI_GetNewCall_Id            ( void ) ;
                          CCB_ID     CCBRI_GetNewCCB                ( BYTE, int, BYTE, BYTE, int, int ) ;

                          BOOLEAN    CCBRI_GetOutInfo               ( BYTE ) ;



                          BOOLEAN    CCBRI_GetOutPkt                ( BYTE, BOOLEAN ) ;
                          CCB_ID     CCBRI_GetDummyCcb              ( int, BYTE, BYTE ) ;
                          int        CCBRI_GetHcbIndex              ( BYTE ) ;
                          BYTE *     CCBRI_GetPublicIE              ( int, int ) ;
                          int        CCBRI_DecodeCallState          ( int * ) ;
                          CCB_ID     CCBRI_GetL3CCB                 ( int, int, BYTE, int, BYTE ) ;
                          CCB_ID     CCBRI_GetHostCCB               ( int, int, BYTE, int, BYTE ) ;
                          void       CCBRI_LinkStatusChange         (  BYTE, int ) ;
                          BOOLEAN    CCBRI_Match                    ( BYTE *, int, BYTE ) ;
                          void       CCBRI_MailtoL3                 ( void ) ;
                          void       CCBRI_MailtoHost               ( BOOLEAN ) ;
                          void       CCBRI_Process_L3_Event         ( int, int, BYTE, int, BYTE ) ;
                          void       CCBRI_Process_Host_Event       ( int, int, BYTE, int, int,  BYTE ) ;
                          void       CCBRI_RejectCall               ( int, int ) ;
                          void       CCBRI_RejectMsg                ( int, int ) ;
                          void       CCBRI_RejectHostCall           ( int ) ;
                          void       CCBRI_RejectHostMsg            ( BYTE ) ;
                          void       CCBRI_RejectSetup              ( int, int ) ;
                          void       CCBRI_ReleaseChan              ( void ) ;
                          void       CCBRI_ReleaseCall              ( void ) ;
                          void       CCBRI_ReleasedLink             ( BYTE, BYTE ) ;
                          void       CCBRI_RemoveCCB                ( CCB_ID ) ;
                          void       CCBRI_ResizeInfo               ( BOOLEAN ) ;
                          void       CCBRI_SendSpid                 ( int, int ) ;



                          int        CCBRI_SetNewChan               ( int, int, BOOLEAN ) ;



                          void       CCBRI_ShutDown                 ( void ) ;
                          BOOLEAN    CCBRI_ValidateHwEvent          ( int, BYTE ) ;






                          void       CCBRI_StatusInd                ( void ) ;






                     		void   		CCBRI_HostConfig(void);
                      		UINT32 		CCBRI_GenerateTWait(void);

                          void       CCBRI_HostDisconnect           ( int ) ;
                          BOOLEAN    CCBRI_HostDNToCes              ( void ) ;
                          int        CCBRI_HostEventFilter          ( int, int, int, int  ) ;
                          void       CCBRI_HostInfo                 ( void ) ;
                          void       CCBRI_HostNotify               ( void ) ;
                          void       CCBRI_HostQuery                ( void ) ;

                     		void		CCBRI_HostNCQuery		    (void) ;

                          int        CCBRI_HostCallRequest          ( void ) ;
                          void       CCBRI_HostConnect              ( void ) ;
                          BYTE *     CCBRI_GetHost_IE               ( BYTE *, BYTE ) ;
                          int        CCBRI_RegisterTerminal         ( int, int ) ;
                          void       CCBRI_Restart                  ( int, BYTE ) ;
                          int        CCBRI_CopyPublic_IE            ( BYTE ) ;

                      BOOLEAN  CCBFIT_CheckEID     ( int * ) ;
                      int      CCBFIT_AcceptEndpointId     ( int *, int *, int * ) ;
                      int      CCBFIT_TerminalInit     ( int, int, int  ) ;
                      void     CCBFIT_TerminalParms     ( int, int, BYTE * ) ;
                      BOOLEAN  CCBFIT_GetEpid     (int *, int *, int *, int, int) ;
                      BOOLEAN  CCBFIT_TerminalDown     ( BYTE, int ) ;




#line 679 "::inc:ccbri.h"









                          void       CCBNI1_ReleaseCall              ( void ) ;
                          void       CCBNI1_Restart                  ( BYTE ) ;
                          void       CCBNI1_Shutdown                 ( void ) ;
                          int        CCBNI1_DecodeCallState          ( BOOLEAN * ) ;
                          void       CCBNI1_ReleasedLink             ( BYTE, BYTE ) ;

                          void       CC_got_nl_est_conf              ( int, int ,int) ;
                          BOOLEAN    CC_got_nl_mim_ind               (int, int, int, PKT_ID) ;


                          int        CCBNI1SS_Notify                 ( void  ) ;
                          void       CCBNI1SS_Host_SS_Request        ( int, int, int ) ;
                          void       CCBNI1SS_L3_SS_Ind              ( int, int, int ) ;


#line 141 "CCBFIT.c"

#line 1 "::inc:cc_chan.h"










































































































                      int     CC_CHAN_GetIdleChan         ( int, int, BYTE, BYTE ) ;
                      int     CC_CHAN_GetProtocol         ( int, int, BYTE ) ;
                      int     CC_CHAN_GetState            ( int, int, BYTE ) ;
                      int     CC_CHAN_GetUser             ( int, int, BYTE ) ;
                      BOOLEAN CC_CHAN_InitTable           ( int, int, int, int, int ) ;
                      void    CC_CHAN_ReleaseChan         ( int, int, BYTE ) ;
                      void    CC_CHAN_SetProtocol         ( int, int, int, BYTE ) ;
                      void    CC_CHAN_SetState            ( int, int, int, BYTE ) ;
                      void    CC_CHAN_SetUser             ( int, int, int, BYTE ) ;



#line 142 "CCBFIT.c"

#line 1 "::inc:gp.h"









































































typedef struct
   {
   char     ldn [             24 ][     (20+1)                           ];   
   char     spid [    (20+1)                           ];          
   }
NUMBER_PARAM;

typedef struct
   {
   char     ldn [             24 ][     (20+1)                           ];            
   char     spid [    (20+1)                           ];          
   BOOLEAN  DN_Routing;                         
   int      Bearer_Type;               

   BOOLEAN  Bearer_Routing;     
   INT16    Host_ID;                         
   int      Ces;                                  
   BOOLEAN Initializing_Terminal;          
   }
NUMBER_PARAM_FIT;






extern NUMBER_PARAM np;                          
extern NUMBER_PARAM_FIT np_fit [                 1 ] [               2  ];











                      void gpBCDtoIA5(BYTE *src, BYTE *dest, INT16 maxIA5len);

                      BOOLEAN gpBuildHostCb     (  char *, char *, BOOLEAN * , int *,
					BOOLEAN *,  INT16 *, int *, BOOLEAN *, int, int ) ;
#line 143 "CCBFIT.c"
























    static           void CCBFIT_SendSpid     (int, int) ;












                      int CCBFIT_AcceptEndpointId(usid, tid, interpreter)

int *usid;
int *tid;
int *interpreter;

{
	int cause =       0            ;

	switch (SwitchType)
	{
#line 208 "CCBFIT.c"

	default:
		IE = CCIE_Get_IE(InPkt->Info, InPkt->InfoLen,
						                0x3B ,                 0             , 1);


		if (IE != ((void *)0) )
		{
			*usid = IE[2] ^ 0x80;
			*tid = IE[3] & 0x3F;
			*interpreter = ((IE[3] & 0x40) == 0x40) ? 1 : 0;
		}
		else
		{
			cause =   0x60                        ;	 
		}
		break;
	}

	return (cause);
}









                      BOOLEAN CCBFIT_CheckEID(ces)
int *ces;						 
{
	BOOLEAN result =       0 ;
	int tmp_usid, our_usid;
	int tmp_tid, our_tid;
	int tmp_inter, our_inter, i = 0, j;		 
	int broadcast_tid;

	broadcast_tid = (SwitchType == BRI_5ESS_STYPE) ?        0x7f  :    0x3f ;

	for (j = 0; ((j <               2 ) && (!result)); j++)
	{
		for (i = 0; ((i <              24 ) && (!result)); i++)
		{
			if ((CCBFIT_GetEpid(&our_usid, &our_tid, &our_inter, i, j)) ==       0 )
			{


				continue;
			}
			else
			{
				if ((CCBFIT_AcceptEndpointId(&tmp_usid, &tmp_tid, &tmp_inter)) ==       0            )
				{
					if (tmp_usid ==        0x7F )
					{
						result =        1 ;
					}

					else if ((tmp_usid == our_usid) || (tmp_usid == 0))
					{
						if (((tmp_inter == 0) &&
							 ((tmp_tid == our_tid) || (tmp_tid == broadcast_tid))) ||
							((tmp_inter == 1) &&
							 (tmp_tid != our_tid)))

						{
							result =        1 ;
						}
						else
						{
							result =       0 ;
						}
					}
					else
					{

						result =       0 ;
					}
				}
				else

				{
					result =        1 ;
					goto bailout;
				}
			}
		}
	}






  bailout:
	return (result);
}






                      BOOLEAN CCBFIT_GetEpid(usid, tid, epsf, eid, host)
int *usid;
int *tid;
int *epsf;
int eid;						 
int host;						 

{
	H_ID hostcb;
	int max_hosts;

	max_hosts =               2 ;		 

	if (max_hosts == 1)
	{
		hostcb = &Host_CB[Dsl_ID][0];
		if (hostcb->epid[eid].valid)
		{
			*usid = hostcb->epid[eid].usid ^ 0x80;
			*tid = hostcb->epid[eid].tid;
			*epsf = hostcb->epid[eid].epsf;
			return (hostcb->epid[eid].valid);
		}
	}
	else
	{

		hostcb = &Host_CB[Dsl_ID][host];
		if (hostcb->epid[eid].valid)
		{
			*usid = hostcb->epid[eid].usid ^ 0x80;
			*tid = hostcb->epid[eid].tid;
			*epsf = hostcb->epid[eid].epsf;
			CCB->Host_Specifier = host;
			return (hostcb->epid[eid].valid);
		}
	}

	return (      0 );

}








                      int CCBFIT_TerminalInit(event, specifier, dsl)
int event;						 
int specifier;					 
int dsl;						 
{

	BYTE event_b;
	H_ID hostcb;
	H_ID tmpHostcb=((void *)0) ;		 
	int k = 0;
	PKT_ID loc_pkt;
	BOOLEAN fullyOp =        1 ;		 
	register int i = 0;
	static UINT16 spidRetry = 0;	 

	hostcb = ((void *)0) ;

	switch (event)

	{

	case       0x0355 :		 


		if (!waitForTwait && (setIsAct || hostIsAct))
		{
			do { if (!DBSuppressed[2]) DBPrintf(2, "Start ISDN stack init: NL_PU_EST_IND\n"); } while (0) ;
			hostcb = &Host_CB[dsl][specifier];

			if (hostcb != ((void *)0)  && hostcb->SPID != ((void *)0) )
			{
				Dsl_ID = dsl;
				if ((CCB = CCBRI_GetNewCCB(     0x99               ,
										                  0                    ,
										         0 ,
										   hostcb->Ces,
										   0,
										   0)) != ((void *)0) )
				{

					do { if (!DBSuppressed[1]) DBPrintf(1, "CCBFIT_TerminalInit: specifier %d hostcb->Ces %d\n",specifier, hostcb->Ces); } while (0) ;


					LIF_SendPkt(         0 ,           0x0400                     , 0,
								          0x0300               ,          0x0300 , dsl,           0        ,
								hostcb->Ces,

								0, hostcb->Ces, 0,       0 ,



								((void *)0) ,                 (UINT32) 0     ,        10         );
					hostcb->Terminal_State =
						     0x03 ;
					CCB->Host_Specifier = specifier;
				}
				else
				{
					CCB = CCBRI_GetDummyCcb(0, hostcb->Ces,       0 );
					CCBRI_RejectHostCall(     0x29                        );
				}
			}  
			else
				do { if (!DBSuppressed[4]) DBPrintf(4, "CCBFIT_TerminalInit NL_PU_EST_IND: NULL SPID\n"); } while (0) ;
		}
		else
			do { if (!DBSuppressed[4]) DBPrintf(4, "NL_PU_EST_IND:No set/host, or wait for t-wait\n"); } while (0) ;
		break;

	case       0x01 :	 

		hostcb = &Host_CB[dsl][specifier];

		if (hostcb != ((void *)0)  && hostcb->SPID != ((void *)0) )
		{
			if ((hostcb->HostMsg = LIF_GetBuffer(LRG_INFO_BTYPE,        1 )) != ((void *)0) )
			{
				memcpy(hostcb->HostMsg, InPkgInfo, InPkgInfoLen) ;
				hostcb->Msglen = InPkgInfoLen;
			}
			else
			{
				CCBRI_Error(CCBRI_GET_OUT_INFO, OUT_OF_INFO_BUFFERS, 0, 0,        1 );
			}
			if ((CCB = CCBRI_GetNewCCB(     0x99               ,
									                  0                    ,
									         0 ,
									   hostcb->Ces,
									   0,
									   0)) != ((void *)0) )
			{
				LIF_SendPkt(         0 ,           0x0400                     ,      0x99               ,
							          0x0300               ,          0x0300 , dsl,           0        ,
							hostcb->Ces,
							0, hostcb->Ces, 0,       0 ,
							((void *)0) ,                 (UINT32) 0     ,        10         );
				hostcb->Terminal_State =
					     0x03 ;
				CCB->Host_Specifier = specifier;
			}
			else
			{
				CCB = CCBRI_GetDummyCcb(0, hostcb->Ces,       0 );
				CCBRI_RejectHostCall(     0x29                        );
			}
		}
		else  
			do { if (!DBSuppressed[4]) DBPrintf(4, "CCBFIT_TerminalInit HOST_CALL_REQUEST: NULL SPID\n"); } while (0) ;
		break;

	case      0x17 :	 
		hostcb = &Host_CB[dsl][specifier];

		if (hostcb != ((void *)0)  && hostcb->SPID != ((void *)0) )
		{
			switch (hostcb->Terminal_State)
			{
			case          0x08 :
				do { if (!DBSuppressed[2]) DBPrintf(2, "ESTABLISHED spec %d\n",specifier); } while (0) ;
				LIF_UpdateTimerP((hostcb->Timer_T415),(           0x0400                     ),(           0x0400                     ),(
								   (          0x1400 |0x22)           ),( dsl),( CCB->Sapi),( hostcb->Ces),( 0),( 0),(
								   (UINT32) 20000 ),((void *)0) ) ;
				LIF_StartTimer(hostcb->Timer_T415);
				hostcb->First_T415 =        1 ;
				CCBFIT_SendSpid(dsl, specifier);
				break;
			case      0x03 :
				do { if (!DBSuppressed[2]) DBPrintf(2, "AWAIT_ESTABLISH spec %d\n",specifier); } while (0) ;
				break;
			case                 0x05 :
				if (!waitForTwait && (setIsAct || hostIsAct) &&
					(layersUp[specifier]>0 && layersUp[specifier]<3))
				{
					do { if (!DBSuppressed[2]) DBPrintf(2, "Start ISDN stack init: INIT\n"); } while (0) ;
					do { if (!DBSuppressed[2]) DBPrintf(2, "INIT spec %d\n",specifier); } while (0) ;
					if ((CCB = CCBRI_GetNewCCB(     0x99               ,
											                  0                    ,
											         0 ,
											   hostcb->Ces,
											   0,
											   0)) != ((void *)0) )
					{
						LIF_SendPkt(         0 ,           0x0400                     ,      0x99               ,
									          0x0300               ,          0x0300 , dsl,           0        ,
									hostcb->Ces,
									0, hostcb->Ces, 0,       0 ,
									((void *)0) ,                 (UINT32) 0     ,        10         );
						hostcb->Terminal_State =
							     0x03 ;
						CCB->Host_Specifier = specifier;
					}
					else
					{
						CCB = CCBRI_GetDummyCcb(0, hostcb->Ces,       0 );
						CCBRI_RejectHostCall(     0x29                        );
					}
				}
				else
					if (layersUp[specifier] == 3)
						do { if (!DBSuppressed[2]) DBPrintf(2, "layer 3 (%d) is up\n",specifier); } while (0) ;
					else
						do { if (!DBSuppressed[4]) DBPrintf(4, "INIT:No set/host, or wait for t-wait, or no layer 1\n"); } while (0) ;
				break;
			case            0x01 :
				do { if (!DBSuppressed[2]) DBPrintf(2, "TERM_DOWN specifier %d\n",specifier); } while (0) ;


				hostcb->Ces = (specifier == 0) ? 1: 2;	 
				if (!waitForTwait && (setIsAct || hostIsAct) && 
					(layersUp[specifier]>0 && layersUp[specifier]<3))
				{
					do { if (!DBSuppressed[2]) DBPrintf(2, "Start ISDN stack init: TERM_DOWN\n"); } while (0) ;
					if ((CCB = CCBRI_GetNewCCB(     0x99               ,
											                  0                    ,
											         0 ,
											   hostcb->Ces,
											   0,
											   0)) != ((void *)0) )
					{
						LIF_SendPkt(         0 ,           0x0400                     ,      0x99               ,
									          0x0300               ,          0x0300 , dsl,           0        ,
									hostcb->Ces,
									0, hostcb->Ces, 0,       0 ,
									((void *)0) ,                 (UINT32) 0     ,        10         );
						hostcb->Terminal_State =
							     0x03 ;
						CCB->Host_Specifier = specifier;
					}
					else
					{
						CCB = CCBRI_GetDummyCcb(0, hostcb->Ces,       0 );
						CCBRI_RejectHostCall(     0x29                        );
					}
				}
				else
					if (layersUp[specifier] == 3)
						do { if (!DBSuppressed[2]) DBPrintf(2, "layer 3 (%d) is up\n",specifier); } while (0) ;
					else
						do { if (!DBSuppressed[4]) DBPrintf(4, "TERM_DOWN:No set/host, or wait for t-wait, or no layer 1\n"); } while (0) ;
				break;
			default:

				do { if (!DBSuppressed[4]) DBPrintf(4, "CCBFIT_TerminalInit ERROR:default specifier %d\n",specifier); } while (0) ;

				break;
			}						 
		}  
		else
			do { if (!DBSuppressed[4]) DBPrintf(4, "CCBFIT_TerminalInit HOST_TERM_REGISTER: NULL SPID\n"); } while (0) ;
		break;

	case         0x0303 :			 
	case          0x0301 :

		CCB = CCB_Head[dsl];
		while ((CCB != ((void *)0) ) &&
			   ((CCB->Call_ID != 0) ||
				(CCB->Sapi !=       0 ) ||
				(CCB->Ces != specifier)))
			CCB = CCB->Next;


		if (CCB == ((void *)0) )
		{
			do { if (!DBSuppressed[2]) DBPrintf(2, "***CCBFIT_TerminalInit:CCB==NULL dsl %d spec %d\n",dsl,
					specifier); } while (0) ;





		}
		else
			do { if (!DBSuppressed[2]) DBPrintf(2, "***CCBFIT_TerminalInit:CCB->State %d\n",CCB->State); } while (0) ;

		if ((CCB != ((void *)0) ) && (CCB->State ==      0x99               ))
		{
			hostcb = &Host_CB[dsl][CCB->Host_Specifier];

			if (hostcb != ((void *)0)  && hostcb->SPID != ((void *)0) )
			{
				hostcb->Terminal_State =          0x08 ;
				if (hostcb->Initializing_Terminal)
				{
					LIF_UpdateTimerP((hostcb->Timer_T415),(           0x0400                     ),(           0x0400                     ),(
									   (          0x1400 |0x22)           ),( dsl),( CCB->Sapi),( hostcb->Ces),( 0),( 0),(
									   (UINT32) 20000 ),((void *)0) ) ;
					LIF_StartTimer(hostcb->Timer_T415);
					hostcb->First_T415 =        1 ;
					CCBFIT_SendSpid(dsl, CCB->Host_Specifier);
				}
				else
				{
					if (hostcb->Msglen)
					{

						if (LIF_SendBuf(hostcb->Host_ID,
										          0x0400                     ,
										hostcb->Msglen,
										LRG_INFO_BTYPE,
										hostcb->HostMsg,
										hostcb->HostMsg[0],
										                (UINT32) 0     ,
										       10         ) ==         -1                           )
							CCBRI_Error(CCBRI_MAIL_TO_HOST, SEND_BUF_FAILED,
										OutPkgInfoLen, 0,        1 );
						hostcb->HostMsg = ((void *)0) ;
						hostcb->Msglen = 0;
					}
					CCBRI_RemoveCCB(CCB);

					CCB = ((void *)0) ;

					hostcb->Terminal_State =                 0x05 ;
				}
			}  
			else
				do { if (!DBSuppressed[4]) DBPrintf(4, "CCBFIT_TerminalInit NL_EST_IND: null hostcb\n"); } while (0) ;
		}
		break;




	case         0x93                :

		if (SwitchType != BRI_5ESS_STYPE)
		{
			CCB = CCB_Head[dsl];

			while ((CCB != ((void *)0) ) &&
				   ((CCB->Call_ID != 0) ||
					(CCB->Sapi !=       0 ) ||
					(CCB->Ces != specifier)))
				CCB = CCB->Next;

			if (CCB != ((void *)0) )
			{
				hostcb = &Host_CB[dsl][CCB->Host_Specifier];
			}

			if ((CCB != ((void *)0) ) && (hostcb->Terminal_State ==           0x04 )
					&& (CCB->State ==      0x99               ))
			{

				if (hostcb != ((void *)0)  && hostcb->SPID != ((void *)0) )
				{
					do { if (!DBSuppressed[2]) DBPrintf(2, "CCBFIT_TerminalInit:!NULL, AWAIT_INIT, TERM_INITIALIZE\n"); } while (0) ;
					LIF_StopTimer(hostcb->Timer_T415);
					loc_pkt = InPkt;
					IE = CCIE_Get_IE(loc_pkt->Info, loc_pkt->InfoLen,
									               0x08                      ,                 0             , 1);



					if (IE != ((void *)0) )
					{
						do { if (!DBSuppressed[4]) DBPrintf(4, "%s failed\n",hostcb->SPID); } while (0) ;

						noteL3Up(0);		 

						if (CCBRI_GetOutInfo(0x19 ))
						{

							OutPkgInfo[OutPkgInfoLen++] = 3;
							OutPkgInfo[OutPkgInfoLen++] =               0x08                      ;
							OutPkgInfo[OutPkgInfoLen++] = 1;
							OutPkgInfo[OutPkgInfoLen++] = IE[2];
							CCBRI_MailtoHost(CCB->CopyPublicMsg);
						}
						hostcb->Terminal_State =            0x01 ;

						do { if (!DBSuppressed[2]) DBPrintf(2, "CCBFIT_TerminalInit:CAUSE_IE->TERM_DOWN\n"); } while (0) ;

#line 707 "CCBFIT.c"

						spidRetry = 0;	 

					}
					else
					{
						do { if (!DBSuppressed[2]) DBPrintf(2, "CCBFIT_TerminalInit:EID\n"); } while (0) ;
						IE = CCIE_Get_IE(loc_pkt->Info, loc_pkt->InfoLen,
										                0x3B ,                 0             , 1);
						if ((IE != ((void *)0) ) && (IE[1] == 2))
						{
							hostcb->Terminal_State =                 0x05 ;
							hostcb->epid[CCB->Ces - 1].usid = IE[2];
							hostcb->epid[CCB->Ces - 1].tid = 0x5f & IE[3];
							hostcb->epid[CCB->Ces - 1].epsf = ((IE[3] & 0x40) != 0);
							hostcb->epid[CCB->Ces - 1].valid =        1 ;
							hostcb->Ces = CCB->Ces;


							do { if (!DBSuppressed[4]) DBPrintf(4, "%s\n",hostcb->SPID); } while (0) ;


							layersUp[hostcb->Ces - 1] = 3;	 
							do { if (!DBSuppressed[2]) DBPrintf(2, "ISDN L3 (hs %d) is UP\n",hostcb->Ces-1); } while (0) ;

							fullyOp =        1 ;
							for (i = 0; i <               2 ; i++)
							{
								tmpHostcb = &Host_CB[dsl][i];
								if (layersUp[i] != 3 && tmpHostcb->SPID != ((void *)0) )
									fullyOp =       0 ;
							}
							if (fullyOp)
							{
								noteL3Up(1);		 
							}


							if (CCBRI_GetOutInfo( 0x18 ))
							{

								OutPkgInfo[OutPkgInfoLen++] = 4;
								OutPkgInfo[OutPkgInfoLen++] =                0x3B ;
								OutPkgInfo[OutPkgInfoLen++] = 2;
								OutPkgInfo[OutPkgInfoLen++] = IE[2];
								OutPkgInfo[OutPkgInfoLen++] = IE[3];
								CCBRI_MailtoHost(CCB->CopyPublicMsg);
							}
						}
					}
					if (hostcb->Msglen)
					{

						if (LIF_SendBuf(hostcb->Host_ID,
										          0x0400                     ,
										hostcb->Msglen,
										LRG_INFO_BTYPE,
										hostcb->HostMsg,
										hostcb->HostMsg[0],
										                (UINT32) 0     ,
										       10         ) ==         -1                           )
							CCBRI_Error(CCBRI_MAIL_TO_HOST, SEND_BUF_FAILED,
										OutPkgInfoLen, 0,        1 );
						hostcb->HostMsg = ((void *)0) ;
						hostcb->Msglen = 0;
					}




						CCBRI_RemoveCCB(CCB);

						CCB = ((void *)0) ;


				}  
				else
					do { if (!DBSuppressed[4]) DBPrintf(4, "CCBFIT_TerminalInit CC_INFO_IND: NULL SPID\n"); } while (0) ;
			}
			else
			{

				do { if (!DBSuppressed[2]) DBPrintf(2, "SWITCH requested initialization\n"); } while (0) ;
				hostcb = &Host_CB[Dsl_ID][k];
				while (hostcb->Ces != specifier)
				{
					hostcb = &Host_CB[Dsl_ID][k++];
				}


				if (hostcb != ((void *)0)  && hostcb->SPID != ((void *)0) )
				{
					loc_pkt = InPkt;
					IE = CCIE_Get_IE(loc_pkt->Info, loc_pkt->InfoLen,
									            0x32             ,                 0             , 1);

					if ((IE != ((void *)0) ) &&		 
						(    (((IE[2])>>( 6))&(~(0xFF<<( 1)))) ) &&	 
						((    (((IE[2])>>( 0))&(~(0xFF<<( 2)))) ) ==            0x03         ))
					{
						do { if (!DBSuppressed[2]) DBPrintf(2, "CCBFIT_TerminalInit:INFO_REQ_IE\n"); } while (0) ;
						if ((CCB = CCBRI_GetNewCCB(     0x99               ,
												                  0                    ,
												         0 ,
												   hostcb->Ces,
												   0,
												   0)) != ((void *)0) )
						{
							LIF_UpdateTimerP((hostcb->Timer_T415),(           0x0400                     ),(           0x0400                     ),(
										     (          0x1400 |0x22)           ),( Dsl_ID),( CCB->Sapi),( hostcb->Ces),( 0),( 0),(
											   (UINT32) 20000 ),((void *)0) ) ;
							LIF_StartTimer(hostcb->Timer_T415);
							hostcb->First_T415 =        1 ;
							CCBFIT_SendSpid(Dsl_ID, specifier);
						}
						else
						{
							;		 
						}
					}
				}  
				else
					do { if (!DBSuppressed[4]) DBPrintf(4, "CCBFIT_TerminalInit CC_INFO_IND: NULL SPID\n"); } while (0) ;
			}
		}						 
		break;

#line 881 "CCBFIT.c"


	case    (          0x1400 |0x22)           :



		hostcb = &Host_CB[dsl][CCB->Host_Specifier];

		if (hostcb != ((void *)0)  && hostcb->SPID != ((void *)0) )
		{
			if (hostcb->First_T415 ==        1 )
			{
				hostcb->First_T415 =       0 ;
				LIF_UpdateTimerP((hostcb->Timer_T415),(           0x0400                     ),(           0x0400                     ),(
								   (          0x1400 |0x22)           ),( dsl),( CCB->Sapi),( hostcb->Ces),( 0),( 0),(
								   (UINT32) 20000 ),((void *)0) ) ;
				LIF_StartTimer(hostcb->Timer_T415);
				CCBFIT_SendSpid(dsl, CCB->Host_Specifier);
			}
			else


			{
				do { if (!DBSuppressed[4]) DBPrintf(4, "%s failed, FIT_TIMEOUT\n",hostcb->SPID); } while (0) ;
				if (CCBRI_GetOutInfo(0x19 ))
				{

					OutPkgInfo[OutPkgInfoLen++] = 3;
					OutPkgInfo[OutPkgInfoLen++] =               0x08                      ;
					OutPkgInfo[OutPkgInfoLen++] = 1;
					OutPkgInfo[OutPkgInfoLen++] = 0x80 |      0x29                        ;
					CCBRI_MailtoHost(      0 );
				}
				if (hostcb->HostMsg != ((void *)0) )
				{
					event_b =     0x06 ;
					CCBRI_DisconnectHostCall(&event_b,           0x66                       );
					LIF_FreeBuffer(LRG_INFO_BTYPE, hostcb->HostMsg);
					hostcb->HostMsg = ((void *)0) ;
					hostcb->Msglen = 0;
				}
				hostcb->Terminal_State =            0x01 ;

				CCBRI_RemoveCCB(CCB);

				CCB = ((void *)0) ;

			}
		}  
		else
			do { if (!DBSuppressed[4]) DBPrintf(4, "CCBFIT_TerminalInit FIT_TIMEOUT: NULL SPID\n"); } while (0) ;
		break;

	default:
		break;

	}

	return (       1 );
}







                      void CCBFIT_TerminalParms(dsl, host_specifier, msg)

int dsl;						 
int host_specifier;				 
BYTE *msg;						 

{
	int IE_index;
	int k;
	int j;
	H_ID hostcb;
	int num_hosts =               2 ;
	int ces;

	hostcb = &Host_CB[dsl][host_specifier];

	do { if (!DBSuppressed[1]) DBPrintf(1, "CCBFIT_TerminalParms: host_specifier %d hostcb->Ces %d\n",host_specifier, hostcb->Ces); } while (0) ;




	if ((IE = CCBRI_GetHost_IE(msg,                0x72 )) != ((void *)0) )
	{

		do { if (!DBSuppressed[1]) DBPrintf(1, "msg[0] %d CES_IE %d IE %d %d %d\n",msg[0],                0x72 ,
				IE[0], IE[1], IE[2]); } while (0) ;
		hostcb->Ces = IE[2];
	}









	ces = hostcb->Ces;





	if (CCBRI_GetHost_IE(msg,  0x83 ) != ((void *)0) )
	{
		hostcb->Initializing_Terminal =        1 ;
	}
	else
	{
		hostcb->Initializing_Terminal =       0 ;
	}


	if (CCBRI_GetHost_IE(msg,         0x86 ) != ((void *)0) )
	{
		hostcb->DN_Routing =        1 ;
	}
	else
	{
		hostcb->DN_Routing =       0 ;
	}


	if ((IE = CCBRI_GetHost_IE(msg,                 0x70 )) != ((void *)0) )
	{
		IE_index = 4;
		j = 0;

		if (num_hosts == 1 && ces <= 0)
		{
			do { if (!DBSuppressed[0]) DBPrintf(0, "CCBFIT_TerminalParms: invalid ces, num_hosts %d\n",num_hosts); } while (0) ;
			return;				 
		}


		if (num_hosts == 1)
		{
			while (IE_index < (IE[1] + 2))
			{
				hostcb->DN[ces - 1][j++] = IE[IE_index++];
			}
			hostcb->DN[ces - 1][j++] = '\0';
		}
		else
		{
			while (IE_index < (IE[1] + 2))
			{
				hostcb->DN[0][j++] = IE[IE_index++];
			}
			hostcb->DN[0][j++] = '\0';
		}
	}
	else
	{

		if (ces <= 0)
		{
			do { if (!DBSuppressed[0]) DBPrintf(0, "CCBFIT_TerminalParms: invalid ces\n"); } while (0) ;
			return;				 
		}

		hostcb->DN[ces - 1][0] = '\0';
	}


	if (CCBRI_GetHost_IE(msg,     0x85 ) != ((void *)0) )
	{
		hostcb->Bearer_Routing =        1 ;
	}
	else
	{
		hostcb->Bearer_Routing =       0 ;
	}


	if ((IE = CCBRI_GetHost_IE(msg,        0x71 )) != ((void *)0) )
	{
		hostcb->Bearer_Type = IE[2];
	}
	else
	{
		hostcb->Bearer_Type = 0x00;
	}



	if ((IE = CCBRI_GetHost_IE(msg,               0x3A )) != ((void *)0) )
	{
		IE_index = 2;
		k = 0;
		while (IE_index < (IE[1] + 2))
		{
			hostcb->SPID[k++] = IE[IE_index++];
		}
		hostcb->SPID[k++] = '\0';
		hostcb->Initializing_Terminal =        1 ;
		hostcb->Terminal_State =            0x01 ;
	}
	else
	{
		hostcb->SPID[0] = '\0';
	}


	do { if (!DBSuppressed[1]) DBPrintf(1, "CCBFIT_TerminalParms: host_specifier %d ces %d\n",host_specifier,
			hostcb->Ces); } while (0) ;



}








    static           void CCBFIT_SendSpid(dsl, specifier)
int dsl;
int specifier;
{
	H_ID hostcb;
	int k = 0;
	int crlen;
	crlen = 0;					 
	hostcb = &Host_CB[dsl][specifier];
	switch (SwitchType)
	{







	default:

		if (!(hostcb->SPID[0] == '\0'))
		{
			OutPkt = LIF_GetPkt(LRG_INFO_BTYPE,         4     ,       0 );
			if (OutPkt != ((void *)0) )
			{
				OutPkt->HeadLen =         4     ;	 
				OutPkt->InfoType = LRG_INFO_BTYPE;	 
				OutPkt->Info[0] =             0x08                ;	 
				OutPkt->Info[1] = crlen;	 
				OutPkt->InfoLen = crlen + 3;	 




				OutPkt->Info[crlen + 2] =                0x7B                    ;		 



				OutPkt->Info[OutPkt->InfoLen++] =               0x3A ;
				OutPkt->Info[OutPkt->InfoLen++] = strlen((char *)hostcb->SPID);
				while (k < (strlen((char *)hostcb->SPID)))
				{
					OutPkt->Info[OutPkt->InfoLen++] = hostcb->SPID[k++];
				}
				CCBRI_MailtoL3();
				hostcb->Terminal_State =           0x04 ;
			}
			else
			{
				CCBRI_Error(CCBRI_GET_OUT_PKT,         0x83                    , CCB->State, OUT_OF_PKTS,        1 );
				CCBRI_RemoveCCB(CCB);

				CCB = ((void *)0) ;

			}
		}
		else
		{


			;
		}
		break;
	}							 

}









                      BOOLEAN CCBFIT_TerminalDown(dsl, ces)
BYTE dsl;
int ces;
{
	H_ID hostcb;
	int k;
	int max_hosts;
	BOOLEAN msg_sent =       0 ;

	k = 0;
	max_hosts =               2 ;
	hostcb = &Host_CB[dsl][k];
	while ((hostcb->Ces != ces) && (k < max_hosts))
	{
		hostcb = &Host_CB[dsl][k++];
	}
	if (hostcb->Ces != ces)	 
	{
		do { if (!DBSuppressed[4]) DBPrintf(4, "CCBFIT_TerminalDown: Invalid hostcb\n"); } while (0) ;
		goto bailout;
	}
	if ((ces == hostcb->Ces) && (hostcb->Terminal_State ==           0x04 ))
	{
		LIF_StopTimer(hostcb->Timer_T415);


	}






		if (layersUp[ces - 1] > 1)	 
			layersUp[ces - 1] = 1;	 
		do { if (!DBSuppressed[4]) DBPrintf(4, "%s failed, TERM_UNREGISTERED\n",hostcb->SPID); } while (0) ;
		do { if (!DBSuppressed[2]) DBPrintf(2, "ISDN L2 (hs %d) is DOWN\n",ces-1); } while (0) ;


		hostcb->Terminal_State =            0x01 ;

		noteL3Up(0);			 


		CCB = CCBRI_GetDummyCcb(0, ces, 0);
		CCB->Host_ID = hostcb->Host_ID;
		CCB->Call_ID = 0x00;
		if (CCBRI_GetOutInfo(0x19 ))
		{
			OutPkgInfo[OutPkgInfoLen++] = 3;	 
			OutPkgInfo[OutPkgInfoLen++] =               0x08                      ;
			OutPkgInfo[OutPkgInfoLen++] = 1;
			OutPkgInfo[OutPkgInfoLen++] = (BYTE)   0x01     ;
			CCBRI_MailtoHost(      0 );
			msg_sent =        1 ;
		}




bailout:
	return (msg_sent);
}
