/************************************************************************
 *************************************************************************
 
 MODULE:        CCBFIT.c
 DESCRIPTION:   Call Control Terminal Initialization Module
 VERSION:       3.34
 
 *************************************************************************
 ****copyright_c**********************************************************
 
 COPYRIGHT STATEMENT
 
 Copyright (c) 1991,92,93
 by Telenetworks
 Petaluma, CA   94952
 (707) 778-8737
 
 All rights reserved. Copying, compilation, modification, distribution
 or any other use whatsoever of this material is strictly prohibited
 except in accordance with a Software License Agreement with
 Telenetworks.
 
 ****copyright_c**********************************************************
 ****history**************************************************************
 
 16 Mar 93  L Chan
 Message ID for SPID is changed from CC_INFO_REQ to INFO in
 CCBFIT_SendSpid().
 
 08 Apr 93  L Chan
 Upgraded to v3.34.
 
 14 Apr 93  L Chan
 Codes added to facilitate TEIs and layer 3 re-initialisation.
 
 10 May 93  L Chan
 Uncommented codes in CCBFIT_CheckEID(), this is needed for NI-1.
 
 12 Jun 93  L Chan
 'layersUp[]' is updated if EID is received or CCBFIT_TerminalDown()
 is called.
 
 23 Jun 93  L Chan
 CCBFIT_TerminalInit(): 'hostcb->Ces' is not reset to zero even if a
 HOST_TERM_REGISTER_NACK message is received. Since host->Ces will
 only be updated if gpbuild() is called, and this is only called
 CCBRI_Init(). To allow dynamic update of SPID, it is necessary to
 retain host->Ces.
 
 15 Jul 93  L Chan
 More debugging information is added to CCBFIT_TerminalInit().
 
 31 Jul 93  L Chan
 Bug fix in CCBFIT_TerminalInit().
 
 23 Aug 93  L Chan
 Modified LED on/off behaviour.
 
 07 Sep 93  Leigh Perry
 setLED_DRIVE is APP_DTA only.
 
 13 Oct 93  L Chan
 Added 'spidRetry' to work around a DMS100 problem, but not very successful.
 Further investigation is required.
 
 28 Oct 93  L Chan
 Increased 'spidRetry' count to 10.
 Modified red LED on/off behaviour. Off now means fully operational.
 
 03 Nov 93  L Chan
 ISDN initialisation will only take place if an active set or host is
 connected.
 
 11 Nov 93	L Chan
 Implement T-wait timer and manual initialisation by pressing any set keys.
 
 13 Nov 93	L Chan
 Reset hostcb->TerminalState to TERM_DOWN in CCBFIT_TerminalDown().
 
 15 Nov 93	L Chan
 Bug fix with layersUp[].
 
 17 Nov 93	L Chan
 Red LED behaviour is now controlled by calling noteL3Up. The LED behaviour
 is (i) steady on when physical connection is gone; (ii) flash when L3 there
 is physical connection, but L3 is still down; (iii) steady off when L3 is
 fully operational.
 
 07 Dec 93	L Chan
 Bug fix in TN code: should set CCB to NULL after any calls to
 CCBRI_RemoveCCB().
 
 16 Dec 93	L Chan
 Initialised hostcb->ces in 'case TERM_DOWN' in CCBFIT_TerminalInit().
 
 18 Dec 93	L Chan
 According to the Bellcore specs. (SR-NWT-001953 8-20), we should not
 retransmit SPID if there is an INFORMATION message with Cause IE. That
 code has been removed from CCBFIT_TerminalInit().
 
 20 Jan 94	L Chan
 The HOST_TERM_REGISTER message from CCGSI used to kick off initialisation
 will not be processed unless ISDN layer 1 is up.
 
 09 Feb 94	L Chan
 Added more debugging information in CCBFIT_TerminalInit().
 
 ****history**************************************************************
 *************************************************************************
 
 
 
 GENERAL NOTES
 
 Terminal Initialization functions for North American Basic Rate
 ISDN (SPID/TID/EID) handling are kept here. 5ESS (5e7 Generic Program ), DMS-100,
 and National ISDN-1 are supported.
 
 *************************************************************************
 *************************************************************************
 */

#define CCBFIT_C

/************************************************************************
 *                 E X T E R N A L   R E F E R E N C E S                 *
 *************************************************************************
 */

/* Standard C library functions/definitions */
#include <stdio.h>
#include    <string.h>
#include "flags.h"
#include "gendef.h"
#include "tune.h"
#include "exec.h"
#include "execuser.h"
#include "lif.h"
#include "ccie.h"
#include "ccmim.h"
#include "ccbri.h"
#include "cc_chan.h"
#include "gp.h"
//#include <hardware.h>
//#include <Pio.h>				/* for setLED_DRIVE() */

/************************************************************************
 *                           C O N S T A N T S                           *
 *************************************************************************
 */

/************************************************************************
 *                     D A T A   A L L O C A T I O N                     *
 *************************************************************************
 */

/************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S              *
 *************************************************************************
 */

/************************************************************************
 *                   P R I V A T E    F U N C T I O N S                  *
 *************************************************************************
 */

PRIVATE void CCBFIT_SendSpid ARGS((int, int));

/************************************************************************
 *                   P U B L I C    F U N C T I O N S                    *
 *************************************************************************
 */
/*************************
 * CCBFIT_AcceptEndpointId *
 **************************
 *
 * Find and validate the ENDPOINT ID info element in the incoming message.
 *
 */
