/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   15.06.2006
*/

//------------------------------------------------------------------------------------
/*
	R16 - ������� �� ����� �� �� ���������
	R17 - ���������� ����� �� ���������
*/
OSSendMessageToTaskF:
	push r16
	push ZL
	push ZH
	
	OSGetDisplacementOfTask r16
	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	st Z+, r17
	OSGetNumberOfCurrentTask r16
	st Z+, r16

	pop ZH
	pop ZL
	pop r16
ret
//------------------------------------------------------------------------------------
/*
		������� ����������� �� ������� ������
	return:
		R16 - ����������� ���������
		R17 - ������ �� ������� ����� �� � ��������
*/
OSReadMessageF:
	push ZL
	push ZH

	OSGetNumberOfCurrentTask r16
	OSGetDisplacementOfTask r16

	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	ld r16, Z+
	ld r17, Z+

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
