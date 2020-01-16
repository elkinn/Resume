;R0, holds char/arithmetic out/pop out
;R5, holds result from push/pop
;R3, R4, holds arithmetic inputs
;R1, R2 temp registers
.ORIG x3000
	
;your code goes here	
IN_LOOP
	GETC
	OUT
	LD R1, MINUS_SPACE
	ADD R2, R1, R0
	BRz IN_LOOP
	JSR EVALUATE
	BRnzp IN_LOOP
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
;R5- holds final value of calculation
;R1- digit counter
;R2- bit counter
;R4- temp storage
;R6- "Digit"
PRINT_HEX
	;LD R0, ASCII_NL
	;OUT
	;LD R0, ASCII_x
	;OUT
	JSR 	POP
	ST 	R0, MATH_SaveR7
	JSR 	POP
	ADD 	R5, R5, #-1
	BRnp 	INVALID			;if stack !empty (after pop), untreated values remain --> invalid
	LD	R0, MATH_SaveR7
	AND 	R5, R5, #0
	ADD 	R5, R5, R0		; load final value into R5
	AND 	R3, R3, #0
	ADD 	R3, R3, R0		; load value to print into R3
	
	AND 	R1, R1, #0		; init digit counter
	AND 	R2, R2, #0		; init bit counter
	AND 	R4, R4, #0		; init temp storage
DIGIT_LOOP
	ADD	R4, R1, #-4
	BRzp	DONE			; check if R1 (digit counter) is less than 4. digit counter >= 4, it's done
	AND 	R6, R6, #0		; init (or clear) "Digit"
	AND	R2, R2, #0		; clear bit counter
BIT_LOOP
	ADD	R4, R2, #-4
	BRzp	READOUT			; check if R2 (bit counter) is less than 4. bit counter >= 4, print char
	ADD	R6, R6, R6		; shift digit left
	ADD	R4, R3, #0		; load R3 to nzp
	BRzp 	R3LEFTSHIFT		; if R3 >= 0 (MSB = 0) just shift R3
	ADD	R6, R6, #1		; R6 += 1
R3LEFTSHIFT
	ADD	R3, R3, R3		; shift R3 left
	ADD	R2, R2, #1		; increment bit counter
	BRnzp BIT_LOOP
READOUT
	ADD	R4, R6, #-9		
	BRn	ADDZERO
	ADD	R6, R6, #15
	ADD	R6, R6, #15
	ADD	R6, R6, #15
	ADD	R6, R6, #10		; add 'A' - 10 (#55)
	BRnzp	DO_OUT
ADDZERO
	ADD	R6, R6, #15
	ADD	R6, R6, #15
	ADD	R6, R6, #15
	ADD	R6, R6, #3		; add '0' (#48)
	BRnzp	DO_OUT
DO_OUT
	AND	R0, R0, #0		; clear R0
	ADD	R0, R6, R0		; load char into R0
	OUT				; print char
	ADD	R1, R1, #1		; increment digit counter
	BRnzp	DIGIT_LOOP
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - current numerical output
;R1, R2- temp storage
;
EVALUATE
	LD R1, MINUS_ZERO
	ADD R2, R1, R0
	BRzp AT_LEAST_ZERO
	LD R1, MINUS_PLUS
	ADD R2, R1, R0
	BRz TO_PLUS
	LD R1, MINUS_MIN
	ADD R2, R1, R0
	BRz TO_MIN
	LD R1, MINUS_MUL
	ADD R2, R1, R0
	BRz TO_MUL
	LD R1, MINUS_DIV
	ADD R2, R1, R0
	BRz TO_DIV
	JSR INVALID

AT_LEAST_ZERO
	LD R1, MINUS_NINE
	ADD R2, R1, R0
	BRp EQ_OR_EXP
	LD R1, MINUS_ZERO
	ADD R0, R0, R1
	JSR PUSH
	BRnzp IN_LOOP
	

EQ_OR_EXP
	LD R1, MINUS_EQ
	ADD R2, R1, R0
	BRz PRINT_HEX
	LD R1, MINUS_EXP
	ADD R2, R1, R0
	BRz TO_EXP
	LD R0, INVALID_STR
	PUTS
	BRnzp DONE
	

;your code goes here
;R3, R4, hold operands
;R5 holds overflow indicator of JSR POP

TO_PLUS
	AND R3, R3, #0
	AND R4, R4, #0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R4, R4, R0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R3, R3, R0
	JSR PLUS
	JSR PUSH
	BRnzp IN_LOOP

TO_MIN
	AND R3, R3, #0
	AND R4, R4, #0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R4, R4, R0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R3, R3, R0
	JSR MIN
	JSR PUSH
	BRnzp IN_LOOP

TO_MUL
	AND R3, R3, #0
	AND R4, R4, #0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R4, R4, R0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R3, R3, R0
	JSR MUL
	JSR PUSH
	BRnzp IN_LOOP

TO_DIV
	AND R3, R3, #0
	AND R4, R4, #0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R4, R4, R0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R3, R3, R0
	JSR DIV
	JSR PUSH
	BRnzp IN_LOOP

TO_EXP
	AND R3, R3, #0
	AND R4, R4, #0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R4, R4, R0
	JSR POP
	ADD R5, R5, #-1
	BRz INVALID
	ADD R3, R3, R0
	JSR EXP
	JSR PUSH
	BRnzp IN_LOOP


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS	
;your code goes here
	AND R0, R0, #0
	ADD R0, R3, R4
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN	
;your code goes here
	AND R0, R0, #0
	NOT R4, R4
	ADD R4, R4, #1	;clear R0 and set R4 to -R4
	ADD R0, R3, R4
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MUL	
;your code goes here
	ST R4, POP_SaveR4
	AND R0, R0, #0
MUL_LOOP
	ADD R0, R0, R3
	ADD R4, R4, #-1
	BRp MUL_LOOP
	LD R4, POP_SaveR4
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0 (quotient), R1 (remainder)
DIV	
;your code goes here
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
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
EXP
;your code goes here
	ST R7, MATH_SaveR7
	AND R0, R0, #0
	AND R1, R1, #0
	ADD R1, R1, R4
	ADD R1, R1, #-1
	AND R4, R4, #0
	ADD R4, R4, R3
EXP_LOOP
	JSR MUL
	AND R3, R3, #0
	ADD R3, R3, R0
	ADD R1, R1, #-1
	BRp EXP_LOOP
	LD R7, MATH_SaveR7
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
INVALID
	LEA R0, INVALID_STR	;load address of "Invalid Expression" to R0
	PUTS			;print
	BRnzp DONE
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


DONE
	HALT			;halts the program


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
MATH_SaveR7	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;
EXP_SaveR4	.BLKW #1
MINUS_EQ	.FILL xFFC3
MINUS_PLUS	.FILL xFFD5
MINUS_MIN	.FILL xFFD3
MINUS_DIV	.FILL xFFD1
MINUS_MUL	.FILL xFFD6
MINUS_SPACE	.FILL xFFE0
MINUS_EXP	.FILL xFFA2
MINUS_ZERO	.FILL xFFD0
MINUS_NINE	.FILL xFFC7
ASCII_x		.FILL x0078
ASCII_NL	.FILL x000A	
INVALID_STR	.STRINGZ "Invalid Expression"

.END


.END
