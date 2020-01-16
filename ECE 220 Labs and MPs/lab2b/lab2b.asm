;number to print in decimal is in R3.
;it will be positive.
;PRINTS ONE TOO MANY DIGITS
.ORIG x3000

	AND R4, R4, #0
	ADD R4, R4, #10 ;set R4 to #10
	LD R3, TEST		;load dividend into R3
MAIN
	JSR DIV
	AND R3, R3, #0	;
	ADD R3, R3, R0	;store quotient as new dividend
	AND R0, R0, #0	;
	ADD R0, R0, R1	;put remainder into R0 to push
	JSR PUSH	
	ADD R0, R0, #0	;check value of R0
	BRp MAIN		;if remainder is positive, repeat, otherwise move to print
PRINT
	JSR POP
	ADD R5, R5, #-1
	BRz DONE		;if underflow is experienced, stack is empty and printing is done
	LD R1, ASCII_0	;load x30 into R1
	ADD R0, R0, R1	;set R0 to ascii value of whatever was in it
	OUT				;print
	BRnzp PRINT

DONE
	HALT
	
TEST	.FILL #123
ASCII_0 .FILL x30
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0-quotient, R1-remainder
DIV	
	AND R0, R0, #0	;
	AND R1, R1, #0	;
	ADD R1, R1, R3	;clear relevant registers and set remainder to dividend (to begin)
	NOT R4, R4
	ADD R4, R4, #1	;make R4 = -R4
DIV_LOOP
	ADD R0, R0, #1	;increment quotient
	ADD R1, R1, R4	;R1 -= R4
	BRzp DIV_LOOP	;if remainder > 0, continue
	ADD R4, R4, #-1
	NOT R4, R4		;un-negative R4
	ADD R1, R1, R4	;add R4 back to R1 once
	ADD R0, R0, #-1	;take one away from quotient
	RET


;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;

.END

