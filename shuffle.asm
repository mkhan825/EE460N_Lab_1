; Name 1: Zhecheng Wang
; Name 2: Masaad Khan
; UTEID 1: zw5668
; UTEID 2: mak4668

        .ORIG x3000

        LEA R1, load
        LDW R1, R1, #0
loop    ADD R0, R0, #-1
        BRN over
        LDB R2, R1, #0
        LDB R3, R1, #1
        STB R2, R1, #1
        STB R3, R1, #0
        ADD R1, R1, #2
        BR loop

over    HALT
load    .FILL x3050

        .END