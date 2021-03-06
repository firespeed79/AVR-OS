/*
	      The^day^of^DooM

	Create date:   26.01.2007
	Last Update:   29.05.2007

							!!! ����� !!!
	
	1) ������ ���������� ���������� ������� ��������� �� ����������.
	   (����� ���� ���� ��� ��������� �� ����� C)

	�������� � �����:
		1) ������ ������ �� ��������� �����, � ������ �� 256 �����,
		   ��������� r16 � ������� ����� � ��������� OSMllocF.
		   �� ����������� �� ������ ������� ����� ���� �� �������
		   ������ � ������ �� ��������� ����� � ��������� �� �������.
		   ����� ��� ������ (����� � ������) � ������ 0 - �� � ��������
		   �����. �� �������� �� ����� ������ � � Z a �������, ���������� 0,
		   � R16

		2) ������ ������ �� ��������� ������ ���� ������
		   ��������� OSFreeF. �� ������ ������� 0 �� ����� � ������ �
		   ��������� �� ������� �� ������� ������. �� � ���������� ����
		   ������ � ��� ������������ �����.

		3) �� ����� ����� ����� �� ������� ������ � ������� �� ������������
		   ����� ���� OSGetPtrToDSF.
*/

// -------------------------------------------------------------------------------------------------------------
OSFreeF:
	push r16
	lds r16, CURRENT_TASK
	call OSKernelFreeF	
	pop r16
ret

/*
	R16 - ������ �� �������
*/
OSKernelFreeF:
	call OSOpenTaskArrayF
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, MALLOC_TABLE_HIGH_BYTE
	clr r16
	st Z+, r16
	st Z+, r16
	st Z+, r16
	call OSCloseTaskArrayF
ret
// -------------------------------------------------------------------------------------------------------------
/*
	R16 - ������� �� �������� �����.
	Return:
		R16		- ������� �� �����. ��� � ���� �� �� ��� �������� �����.
		ZH:ZL 	- ������ �� �����. =//=. ��������� �� ����������� (lds Rx, Z+)
*/
OSMallocF:
	push r17
	lds r17, CURRENT_TASK
	call OSKernelMallocF
	pop r17
ret
// -------------------------------------------------------------------------------------------------------------
/*
	register char size_temp = r16;		 // r2 - ���������� ������ ������� �� �������� �����
	register char size_of_current_block; // r3 - ���������� � ����� ��������� ������� �� ������� ���� �� taskN
	register char task_number_temp;		 // r6 - ����������� ������ ������ �� �������
	reister int temp,  // R25:R24 - �� ���� �������� ������� � ������������
				temp0, // R27:R26 - K��� �� ������ �� ���������

				temp1; // R29:R28 - ������� ����� �� ����� �� ������� ��������� �� �������� ���� �����.
			
				temp3, // R15:R14 \  �������� � ������� ����� ��
				temp4, // R13:R12 / ����� ����� �� ��������

				temp5, // R11:R10 \  ������� � ������� ����� ��
				temp6, // R9:R8   / ������ ����� ������ taskN

	� ���� ����� ������ � ��������:

		1) ��������� �� ����� ������� �� ���� �� �������, temp1
		2) ����� ��� ��������� �������� �������� � ���� �� ����� ����� �� ��������, temp4:temp3
		4) ��������� ������� �� ��� ������� �����: ������� �� ���� � �������� ���� � ��������
		    (��������� ���������� ����� �� ������ ������� - ���� ��� ����������)
		5) ���������� ��������� ��������� �� ���� �� ������ ��� �������� �, ���������� temp1
		   �� �����������.
		6) ��� ����� dec �� temp1 �� ��������� ������ ������� �� temp3:temp4
		   (temp4=temp1; temp3=temp1-������� �� �����). ���� ���� �� ���������
		   ���� ����� �� �������� ����� ����� �����, ���� �� �������� ������ �������.
		   �������� �� ���� (r4) ����� ������� ���� �� ��������� � ��� ���������� �� ����� �������
		   �� �������� � 1. ��� ����� ���� - ����� � �������� � �� �����������. ����� �� ����� �� 6)
		   ������ �� ������ ���� ��� 


	R16 - ������� ������ �����.
	R17 - ������� �� ����� �� �������� �����.
*/