PUBLIC int CCBFIT_AcceptEndpointId(usid, tid, interpreter)

int *usid;
int *tid;
int *interpreter;

{
	int cause = NOERR;

	switch (SwitchType)
	{
#ifdef BRI_5ESS
	case BRI_5ESS_STYPE:
		IE = CCIE_Get_IE(InPkt->Info, InPkt->InfoLen,
						 ENDPOINT_ID_IE, CODESET_6, 1);
	   /* there is no error checking for this info element.  It may be *  added later if 
	    * there is a need for it. */
		if (IE != NULL)
		{
			*usid = IE[2];
			*tid = IE[3] & 0x7F;
			*interpreter = ((IE[3] & 0x80) == 0x80) ? 1 : 0;
		}
		else
		{
			cause = MANDATORY_IE_MISSING;	/* Cause 96 */
		}
		break;
#endif
	default:
		IE = CCIE_Get_IE(InPkt->Info, InPkt->InfoLen,
						 EID_IE, CODESET_0, 1);
	   /* there is no error checking for this info element.  It may be *  added later if 
	    * there is a need for it. */
		if (IE != NULL)
		{
			*usid = IE[2] ^ 0x80;
			*tid = IE[3] & 0x3F;
			*interpreter = ((IE[3] & 0x40) == 0x40) ? 1 : 0;
		}
		else
		{
			cause = MANDATORY_IE_MISSING;	/* Cause 96 */
		}
		break;
	}

	return (cause);
}
/*********************
 * CCBFIT_CheckEID    *
 **********************
 *
 * EID checks are performed for Initializing Terminals, that is the
 * EID on the incoming message is compared with the EID stored in the
 * Host Table.
 *
 */
PUBLIC BOOLEAN CCBFIT_CheckEID(ces)
int *ces;						/* we return the ces value to the caller */
{
	BOOLEAN result = FALSE;
	int tmp_usid, our_usid;
	int tmp_tid, our_tid;
	int tmp_inter, our_inter, i = 0, j;		/* NTW: i=0 to suppress compiler warning */
	int broadcast_tid;

	broadcast_tid = (SwitchType == BRI_5ESS_STYPE) ? BROADCAST_TID : BROADCAST_TID_NI1;

	for (j = 0; ((j < MAX_HOSTS) && (!result)); j++)
	{
		for (i = 0; ((i < MAX_EID) && (!result)); i++)
		{
			if ((CCBFIT_GetEpid(&our_usid, &our_tid, &our_inter, i, j)) == FALSE)
			{
			   /* either we are not on a multipoint, or we do not have *   our own
			    * USID/TID or the incoming message does not carry *  USID/TID */
				continue;
			}
			else
			{
				if ((CCBFIT_AcceptEndpointId(&tmp_usid, &tmp_tid, &tmp_inter)) == NOERR)
				{
					if (tmp_usid == RESERVED_USID)
					{
						result = TRUE;
					}
				   /* verify USID/TID match */
					else if ((tmp_usid == our_usid) || (tmp_usid == 0))
					{
						if (((tmp_inter == 0) &&
							 ((tmp_tid == our_tid) || (tmp_tid == broadcast_tid))) ||
							((tmp_inter == 1) &&
							 (tmp_tid != our_tid)))
						   /* USID/TID test positive, the call will proceed. */
						{
							result = TRUE;
						}
						else
						{
							result = FALSE;
						}
					}
					else
					{
					   /* no match in TID or USID - the call *  will not proceed. */
						result = FALSE;
					}
				}
				else
				   /* No EID on the SETUP, we will let it pass thru to the *  Host */
				{
					result = TRUE;
					goto bailout;
				}
			}
		}
	}
#ifndef NTW						// NTW: ??? don't know what it is doing.
	if (result == TRUE)
	{
		*ces = ((i) + ((j - 1) * MAX_EID));
	}
#endif
  bailout:
	return (result);
}
/*****************
 * CCBFIT_GetEpid *
 ******************
 *
 * For 5ESS and NI1 FIT Terminals return EPID which is stored in Host Cb
 */
PUBLIC BOOLEAN CCBFIT_GetEpid(usid, tid, epsf, eid, host)
int *usid;
int *tid;
int *epsf;
int eid;						/* index of  eid */
int host;						/* index to host */

{
	H_ID hostcb;
	int max_hosts;

	max_hosts = MAX_HOSTS;		/* Eliminate compiler warnings */

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
	   /* This code assumes multiple Hosts */
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

	return (FALSE);

}

/*************************
 * CCBFIT_TerminalInit    *
 **************************
 *
 *  The several steps involved with SPID/EID exchange are handled here
 *
 */
