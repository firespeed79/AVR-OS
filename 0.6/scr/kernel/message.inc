/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   16.05.2006
*/

//------------------------------------------------------------------------------------
/*
	R16 - ������� �� ����� �� �� ���������
	R17 - ���������� ����� �� ���������
*/
OSSendMessageToTaskF:
	push ZL
	push ZH
	OSGetDisplacementOfTaskR r16

	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	st Z, r17

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
/*
		������� ����������� �� ������� ������
	return:
		R16 - ����������� ���������
*/
OSReadMessageF:
	push ZL
	push ZH

	OSGetNumberOfCurrentTask r16
	OSGetDisplacementOfTaskR r16

	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	ld r16, Z

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
