/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   28.01.2007
*/

//------------------------------------------------------------------------------------
.macro OSExit
	OSExitFromTask
.endmacro

.macro OSExitFromTask
	jmp OSExitFromTaskL
.endmacro
//------------------------------------------------------------------------------------
.macro OSChangeTaskPriorityC
	push r16
	push XL
	push XH
	ldi r16, (@0)	 ; ������ �� �������
	ldi XL, low(@1)  ; ����� ���������
	ldi XH, high(@1) ; �� �������
	call OSChangeTaskPriorityF
	pop XH
	pop XL
	pop r16
.endmacro
//------------------------------------------------------------------------------------
.macro OSRemoveTaskByNumber
#if @0!=R16
	push r16
	mov r16, @0
#endif
	call OSRemoveTaskF
#if @0!=R16
	pop r16
#endif
.endmacro

.macro OSRemoveTaskByNumberC
	ldi r16, @0
	call OSRemoveTaskF
.endmacro
//------------------------------------------------------------------------------------
/*
	@0 - ����� �� ������� (label)
	@1 - ���� �� ������ ����� � ����� ��������� �������
*/
.macro OSCreateTaskC
	ldi XH, high(@0)
	ldi XL, low(@0)

	ldi YH, high(@1)
	ldi YL, low(@1)
	call OSCreateTaskF
.endmacro
//------------------------------------------------------------------------------------
.macro OSGetDisplacementOfCurrentTask
	call OSGetDisplacementOfCurrentTaskF
.endmacro

.macro OSGetDisplacementOfTask
#if @0!=R16
	push r16
	mov r16, @0
#endif
	call OSGetDisplacementOfTaskF
#if @0!=R16
	pop r16
#endif
.endmacro
//------------------------------------------------------------------------------------
.macro OSGetNumberOfCurrentTask
	lds @0, CURRENT_TASK
.endmacro

.macro OSSaveNumberOfCurrentTask
	sts CURRENT_TASK, @0
.endmacro
//------------------------------------------------------------------------------------
