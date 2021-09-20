/*
	Name 1: Masaad Khan
	Name 2: Rithvik Dyava
	UTEID 1: mak4668
	UTEID 2: rd29228
*/

#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */
#include <stdbool.h>
#include "assembler.h"


// char* opcode_string[] = {"ADD", "AND", ""};
uint16_t PC = 0;

/***************************************************************************************/
/*                                                                                     */
/*                             FUNCTION IMPLEMENTATIONS                                */
/*                                                                                     */
/***************************************************************************************/
int
readAndParse(FILE* pInfile, char* pLine, char** pLabel,
             char** pOpcode, char** pArg1, char** pArg2,
             char** pArg3, char** pArg4) {
  char* lRet;
  char* lPtr;
  int i;

  if(!fgets(pLine, MAX_LINE_LENGTH, pInfile)) {
    return(DONE);
  }

  /* convert entire line to lowercase */
  for(i = 0; i < strlen(pLine); i++) {
    pLine[i] = tolower(pLine[i]);
  }

  *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

  /* ignore the comments */
  lPtr = pLine;

  while(*lPtr != ';' && *lPtr != '\0' && *lPtr != '\n') {
    lPtr++;
  }

  *lPtr = '\0';
  if(!(lPtr = strtok(pLine, "\t\n ,"))) {
    return(EMPTY_LINE);
  }

  /* found a label */
  if(!isOpcode(lPtr) && lPtr[0] != '.') {
    *pLabel = lPtr;
    if(!(lPtr = strtok( NULL, "\t\n ,"))) {
      // checks for end of line
      return(OK);
    }
  }
    
  *pOpcode = lPtr;

  if(!(lPtr = strtok(NULL, "\t\n ,"))) {
    return(OK);
  }
    
  *pArg1 = lPtr;
    
  if(!(lPtr = strtok(NULL, "\t\n ,"))) {
    return( OK );
  }

  *pArg2 = lPtr;

  if(!(lPtr = strtok(NULL, "\t\n ,"))) {
    return( OK );
  }

  *pArg3 = lPtr;

  if(!(lPtr = strtok(NULL, "\t\n ,"))) {
    return( OK );
  }

  *pArg4 = lPtr;

  return(OK);
}

int
toNum(char* pStr) {
  char* t_ptr;
  char* orig_pStr;
  int t_length;
  int k;
  int lNum = 0;
  int lNeg = 0;
  long int lNumLong;

  orig_pStr = pStr;
  /* decimal */
  if(*pStr == '#') {
    pStr++;
    /* dec is negative */
    if(*pStr == '-') {
      lNeg = 1;
      pStr++;
    }

    t_ptr = pStr;
    t_length = strlen(t_ptr);
    for(k=0; k < t_length; k++) {
      if (!isdigit(*t_ptr)) {
        printf("Error: invalid decimal operand, %s\n",orig_pStr);
        #if (DEBUG)
        printf("Error Code: %d\n", 4);
        #endif

        exit(4);
      }
      t_ptr++;
    }

    lNum = atoi(pStr);
    if (lNeg) {
      lNum = -lNum;
    }

    return lNum;

  /* hex */
  } else if(*pStr == 'x') {
    pStr++;
    /* hex is negative */
    if(*pStr == '-') {
      lNeg = 1;
      pStr++;
    }

    t_ptr = pStr;
    t_length = strlen(t_ptr);
    for(k=0; k < t_length; k++) {
      if (!isxdigit(*t_ptr)) {
        printf("Error: invalid hex operand, %s\n",orig_pStr);
        #if (DEBUG)
        printf("Error Code: %d\n", 4);
        #endif

        exit(4);
      }
      t_ptr++;
    }

    /* convert hex string into integer */
    lNumLong = strtol(pStr, NULL, 16);
    lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;

    if(lNeg) {
      lNum = -lNum;
    }
    return lNum;

  } else {
    printf( "Error: invalid operand, %s\n", orig_pStr);
    /* This has been changed from error code 3 to error code 4, see clarification 12 */
    #if (DEBUG)
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }
}

