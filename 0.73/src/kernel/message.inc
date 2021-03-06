/*
		The^day^of^DooM

	Create date:	30.04.2006
	Last Update:	11.05.2007
	Last Update:	17.01.2008
*/

//------------------------------------------------------------------------------------
/*
	R16 - �� ���� � �������
	R17 - �� ���� � �������
	R18 - ������ �����������
*/
OSSendMessageF:
	push r17
	push ZL
	push ZH

	tst r17
	brne OSSendMessageToTaskL

	; Send message to kernel
	sts SYSTEM_MESSAGE_FROM, r16 ; From...
	sts SYSTEM_MESSAGE, r18   ; Message...
	jmp OSSendMessageRet

  OSSendMessageToTaskL:
	call OSOpenTaskArrayF

	push r16
	mov r16, r17
	call OSGetDisplacementOfTaskF
	pop r16

	clr r17
	cp ZL, r17
	cpc ZH, r17
	breq OSSendMessageToTaskEndL

	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	st Z+, r18 ; Message...
	st Z+, r16 ; From...
	
  OSSendMessageToTaskEndL:
	call OSCloseTaskArrayF

OSSendMessageRet:
	pop ZH
	pop ZL
	pop r17
ret
//------------------------------------------------------------------------------------
/*
	R16 - �� ����
	return:
		R16 - O� ����
		R17 - ������ ���������
*/
OSReadMessageF:
	push ZL
	push ZH

	tst r16
	brne OSReadTaskMessageL

	; Read kernel message
	lds r16, SYSTEM_MESSAGE_FROM ; From...
	lds r17, SYSTEM_MESSAGE   ; Message...
	jmp OSReadMessageRet

  OSReadTaskMessageL:
	call OSOpenTaskArrayF

	call OSGetDisplacementOfTaskF
	clr r16
	cp ZL, r16
	cpc ZH, r16
	breq OSReadTaskMessageEndL

	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	ld r17, Z+ ; Message...
	ld r16, Z+ ; From...

  OSReadTaskMessageEndL:
	call OSCloseTaskArrayF

OSReadMessageRet:
	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
