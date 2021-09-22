;
;	Name 1: Masaad Khan
;	Name 2: Rithvik Dyava
;	UTEID 1: mak4668
;	UTEID 2: rd29228
;
       .ORIG x3000
       STW R3, R1, x40
       ; We got a pointer to the label
       ; R0 gets ptr offset of 38

       LEA R0, ADDR
       ; We get a pointer to x3050
       ; When we derefence we get x3050
      
       ; R0 = x3050
       LDW R0, R0, #0

       ; We get the actual data into R1 from x3050
       ; When we derefence we get contents of x3050
       ; R1 = 0xFF88
       LDB R1, R0, #0

       ; Mask out the bits for nibble1
       ; R2 = 0x8
       AND R2, R1, #15
       ; Figure out if the sig-bit is 1/0 for nibble1
       LEA R7, BACK1
       ;R3 = x8
       AND R3, R2, x8
       ; if top bit is a one
       BRP NEG1

       ; Mask out the bits for nibble2
       ; R4 = xFF8
BACK1  RSHFL R4, R1, #4
       ; R4 = x8
       AND R4, R4, #15
       ; Figure out if this is neg
       LEA R7, BACK2
       ; R3 = x8
       AND R3, R4, x8
       ; if top bit is a one
       BRP NEG2

       ; Add both sign extended nibbles
       ; R5 = xF8 + xF8 = xF0
BACK2  ADD R5, R2, R4
       ; MEM[R0 + 1] = R5
       STB R5, R0, #1
       HALT

       ; Sign extends R2
       ; R2 = xF8
NEG1   XOR R2, R2, x-10
       RET

       ; Sign extends R4
       ; R4 = xF8
NEG2   XOR R4, R4, x-10
       RET

ADDR   .FILL x3050
       .END