/*
			AVR-Os Version 0.3
		
		The^day^of^DooM

	Create date:   03.04.2006
	Last Update:   30.04.2006

                                  ����� �� �������:
   -------------------------------------------------------------------------------------
        ������������� ���� �� OS (������ ����� 0) [3 �����]:	
                $61 - ���� �� ���������
                $62 - ������ �� ������� ���������� � �������
                $63 - SYSTEM_MESSAGE
   -------------------------------------------------------------------------------------
        ����� �� ��������� - �������� �� ��������� � ����� SIZE_OF_TASK_STRUCTURE:

                (((����� �� �������)*8)+$60)+0 - SREG �� �������

                (((����� �� �������)*8)+$60)+1 - H \    ������ �������
                (((����� �� �������)*8)+$60)+2 - L /    � �������

                (((����� �� �������)*8)+$60)+3 - H \    ������� �� �������
                (((����� �� �������)*8)+$60)+4 - L /    (� ������ �����)

                (((����� �� �������)*8)+$60)+5 - L -    ����������� ����� �� ���������
                (((����� �� �������)*8)+$60)+6 - L \    ����� �� �����������
                (((����� �� �������)*8)+$60)+7 - L /    ����������� �������������

				(((����� �� �������)*8)+$60)+8 - Message Box
   -------------------------------------------------------------------------------------

                                     *�����*

      1)  ��� ���������� �� ����� ������ �� ��������� ���� ���� (��� ���)
	 �� ������� ������ � ���� ������.
*/

.include "AVR-OS-IRQ-API.inc"
.include "AVR-OS-TASK-API.inc"
.include "AVR-OS-TIMER-API.inc"
.include "AVR-OS-MESSAGE-API.inc"

// -----------------------------------------
/*
	�������� ���������:
*/
.equ NULL=0
.equ SIZE_OF_TASK_STRUCTURE=9
// -----------------------------------------

.macro OSInit
.cseg
.org $000
	jmp OSReset
.org INT0addr
	jmp OSINT0
.org INT1addr
	jmp OSINT1

.org OC2addr
.org OVF2addr
.org ICP1addr
.org OC1Aaddr
.org OC1Baddr
.org OVF1addr
.org OVF0addr
.org SPIaddr
.org URXCaddr
.org UDREaddr
.org UTXCaddr
.org ADCCaddr
.org ERDYaddr
.org ACIaddr
.org TWIaddr
.org INT2addr

.org OC0addr
		rjmp OSTaskManagerF

/*
.equ	OC0addr	= 0x0026	; Timer/Counter0 Compare Match
.equ	SPMRaddr	= 0x0028	; Store Program Memory Ready
*/
OSReset:
	ldi r16, low(ramend)
	out spl, r16
	ldi r16, high(ramend)
	out sph, r16

	clr r16
	sts CURRENT_TASK, r16
	sts TASK_COUNTER, r16
.endmacro