PUBLIC int CCBFIT_TerminalInit(event, specifier, dsl)
int event;						/* the main event */
int specifier;					/* Could be from L3 ( ces ) or the Host */
int dsl;						/* DSL */
{

	BYTE event_b;
	H_ID hostcb;
	H_ID tmpHostcb=NULL;		/* NTW */
	int k = 0;
	PKT_ID loc_pkt;
	BOOLEAN fullyOp = TRUE;		/* is layer 3 (all SPIDs) fully operational */
	register int i = 0;
	static UINT16 spidRetry = 0;	/* NTW */

	hostcb = NULL;

	switch (event)

	{

	case NL_PU_EST_IND:		/* kicks off initialization at power up */
	   /* NTW: init only if set/host is connected and not waiting for T-wait
	    * timer */
		if (!waitForTwait && (setIsAct || hostIsAct))
		{
			printf2("Start ISDN stack init: NL_PU_EST_IND\n");
			hostcb = &Host_CB[dsl][specifier];
			/* NTW: added hostcb and hostcb->SPID check */
			if (hostcb != NULL && hostcb->SPID != NULL)
			{
				Dsl_ID = dsl;
				if ((CCB = CCBRI_GetNewCCB(TERM_INITIALIZE,
										   NO_CHANNEL,
										   SIGNALING_SAPI,
										   hostcb->Ces,
										   0,
										   0)) != NULL)
				{
	#ifdef DEBUG
					printf1("CCBFIT_TerminalInit: specifier %d hostcb->Ces %d\n",
							specifier, hostcb->Ces);
	#endif
					LIF_SendPkt(L2P_LAPD, CC_ID, 0,
								L3_ID, NL_EST_REQ, dsl, Q931_SAPI,
								hostcb->Ces,
	#ifdef NTW /* Bug fix: use different call_id for different register messages */
								0, hostcb->Ces, 0, FALSE,
	#else
								0, 1, 0, FALSE,
	#endif
								NULL, NOW, REGULAR_MAIL);
					hostcb->Terminal_State =
						AWAIT_ESTABLISH;
					CCB->Host_Specifier = specifier;
				}
				else
				{
					CCB = CCBRI_GetDummyCcb(0, hostcb->Ces, SIGNALING_SAPI);
					CCBRI_RejectHostCall(TEMPORARY_FAILURE);
				}
			} /* end if !hostcb->SPID */
			else
				printf4("CCBFIT_TerminalInit NL_PU_EST_IND: NULL SPID\n");
		}
		else
			printf4("NL_PU_EST_IND:No set/host, or wait for t-wait\n");
		break;

	case HOST_CALL_REQUEST:	/* Host Call - we must be initialized first */

		hostcb = &Host_CB[dsl][specifier];
		/* NTW: added hostcb and hostcb->SPID check */
		if (hostcb != NULL && hostcb->SPID != NULL)
		{
			if ((hostcb->HostMsg = LIF_GetBuffer(LRG_INFO_BTYPE, TRUE)) != NULL)
			{
				LIF_N_Cpy(hostcb->HostMsg, InPkgInfo, InPkgInfoLen);
				hostcb->Msglen = InPkgInfoLen;
			}
			else
			{
				CCBRI_Error(CCBRI_GET_OUT_INFO, OUT_OF_INFO_BUFFERS, 0, 0, TRUE);
			}
			if ((CCB = CCBRI_GetNewCCB(TERM_INITIALIZE,
									   NO_CHANNEL,
									   SIGNALING_SAPI,
									   hostcb->Ces,
									   0,
									   0)) != NULL)
			{
				LIF_SendPkt(L2P_LAPD, CC_ID, TERM_INITIALIZE,
							L3_ID, NL_EST_REQ, dsl, Q931_SAPI,
							hostcb->Ces,
							0, hostcb->Ces, 0, FALSE,
							NULL, NOW, REGULAR_MAIL);
				hostcb->Terminal_State =
					AWAIT_ESTABLISH;
				CCB->Host_Specifier = specifier;
			}
			else
			{
				CCB = CCBRI_GetDummyCcb(0, hostcb->Ces, SIGNALING_SAPI);
				CCBRI_RejectHostCall(TEMPORARY_FAILURE);
			}
		}
		else /* end if !hostcb->SPID */
			printf4("CCBFIT_TerminalInit HOST_CALL_REQUEST: NULL SPID\n");
		break;

	case HOST_TERM_REGISTER:	/* Host wishes to register with network */
		hostcb = &Host_CB[dsl][specifier];
		/* NTW: added hostcb->SPID check */
		if (hostcb != NULL && hostcb->SPID != NULL)
		{
			switch (hostcb->Terminal_State)
			{
			case ESTABLISHED:
				printf2("ESTABLISHED spec %d\n", specifier);
				LIF_UpdateTimer(hostcb->Timer_T415, CC_ID, CC_ID,
								FIT_TIMEOUT, dsl, CCB->Sapi, hostcb->Ces, 0, 0,
								FIT_TIMEOUT_DEFAULT);
				LIF_StartTimer(hostcb->Timer_T415);
				hostcb->First_T415 = TRUE;
				CCBFIT_SendSpid(dsl, specifier);
				break;
			case AWAIT_ESTABLISH:
				printf2("AWAIT_ESTABLISH spec %d\n", specifier);
				break;
			case INIT:
				if (!waitForTwait && (setIsAct || hostIsAct) &&
					(layersUp[specifier]>0 && layersUp[specifier]<3))
				{
					printf2("Start ISDN stack init: INIT\n");
					printf2("INIT spec %d\n", specifier);
					if ((CCB = CCBRI_GetNewCCB(TERM_INITIALIZE,
											   NO_CHANNEL,
											   SIGNALING_SAPI,
											   hostcb->Ces,
											   0,
											   0)) != NULL)
					{
						LIF_SendPkt(L2P_LAPD, CC_ID, TERM_INITIALIZE,
									L3_ID, NL_EST_REQ, dsl, Q931_SAPI,
									hostcb->Ces,
									0, hostcb->Ces, 0, FALSE,
									NULL, NOW, REGULAR_MAIL);
						hostcb->Terminal_State =
							AWAIT_ESTABLISH;
						CCB->Host_Specifier = specifier;
					}
					else
					{
						CCB = CCBRI_GetDummyCcb(0, hostcb->Ces, SIGNALING_SAPI);
						CCBRI_RejectHostCall(TEMPORARY_FAILURE);
					}
				}
				else
					if (layersUp[specifier] == 3)
						printf2("layer 3 (%d) is up\n", specifier);
					else
						printf4("INIT:No set/host, or wait for t-wait, or no layer 1\n");
				break;
			case TERM_DOWN:
				printf2("TERM_DOWN specifier %d\n", specifier);
			   /* NTW: check if host or set is connected before sending
				* the NL_EST_REQ */
				hostcb->Ces = (specifier == 0) ? 1: 2;	/* NTW: init hostcb fields */
				if (!waitForTwait && (setIsAct || hostIsAct) && 
					(layersUp[specifier]>0 && layersUp[specifier]<3))
				{
					printf2("Start ISDN stack init: TERM_DOWN\n");
					if ((CCB = CCBRI_GetNewCCB(TERM_INITIALIZE,
											   NO_CHANNEL,
											   SIGNALING_SAPI,
											   hostcb->Ces,
											   0,
											   0)) != NULL)
					{
						LIF_SendPkt(L2P_LAPD, CC_ID, TERM_INITIALIZE,
									L3_ID, NL_EST_REQ, dsl, Q931_SAPI,
									hostcb->Ces,
									0, hostcb->Ces, 0, FALSE,
									NULL, NOW, REGULAR_MAIL);
						hostcb->Terminal_State =
							AWAIT_ESTABLISH;
						CCB->Host_Specifier = specifier;
					}
					else
					{
						CCB = CCBRI_GetDummyCcb(0, hostcb->Ces, SIGNALING_SAPI);
						CCBRI_RejectHostCall(TEMPORARY_FAILURE);
					}
				}
				else
					if (layersUp[specifier] == 3)
						printf2("layer 3 (%d) is up\n", specifier);
					else
						printf4("TERM_DOWN:No set/host, or wait for t-wait, or no layer 1\n");
				break;
			default:
	#if DEBUG
				printf4("CCBFIT_TerminalInit ERROR:default specifier %d\n", specifier);
	#endif
				break;
			}						/* End of Case statement */
		} /* end if !hostcb->SPID */
		else
			printf4("CCBFIT_TerminalInit HOST_TERM_REGISTER: NULL SPID\n");
		break;

	case NL_EST_CONF:			/* L3 tells us of Link Establishment */
	case NL_EST_IND:

		CCB = CCB_Head[dsl];
		while ((CCB != NULL) &&
			   ((CCB->Call_ID != 0) ||
				(CCB->Sapi != SIGNALING_SAPI) ||
				(CCB->Ces != specifier)))
			CCB = CCB->Next;

#if DEBUG
		if (CCB == NULL)
		{
			printf2("***CCBFIT_TerminalInit:CCB==NULL dsl %d spec %d\n", dsl,
					specifier);
#if 0 // LC: take it out for now!!!
			/* NTW: need to set the terminal state */
			hostcb = &Host_CB[dsl][specifier-1];
			hostcb->Terminal_State = ESTABLISHED;
#endif
		}
		else
			printf2("***CCBFIT_TerminalInit:CCB->State %d\n", CCB->State);
#endif
		if ((CCB != NULL) && (CCB->State == TERM_INITIALIZE))
		{
			hostcb = &Host_CB[dsl][CCB->Host_Specifier];
			/* NTW: added hostcb and hostcb->SPID check */
			if (hostcb != NULL && hostcb->SPID != NULL)
			{
				hostcb->Terminal_State = ESTABLISHED;
				if (hostcb->Initializing_Terminal)
				{
					LIF_UpdateTimer(hostcb->Timer_T415, CC_ID, CC_ID,
									FIT_TIMEOUT, dsl, CCB->Sapi, hostcb->Ces, 0, 0,
									FIT_TIMEOUT_DEFAULT);
					LIF_StartTimer(hostcb->Timer_T415);
					hostcb->First_T415 = TRUE;
					CCBFIT_SendSpid(dsl, CCB->Host_Specifier);
				}
				else
				{
					if (hostcb->Msglen)
					{
					   /* Mail to CC as if its coming from the Host */
						if (LIF_SendBuf(hostcb->Host_ID,
										CC_ID,
										hostcb->Msglen,
										LRG_INFO_BTYPE,
										hostcb->HostMsg,
										hostcb->HostMsg[0],
										NOW,
										REGULAR_MAIL) == ERR)
							CCBRI_Error(CCBRI_MAIL_TO_HOST, SEND_BUF_FAILED,
										OutPkgInfoLen, 0, TRUE);
						hostcb->HostMsg = NULL;
						hostcb->Msglen = 0;
					}
					CCBRI_RemoveCCB(CCB);
#ifdef NTW
					CCB = NULL;
#endif
					hostcb->Terminal_State = INIT;
				}
			} /* end if !hostcb->SPID */
			else
				printf4("CCBFIT_TerminalInit NL_EST_IND: null hostcb\n");
		}
		break;

	   /* This INFO should contain the EID, which completes Terminal Initialization *
	    * activities. The other possible is that the switch is requesting *  terminal
	    * initialization due to some error circumstance. */
	case CC_INFO_IND:

		if (SwitchType != BRI_5ESS_STYPE)
		{
			CCB = CCB_Head[dsl];

			while ((CCB != NULL) &&
				   ((CCB->Call_ID != 0) ||
					(CCB->Sapi != SIGNALING_SAPI) ||
					(CCB->Ces != specifier)))
				CCB = CCB->Next;

			if (CCB != NULL)
			{
				hostcb = &Host_CB[dsl][CCB->Host_Specifier];
			}

			if ((CCB != NULL) && (hostcb->Terminal_State == AWAIT_INIT)
					&& (CCB->State == TERM_INITIALIZE))
			{
				/* NTW: added hostcb and hostcb->SPID check */
				if (hostcb != NULL && hostcb->SPID != NULL)
				{
					printf2("CCBFIT_TerminalInit:!NULL, AWAIT_INIT, TERM_INITIALIZE\n");
					LIF_StopTimer(hostcb->Timer_T415);
					loc_pkt = InPkt;
					IE = CCIE_Get_IE(loc_pkt->Info, loc_pkt->InfoLen,
									 CAUSE_IE, CODESET_0, 1);
	
				   /* The initialization failed if there's a Cause.  We'll extract it and *
					* pass it up to the Host. */
					if (IE != NULL)
					{
						printf4("%s failed\n", hostcb->SPID);
#ifdef APP_DTA
						noteL3Up(0);		/* flash LED */
#endif
						if (CCBRI_GetOutInfo(HOST_TERM_REGISTER_NACK))
						{
						   /* total length of private IE's = 3. */
							OutPkgInfo[OutPkgInfoLen++] = 3;
							OutPkgInfo[OutPkgInfoLen++] = CAUSE_IE;
							OutPkgInfo[OutPkgInfoLen++] = 1;
							OutPkgInfo[OutPkgInfoLen++] = IE[2];
							CCBRI_MailtoHost(CCB->CopyPublicMsg);
						}
						hostcb->Terminal_State = TERM_DOWN;
						
						printf2("CCBFIT_TerminalInit:CAUSE_IE->TERM_DOWN\n");
	
#if 0	/* NTW: According to the Bellcore specs. (SR-NWT-001953 8-20), we
		 * should not retransmit SPID if there is a Cause IE.*/
					   /* NTW: a work-around to a DMS100 initialisation problem. */
						++spidRetry;
						if (spidRetry <= 10)
						{
							printf2("spidRetry %d\n", spidRetry);
							hostcb->Terminal_State = AWAIT_ESTABLISH;
							CCBFIT_SendSpid(dsl, CCB->Host_Specifier);
						}
						else
#endif // 0
						spidRetry = 0;	/* reset it to zero */
//						hostcb->Ces = 0; /* NTW: Ces will be kept as is. */
					}
					else
					{
						printf2("CCBFIT_TerminalInit:EID\n");
						IE = CCIE_Get_IE(loc_pkt->Info, loc_pkt->InfoLen,
										 EID_IE, CODESET_0, 1);
						if ((IE != NULL) && (IE[1] == 2))
						{
							hostcb->Terminal_State = INIT;
							hostcb->epid[CCB->Ces - 1].usid = IE[2];
							hostcb->epid[CCB->Ces - 1].tid = 0x5f & IE[3];
							hostcb->epid[CCB->Ces - 1].epsf = ((IE[3] & 0x40) != 0);
							hostcb->epid[CCB->Ces - 1].valid = TRUE;
							hostcb->Ces = CCB->Ces;
						   /* Mail the EID to the Host */
#if DEBUG // NTW
							printf4("%s\n", hostcb->SPID);
#endif
#ifdef NTW
							layersUp[hostcb->Ces - 1] = 3;	/* NTW: ISDN layer 3 is up */
							printf2("ISDN L3 (hs %d) is UP\n", hostcb->Ces-1);
	#ifdef APP_DTA
							fullyOp = TRUE;
							for (i = 0; i < MAX_HOSTS; i++)
							{
								tmpHostcb = &Host_CB[dsl][i];
								if (layersUp[i] != 3 && tmpHostcb->SPID != NULL)
									fullyOp = FALSE;
							}
							if (fullyOp)
							{
								noteL3Up(1);		/* turn LED off */
							}
	#endif
#endif
							if (CCBRI_GetOutInfo(HOST_TERM_REGISTER_ACK))
							{
							   /* total length of private IE's = 4. */
								OutPkgInfo[OutPkgInfoLen++] = 4;
								OutPkgInfo[OutPkgInfoLen++] = EID_IE;
								OutPkgInfo[OutPkgInfoLen++] = 2;
								OutPkgInfo[OutPkgInfoLen++] = IE[2];
								OutPkgInfo[OutPkgInfoLen++] = IE[3];
								CCBRI_MailtoHost(CCB->CopyPublicMsg);
							}
						}
					}
					if (hostcb->Msglen)
					{
					   /* Mail to CC as if its coming from the Host */
						if (LIF_SendBuf(hostcb->Host_ID,
										CC_ID,
										hostcb->Msglen,
										LRG_INFO_BTYPE,
										hostcb->HostMsg,
										hostcb->HostMsg[0],
										NOW,
										REGULAR_MAIL) == ERR)
							CCBRI_Error(CCBRI_MAIL_TO_HOST, SEND_BUF_FAILED,
										OutPkgInfoLen, 0, TRUE);
						hostcb->HostMsg = NULL;
						hostcb->Msglen = 0;
					}
//#ifdef NTW
//					if (spidRetry == 0)
//					{
//#endif
						CCBRI_RemoveCCB(CCB);
#ifdef NTW
						CCB = NULL;
//					}
#endif
				} /* end if !hostcb->SPID */
				else
					printf4("CCBFIT_TerminalInit CC_INFO_IND: NULL SPID\n");
			}
			else
			{
			   /* The Switch has requested initialization */
				printf2("SWITCH requested initialization\n");
				hostcb = &Host_CB[Dsl_ID][k];
				while (hostcb->Ces != specifier)
				{
					hostcb = &Host_CB[Dsl_ID][k++];
				}

				/* NTW: added hostcb and hostcb->SPID check */
				if (hostcb != NULL && hostcb->SPID != NULL)
				{
					loc_pkt = InPkt;
					IE = CCIE_Get_IE(loc_pkt->Info, loc_pkt->InfoLen,
									 INFO_REQ_IE, CODESET_0, 1);
	
					if ((IE != NULL) &&		/* The Info Req IE should be present */
						(SUB_BYTE(IE[2], 6, 1)) &&	/* Info Req Ind */
						((SUB_BYTE(IE[2], 0, 2)) == TERMINAL_ID))
					{
						printf2("CCBFIT_TerminalInit:INFO_REQ_IE\n");
						if ((CCB = CCBRI_GetNewCCB(TERM_INITIALIZE,
												   NO_CHANNEL,
												   SIGNALING_SAPI,
												   hostcb->Ces,
												   0,
												   0)) != NULL)
						{
							LIF_UpdateTimer(hostcb->Timer_T415, CC_ID, CC_ID,
										  FIT_TIMEOUT, Dsl_ID, CCB->Sapi, hostcb->Ces, 0, 0,
											FIT_TIMEOUT_DEFAULT);
							LIF_StartTimer(hostcb->Timer_T415);
							hostcb->First_T415 = TRUE;
							CCBFIT_SendSpid(Dsl_ID, specifier);
						}
						else
						{
							;		/* Ignore it */
						}
					}
				} /* end if !hostcb->SPID */
				else
					printf4("CCBFIT_TerminalInit CC_INFO_IND: NULL SPID\n");
			}
		}						/* Switch != BRI_5ESS_STYPE */
		break;

#ifdef BRI_5ESS
	   /* NOTE: you need to link in module ccmim.c in order to process this
	    * message ! */
	case NL_MIM_IND:			/* AT&T TID sent */
		CCB = CCB_Head[dsl];
		while ((CCB != NULL) &&
			   ((CCB->Call_ID != 0) ||
				(CCB->Sapi != SIGNALING_SAPI) ||
				(CCB->Ces != specifier)))
			CCB = CCB->Next;
		if (CCB != NULL)
		{
			hostcb = &Host_CB[Dsl_ID][CCB->Host_Specifier];
			/* NTW: added the hostcb and hostcb->SPID check */
			if (hostcb != NULL && hostcb->SPID != NULL)
			{
				if (CC_got_nl_mim_ind(Q931_DSL_CHAN_ID, CCB->Ces, CCB->Call_ID, InPkt))
				{
				   /* In this case the 5ESS has responded to the SPID 
					* request with the USID/Epid */
					LIF_StopTimer(hostcb->Timer_T415);
					hostcb->Terminal_State = INIT;
					if (CCBRI_GetOutInfo(HOST_TERM_REGISTER_ACK))
					{
					   /* total length of private IE's = */
						OutPkgInfo[OutPkgInfoLen++] = 4;
						OutPkgInfo[OutPkgInfoLen++] = EID_IE;
						OutPkgInfo[OutPkgInfoLen++] = 2;
						OutPkgInfo[OutPkgInfoLen++] = hostcb->epid[CCB->Ces - 1].usid;
						OutPkgInfo[OutPkgInfoLen++] = hostcb->epid[CCB->Ces - 1].tid;
						CCBRI_MailtoHost(CCB->CopyPublicMsg);
					}
					CCBRI_RemoveCCB(CCB);
	#ifdef NTW
					CCB = NULL;
	#endif
				}
			} /* end if !hostcb->SPID */
			else
				printf4("CCBFIT_TerminalInit NL_MIM_IND: NULL SPID\n");
		}
		else
		{
		   /* an unsolicited MAN request */
			(void)CC_got_nl_mim_ind(Q931_DSL_CHAN_ID, specifier, 0, InPkt);
		}
		break;
#endif

	case FIT_TIMEOUT:

	   /* We've timed-out waiting for Terminal initialization activities to
	    * complete. We re-transmit the INFO w/SPID the first time it happens */
		hostcb = &Host_CB[dsl][CCB->Host_Specifier];
		/* NTW: added hostcb and hostcb->SPID check */
		if (hostcb != NULL && hostcb->SPID != NULL)
		{
			if (hostcb->First_T415 == TRUE)
			{
				hostcb->First_T415 = FALSE;
				LIF_UpdateTimer(hostcb->Timer_T415, CC_ID, CC_ID,
								FIT_TIMEOUT, dsl, CCB->Sapi, hostcb->Ces, 0, 0,
								FIT_TIMEOUT_DEFAULT);
				LIF_StartTimer(hostcb->Timer_T415);
				CCBFIT_SendSpid(dsl, CCB->Host_Specifier);
			}
			else
			   /* We've retransmitted our SPID, yet timed-out again Cancel all
				* initiation activities. */
			{
				printf4("%s failed, FIT_TIMEOUT\n", hostcb->SPID);
				if (CCBRI_GetOutInfo(HOST_TERM_REGISTER_NACK))
				{
				   /* total length of private IE's = 3. */
					OutPkgInfo[OutPkgInfoLen++] = 3;
					OutPkgInfo[OutPkgInfoLen++] = CAUSE_IE;
					OutPkgInfo[OutPkgInfoLen++] = 1;
					OutPkgInfo[OutPkgInfoLen++] = 0x80 | TEMPORARY_FAILURE;
					CCBRI_MailtoHost(FALSE);
				}
				if (hostcb->HostMsg != NULL)
				{
					event_b = HOST_DISCONNECT_ACK;
					CCBRI_DisconnectHostCall(&event_b, TIMER_EXPIRY);
					LIF_FreeBuffer(LRG_INFO_BTYPE, hostcb->HostMsg);
					hostcb->HostMsg = NULL;
					hostcb->Msglen = 0;
				}
				hostcb->Terminal_State = TERM_DOWN;
	//			hostcb->Ces = 0; /* NTW: why would I want to remove the ces?!!! */
				CCBRI_RemoveCCB(CCB);
	#ifdef NTW
				CCB = NULL;
	#endif
			}
		} /* end if !hostcb->SPID */
		else
			printf4("CCBFIT_TerminalInit FIT_TIMEOUT: NULL SPID\n");
		break;

	default:
		break;

	}

	return (TRUE);
}
/*************************
 * CCBFIT_TerminalParms   *
 **************************
 *
 *  Host may update terminal paramaters via a Host Registration message
 *
 */
