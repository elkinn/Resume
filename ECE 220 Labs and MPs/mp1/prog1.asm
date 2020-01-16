;
; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR     ; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD  R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	
	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP			; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD  R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD  R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD  R5,AT_MIN_BQ	; set R5 to difference between ASCII '@' and '`'
	LD  R1,STR_START	; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	AND R6, R6, #0		; clears R6 for use at beginning of PRINT_HIST
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z     ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	
	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA			; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop


PRINT_HIST

; you will need to insert your code to print the histogram here

; do not forget to write a brief description of the approach/algorithm
; for your implementation, list registers used in this part of the code,
; and provide sufficient comments
; loop through histogram entries and use printing code to print out their contents.

; Register table:
; R0: points to first histogram entry, holds char for OUT (reset with LD R0, HIST_ADDR) at beginning of counter
; R1: digit counter
; R2: bit counter
; R3: holds contents of current histogram entry
; R4: temp storage
; R5: "digit"
; R6: counter to 27

	AND 	R1, R1, #0		; init digit counter
	AND 	R2, R2, #0		; init bit counter
	AND 	R4, R4, #0		; init temp storage

	LDI	R3, HIST_ADDR		; load contents of current hist entry

PRINT_CHAR
	ADD		R4, R6, #15
	ADD		R4, R4, #15
	ADD		R4, R4, #15
	ADD		R4, R4, #15
	AND		R0, R0, #0
	ADD		R0, R4, #4		; load ascii value of what histogram is counting into R0 and print it
	OUT
	LD		R0, ASCII_SPACE
	OUT						; print " " after char
	AND 	R4, R4, #0		; clear R4

DIGIT_LOOP
	ADD		R4, R1, #-4
	BRzp	NEXT_HIST		; check if R1 (digit counter) is less than 4. digit counter >= 4, it's done
	AND 	R5, R5, #0		; init (or clear) "Digit"
	AND		R2, R2, #0		; clear bit counter
BIT_LOOP
	ADD		R4, R2, #-4
	BRzp	READOUT			; check if R2 (bit counter) is less than 4. bit counter >= 4, print char
	ADD		R5, R5, R5		; shift digit left
	ADD		R4, R3, #0		; load R3 to nzp
	BRzp 	R3LEFTSHIFT		; if R3 >= 0 (MSB = 0) just shift R3
	ADD		R5, R5, #1		; R5 += 1
R3LEFTSHIFT
	ADD		R3, R3, R3		; shift R3 left
	ADD		R2, R2, #1		; increment bit counter
	BRnzp BIT_LOOP
READOUT
	ADD		R4, R5, #-9		
	BRnz		ADDZERO
	ADD		R5, R5, #15
	ADD		R5, R5, #15
	ADD		R5, R5, #15
	ADD		R5, R5, #10		; add 'A' - 10 (#55)
	BRnzp	DO_OUT
ADDZERO
	ADD		R5, R5, #15
	ADD		R5, R5, #15
	ADD		R5, R5, #15
	ADD		R5, R5, #3		; add '0' (#48)
	BRnzp	DO_OUT
DO_OUT
	AND		R0, R0, #0		; clear R0
	ADD		R0, R5, R0		; load char into R0
	OUT						; print char
	ADD		R1, R1, #1		; increment digit counter
	AND		R4, R4, #0		
	ADD		R4, R1, #-4		; print newline if 4 digits have been printed.
	BRn	GO_NEXT
	LD		R0, ASCII_NEWLINE
	OUT
GO_NEXT
	BRnzp	DIGIT_LOOP
NEXT_HIST
	AND		R4, R4, #0	; clear R4
	ADD		R4, R6, #-16
	ADD		R4, R4, #-10	; hist counter - 27, if > 0 then done
	BRzp		DONE
	ADD		R6, R6, #1	; increment R6 (histogram offset)
	LD		R3, HIST_ADDR	; load current hist to R3
	ADD		R3, R3, #1	; increment to next hist entry
	ST		R3, HIST_ADDR	; store next hist addr at HIST_ADDR
	BRnzp	PRINT_HIST		; unconditional jump back to print loop

DONE	
	HALT			; done


; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00 ; histogram starting address
STR_START	.FILL x4000	; string starting address
ASCII_NEWLINE
			.FILL x000A	; ASCII newline
ASCII_SPACE	.FILL x0020 ; ASCII " "

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
