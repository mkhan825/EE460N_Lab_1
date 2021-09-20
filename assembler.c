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
        printf("Found our label: %s\n", g_labels[i].label);
        #endif
        return &g_labels[i];
      }
    }
  }

  #if (DEBUG)
  printf("Did not find label: %s\n", label);
  #endif
  exit(4);
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
add_label(char* new_label, uint16_t PC) {
  /* If we find a pre-existing label, then label_exists will exit(4) */
  label_exists(new_label);

  for (int i = 0; i < MAX_NUM_LABELS; i++) {
    if (g_labels[i].label[0] == '\0') {
      #if (DEBUG)
      printf("Added g_labels[%d]: %s at address: 0x%x\n", i, new_label, PC);
      #endif

      g_labels[i].addr = PC;
      strcpy(g_labels[i].label, new_label);
      break;
    }
  }
}

void
convert_args(char* arg) {
  /* We can only get passed in registers or immediate values */
  /* But you can get passed in labels for LEA, JSR, JSRR, and BR */
  // if (arg[0] == 'r') {
  //   // This argument must be a register
  //   "r10"
  //   int sum = 0;
  //   for (int i = 1; i < strlen("r10"); i++) {
  //     sum += arg[i];
  //   }
  //   sum -= arg[0]];
  //   convert_str_to_num("10");
  // }
}

int
check_register(char* reg) {
  if (reg[0] == 'r') {
    char* reg_copy;

    reg_copy = malloc(strlen(reg)*sizeof(char));
    strcpy(reg_copy, reg);
    reg_copy[0] = '#';

    // rabc r101231231a ra123123123 r#123123 rx-123123 r#-12312321
    int reg_value = toNum(reg_copy);

    free(reg_copy);

    if (reg_value > 7) {
      /* Register Value is too big */
      #if (DEBUG)
      printf("Given register value (%d) is too big!\n", reg_value);
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
    #endif
    exit(1);
  }
}

enum run_type
run(FILE* outfile, char* Label, char* Opcode, char* arg1, char* arg2, char* arg3, char* arg4) {
  uint16_t instruction = 0;

  // todo: check for which immediates are signed, which ones are not signed...
  // todo: check if there is a # or x on the immediate arguments we get passed...
  // todo: check for each instruction arg1, 2, 3, 4... 
  // todo: check whether the negative values stuff is working as intended
  // todo: check if all registers accessed are proper, etc...
  // todo: check if the pcoffset, etc that the user inputs is valid
  // todo: check the specifics of boffset6 
  // todo: convert arg1.... to nums first.... but we also need to check labels
  // todo: check if we need arg4... Is checking if an argument is NULL a good check for bad input... exit?
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
        instruction = SET_OPCODE(ADD) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(IMMEDIATE) | SET_IMM5(toNum(arg3));
      }

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
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
      if(reg_value3 != -1){
        instruction = SET_OPCODE(AND) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(REGISTER) | SET_SR2(reg_value3);
      } else{
        instruction = SET_OPCODE(AND) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(IMMEDIATE) | SET_IMM5(toNum(arg3));
      }

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
        #if (DEBUG)
        printf("Bad register input: %s %s!\n", arg1, arg2);
        #endif
        exit(1); // todo: idk if this is exit(1)
    }
  }

  else if(string_equal(Opcode, "br")) {

    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label == NULL) {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_OFF) | SET_CC_P(P_OFF) | SET_PCOFFSET9(toNum(arg1));
    } else {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_OFF) | SET_CC_P(P_OFF) | SET_PCOFFSET9(label->addr - PC);
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
    if (label == NULL) {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_OFF) | SET_CC_P(P_ON) | SET_PCOFFSET9(toNum(arg1));
    } else {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_OFF) | SET_CC_P(P_ON) | SET_PCOFFSET9(label->addr - PC);
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
    if (label == NULL) {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_ON) | SET_CC_P(P_OFF) | SET_PCOFFSET9(toNum(arg1));
    } else {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_ON) | SET_CC_P(P_OFF) | SET_PCOFFSET9(label->addr - PC);
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
    if (label == NULL) {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(toNum(arg1));
    } else {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(label->addr - PC);
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
    if (label == NULL) {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_OFF) | SET_CC_P(P_OFF) | SET_PCOFFSET9(toNum(arg1));
    } else {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_OFF) | SET_CC_P(P_OFF) | SET_PCOFFSET9(label->addr - PC);
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
    if (label == NULL) {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_OFF) | SET_CC_P(P_ON) | SET_PCOFFSET9(toNum(arg1));
    } else {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_OFF) | SET_CC_P(P_ON) | SET_PCOFFSET9(label->addr - PC);
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
    if (label == NULL) {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_OFF) | SET_PCOFFSET9(toNum(arg1));
    } else {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_OFF) | SET_PCOFFSET9(label->addr - PC);
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
    if (label == NULL) {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(toNum(arg1));
    } else {
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(label->addr - PC);
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
    if (label == NULL) {
      instruction = SET_OPCODE(JSR) | SET_IMM_OR_REG_B11(IMMEDIATE) | SET_PCOFFSET11(toNum(arg1));
    } else {
      instruction = SET_OPCODE(JSR) | SET_IMM_OR_REG_B11(IMMEDIATE) | SET_PCOFFSET11(label->addr - PC);
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
      if (label == NULL) {
        instruction = SET_OPCODE(LEA) | SET_DR(reg_value1) | SET_PCOFFSET9(toNum(arg2));
      } else {
        instruction = SET_OPCODE(LEA) | SET_DR(reg_value1) | SET_PCOFFSET9(label->addr - PC);
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
      instruction = SET_OPCODE(LSHF) | SET_DR(reg_value1) | SET_SR(reg_value2) | SET_SHF_TYPE(LSHF_TYPE) | SET_AMOUNT4(toNum(arg3));

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
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
    }else {
      #if (DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
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
        instruction = SET_OPCODE(XOR) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(IMMEDIATE) | SET_IMM5(toNum(arg3));
      }

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
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

    // todo: lots of edge cases with negative numbers
    instruction = toNum(arg1);

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
    #endif
    // todo: exit here?!
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
    #endif
    exit(4);
  }

  /* open the source file */
  *infile = fopen(iFileName, "r");
  *outfile = fopen(oFileName, "w");
    
  if (!(*infile)) {
    printf("Error: Cannot open file %s\n", iFileName);
    exit(4);
  }

  if (!(*outfile)) {
    printf("Error: Cannot open file %s\n", oFileName);
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

  int lRet;

  /* Do the First Pass and check for .ORIG/.END and Labels */
  bool orig = false;
  bool end = false;
  uint16_t temp_PC = 0;
  do {
    lRet = readAndParse(infile, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      if (**(pLabel) != '\0') {
        add_label(*pLabel, temp_PC);
      }

      if (string_equal(*pOpcode, ".end")) {
        end = true;
      }

      if (string_equal(*pOpcode, ".orig")) {
        #if (DEBUG)
        printf("Here is our PC value: 0x%x\n", toNum(*pArg1));
        #endif

        temp_PC = toNum(*pArg1);
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

  rewind(infile);

  do {
    lRet = readAndParse(infile, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      if (run(outfile, *pLabel, *pOpcode, *pArg1, *pArg2, *pArg3, *pArg4) == FINISH) {
        break;
      }
      PC += 2;
    }
  } while(lRet != DONE);

  fclose(infile);
  fclose(outfile);
}