PUBLIC void CCBFIT_TerminalParms(dsl, host_specifier, msg)

int dsl;						/* digital subscriber loop */
int host_specifier;				/* index to host control block */
BYTE *msg;						/* host registration message */

{
	int IE_index;
	int k;
	int j;
	H_ID hostcb;
	int num_hosts = MAX_HOSTS;
	int ces;

	hostcb = &Host_CB[dsl][host_specifier];
   // DEBUG
	printf1("CCBFIT_TerminalParms: host_specifier %d hostcb->Ces %d\n",
			host_specifier, hostcb->Ces);

   /* The CES may be passed * NTW: this is moved to the top of this function, as we need 
    * the * ces if it's there. */
	if ((IE = CCBRI_GetHost_IE(msg, CES_IE)) != NULL)
	{
	   // DEBUG
		printf1("msg[0] %d CES_IE %d IE %d %d %d\n", msg[0], CES_IE,
				IE[0], IE[1], IE[2]);
		hostcb->Ces = IE[2];
	}
#ifndef NTW						// NTW: TN code does not work if more than 1 host exist.
	else
	{
		hostcb->Ces = (InPkgInfo[2]) ? InPkgInfo[2] : SIGNALING_CES;
	}
#endif							// NTW

   /* Extract the Ces */
#ifdef NTW
	ces = hostcb->Ces;
#else
	ces = (int)msg[2];			// NTW: this won't work for 2 hosts situation.
#endif

   /* First we determine if its a Fit type terminal */
	if (CCBRI_GetHost_IE(msg, INITIALIZING_TERM_IE) != NULL)
	{
		hostcb->Initializing_Terminal = TRUE;
	}
	else
	{
		hostcb->Initializing_Terminal = FALSE;
	}
   /* Are we doing DN routing? */

	if (CCBRI_GetHost_IE(msg, DN_ROUTING_IE) != NULL)
	{
		hostcb->DN_Routing = TRUE;
	}
	else
	{
		hostcb->DN_Routing = FALSE;
	}

   /* DN available */
	if ((IE = CCBRI_GetHost_IE(msg, DN_IE)) != NULL)
	{
		IE_index = 4;
		j = 0;
#if DEBUG						// NTW: error handling
		if (num_hosts == 1 && ces <= 0)
		{
			printf("CCBFIT_TerminalParms: invalid ces, num_hosts %d\n", num_hosts);
			return;				/* ignore the rest of the rego message */
		}
#endif

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
#if DEBUG						// NTW: error handling
		if (ces <= 0)
		{
			printf("CCBFIT_TerminalParms: invalid ces\n");
			return;				/* ignore the rest of the rego message */
		}
#endif
		hostcb->DN[ces - 1][0] = '\0';
	}

   /* Are we doing Bearer Routing? */
	if (CCBRI_GetHost_IE(msg, BEARER_ROUTING_IE) != NULL)
	{
		hostcb->Bearer_Routing = TRUE;
	}
	else
	{
		hostcb->Bearer_Routing = FALSE;
	}

   /* Pick up Bearer Type - which may be Voice 0x01, Circuit 0x02, or Packet 0x04 */
	if ((IE = CCBRI_GetHost_IE(msg, BEARER_TYPE_IE)) != NULL)
	{
		hostcb->Bearer_Type = IE[2];
	}
	else
	{
		hostcb->Bearer_Type = 0x00;
	}

   /* Then the SPID.  It is copied into the Host control block where it will be
    * transmitted from when the link is established. */
	if ((IE = CCBRI_GetHost_IE(msg, SPID_IE)) != NULL)
	{
		IE_index = 2;
		k = 0;
		while (IE_index < (IE[1] + 2))
		{
			hostcb->SPID[k++] = IE[IE_index++];
		}
		hostcb->SPID[k++] = '\0';
		hostcb->Initializing_Terminal = TRUE;
		hostcb->Terminal_State = TERM_DOWN;
	}
	else
	{
		hostcb->SPID[0] = '\0';
	}

#if DEBUG
	printf1("CCBFIT_TerminalParms: host_specifier %d ces %d\n",
			host_specifier,
			hostcb->Ces);

#endif
}

