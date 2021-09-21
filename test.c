#include <stdio.h>
#include <stdint.h>

#define TEST 1
#define DEBUG 1

int
power(int value, int exponent) {
  int sum = value;
  for (int i = exponent; i >= 2; i--) {
    sum = sum*value;
  }
  return sum;
}

void
CHECK_IMM5(int16_t IMM5) {
  if (IMM5 > (power(2, 4) - 1)) {
    /* IMM5 is greater than the max positive value */
    #if (TEST || DEBUG)
    printf("IMM5 %d is too big!\n", IMM5);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  } else if (IMM5 < (-power(2,4))) {
    /* IMM5 is smaller than the most negative value */
    #if (TEST || DEBUG)
    printf("IMM5 %d is too small!\n", IMM5);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  }
}

void
CHECK_PCOFFSET9(int16_t PC_OFFSET9) {
  if (PC_OFFSET9 > (power(2, 8) - 1)) {
    /* PC offset is greater than the max positive value */
    #if (TEST || DEBUG)
    printf("PC_OFFSET9 %d is too big!\n", PC_OFFSET9);
    printf("Error Code: %d\n", 4);
    #endif

    // exit(4);
  } else if (PC_OFFSET9 < (-power(2,8))) {
    /* PC offset is smaller than the most negative value */
    #if (TEST || DEBUG)
    printf("PC_OFFSET9 %d is too small!\n", PC_OFFSET9);
    printf("Error Code: %d\n", 4);
    #endif

    // exit(4);
  }
}

void
CHECK_PCOFFSET11(int16_t PC_OFFSET11) {
  if (PC_OFFSET11 > (power(2, 10) - 1)) {
    /* PC offset is greater than the max positive value */
    #if (TEST || DEBUG)
    printf("PC_OFFSET11 %d is too big!\n", PC_OFFSET11);
    printf("Error Code: %d\n", 4);
    #endif

    // exit(4);
  } else if (PC_OFFSET11 < (-power(2,10))) {
    /* PC offset is smaller than the most negative value */
    #if (TEST || DEBUG)
    printf("PC_OFFSET11 %d is too small!\n", PC_OFFSET11);
    printf("Error Code: %d\n", 4);
    #endif

    // exit(4);
  }
}

void
CHECK_BOFFSET6(int16_t BOFFSET6) {
  if (BOFFSET6 > (power(2, 5) - 1)) {
    /* BOFFSET6 is greater than the max positive value */
    #if (TEST || DEBUG)
    printf("BOFFSET6 %d is too big!\n", BOFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  } else if (BOFFSET6 < (-power(2,5))) {
    /* BOFFSET6 is smaller than the most negative value */
    #if (TEST || DEBUG)
    printf("BOFFSET6 %d is too small!\n", BOFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  }
}

void
CHECK_OFFSET6(int16_t OFFSET6) {
  if (OFFSET6 > (power(2, 5) - 1)) {
    /* OFFSET6 is greater than the max positive value */
    #if (TEST || DEBUG)
    printf("OFFSET6 %d is too big!\n", OFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  } else if (OFFSET6 < (-power(2,5))) {
    /* OFFSET6 is smaller than the most negative value */
    #if (TEST || DEBUG)
    printf("OFFSET6 %d is too small!\n", OFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  }
}

void
CHECK_AMOUNT4(int16_t AMOUNT4) {
  if (AMOUNT4 > (power(2, 4) - 1)) {
    /* AMOUNT4 is greater than the max positive value */
    #if (TEST || DEBUG)
    printf("AMOUNT4 %d is too big!\n", AMOUNT4);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);

  } else if (AMOUNT4 < 0) {
    /* AMOUNT4 cannot be negative */
    #if (TEST || DEBUG)
    printf("AMOUNT4 %d is too small!\n", AMOUNT4);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  }
}

void
CHECK_TRAPVECT8(int16_t TRAPVECT8) {
  if (TRAPVECT8 > (power(2, 8) - 1)) {
    /* TRAPVECT8 is greater than the max positive value */
    #if (TEST || DEBUG)
    printf("TRAPVECT8 %d is too big!\n", TRAPVECT8);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  } else if (TRAPVECT8 < 0) {
    /* TRAPVECT8 cannot be a negative value */
    #if (TEST || DEBUG)
    printf("TRAPVECT8 %d cannot be negative!\n", TRAPVECT8);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  }
}

//todo: check the int16_t, int32_t, etc
void
CHECK_FILL(int32_t FILL) {
  if (FILL > (power(2, 16) - 1)) {
    /* FILL is greater than the max positive value */
    #if (TEST || DEBUG)
    printf("FILL %d is too big!\n", FILL);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  } else if (FILL < (-power(2,15))) {
    /* FILL cannot be a negative value */
    #if (TEST || DEBUG)
    printf("FILL %d is too small!\n", FILL);
    printf("Error Code: %d\n", 3);
    #endif

    // exit(3);
  }
}

int main () {
  CHECK_IMM5(15);
  CHECK_IMM5(-16);
  CHECK_PCOFFSET9(255);
  CHECK_PCOFFSET9(-256);
  CHECK_PCOFFSET11(1023);
  CHECK_PCOFFSET11(-1024);
  CHECK_BOFFSET6(31);
  CHECK_BOFFSET6(-32);
  CHECK_OFFSET6(31);
  CHECK_OFFSET6(-32);
  CHECK_AMOUNT4(15);
  CHECK_AMOUNT4(0);
  CHECK_TRAPVECT8(255);
  CHECK_TRAPVECT8(0);
  CHECK_FILL(65535);
  CHECK_FILL(-32768);
}