bool
isOpcode(char* str) {
  //todo: isOpcode
  if (string_equal(str, "add")) {
    return true;
  } else if (string_equal(str, "and")) {
    return true;
  }
  else if(string_equal(str, "br") ||
          string_equal(str, "brp") || 
          string_equal(str, "brz") ||
          string_equal(str, "brzp") ||
          string_equal(str, "brn") ||
          string_equal(str, "brnp") ||
          string_equal(str, "brnz") ||
          string_equal(str, "brnzp")) {
    return true;
  }
  else if(string_equal(str, "jmp") ||
          string_equal(str, "ret")){
    return true;
  }
  else if(string_equal(str, "jsr") ||
          string_equal(str, "jsrr")){
    return true;
  }
  else if(string_equal(str, "ldb")){
    return true; 
  }
  else if(string_equal(str, "ldw")){
    return true;
  }
  else if(string_equal(str, "lea")){
    return true; 
  }
  else if(string_equal(str, "rti")){
    return true;
  }
  else if(string_equal(str, "lshf") ||
          string_equal(str, "rshfl") ||
          string_equal(str, "rshfa")){
    return true;
  }
  else if(string_equal(str, "stb")){
    return true;
  }
  else if(string_equal(str, "stw")){
    return true;
  }
  else if(string_equal(str, "trap")){
    return true;
  }
  else if(string_equal(str, "xor") ||
          string_equal(str, "not")){
    return true;
  }
  else if (string_equal(str, ".orig") ||
           string_equal(str, ".end") ||
           string_equal(str, "halt") ||
           string_equal(str, ".fill")) {
    return true;
  }

  /* String is not an Opcode */
  return false;
}

struct label g_labels[MAX_NUM_LABELS];

void
label_exists(char* label) {
  for (int i = 0; i < MAX_NUM_LABELS; i++) {
    if (g_labels[i].label[0] != '\0') {
      if (string_equal(g_labels[i].label, label)) {
        #if (DEBUG)
        printf("Found a label that is already init-ed\n");
        printf("Error Code: %d\n", 4);
        #endif

        exit(4);
      }
    }
  }
}

struct label*
find_label(char* label) {
  for (int i = 0; i < MAX_NUM_LABELS; i++) {
    if (g_labels[i].label[0] != '\0') {
      if (string_equal(g_labels[i].label, label)) {
        #if (DEBUG)
        printf("Found our label: %s at address: 0x%x\n", g_labels[i].label, g_labels[i].addr);
        #endif
        return &g_labels[i];
      }
    }
  }

  #if (DEBUG)
  printf("Did not find label: %s\n", label);
  #endif

  /* Assuming that we can take immediate values... todo*/
  return NULL;
}

bool
search_label(char* infile, char* label) {
  FILE* input = fopen(infile, "r");
  int lRet;
  char pLine[MAX_LINE_LENGTH + 1];
  char* pLabel[MAX_LINE_LENGTH + 1];
  char* pOpcode[MAX_LINE_LENGTH + 1];
  char* pArg1[MAX_LINE_LENGTH + 1];
  char* pArg2[MAX_LINE_LENGTH + 1];
  char* pArg3[MAX_LINE_LENGTH + 1];
  char* pArg4[MAX_LINE_LENGTH + 1];

  do {
    lRet = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (string_equal(*pOpcode, "br") ||
        string_equal(*pOpcode, "brp") ||
        string_equal(*pOpcode, "brz") ||
        string_equal(*pOpcode, "brzp") ||
        string_equal(*pOpcode, "brn") ||
        string_equal(*pOpcode, "brnp") ||
        string_equal(*pOpcode, "brnz") ||
        string_equal(*pOpcode, "brnzp")) {
      if (string_equal(*pArg1, label)) {
        return true;
      }
    }

    if (string_equal(*pOpcode, "lea")) {
      if (string_equal(*pArg2, label)) {
        return true;
      }
    }

    if (string_equal(*pOpcode, "jsr")) {
      if (string_equal(*pArg1, label)) {
        return true;
      }
    }
    
  } while(lRet != DONE);

  /* Was not able to see this label as an argument */
  #if (DEBUG)
  printf("Found unused label: %s\n", label);
  printf("Error Code: %d\n", 4);
  #endif

  exit(4);
}

