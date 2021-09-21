      .ORIG x3000
       ; We got a pointer to the label
       LEA R0, ADDR
       ; We get a pointer to x3050
       LDW R0, R0, #0
       ; We get the actual data in x3050
       LDB R1, R0, #0

       ; Mask out the bits for number1
       AND R2, R1, #15
       ; Figure out if the sig-bit is 1/0 for nibble1
       AND R4, R2, 0x8
       BRZ POSIT
       XOR R2, R2, x-10

; Figure out if the sig-bit is 1/0 for nibble2
POSIT  RSHFA R5, R1, #4
       ADD R6, R5, R2
       STB R6, R0, #1
       HALT
ADDR   .FILL x3050
       .END