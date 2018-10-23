
; asmSwitchToTask(sp_t *pOldTaskSP, sp_t newTaskSP)

_asmSwitchToTask:
	movew	sr,sp@-
	movel	sp@(6),a0			; Pointer to old task's SP
	movel	sp,a0@
	movel	sp@(10),sp
	rte

_asmTimerISR:
	moveml	d0/d1/a0/a1,sp@-
;	movew	sr,d0				; addw is atomic
;	orw		#0x0700,sr
	addw	#1,_isrLevel
;	movew	d0,sr
	jbsr	_timerISR
	orw		#0x0700,sr			; Don't need to save SR: we'll soon hit an RTE
	movew	sp@(16),d1			; SR in stack
	andw	#0x0700,d1
	jbne	nested				; Don't switch if we're nested in ISRs
	tstw	_switchPending
	jbeq	noSwitch
	movew	d1,_switchPending	; d1 is zero
	jbsr	_nextTask			; Do the switch here.

noSwitch:
nested:
	moveml	sp@+,d0/d1/a0/a1
	rte

	.comm	_isrLevel,2
	.comm	_switchPending,2

