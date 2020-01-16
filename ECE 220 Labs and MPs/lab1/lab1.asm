.ORIG x3000
; write your code here (0-->1, 1-->2, 2-->4, 4-->5)
	AND 	R1, R1, #0		; init digit counter
	AND 	R2, R2, #0		; init bit counter
	AND 	R4, R4, #0		; init temp storage
; R3 is hex # to print
DIGIT_LOOP
	ADD		R4, R1, #-4
	BRzp	DONE			; check if R1 (digit counter) is less than 4. digit counter >= 4, it's done
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
	BRn		ADDZERO
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
	BRnzp	DIGIT_LOOP
DONE
	HALT
	.END
	