void
first_pass(char* infile) {
  int lRet;
  FILE* input = fopen(infile, "r");

  char pLine[MAX_LINE_LENGTH + 1];
  char* pLabel[MAX_LINE_LENGTH + 1];
  char* pOpcode[MAX_LINE_LENGTH + 1];
  char* pArg1[MAX_LINE_LENGTH + 1];
  char* pArg2[MAX_LINE_LENGTH + 1];
  char* pArg3[MAX_LINE_LENGTH + 1];
  char* pArg4[MAX_LINE_LENGTH + 1];

  /* Do the First Pass and check for .ORIG/.END and Labels */
  bool orig = false;
  bool end = false;
  uint16_t init_PC = 0;
  uint16_t temp_PC = 0;

  do {
    lRet = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      if (string_equal(*pOpcode, "br") ||
        string_equal(*pOpcode, "brp") ||
        string_equal(*pOpcode, "brz") ||
        string_equal(*pOpcode, "brzp") ||
        string_equal(*pOpcode, "brn") ||
        string_equal(*pOpcode, "brnp") ||
        string_equal(*pOpcode, "brnz") ||
        string_equal(*pOpcode, "brnzp")) {
        if (!find_label(*pArg1)) {
          #if (DEBUG)
          printf("Found undefined label: %s\n", *pArg1);
          printf("Error Code: %d\n", 4);
          #endif

          exit(4);
        }
      }

      if (string_equal(*pOpcode, "lea")) {
        if (!find_label(*pArg2)) {
          #if (DEBUG)
          printf("Found undefined label: %s\n", *pArg2);
          printf("Error Code: %d\n", 4);
          #endif

          exit(4);
        }
      }

      if (string_equal(*pOpcode, "jsr")) {
        if (!find_label(*pArg1)) {
          #if (DEBUG)
          printf("Found undefined label: %s\n", *pArg1);
          printf("Error Code: %d\n", 4);
          #endif

          exit(4);
        }
      }

      if (string_equal(*pOpcode, ".end")) {
        end = true;
      }

      if (string_equal(*pOpcode, ".orig")) {
        if (pArg1[0][1] == '-') {
          #if (DEBUG)
          printf("Error Code: %d\n", 3);
          #endif

          exit(3);
        }

        temp_PC = toNum(*pArg1);

        /* PC value cannot be odd */
        if ((temp_PC % 2) == 1) {
          #if (DEBUG)
          printf("Error Code: %d\n", 3);
          #endif

          exit(3);
        }

        init_PC = temp_PC;
        PC = init_PC;
        orig = true;
      } else {
        temp_PC += 2;
      }
    }
  } while(lRet != DONE);

  if (!orig || !end) {
    #if (DEBUG)
    printf("Was not able to find a .ORIG OR .END\n");
    #endif
    exit(0);
  }
}

void
second_pass(char* infile, char* outfile) {
  int lRet;
  FILE* input = fopen(infile, "r");
  FILE* output = fopen(outfile, "w");

  char pLine[MAX_LINE_LENGTH + 1];
  char* pLabel[MAX_LINE_LENGTH + 1];
  char* pOpcode[MAX_LINE_LENGTH + 1];
  char* pArg1[MAX_LINE_LENGTH + 1];
  char* pArg2[MAX_LINE_LENGTH + 1];
  char* pArg3[MAX_LINE_LENGTH + 1];
  char* pArg4[MAX_LINE_LENGTH + 1];

  do {
    lRet = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      if (run(output, *pLabel, *pOpcode, *pArg1, *pArg2, *pArg3, *pArg4) == FINISH) {
        break;
      }
      PC += 2;
    }
  } while(lRet != DONE);

  fclose(input);
  fclose(output);
}

void
create_all_labels(char* infile) {
  FILE* input = fopen(infile, "r");
  int lRet;

  char pLine[MAX_LINE_LENGTH + 1];
  char* pLabel[MAX_LINE_LENGTH + 1];
  char* pOpcode[MAX_LINE_LENGTH + 1];
  char* pArg1[MAX_LINE_LENGTH + 1];
  char* pArg2[MAX_LINE_LENGTH + 1];
  char* pArg3[MAX_LINE_LENGTH + 1];
  char* pArg4[MAX_LINE_LENGTH + 1];

  uint16_t init_PC = 0;
  uint16_t temp_PC = 0;
  do {
    lRet = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      if (**(pLabel) != '\0') {
        check_valid_label(*pLabel);
        add_label(*pLabel, temp_PC);
      }

      if (string_equal(*pOpcode, ".orig")) {
        if (pArg1[0][1] == '-') {
          #if (DEBUG)
          printf("Error Code: %d\n", 3);
          #endif

          exit(3);
        }

        #if (DEBUG)
        printf("Here is our PC value: 0x%x\n", toNum(*pArg1));
        #endif

        temp_PC = toNum(*pArg1);

        /* PC value cannot be odd */
        if ((temp_PC % 2) == 1) {
          #if (DEBUG)
          printf("Error Code: %d\n", 3);
          #endif

          exit(3);
        }

        init_PC = temp_PC;
        PC = init_PC;
      } else {
        temp_PC += 2;
      }
    }
  } while(lRet != DONE);

  fclose(input);
}

