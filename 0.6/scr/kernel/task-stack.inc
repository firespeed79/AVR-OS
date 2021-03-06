/*
	      The^day^of^DooM

	Create date:   15.05.2006
	Last Update:   17.05.2006
*/

//------------------------------------------------------------------------------------
OSPushF:
	push ZL
	push ZH

	push r16
	call OSGetTaskStackF
	
	inc r16    // ����������� � ���������
	st Z, r16 // ����� ���� �������� � �����
	
	add ZL, r16 // ����������� ������������
	clr r16     // ��
	adc ZH, r16 // ��������� �������

	pop r16     // ������������� �
	st Z, r16   // ��������� ��������

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
OSPopF:
	push ZL
	push ZH
	
	call OSGetTaskStackF
	push r16
	dec r16
	st Z, r16
	pop r16

	add ZL, r16 // ����������� ������������
	clr r16     // ��
	adc ZH, r16 // ��������� �������

	ld r16, Z   // ��������� �������� �������

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
/*
	return:
		R16 - ������� ���� �������� � �����
		  Z - ������������ �� �����
*/
OSGetTaskStackF:
	OSGetNumberOfCurrentTask r16
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, DISPLACEMENT_OF_TASK_STACK
	nop
	ld r16, Z // ��������� ������� ���� �� �������� � �����
ret
//------------------------------------------------------------------------------------
