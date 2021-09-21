        .ORIG   x3000
        LEA     R1, ARRPTR
        LDW     R1, R1, #0  ; R1 has x3050
        XOR     R2, R2, R2  ; int i = 0
STRTFOR NOT     R3, R2
        ADD     R3, R3, #1
        ADD     R3, R3, R0
        BRzp    ENDFOR      ; i<R0

        XOR     R4, R4, R4  ; R4 will be LSB (in little endian)
        XOR     R5, R5, R5  ; R5 will be MSB (in little endian)
        LSHF    R6, R2, #1
        ADD     R6, R6, R1  ; R6 is pointer to current word in array
        LDB     R4, R6, #0
        LDB     R5, R6, #1
        STB     R4, R6, #1
        STB     R5, R6, #0

        ADD     R2, R2, #1  ; i++
        BR      STRTFOR
ENDFOR  HALT
ARRPTR  .FILL   x3050
        .END