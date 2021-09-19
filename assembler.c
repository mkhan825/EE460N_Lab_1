#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */
#include <stdbool.h>
#include "assembler.h"

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
  if (string_equal(*pOpcode, ".end")) {
    printf("We found .end!\n");
  }
  printf("%s\n", *pOpcode);

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
          string_equal(str, "brnp") ||
          string_equal(str, "brnz") ||
          string_equal(str, "brnzp") ||
          string_equal(str, "brzp")) {
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

  /* String is not an Opcode */
  return false;
}

struct label g_labels[MAX_NUM_LABELS];

void
label_exists(char* label) {
  for (int i = 0; i < MAX_NUM_LABELS; i++) {
    if (g_labels[i].label != NULL) {
      if (string_equal(g_labels[i].label, label)) {
        printf("Found a label that is already init-ed\n");
        exit(4);
      }
    }
  }
}

void
add_label(char* new_label, uint16_t* PC) {
  /* If we find a pre-existing label, then label_exists will exit(4) */
  label_exists(new_label);

  for (int i = 0; i < MAX_NUM_LABELS; i++) {
    if (g_labels[i].label != NULL) {
      g_labels->addr = *PC;
      strcpy(g_labels->label, new_label);
      break;
    }
  }
}

void
valid_register() {

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

    int reg_value = toNum(reg_copy);

    free(reg_copy);

    if (reg_value > 7) {
      return -1;
    } else {
      return reg_value;
    }
  }

  return -1;
}