/******************
 * CCBFIT_SendSpid  *
 *******************
 *
 * Send the SPID to register a Fully Initializing Terminal
 *
 */
PRIVATE void CCBFIT_SendSpid(dsl, specifier)
int dsl;
int specifier;
{
	H_ID hostcb;
	int k = 0;
	int crlen;
	crlen = 0;					/* SPID sent with null call ref */
	hostcb = &Host_CB[dsl][specifier];
	switch (SwitchType)
	{
#ifdef BRI_5ESS
	case BRI_5ESS_STYPE:
		CC_got_nl_est_conf(Q931_DSL_CHAN_ID, hostcb->Ces, CCB->Call_ID);
		hostcb->Terminal_State = AWAIT_INIT;
		break;
#endif
	   /* Assume here that DMS100 and NI1 have the same SPID format */
	default:
	   /* Assume here that DMS100 and NI1 have the same SPID format */
		if (!(hostcb->SPID[0] == '\0'))
		{
			OutPkt = LIF_GetPkt(LRG_INFO_BTYPE, DATA_OFFSET, FALSE);
			if (OutPkt != NULL)
			{
				OutPkt->HeadLen = DATA_OFFSET;	/* 4 */
				OutPkt->InfoType = LRG_INFO_BTYPE;	/* default */
				OutPkt->Info[0] = Qdot931;	/* protocol */
				OutPkt->Info[1] = crlen;	/* null call ref */
				OutPkt->InfoLen = crlen + 3;	/* leave space for l3hdr */

#ifdef NTW
			   /* NTW: IMPORTANT: need to set message ID to INFO * in order to get
			    * through L3_GetInfo */
				OutPkt->Info[crlen + 2] = INFO;		/* message ID */
#else							/* NTW */
				OutPkt->Info[crlen + 2] = CC_INFO_REQ;	/* message ID */
#endif							/* NTW */
				OutPkt->Info[OutPkt->InfoLen++] = SPID_IE;
				OutPkt->Info[OutPkt->InfoLen++] = strlen((char *)hostcb->SPID);
				while (k < (strlen((char *)hostcb->SPID)))
				{
					OutPkt->Info[OutPkt->InfoLen++] = hostcb->SPID[k++];
				}
				CCBRI_MailtoL3();
				hostcb->Terminal_State = AWAIT_INIT;
			}
			else
			{
				CCBRI_Error(CCBRI_GET_OUT_PKT, CC_INFO_REQ, CCB->State, OUT_OF_PKTS, TRUE);
				CCBRI_RemoveCCB(CCB);
#ifdef NTW
				CCB = NULL;
#endif
			}
		}
		else
		{
		   /*  *  May not be an error - the Host hasn't sent a Registration Message *
		    * for instance. */
			;
		}
		break;
	}							/* End of Case statement */

}

