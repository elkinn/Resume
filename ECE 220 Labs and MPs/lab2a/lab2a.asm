.ORIG x3000
; Write code to read in characters and echo them
; till a newline character is entered.
		AND R0, R0, #0		;clear R0
		AND R2, R2, #0		;clear R2 for use as a temp register
		AND R5, R5, #0		;clear R5

GET_IN
		GETC
		OUT
		LD R3, MINUS_SPACE		;load -' ' to R3
		ADD R2, R0, R3			;check if R0 = ' '	
		BRz GET_IN
		LD R3, MINUS_OPEN_PAR	;load -'(' to R3
		AND R2, R2, #0			;clear R2
		ADD R2, R0, R3			;check if R0 = '('
		BRz IS_OPEN
		AND R2, R2, #0			;clear R2 again
		LD R3, MINUS_CLOSE_PAR
		ADD R2, R0, R3			;check if R0 = ')'
		BRz IS_CLOSED
		BRnzp GET_IN			;loop to next char if not space, (, or )

IS_OPEN
		JSR PUSH
		BRnzp GET_IN

IS_CLOSED
		JSR IS_BALANCED
		BRnzp GET_IN


SPACE   		.FILL x0020
MINUS_SPACE		.FILL xFFE0
MINUS_OPEN_PAR	.FILL xFFD8
MINUS_CLOSE_PAR	.FILL xFFD7
NEW_LINE        .FILL x000A
CHAR_RETURN     .FILL x000D
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;if '(' push onto stack if ')' pop from stack and check if popped value is '('
;input - R0 holds the input
;output - R5 set to -1 if unbalanced. else 1.
IS_BALANCED
		JSR POP		;load R0 with latest element in stack. if stack is empty, R5 = 1
		AND R2, R2, #0
		ADD R2, R5, #-1
		BRz UNBALANCED
		AND R5, R5, #0
		ADD R5, R5, #1	;set R5 to 1, parantheses are balanced
		BRnzp GET_IN

UNBALANCED
		AND R5, R5, #0
		ADD R5, R5, #-1
		BRnzp DONE
			

NEG_OPEN .FILL xFFD8
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH
        ST R3, PUSH_SaveR3      ;save R3
        ST R4, PUSH_SaveR4      ;save R4
        AND R5, R5, #0          ;
        LD R3, STACK_END        ;
        LD R4, STACk_TOP        ;
        ADD R3, R3, #-1         ;
        NOT R3, R3              ;
        ADD R3, R3, #1          ;
        ADD R3, R3, R4          ;
        BRz OVERFLOW            ;stack is full
        STR R0, R4, #0          ;no overflow, store value in the stack
        ADD R4, R4, #-1         ;move top of the stack
        ST R4, STACK_TOP        ;store top of stack pointer
        BRnzp DONE_PUSH         ;
OVERFLOW
        ADD R5, R5, #1          ;
DONE_PUSH
        LD R3, PUSH_SaveR3      ;
        LD R4, PUSH_SaveR4      ;
        RET


PUSH_SaveR3     .BLKW #1        ;
PUSH_SaveR4     .BLKW #1        ;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP
        ST R3, POP_SaveR3       ;save R3
        ST R4, POP_SaveR4       ;save R3
        AND R5, R5, #0          ;clear R5
        LD R3, STACK_START      ;
        LD R4, STACK_TOP        ;
        NOT R3, R3              ;
        ADD R3, R3, #1          ;
        ADD R3, R3, R4          ;
        BRz UNDERFLOW           ;
        ADD R4, R4, #1          ;
        LDR R0, R4, #0          ;
        ST R4, STACK_TOP        ;
        BRnzp DONE_POP          ;
UNDERFLOW
        ADD R5, R5, #1          ;
DONE_POP
        LD R3, POP_SaveR3       ;
        LD R4, POP_SaveR4       ;
        RET


POP_SaveR3      .BLKW #1        ;
POP_SaveR4      .BLKW #1        ;
STACK_END       .FILL x3FF0     ;
STACK_START     .FILL x4000     ;
STACK_TOP       .FILL x4000     ;


DONE
	HALT

.END