void
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
    if (arg4 != NULL) {
      // todo: maybe do an exit here...
      printf("arg4 is not NULL\n");
      exit(1);
    }

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);
    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      /* This third argument tells us whether it could be an immediate or a register */
      /* todo: Should reg_value3 be able to be negative... */
      int reg_value3 = check_register(arg3);
      printf("reg_3: %d\n", reg_value3);
      if (reg_value3 != -1) {
        instruction = SET_OPCODE(ADD) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(REGISTER) | SET_SR2(reg_value3);
      } else {
        printf("We made it heere!! Bit_5: %d\n", SET_IMM_OR_REG_B5(1));
        instruction = SET_OPCODE(ADD) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(IMMEDIATE) | SET_IMM5(toNum(arg3));
        printf("tonum: %d\n", toNum(arg3));
      }
    }
    printf("0x%x\n", instruction);
    fprintf(outfile, "0x%x\n", instruction);

  }

  else if (string_equal(Opcode, "and")) {
    if(check_register(arg3)){
      instruction = SET_OPCODE(AND) | SET_DR(toNum(arg1)) | SET_SR1(arg2) | SET_IMM_OR_REG_B5(0) | SET_SR2(arg3);
    } else{
      instruction = SET_OPCODE(AND) | SET_DR(toNum(arg1)) | SET_SR1(arg2) | SET_IMM_OR_REG_B5(1) | SET_IMM5(arg3);
    }
  }

  else if(string_equal(Opcode, "br")) {
    instruction = SET_OPCODE(BR) | SET_CC_N(0) | SET_CC_Z(0) | SET_CC_P(0);
  }

  else if(string_equal(Opcode, "brp")) {
    instruction = SET_OPCODE(BR) | SET_CC_N(0) | SET_CC_Z(0) | SET_CC_P(1);
  }

  else if(string_equal(Opcode, "brz")) {
    instruction = SET_OPCODE(BR) | SET_CC_N(0) | SET_CC_Z(1) | SET_CC_P(0);
  }

  else if(string_equal(Opcode, "brzp")) {
    instruction = SET_OPCODE(BR) | SET_CC_N(0) | SET_CC_Z(1) | SET_CC_P(1);
  }

  else if(string_equal(Opcode, "brn")) {
    instruction = SET_OPCODE(BR) | SET_CC_N(1) | SET_CC_Z(0) | SET_CC_P(0);
  }

  else if(string_equal(Opcode, "brnp")) {
    instruction = SET_OPCODE(BR) | SET_CC_N(1) | SET_CC_Z(0) | SET_CC_P(1);
  }

  else if(string_equal(Opcode, "brnz")) {
    instruction = SET_OPCODE(BR) | SET_CC_N(1) | SET_CC_Z(1) | SET_CC_P(0);
  }

  else if(string_equal(Opcode, "brnzp")) {
    instruction = SET_OPCODE(BR) | SET_CC_N(1) | SET_CC_Z(1) | SET_CC_P(1);
  }

  else if(string_equal(Opcode, "jmp")) {
    instruction = SET_OPCODE(JMP) | SET_BASE_R(toNum(arg1));
  }

  else if(string_equal(Opcode, "ret")) {
    instruction = SET_OPCODE(RET) | SET_BASE_R(0x7);
  }

  else if(string_equal(Opcode, "jsr")) {
    instruction = SET_OPCODE(JSR) | SET_IMM_OR_REG_B11(1) | SET_PCOFFSET11(toNum(arg1));
  }

  else if(string_equal(Opcode, "jsrr")) {
    instruction = SET_OPCODE(JSRR) | SET_IMM_OR_REG_B11(0) | SET_BASE_R(toNum(arg1));
  }

  else if(string_equal(Opcode, "ldb")){
    instruction = SET_OPCODE(LDB) | SET_DR(toNum(arg1)) | SET_BASE_R(arg2) | SET_BOFFSET6(arg3);
  }

  else if(string_equal(Opcode, "ldw")){
    instruction = SET_OPCODE(LDW) | SET_DR(toNum(arg1)) | SET_BASE_R(arg2) | SET_OFFSET6(arg3);
  }

  else if(string_equal(Opcode, "lea")){
    instruction = SET_OPCODE(LEA) | SET_DR(toNum(arg1)) | SET_PCOFFSET9(arg2);
  }

  else if(string_equal(Opcode, "rti")){
    instruction = SET_OPCODE(RTI);
  }

  else if(string_equal(Opcode, "lshf") {
    instruction = SET_OPCODE(LSHF) | SET_DR(toNum(arg1)) | SET_SR(arg2) | SET_SHF_TYPE(0) | SET_AMOUNT4(arg3);
  }

  else if(string_equal(Opcode, "rshfl") {
    instruction = SET_OPCODE(RSHFL) | SET_DR(toNum(arg1)) | SET_SR(arg2) | SET_SHF_TYPE(1) | SET_AMOUNT4(arg3);
  }

  else if(string_equal(Opcode, "rshfa") {
    instruction = SET_OPCODE(RSHFA) | SET_DR(toNum(arg1)) | SET_SR(arg2) | SET_SHF_TYPE(3) | SET_AMOUNT4(arg3);
  }

  else if(string_equal(Opcode, "stb")){
    instruction = SET_OPCODE(STB) | SET_ST_SR(toNum(arg1)) | SET_BASE_R(arg2) | SET_BOFFSET6(arg3);
  }

  else if(string_equal(Opcode, "stw")){
    instruction = SET_OPCODE(STW) | SET_ST_SR(toNum(arg1)) | SET_BASE_R(arg2) | SET_OFFSET6(arg3);
  }

  else if(string_equal(Opcode, "trap")){
    instruction = SET_OPCODE(TRAP) | SET_TRAPVECT8(toNum(arg1));
  }

  else if(string_equal(Opcode, "xor")) {
    if(check_register(arg3)){
      instruction = SET_OPCODE(XOR) | SET_DR(toNum(arg1)) | SET_SR1(arg2) | SET_IMM_OR_REG_B5(0) | SET_SR2(arg3);
    } else{
      instruction = SET_OPCODE(XOR) | SET_DR(toNum(arg1)) | SET_SR1(arg2) | SET_IMM_OR_REG_B5(1) | SET_IMM5(arg3);
    }
  }

  else if(string_equal(Opcode, "not")) {
    instruction = SET_OPCODE(NOT) | SET_DR(toNum(arg1)) | SET_SR1(arg2) | SET_IMM_OR_REG_B5(1) | SET_SR2(0x1f);
  }

  else {
    printf("Invalid Operand!\n");
    // todo: exit here?!
  }
}

void
assembler_init(FILE** infile, FILE** outfile, char* prgName, char* iFileName, char* oFileName,
               char** pLine, char*** pLabel, ) {
  if (prgName == NULL ||
      iFileName == NULL ||
      oFileName == NULL) {
    printf("Missing command line argument(s)!\n");
    exit(4);
  }

  #if (DEBUG)
  printf("program name = '%s'\n", prgName);
  printf("input file name = '%s'\n", iFileName);
  printf("output file name = '%s'\n", oFileName);
  #endif

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

  assembler_init(&infile, &outfile, prgName, iFileName, oFileName, &pLine, &pLabel, &pOpcode, &pArg1, &pArg2, &pArg3, &pArg4);

  //maybe we change tonum function
  // printf("-1 toNum: %d\n", toNum("x-1"));

  int lRet;

  /* Do the First Pass and check for .ORIG/.END and Labels */
  bool orig = false;
  bool end = false;
  do {
    lRet = readAndParse(infile, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      // do stuff and write to out file
      if (string_equal(*pOpcode, ".orig")) {
        orig = true;
      }
      if (string_equal(*pOpcode, ".end")) {
        printf("We got into this if!\n");
        end = true;
      }
      // printf("%s\n", *pOpcode);
    }
  } while(lRet != DONE);

  if (!orig || !end) {
    #if (DEBUG)
    printf("Was not able to find a .ORIG OR .END\n");
    #endif
    // exit(0); // todo: figure out why it can't detect .end...
  }

  rewind(infile);

  do {
    lRet = readAndParse(infile, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      // do stuff and write to out file
      run(outfile, *pLabel, *pOpcode, *pArg1, *pArg2, *pArg3, *pArg4);
    }
  } while(lRet != DONE);

  fclose(infile);
  fclose(outfile);
}