/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   13.05.2006
*/

//------------------------------------------------------------------------------------
OSTaskManagerF:
	cli
	OSStopTimer
	
	OSSaveToTempL r16 // ��������� R16 � SREG
	in r16, SREG      // �� ��-����� ���������
	OSSaveToTempH r16 // � OSSaveCurrentTask


	// ��������� ���������� �������� ��������� ��
	OSReadSystemMessage          // ������������ �������, �� �� �������� ����� � ���
	cpi r16, THIS_IS_SYSTEM_TASK // ������� ����� � ����������. � ������, �� � 
	breq OSThisIsSystemTask      // ��������, ����������� ����������� �� ������� ������
	// (����� �� �� ������� ������).

	OSSaveCurrentTask

  OSThisIsSystemTask:
  OSTaskManagerPointOfFirstRun:

  OSTaskManagerCrash:
  	lds r18, TASK_COUNTER
	tst r18
  breq OSTaskManagerCrash

	lds r16, CURRENT_TASK
	inc r16
	cp r18, r16
	brlo OSTaskManagerGoToTaskOne
  
  	jmp OSTaskManagerIJmpToTask

  OSTaskManagerGoToTaskOne:
	ldi r16, 1
 
  OSTaskManagerIJmpToTask:
	sts CURRENT_TASK, r16
#if LCD && LCD_DEBUG
	DelayMsC 24
	LCDClear
	ldi r17, 48
	add r17, r16
	LCDPutStrC AVR_OS_MESSAGE, 3, 0
	LCDPutStrC AVR_OS_THIS_IS_TASK_X, 0x41, 0
	LCDPutCharRC r17, 0x4A
#endif
	OSSendSystemMessageC NO_MESSAGE
	OSLoadCurrentTask
	OSRunTimer
  reti
//------------------------------------------------------------------------------------
/*
	X - ������ �� �������
	Y - ���� �� ������ ����� �� ����������
*/
OSCreateTaskF:
	push r16
	push ZL
	push ZH

	lds r16, TASK_COUNTER
	inc r16
	sts TASK_COUNTER, r16
	call OSGetTaskDisplacementF

	in r16, SREG
	st Z+, r16
	st Z+, XH // ������ �������
	st Z+, XL // � �������

	st Z+, YH // ���� �� ������ ����� ��
	st Z+, YL // ���������� �� �������

	ldi r16, NULL
	st Z+, r16
	st Z+, r16
	st Z+, r16

	//OSSendSystemMessageC THIS_IS_SYSTEM_TASK

	pop ZH
	pop ZL
	pop r16
ret
//------------------------------------------------------------------------------------
OSExitFromTaskF:
	cli
	push r16
	lds r16, CURRENT_TASK
	call OSRemoveTaskF
	pop r16
	sei
OSExitFromTaskLoop:
jmp OSExitFromTaskLoop
//------------------------------------------------------------------------------------
/*
	R16 - ������ �� ������� �����
	      ������ �� ���������
*/
OSRemoveTaskF:
	push r16
	push r17
	push r18
	push r19
 	push YL
	push YH
	push ZL
	push ZH

	lds r17, TASK_COUNTER
	
	tst r17               // ��� ���� �� ������� � ����
	breq OSRemoveTaskRet  // �������� �������� �� ���������

	cp r16, r17           // � ���� � �� ��� ������ �� ������ ��������� ������:
	breq OSRemoveLastTask // ������ ������ ��� �������� ���� � ����

	cp r17, r16           // ���� �� ��� ��� ������ �� ����������� ������?
	brlo OSRemoveTaskRet  //  (��-����� ����� �� ������� �� ����)
	                      // ��� ������ ������� ��������� ����������� :)
// - - - - - - - - - - - - - - - - - 
 OSRemoveTaskLoop:
	call OSGetTaskDisplacementF
	movw YL, ZL
	inc r16
	call OSGetTaskDisplacementF

	ldi r18, SIZE_OF_TASK_STRUCTURE
   OSRemoveTaskSwap:
    ld r19, Z+
	st Y+, r19
	dec r18
   brne OSRemoveTaskSwap

    cp r16, r17
 brne OSRemoveTaskLoop
// - - - - - - - - - - - - - - - - -
 OSRemoveLastTask:
	dec r17
	sts TASK_COUNTER, r17
	lds r16, CURRENT_TASK
	tst r16
	breq OSRemoveTaskRet

	dec r16
	cp r17, r16
	brlo L1

	jmp L2
L1:
	clr r16
L2:
	sts CURRENT_TASK, r16	
 OSRemoveTaskRet:
 	OSSendSystemMessageC THIS_IS_SYSTEM_TASK
	pop ZH
	pop ZL
	pop YH
	pop YL
	pop r19
	pop r18
	pop r17
	pop r16
ret
/*
	R16 - ����� �� �������
	retutn:
		Z - ������������ �� �������
*/
OSGetTaskDisplacementF:
	push r0
	push r1
	push r17

	ldi r17, SIZE_OF_TASK_STRUCTURE
	mul r16, r17

	ldi r17, ($61+SIZE_OF_OS_STRUCTURE)
	add r0, r17
	clr r17
	adc r1, r17

	movw ZL, r0

	pop r17
	pop r1
	pop r0
ret
//------------------------------------------------------------------------------------
OSGetTaskDisplacementOfTaskRegistersF:
	call OSGetTaskDisplacementF
	adiw ZH:ZL, SIZE_OF_TASK_STRUCTURE-32
ret
//------------------------------------------------------------------------------------
