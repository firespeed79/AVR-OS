/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   11.05.2006
*/

//------------------------------------------------------------------------------------
/*
	R16 - ������� �� ����� �� �� ���������
	R17 - ���������� ����� �� ���������
*/
OSSendMessageToTaskF:
	push ZH
	push ZL
	OSGetDisplacementOfTaskTableR r16

	adiw ZH:ZL, 8
	st Z, r17

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
/*
	R16 - ������� �� ����� �� ����� �����������
	return:
		R16 - ����������� ���������
*/
OSReadTaskMessageF:
	push ZH
	push ZL
	OSGetDisplacementOfTaskTableR r16

	//OSSendSystemMessageC THIS_IS_SYSTEM_TASK

	adiw ZH:ZL, 8
	ld r16, Z

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