/***********************
 * CCBFIT_TerminalDown  *
 ************************
 *
 * The Network may remove our tei and we notify the Host of
 * this fact.
 *
 */
PUBLIC BOOLEAN CCBFIT_TerminalDown(dsl, ces)
BYTE dsl;
int ces;
{
	H_ID hostcb;
	int k;
	int max_hosts;
	BOOLEAN msg_sent = FALSE;

	k = 0;
	max_hosts = MAX_HOSTS;
	hostcb = &Host_CB[dsl][k];
	while ((hostcb->Ces != ces) && (k < max_hosts))
	{
		hostcb = &Host_CB[dsl][k++];
	}
	if (hostcb->Ces != ces)	/* NTW: in case we only have one SPID */
	{
		printf4("CCBFIT_TerminalDown: Invalid hostcb\n");
		goto bailout;
	}
	if ((ces == hostcb->Ces) && (hostcb->Terminal_State == AWAIT_INIT))
	{
		LIF_StopTimer(hostcb->Timer_T415);
//		hostcb->Terminal_State = TERM_DOWN;	/* this is reset unconditionally below */
//		hostcb->Ces = 0;	/* NTW: why would I want to reset the ces value?!!! */
	}
#if 0 /* NTW: L2 can be in TEI_ASSIGNED state, but L3 has been released.*/
	if (Terminal_Status == TERM_UNREGISTERED)
	{
#endif

#ifdef NTW
		if (layersUp[ces - 1] > 1)	/* at least layer 2 was up previously */
			layersUp[ces - 1] = 1;	/* layer 2 went down */
		printf4("%s failed, TERM_UNREGISTERED\n", hostcb->SPID);
		printf2("ISDN L2 (hs %d) is DOWN\n", ces-1);
		
		/* NTW IMPORTANT: reset Terminal_State unconditionally */
		hostcb->Terminal_State = TERM_DOWN;
#ifdef APP_DTA
		noteL3Up(0);			/* flash red LED */
#endif
#endif
		CCB = CCBRI_GetDummyCcb(0, ces, 0);
		CCB->Host_ID = hostcb->Host_ID;
		CCB->Call_ID = 0x00;
		if (CCBRI_GetOutInfo(HOST_TERM_REGISTER_NACK))
		{
			OutPkgInfo[OutPkgInfoLen++] = 3;	/* total length of private IE */
			OutPkgInfo[OutPkgInfoLen++] = CAUSE_IE;
			OutPkgInfo[OutPkgInfoLen++] = 1;
			OutPkgInfo[OutPkgInfoLen++] = (BYTE) TERM_UNREGISTERED;
			CCBRI_MailtoHost(FALSE);
			msg_sent = TRUE;
		}
#if 0 // NTW
	}
#endif

bailout:
	return (msg_sent);
}
