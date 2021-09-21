        .ORIG   x3000
        ADD     R0, R0, R0
        ADD     R0, R0, #15
        AND     R0, R0, R0

        AND     R0, R0, xF
        BRp     SKIP
BACK    LEA     R7, CONT

        RET
SKIP    LEA     R1, BACK

        JMP     R1

CONT    JSR     JSRTEST

        LEA     R2, JSRRTST
        JSRR    R2
        NOT     R4, R3


        XOR     R3, R3, R3

        HALT
        RTI                 ; Just for testing
        LSHF    R2, R3, #3  ; R2 ê LSHF(R3, #3)

        RSHFL   R2, R3, #7  ; R2 ê RSHF(R3, #7, 0)

        RSHFA   R2, R3, #7  ; R2 ê RSHF(R3, #7, R3[15])
        STB     R2, R7, #3

        STW     R2, R7, #1


        XOR     R2, R2, #0
        TRAP    x25

FILL    .FILL   xEEFF

JSRTEST LDB     R3, R7, x5  ;


        RET


JSRRTST LDW     R3, R7, x4  ;


        RET
        .END