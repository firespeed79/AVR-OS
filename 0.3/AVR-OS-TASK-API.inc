/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   30.04.2006
*/

/*
	������ �� ���������� ����������:
*/
.equ TASK_COUNTER=$61
.equ CURRENT_TASK=$62

//------------------------------------------------------------------------------------
.macro OSRun
	jmp OSTaskManagerPointOfFirstRun
.endmacro
//------------------------------------------------------------------------------------
.macro OSExitFromTask
	jmp OSExitFromTaskF
.endmacro
//------------------------------------------------------------------------------------
.macro OSRemoveTaskBuNumber
	cli
	push r16
	ldi r16, @0
	call OSRemoveTaskF
	pop r16
	sei
.endmacro
//------------------------------------------------------------------------------------
/*
	@0 - ����� �� ������� (label)
	@1 - ���� �� ������ ����� � ����� ��������� �������
*/
.macro OSCreateTask
	cli
	push XL
	push XH
	push YL
	push YH

	ldi XH, high(@0)
	ldi XL, low(@0)

	ldi YH, high(@1)
	ldi YL, low(@1)
	call OSCreateTaskF

	pop YH
	pop YL
	pop XH
	pop XL
	sei
.endmacro
//------------------------------------------------------------------------------------
.macro OSSaveCurrentTask
	pop XH // ������� ��������� ��
	pop XL // ������� ������ �� �����
	/*                 !!!��������!!!     
	   �� ������ �������� ���� ��� � ������� ��� ����������
	   �� ���������� �� ��������� ������ !!! 
	*/

	push r16
	lds r16, CURRENT_TASK
	call OSSaveTaskF
	pop r16
.endmacro
//------------------------------------------------------------------------------------
.macro OSLoadCurrentTask
	push r16
	lds r16, CURRENT_TASK
	call OSLoadTaskF
	pop r16
.endmacro
//------------------------------------------------------------------------------------
.macro OSGetCurrentTaskNumber
	lds r16, CURRENT_TASK
.endmacro
//------------------------------------------------------------------------------------
.macro OSGetDisplacementOfTaskTableR
	push r16
	mov r16, @0
	call OSGetTaskDisplacementF
	pop r16
.endmacro

.macro OSGetDisplacementOfTaskTableC
	push r16
	ldi r16, @0
	call OSGetTaskDisplacementF
	pop r16
.endmacro
//------------------------------------------------------------------------------------
.macro OSGetNumberOfCurrentTask
	lds r16, CURRENT_TASK
.endmacro