bool
check_valid_label(char* label) {
  if ((label[0] == 'x') ||
      isdigit(label[0])) {
    printf("Found invalid label: %s\n", label);

    #if (DEBUG)
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }
  
  for (int i = 0; i < strlen(label); i++) {
    if (!isalnum(label[i])) {
      printf("Found invalid label: %s\n", label);

      #if (DEBUG)
      printf("Error Code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  return true;
}

void
print_labels(void) {
  for (int i = 0; i < MAX_NUM_LABELS; i++) {
    if (g_labels[i].label[0] != '\0') {
      #if (DEBUG)
      printf("g_labels[%d]: %s, at address: 0x%x\n", i, g_labels[i].label, g_labels[i].addr);
      #endif
    }
  }
}

void
add_label(char* new_label, uint16_t current_PC) {
  /* If we find a pre-existing label, then label_exists will exit(4) */
  label_exists(new_label);

  for (int i = 0; i < MAX_NUM_LABELS; i++) {
    if (g_labels[i].label[0] == '\0') {
      #if (DEBUG)
      printf("Added g_labels[%d]: %s at address: 0x%x\n", i, new_label, current_PC);
      #endif

      g_labels[i].addr = current_PC;
      strcpy(g_labels[i].label, new_label);
      break;
    }
  }
}

int
check_register(char* reg) {
  if (reg[0] == 'r') {
    char* reg_copy;

    reg_copy = malloc(strlen(reg)*sizeof(char));
    strcpy(reg_copy, reg);
    reg_copy[0] = '#';

    int reg_value = toNum(reg_copy);

    free(reg_copy);

    if (reg_value > 7) {
      /* Register Value is too big */
      #if (DEBUG)
      printf("Given register value (%d) is too big!\n", reg_value);
      printf("Error Code: %d\n", 4);
      #endif

      exit(4);
    } else {
      return reg_value;
    }
  }

  return -1;
}

void
check_arguments(char* Opcode, char* argument) {
  if (*argument != '\0') {
    #if (DEBUG)
    printf("Argument (%s), passed to %s should be empty\n", argument, Opcode);
    printf("Exit code: %d\n", 1);
    #endif

    exit(1);
  }
}

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
    #if (DEBUG)
    printf("IMM5 %d is too big!\n", IMM5);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (IMM5 < (-power(2,4))) {
    /* IMM5 is smaller than the most negative value */
    #if (DEBUG)
    printf("IMM5 %d is too small!\n", IMM5);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

void
CHECK_PCOFFSET9(int16_t PC_OFFSET9) {
  if (PC_OFFSET9 > (power(2, 8) - 1)) {
    /* PC offset is greater than the max positive value */
    #if (DEBUG)
    printf("PC_OFFSET9 %d is too big!\n", PC_OFFSET9);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (PC_OFFSET9 < (-power(2,8))) {
    /* PC offset is smaller than the most negative value */
    #if (DEBUG)
    printf("PC_OFFSET9 %d is too small!\n", PC_OFFSET9);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

void
CHECK_PCOFFSET11(int16_t PC_OFFSET11) {
  if (PC_OFFSET11 > (power(2, 10) - 1)) {
    /* PC offset is greater than the max positive value */
    #if (DEBUG)
    printf("PC_OFFSET11 %d is too big!\n", PC_OFFSET11);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (PC_OFFSET11 < (-power(2,10))) {
    /* PC offset is smaller than the most negative value */
    #if (DEBUG)
    printf("PC_OFFSET11 %d is too small!\n", PC_OFFSET11);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

void
CHECK_BOFFSET6(int16_t BOFFSET6) {
  if (BOFFSET6 > (power(2, 5) - 1)) {
    /* BOFFSET6 is greater than the max positive value */
    #if (DEBUG)
    printf("BOFFSET6 %d is too big!\n", BOFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (BOFFSET6 < (-power(2,5))) {
    /* BOFFSET6 is smaller than the most negative value */
    #if (DEBUG)
    printf("BOFFSET6 %d is too small!\n", BOFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

void
CHECK_OFFSET6(int16_t OFFSET6) {
  if (OFFSET6 > (power(2, 5) - 1)) {
    /* OFFSET6 is greater than the max positive value */
    #if (DEBUG)
    printf("OFFSET6 %d is too big!\n", OFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (OFFSET6 < (-power(2,5))) {
    /* OFFSET6 is smaller than the most negative value */
    #if (DEBUG)
    printf("OFFSET6 %d is too small!\n", OFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

void
CHECK_DR(int16_t DR) { // todo: check if i need this...
  if (DR > (power(2, 5) - 1)) {
    /* DR is greater than the max positive value */
    #if (DEBUG)
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (DR < (-power(2,5))) {
    /* DR is smaller than the most negative value */
    #if (DEBUG)
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

void
CHECK_AMOUNT4(int16_t AMOUNT4) {
  if (AMOUNT4 > (power(2, 4) - 1)) {
    /* AMOUNT4 is greater than the max positive value */
    #if (DEBUG)
    printf("AMOUNT4 %d is too big!\n", AMOUNT4);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);

  } else if (AMOUNT4 < 0) {
    /* AMOUNT4 cannot be negative */
    #if (DEBUG)
    printf("AMOUNT4 %d is too small!\n", AMOUNT4);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

void
CHECK_TRAPVECT8(int16_t TRAPVECT8) {
  if (TRAPVECT8 > power(2, 8)) {
    /* TRAPVECT8 is greater than the max positive value */
    #if (DEBUG)
    printf("TRAPVECT8 %d is too big!\n", TRAPVECT8);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (TRAPVECT8 < 0) {
    /* TRAPVECT8 cannot be a negative value */
    #if (DEBUG)
    printf("TRAPVECT8 %d is too big!\n", TRAPVECT8);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

void
CHECK_FILL(int16_t FILL) {
  if (FILL > (power(2, 16) - 1)) {
    /* FILL is greater than the max positive value */
    #if (DEBUG)
    printf("FILL %d is too big!\n", FILL);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (FILL < (-power(2,15))) {
    /* FILL cannot be a negative value */
    #if (DEBUG)
    printf("FILL %d is too big!\n", FILL);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

enum run_type
run(FILE* outfile, char* Label, char* Opcode, char* arg1, char* arg2, char* arg3, char* arg4) {
  uint16_t instruction = 0;

  // todo: check for which immediates are signed, which ones are not signed...
  // todo: check whether the negative values stuff is working as intended
  // todo: check if the pcoffset, etc that the user inputs is valid
  // todo: check the specifics of boffset6
  // todo: .orig followed by negative number
  if (string_equal(Opcode, "add")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      /* This third argument tells us whether it could be an immediate or a register */
      /* todo: Should reg_value3 be able to be negative... */
      int reg_value3 = check_register(arg3);
      if (reg_value3 != -1) {
        instruction = SET_OPCODE(ADD) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(REGISTER) | SET_SR2(reg_value3);
      } else {
        int16_t IMM5 = toNum(arg3);
        CHECK_IMM5(IMM5);
        instruction = SET_OPCODE(ADD) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(IMMEDIATE) | SET_IMM5(IMM5);
      }

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
    
  }

  else if (string_equal(Opcode, "and")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      int reg_value3 = check_register(arg3);
      if(reg_value3 != -1) {
        instruction = SET_OPCODE(AND) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(REGISTER) | SET_SR2(reg_value3);
      } else {
        int16_t IMM5 = toNum(arg3);
        CHECK_IMM5(IMM5);
        printf("Here is IMM5: %d, and unsigned: %d\n", IMM5&0x1f, (uint16_t)IMM5 & 0x1f);
        instruction = SET_OPCODE(AND) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(IMMEDIATE) | SET_IMM5(IMM5);
      }

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
      printf("Exit code: %d\n", 1);
      #endif

      exit(1);
    }
  }

  else if(string_equal(Opcode, "br")) {

    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int16_t PC_OFFSET9 = ((int16_t)label->addr - (int16_t)PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      printf("Here is the label->addr: %d and PC: %d\n", label->addr, PC);
      printf("Here it is before/after formatting: %x/%x\n", PC_OFFSET9, SET_PCOFFSET9(PC_OFFSET9));
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (DEBUG)
      printf("Found an invalid operand: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "brp")) {
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int16_t PC_OFFSET9 = ((int16_t)label->addr - (int16_t)PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_OFF) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (DEBUG)
      printf("Found an invalid operand: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);

  }

  else if(string_equal(Opcode, "brz")) {

    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int16_t PC_OFFSET9 = ((int16_t)label->addr - (int16_t)PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_ON) | SET_CC_P(P_OFF) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (DEBUG)
      printf("Found an invalid operand: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);

  }

  else if(string_equal(Opcode, "brzp")) {

    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int16_t PC_OFFSET9 = ((int16_t)label->addr - (int16_t)PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (DEBUG)
      printf("Found an invalid operand: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "brn")) {
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int16_t PC_OFFSET9 = ((int16_t)label->addr - (int16_t)PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_OFF) | SET_CC_P(P_OFF) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (DEBUG)
      printf("Found an invalid operand: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "brnp")) {

    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int16_t PC_OFFSET9 = ((int16_t)label->addr - (int16_t)PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_OFF) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (DEBUG)
      printf("Found an invalid operand: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "brnz")) {

    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int16_t PC_OFFSET9 = ((int16_t)label->addr - (int16_t)PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_OFF) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (DEBUG)
      printf("Found an invalid operand: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "brnzp")) {

    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int16_t PC_OFFSET9 = ((int16_t)label->addr - (int16_t)PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (DEBUG)
      printf("Found an invalid operand: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "jmp")) {
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);

    if (!(reg_value1 < 0)) {
      instruction = SET_OPCODE(JMP) | SET_BASE_R(reg_value1);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad register input: %s!\n", arg1);
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "ret")) {
    check_arguments(Opcode, arg1);
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    instruction = SET_OPCODE(RET) | SET_BASE_R(0x7);

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "jsr")) {
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int16_t PC_OFFSET11 = ((int16_t)label->addr - (int16_t)PC) / 2;
      CHECK_PCOFFSET11(PC_OFFSET11);
      instruction = SET_OPCODE(JSR) | SET_IMM_OR_REG_B11(IMMEDIATE) | SET_PCOFFSET11(PC_OFFSET11);
    } else {
      #if (DEBUG)
      printf("Found an invalid operand: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "jsrr")) {
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);

    if (!(reg_value1 < 0)) {
      instruction = SET_OPCODE(JSRR) | SET_IMM_OR_REG_B11(REGISTER) | SET_BASE_R(reg_value1);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad register input: %s!\n", arg1);
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "ldb")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      instruction = SET_OPCODE(LDB) | SET_DR(reg_value1) | SET_BASE_R(reg_value2) | SET_BOFFSET6(toNum(arg3));

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "ldw")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      instruction = SET_OPCODE(LDW) | SET_DR(reg_value1) | SET_BASE_R(reg_value2) | SET_OFFSET6(toNum(arg3));

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "lea")) {
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    if (!(reg_value1 < 0)) {
      struct label* label = find_label(arg2);

      if (label != NULL) {
        #if (DEBUG)
        printf("These are the addresses: 0x%x 0x%x\n", label->addr, PC);
        #endif

        int16_t PC_OFFSET9 = ((int16_t)label->addr - (int16_t)PC) / 2;
        CHECK_PCOFFSET9(PC_OFFSET9);
        instruction = SET_OPCODE(LEA) | SET_DR(reg_value1) | SET_PCOFFSET9(PC_OFFSET9);
      } else {
        #if (DEBUG)
        printf("Found an invalid operand: %s!\n", arg2);
        printf("Exit code: %d\n", 4);
        #endif

        exit(4);
      }

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    }
  }

  else if(string_equal(Opcode, "rti")) {
    check_arguments(Opcode, arg1);
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    instruction = SET_OPCODE(RTI);

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "lshf")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      // todo: AMOUNT4 CANNOT BE NEGATIVE
      instruction = SET_OPCODE(LSHF) | SET_DR(reg_value1) | SET_SR(reg_value2) | SET_SHF_TYPE(LSHF_TYPE) | SET_AMOUNT4(toNum(arg3));

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "rshfl")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      instruction = SET_OPCODE(RSHFL) | SET_DR(reg_value1) | SET_SR(reg_value2) | SET_SHF_TYPE(RSHFL_TYPE) | SET_AMOUNT4(toNum(arg3));

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "rshfa")) {
    check_arguments(Opcode, arg4);
    
    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      instruction = SET_OPCODE(RSHFA) | SET_DR(reg_value1) | SET_SR(reg_value2) | SET_SHF_TYPE(RSHFA_TYPE) | SET_AMOUNT4(toNum(arg3));

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "stb")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      instruction = SET_OPCODE(STB) | SET_ST_SR(reg_value1) | SET_BASE_R(reg_value2)| SET_BOFFSET6(toNum(arg3));

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "stw")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      instruction = SET_OPCODE(STW) | SET_ST_SR(reg_value1) | SET_BASE_R(reg_value2) | SET_OFFSET6(toNum(arg3));

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "trap")) {
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    instruction = SET_OPCODE(TRAP) | SET_TRAPVECT8(toNum(arg1));

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "xor")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      int reg_value3 = check_register(arg3);
      if (reg_value3 != -1) {
        instruction = SET_OPCODE(XOR) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(REGISTER) | SET_SR2(reg_value3);
      } else {
        int16_t IMM5 = toNum(arg3);
        CHECK_IMM5(IMM5);
        instruction = SET_OPCODE(XOR) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(IMMEDIATE) | SET_IMM5(IMM5);
      }

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "not")) {
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      instruction = SET_OPCODE(NOT) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(IMMEDIATE) | SET_IMM5(0x1f);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
      printf("Exit code: %d\n", 1);
      #endif

      exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, ".orig")) {
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    instruction = toNum(arg1);

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "halt")) {
    check_arguments(Opcode, arg1);
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    instruction = HALT;
    
    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, ".fill")) {
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    int16_t FILL = toNum(arg1);
    CHECK_FILL(FILL);
    instruction = FILL;

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, ".end")) {
    check_arguments(Opcode, arg1);
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    return FINISH;
  }

  else {
    #if (DEBUG)
    printf("Invalid Operand Found: %s!\n", Opcode);
    printf("Error Code: %d\n", 2);
    #endif

    exit(2);
  }

  return NOT_FINISH;
}

void
assembler_init(FILE** infile, FILE** outfile, char* prgName, char* iFileName, char* oFileName,
               char** pLabel, char** pOpcode, char** pArg1, char** pArg2, char** pArg3, char** pArg4) {
  if (prgName == NULL ||
      iFileName == NULL ||
      oFileName == NULL) {
    #if (DEBUG)
    printf("Missing command line argument(s)!\n");
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }

  /* open the source file */
  *infile = fopen(iFileName, "r");
  *outfile = fopen(oFileName, "w");
    
  if (!(*infile)) {
    printf("Error: Cannot open file %s\n", iFileName);

    #if (DEBUG)
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }

  if (!(*outfile)) {
    printf("Error: Cannot open file %s\n", oFileName);
    #if (DEBUG)
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }

  /* Don't need to initialize the label global array everything set to 0 */
}

int
main(int argc, char* argv[]) {
  char *prgName   = argv[0];
  char *iFileName = argv[1];
  char *oFileName = argv[2];

  FILE* infile;
  FILE* outfile;

  char pLine[MAX_LINE_LENGTH + 1];
  char* pLabel[MAX_LINE_LENGTH + 1];
  char* pOpcode[MAX_LINE_LENGTH + 1];
  char* pArg1[MAX_LINE_LENGTH + 1];
  char* pArg2[MAX_LINE_LENGTH + 1];
  char* pArg3[MAX_LINE_LENGTH + 1];
  char* pArg4[MAX_LINE_LENGTH + 1];

  assembler_init(&infile, &outfile, prgName, iFileName, oFileName, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

  create_all_labels(argv[1]);
  first_pass(argv[1]);
  second_pass(argv[1], argv[2]);
}