OSKernelMallocF:
	call OSOpenTaskArrayF
	//////////////////////////////////////////////
	mov r2, r16 // Init size_temp
	mov r6, r17 // Init task_number_temp
	//////////////////////////////////////////////
	// Init temp0
	ldi r25, 100
	lds r24, TASK_COUNTER
	mul r25, r24
	movw r27:r26, r1:r0
	adiw r27:r26, 0x31
	adiw r27:r26, 0x31
	//////////////////////////////////////////////
	// temp 1 = MALLOC_SRAM_END
	ldi r29, high(MALLOC_SRAM_END_H)
	ldi r28, low(MALLOC_SRAM_END_L)
	//////////////////////////////////////////////
  OSKernelMallocSerachFreeBlockL:
	// temp4 = temp1
	mov r13, r29
	mov r12, r28
	////////////////////////////////////////////////
	// temp3 = temp4 - size;
	mov r15, r13
	mov r14, r12
	clr r0
	sub r14, r2
	sbc r15, r0
	////////////////////////////////////////////////
	// if(temp3 < temp0) break; - ����� ����� �����
	cp r14, r26  // ������ �� �������� � ��������
	cpc r15, r27 // � ������ �� ���������, �� ���
	             //�������� �������� �����.
	brlo OSKernelMallocThisIsNotFreeBlockForAllTaskL
	////////////////////////////////////////////////
  	lds r16, TASK_COUNTER
	mov r4, r16
	OSKernelMallocTestThisBlockInAllTaskL:
	   call OSGetDisplacementOfTaskF
	   adiw ZH:ZL, MALLOC_TABLE_HIGH_BYTE
	   ld r11, Z+ // \  Init 
	   ld r10, Z+ // / temp5
	   ld r3,  Z+ // Init size_of_current_block
	   tst r16
	   breq OSKernelMallocThisIsFreeBlockForThisTaskL
	   //////////////////////////////////////////////
	   // Init temp6
	   clr r0
	   mov r9, r11
	   mov r8, r10
	   add r8, r3
	   adc r9, r0
	   //////////////////////////////////////////////
	   // if(temp3>=temp6 || temp5>=temp4) {
		cp r14, r8
		cpc r15, r9
	   brge OSKernelMallocThisIsFreeBlockForThisTaskL

		cp r10, r12
		cpc r11, r13
	   brlo OSKernelMallocSerachFreeBlockNextAddressL

	   //////////////////////////////////////////////
	   // ������� �� ������� ������ ���� ����������
	   // � ������� ����� ������ �� ��������.
	   // ������ r4==0 ��� �������� �������� ����
	   // �� �������� �������. ���� ������������
	   // ��������� ���� ���������� ������ ��
	   // ����� � ������� ������
	   OSKernelMallocThisIsFreeBlockForThisTaskL:
	   	 dec r4
	   // }
	   //////////////////////////////////////////////
	   dec r16
	brne OSKernelMallocTestThisBlockInAllTaskL
	OSKernelMallocTestThisBlockInAllTaskBreakL:
	
	tst r4
	breq OSKernelMallocThisIsFreeBlockForAllTaskL

	OSKernelMallocSerachFreeBlockNextAddressL:
	subi r29:r28, 1 // --temp1
  jmp OSKernelMallocSerachFreeBlockL

  OSKernelMallocThisIsFreeBlockForAllTaskL:
	mov r29, r15
	mov r28, r14
   jmp OSKernelMallocRetL

  OSKernelMallocThisIsNotFreeBlockForAllTaskL: // �� ��� �������� �������� ���� �����
	clr r2
	clr r29
	clr r28

 OSKernelMallocRetL:
 	mov r16, r6
	call OSGetDisplacementOfTaskF
 	adiw ZH:ZL, MALLOC_TABLE_HIGH_BYTE
	st Z+, r29
	st Z+, r28
	st Z+, r2

	mov r16, r12 // ��� � �������� �� ���� - ������ ��� �� �������� �����
	movw ZH:ZL, r29:r28 // ������ ���� �������

	call OSCloseTaskArrayF
ret
// -------------------------------------------------------------------------------------------------------------
OSGetPtrToDSF:
	lds r16, CURRENT_TASK
	call OSKernelGetPtrToDSF
ret

/*
	R16 - ������ �� �������
	Return:
		R16		- ������� �� �������
		ZH:ZL	- ������ �� �������
*/
OSKernelGetPtrToDSF:
	pop XL
	pop XH
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, MALLOC_TABLE_HIGH_BYTE
	movw XH:XL, ZH:ZL
	ld ZH, X+
	ld ZL, X+
	ld r16, X+
	pop XH
	pop XL
ret
// -------------------------------------------------------------------------------------------------------------
