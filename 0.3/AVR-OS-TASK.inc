/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   30.04.2006
*/

//------------------------------------------------------------------------------------
OSTaskManagerF:
	cli                             // ����������� �������������,
	push r16                        // ����������� R16 � ������� �������
	clr r16                         // � �������� ������ ���-��������
	out TCCR0, r16                  // �� �������� � OSTaskManagerF...
// -------------------------------
	OSReadSystemMessage             // ���� �� ���������� �����������
	cpi r16, THIS_IS_SYSTEM_TASK    // �������� �� ��������� ���������� ������.
	pop r16
	breq OSThisIsSystemTask
// -------------------------------
	OSSaveCurrentTask

  OSThisIsSystemTask:
  OSTaskManagerPointOfFirstRun:
  
  OSTaskManagerCheckTaskCounter:
  	push r16
	push r18
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
	OSLoadCurrentTask
	pop r18
	pop r16

	OSSendSystemMessageC NO_MESSAGE
	OSRunTimer
	sei
  ijmp
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

	OSSendSystemMessageC THIS_IS_SYSTEM_TASK

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
	lds r16, CURRENT_TASK
	cp r16, r17
	brge OSRemoveTask_L1
	jmp OSRemoveTask_L2
 OSRemoveTask_L1:
    dec r16
 OSRemoveTask_L2:
	dec r17
	sts TASK_COUNTER, r17
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
//------------------------------------------------------------------------------------
/*
	R16 - ����� �� �������
	X - ������ ������� � �������

		 � ���� ��������� ��������� �������� ������� � ������� � SREG-a ��.
		������ � ������ �� ������� � ������ �������� ������� � X ���������.
*/
OSSaveTaskF:
	push r16
	push ZL
	push ZH
	
	call OSGetTaskDisplacementF
	
	in r16, SREG
	st Z+, r16

	st Z+, XH // ������ �������
	st Z+, XL // � �������
	
	pop ZH
	pop ZL
	pop r16
ret
//------------------------------------------------------------------------------------
/*
	R16 - ����� �� �������
	return:
		Z - ������ ������� � �������
		
		��������� � � ������ ��������:
		 1) ����������� SREG �� ������� ������, ������� ���� �� �����
		   ����� �������� �� ���� � �������� ������.
		 2) ����� � Z ��������� �������� ������� � ������� � ���
		   ���������� ��� ��� ���� ������������ IJMP
*/
OSLoadTaskF:
	push r17
	push XL
	push XH
	push YL
	push YH

	call OSInitTaskIRQF
	call OSGetTaskDisplacementF
		
	ld r17, Z+
	out SREG, r16

	ld XH, Z+ // ������ �������
	ld XL, Z+ // � �������

	ld YH, Z+ // ����������� ������
	ld YL, Z+ // ����� �� �������

	out OCR0, YL
	
	movw ZL, XL

	pop YH
	pop YL
	pop XH
	pop XL
	pop r17
ret

//------------------------------------------------------------------------------------
/*
	R16 - ����� �� �������
	retutn:
		Z - ������������ �� �������

		���� ������� ����� ������ � SRAM �� ����� � ��������
	�������� ���������� �� �������. ������ � ������ �� ������� ������
*/
OSGetTaskDisplacementF:
	push r0
	push r1
	push r17

	ldi r17, SIZE_OF_TASK_STRUCTURE
	mul r16, r17

	ldi r17, $5F
	add r0, r17
	clr r17
	adc r1, r17

	movw ZL, r0

	pop r17
	pop r1
	pop r0
ret
