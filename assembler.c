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
#include <string.h>
#include <stdint.h>

/***************************************************************************************/
/*                                                                                     */
/*                               DEFINES/MACROS/ENUMS                                  */
/*                                                                                     */
/***************************************************************************************/

#define DEBUG 1
#define TEST 0

enum
{
  DONE, OK, EMPTY_LINE
};

enum run_type {
  FINISH,
  NOT_FINISH,
};

/* Note: MAX_LINE_LENGTH, OK, EMPTY_LINE, and DONE are defined values */
#define MAX_LINE_LENGTH 255
#define MAX_NUM_LABELS 255
#define MAX_LABEL_LEN 21

#define address uint16_t

// Labels
struct label {
	address addr;
	char label[MAX_LABEL_LEN];
};

// Opcodes
enum opcode {
  ADD = 0x1,
  AND = 0x5,

  BR = 0x0,
  BRP = 0x0,
  BRZ = 0x0,
  BRZP = 0x0,
  BRN = 0x0,
  BRNP = 0x0,
  BRNZ = 0x0,
  BRNZP = 0x0,

  JMP = 0xC,
  RET = 0xC,

  JSR = 0x4,
  JSRR = 0x4,

  LDB = 0x2,
  LDW = 0x6,
  LEA = 0xE,
  RTI = 0x8,

  LSHF = 0xD,
  RSHFL = 0xD,
  RSHFA = 0xD,

  STB = 0x3,
  STW = 0x7,
  TRAP = 0xF,

  XOR = 0x9,
  NOT = 0x9,

  INVALID = 0xFF,
};

#define string_equal(str1, str2) ((bool)(strcmp(str1, str2) == 0))

// Getters
#define GET_DR(x) ((((uint16_t)x) & 0x0e00) >> 9)
#define GET_SR1(x) ((((uint16_t)x) & 0x01c0) >> 6)
#define GET_BASE_R(x) ((((uint16_t)x) & 0x01c0) >> 6)

#define GET_IMM_OR_REG(x) ((((uint16_t)x) & 0x0020) >> 5)

#define GET_CC_N(x) ((((uint16_t)x) & 0x0800) >> 11)
#define GET_CC_Z(x) ((((uint16_t)x) & 0x0400) >> 10)
#define GET_CC_P(x) ((((uint16_t)x) & 0x0200) >> 9)

#define GET_PCoffset9(x) (((uint16_t)x) & 0x01ff)

//Setters
#define SET_OPCODE(op) ((((uint16_t)op) & 0xf) << 12)

#define SET_DR(dr) ((((uint16_t)dr) & 0x7) << 9)
#define SET_SR(sr) ((((uint16_t)sr) & 0x7) << 6)
#define SET_ST_SR(st_sr) ((((uint16_t)st_sr) & 0x7) << 9)
#define SET_SR1(sr1) ((((uint16_t)sr1) & 0x7) << 6)
#define SET_SR2(sr2) ((((uint16_t)sr2) & 0x7) << 0)
#define SET_BASE_R(base_r) ((((uint16_t)base_r) & 0x7) << 6)

#define SET_IMM_OR_REG_B5(x) ((((uint16_t)x) & 0x1) << 5)
#define SET_IMM_OR_REG_B11(x) ((((uint16_t)x) & 0x1) << 11)
#define SET_SHF_TYPE(x) ((((uint16_t)x) & 0x3) << 4)

#define SET_IMM5(imm5) ((((uint16_t)imm5) & 0x1f) << 0)
#define SET_AMOUNT4(amount4) ((((uint16_t)amount4) & 0xf) << 0)

#define SET_PCOFFSET9(pcoffset9) ((((uint16_t)pcoffset9) & 0x1ff) << 0)
#define SET_PCOFFSET11(pcoffset11) ((((uint16_t)pcoffset11) & 0x7ff) << 0)
#define SET_BOFFSET6(boffset6) ((((uint16_t)boffset6) & 0x3f) << 0)
#define SET_OFFSET6(offset6) ((((uint16_t)offset6) & 0x3f) << 0)

#define SET_CC_N(cc_n) ((((uint16_t)cc_n) & 0x1) << 11)
#define SET_CC_Z(cc_z) ((((uint16_t)cc_z) & 0x1) << 10)
#define SET_CC_P(cc_p) ((((uint16_t)cc_p) & 0x1) << 9)

#define SET_TRAPVECT8(trapvect8) ((((uint16_t)trapvect8) & 0xff) << 0)

// Magic Numbers
#define REGISTER 0
#define IMMEDIATE 1

#define N_ON 1
#define Z_ON 1
#define P_ON 1

#define N_OFF 0
#define Z_OFF 0
#define P_OFF 0

#define LSHF_TYPE 0
#define RSHFL_TYPE 1
#define RSHFA_TYPE 3

#define HALT 0xF025

/***************************************************************************************/
/*                                                                                     */
/*                               FUNCTION PROTOTYPES                                   */
/*                                                                                     */
/***************************************************************************************/

int
toNum(char* pStr);

int
readAndParse(FILE* pInfile, char* pLine, char** pLabel,
             char** pOpcode, char** pArg1, char** pArg2,
             char** pArg3, char** pArg4);

bool
isOpcode(char* str);

bool
check_valid_label(char* label);

void
add_label(char* new_label, uint16_t current_PC);

enum run_type
run(FILE* outfile, uint16_t* PC, char* Label, char* Opcode, char* arg1, char* arg2, char* arg3, char* arg4);

int
check_register(char* reg);

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
  if (isOpcode(*pArg1)) {
    #if (TEST || DEBUG)
    printf("'%s' cannot be an argument!\n", *pArg1);
    printf("Exit code: 4\n");
    #endif

    exit(4);
  }
    
  if(!(lPtr = strtok(NULL, "\t\n ,"))) {
    return( OK );
  }

  *pArg2 = lPtr;
  if (isOpcode(*pArg2)) {
    #if (TEST || DEBUG)
    printf("'%s' cannot be an argument!\n", *pArg2);
    printf("Exit code: 4\n");
    #endif

    exit(4);
  }

  if(!(lPtr = strtok(NULL, "\t\n ,"))) {
    return( OK );
  }

  *pArg3 = lPtr;
  if (isOpcode(*pArg3)) {
    #if (TEST || DEBUG)
    printf("'%s' cannot be an argument!\n", *pArg3);
    printf("Exit code: 4\n");
    #endif

    exit(4);
  }

  if(!(lPtr = strtok(NULL, "\t\n ,"))) {
    return( OK );
  }

  *pArg4 = lPtr;
  if (isOpcode(*pArg4)) {
    #if (TEST || DEBUG)
    printf("'%s' cannot be an argument!\n", *pArg4);
    printf("Exit code: 4\n");
    #endif

    exit(4);
  }

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
        #if (TEST || DEBUG)
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
        #if (TEST || DEBUG)
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
    #if (TEST || DEBUG)
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }
}

bool
isOpcode(char* str) {
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
  else if (string_equal(str, "nop")) {
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
        #if (TEST || DEBUG)
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
  return false;
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
  uint32_t init_PC = 0;
  uint32_t temp_PC = 0;

  do {
    lRet = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      if (string_equal(*pOpcode, ".end")) {
        /* If there is a label next to end */
        if (*pLabel[0] != '\0') {
          #if (DEBUG)
          printf("There is a label (%s) next to .end\n", *pLabel);
          printf("Exit code: 4\n");
          #endif

          exit(4);
        }

        end = true;
      }

      if (string_equal(*pOpcode, ".orig")) {
        /* If there is a label next to orig */
        if (*pLabel[0] != '\0') {
          #if (DEBUG)
          printf("There is a label (%s) next to .orig\n", *pLabel);
          printf("Exit code: 4\n");
          #endif

          exit(4);
        }
        /* PC value cannot be negative */
        if (pArg1[0][1] == '-') {
          #if (TEST || DEBUG)
          printf("PC value cannot be negative!\n");
          printf("Error Code: %d\n", 3);
          #endif

          exit(3);
        }

        temp_PC = toNum(*pArg1);

        /* PC value cannot be odd */
        if ((temp_PC % 2) == 1) {
          #if (TEST || DEBUG)
          printf("Error Code: %d\n", 3);
          #endif

          exit(3);
        }

        /* PC value being out of bounds */
        if (temp_PC > 0xFFFF) {
          #if (TEST || DEBUG)
          printf(".ORIG PC value is greater than 0xFFFF");
          printf("Exit code: 3\n");
          #endif

          exit(3);
        }

        init_PC = temp_PC;
        orig = true;
      } else {
        temp_PC += 2;
      }

      if (orig && !(end)) {
        if (string_equal(*pOpcode, "br") ||
          string_equal(*pOpcode, "brp") ||
          string_equal(*pOpcode, "brz") ||
          string_equal(*pOpcode, "brzp") ||
          string_equal(*pOpcode, "brn") ||
          string_equal(*pOpcode, "brnp") ||
          string_equal(*pOpcode, "brnz") ||
          string_equal(*pOpcode, "brnzp")) {
          if (!find_label(*pArg1)) {
            if (pArg1[0][0] == '\0') {
              #if (TEST || DEBUG)
              printf("Missing label, bad syntax: %s\n", *pArg2);
              printf("Error Code: %d\n", 4);
              #endif

              exit(4);
            }

            if (pArg1[0][0] == 'x' ||
                pArg1[0][0] == '#') {
              #if (TEST || DEBUG)
              printf("Invalid syntax of br instructions: %s\n", *pArg1);
              printf("Error Code: %d\n", 4);
              #endif

              exit(4);
            }

            #if (TEST || DEBUG)
            printf("Found undefined label: %s\n", *pArg1);
            printf("Error Code: %d\n", 1);
            #endif

            exit(1);
          }
        }

        if (string_equal(*pOpcode, "lea")) {
          if (!find_label(*pArg2)) {
            if (pArg1[0][0] == '\0') {
              #if (TEST || DEBUG)
              printf("Missing register, bad syntax: %s\n", *pArg2);
              printf("Error Code: %d\n", 4);
              #endif

              exit(4);
            }

            if (pArg1[0][0] == 'x' ||
                pArg1[0][0] == '#') {
              #if (TEST || DEBUG)
              printf("Invalid syntax of lea instructions: %s\n", *pArg2);
              printf("Error Code: %d\n", 4);
              #endif

              exit(4);
            }

            if (pArg2[0][0] == 'x' ||
                pArg2[0][0] == '#') {
              #if (TEST || DEBUG)
              printf("Invalid syntax of lea instructions: %s\n", *pArg2);
              printf("Error Code: %d\n", 4);
              #endif

              exit(4);
            }

            if (*pArg2[0] == '\0') {
              #if (TEST || DEBUG)
              printf("Did not find a label: %s\n", *pArg2);
              printf("Error Code: %d\n", 4);
              #endif

              exit(4);
            }

            #if (TEST || DEBUG)
            printf("Found undefined label: %s\n", *pArg2);
            printf("Error Code: %d\n", 1);
            #endif

            exit(1);
          }
        }

        if (string_equal(*pOpcode, "jsr")) {
          if (!find_label(*pArg1)) {
            if (pArg1[0][0] == '\0') {
              #if (TEST || DEBUG)
              printf("Missing label, bad syntax: %s\n", *pArg2);
              printf("Error Code: %d\n", 4);
              #endif

              exit(4);
            }

            if (pArg1[0][0] == 'x' ||
                pArg1[0][0] == '#') {
              #if (TEST || DEBUG)
              printf("Invalid syntax of jsr instructions: %s\n", *pArg1);
              printf("Error Code: %d\n", 4);
              #endif

              exit(4);
            }

            #if (TEST || DEBUG)
            printf("Found undefined label: %s\n", *pArg1);
            printf("Error Code: %d\n", 1);
            #endif

            exit(1);
          }
        }

        if (*pLabel[0] != '\0') {
          if (!search_label(infile, *pLabel)) {
            if (!isOpcode(*pOpcode)) {
              #if (TEST || DEBUG)
              printf("Found invalid opcode: %s\n", *pOpcode);
              printf("Error Code: %d\n", 2);
              #endif

              exit(2);
            }

            // #if (TEST || DEBUG)
            // printf("Found unused label: %s\n", *pLabel);
            // printf("Error Code: %d\n", 4);
            // #endif

            // exit(4);
          }
        }
      }
    }
  } while(lRet != DONE);

  if (!orig || !end) {
    #if (TEST || DEBUG)
    printf("Was not able to find a .ORIG OR .END\n");
    printf("Exit code: 4\n");
    #endif

    exit(4);
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

  uint16_t PC = 0;

  bool orig = false;
  bool end = false;
  do {
    lRet = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      if (string_equal(*pOpcode, ".orig")) {
        PC = toNum(*pArg1);
        orig = true;
      }

      if (string_equal(*pOpcode, ".end")) {
        end = true;
        break;
      }

      if (orig && !(end)) {
        if (run(output, &PC, *pLabel, *pOpcode, *pArg1, *pArg2, *pArg3, *pArg4) == FINISH) {
          break;
        }
        
        PC += 2;

        if (PC == 0) {
          #if (DEBUG)
          printf("PC overflowed!\n");
          printf("Exit code: 4\n");
          #endif

          exit(4);
        }
      }
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

  uint32_t init_PC = 0;
  uint32_t temp_PC = 0;

  bool orig = false;
  bool end = false;
  do {
    lRet = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      if (string_equal(*pOpcode, ".end")) {
        end = true;
      }

      if (orig && !(end)) {
        if (**(pLabel) != '\0') {
          if (*pOpcode[0] != '\0') {
            check_valid_label(*pLabel);
            add_label(*pLabel, temp_PC);
          } else {
            // todo: not sure about
            // #if (TEST || DEBUG)
            // printf("Labels must be associated with Opcodes!\n");
            // printf("Error Code: %d\n", 4);
            // #endif

            // exit(4);
          }
        }
      }

      if (string_equal(*pOpcode, ".orig")) {
        /* PC value cannot be negative */
        if (pArg1[0][1] == '-') {
          #if (TEST || DEBUG)
          printf("PC value cannot be negative!\n");
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
          #if (TEST || DEBUG)
          printf("PC value cannot be odd!\n");
          printf("Error Code: %d\n", 3);
          #endif

          exit(3);
        }

        /* PC value being out of bounds */
        if (temp_PC > 0xFFFF) {
          #if (TEST || DEBUG)
          printf(".ORIG PC value is greater than 0xFFFF\n");
          printf("Exit code: 3\n");
          #endif

          exit(3);
        }

        orig = true;
        init_PC = temp_PC;
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
    #if (TEST || DEBUG)
    printf("Found invalid label: %s\n", label);
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }
  
  for (int i = 0; i < strlen(label); i++) {
    if (!isalnum(label[i])) {
      #if (TEST || DEBUG)
      printf("Found invalid label: %s\n", label);
      printf("Error Code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  if (string_equal(label, "getc") ||
      string_equal(label, "in") ||
      string_equal(label, "out") ||
      string_equal(label, "puts")) {
    #if (TEST || DEBUG)
    printf("Trap instructions cannot be labels: %s\n", label);
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }

  if (isOpcode(label)) {
    #if (TEST || DEBUG)
    printf("Opcodes cannot be labels: %s\n", label);
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }

  if (label[0] == 'r') {
    bool all_digits = true;
    for (int i = 1; i < strlen(label); i++) {
      if (!isdigit(label[i])) {
        all_digits = false;
        break;
      }
    }

    /* All of the characters after r were numbers */
    if (all_digits) {
      char temp[MAX_LABEL_LEN];
      strcpy(temp, label);
      temp[0] = '#';

      /* If the number after r is less than 7, */
      if (!(toNum(temp) > 7)) {
        #if (DEBUG)
        printf("You cannot have registers as labels!\n");
        printf("Exit code: 4\n");
        #endif

        exit(4);
      }
    }
  }

  // if (check_register(label) != -1) {
  //   #if (TEST || DEBUG)
  //   printf("Labels cannot be registers: %s\n", label);
  //   printf("Error Code: %d\n", 4);
  //   #endif

  //   exit(4);
  // }

  if (strlen(label) > 20) {
    #if (TEST || DEBUG)
    printf("Labels cannot be longer than 20 chars: %s\n", label);
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
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
      #if (TEST || DEBUG)
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
    #if (TEST || DEBUG)
    printf("Argument (%s), passed to %s should be empty\n", argument, Opcode);
    printf("Exit code: %d\n", 4);
    #endif

    exit(4);
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
    #if (TEST || DEBUG)
    printf("IMM5 %d is too big!\n", IMM5);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (IMM5 < (-power(2,4))) {
    /* IMM5 is smaller than the most negative value */
    #if (TEST || DEBUG)
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
    #if (TEST || DEBUG)
    printf("PC_OFFSET9 %d is too big!\n", PC_OFFSET9);
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  } else if (PC_OFFSET9 < (-power(2,8))) {
    /* PC offset is smaller than the most negative value */
    #if (TEST || DEBUG)
    printf("PC_OFFSET9 %d is too small!\n", PC_OFFSET9);
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
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

    exit(4);
  } else if (PC_OFFSET11 < (-power(2,10))) {
    /* PC offset is smaller than the most negative value */
    #if (TEST || DEBUG)
    printf("PC_OFFSET11 %d is too small!\n", PC_OFFSET11);
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
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

    exit(3);
  } else if (BOFFSET6 < (-power(2,5))) {
    /* BOFFSET6 is smaller than the most negative value */
    #if (TEST || DEBUG)
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
    #if (TEST || DEBUG)
    printf("OFFSET6 %d is too big!\n", OFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (OFFSET6 < (-power(2,5))) {
    /* OFFSET6 is smaller than the most negative value */
    #if (TEST || DEBUG)
    printf("OFFSET6 %d is too small!\n", OFFSET6);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
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

    exit(3);

  } else if (AMOUNT4 < 0) {
    /* AMOUNT4 cannot be negative */
    #if (TEST || DEBUG)
    printf("AMOUNT4 %d cannot be negative!\n", AMOUNT4);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
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

    exit(3);
  } else if (TRAPVECT8 < 0) {
    /* TRAPVECT8 cannot be a negative value */
    #if (TEST || DEBUG)
    printf("TRAPVECT8 %d cannot be negative!\n", TRAPVECT8);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

void
CHECK_FILL(int32_t FILL) {
  if (FILL > (power(2, 16) - 1)) {
    /* FILL is greater than the max positive value */
    #if (TEST || DEBUG)
    printf("FILL %d is too big!\n", FILL);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  } else if (FILL < (-power(2,15))) {
    /* FILL cannot be a negative value */
    #if (TEST || DEBUG)
    printf("FILL %d is too small!\n", FILL);
    printf("Error Code: %d\n", 3);
    #endif

    exit(3);
  }
}

enum run_type
run(FILE* outfile, uint16_t* PC, char* Label, char* Opcode, char* arg1, char* arg2, char* arg3, char* arg4) {
  uint16_t instruction = 0;

  if (string_equal(Opcode, "add")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      /* This third argument tells us whether it could be an immediate or a register */
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
      
      // @Wenchi @95: "is probably fine"
      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (TEST || DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
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
        // printf("Here is IMM5: %d, and unsigned: %d\n", IMM5&0x1f, (uint16_t)IMM5 & 0x1f);
        instruction = SET_OPCODE(AND) | SET_DR(reg_value1) | SET_SR1(reg_value2) | SET_IMM_OR_REG_B5(IMMEDIATE) | SET_IMM5(IMM5);
      }

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (TEST || DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  else if(string_equal(Opcode, "br")) {

    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    struct label* label = find_label(arg1);
    if (label != NULL) {
      int32_t PC_OFFSET9 = (label->addr - *PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);

      #if (DEBUG)
      printf("Here is the label->addr: %d and PC: %d\n", label->addr, *PC);
      printf("Here it is before/after formatting: %x/%x\n", PC_OFFSET9, SET_PCOFFSET9(PC_OFFSET9));
      #endif

      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (TEST || DEBUG)
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
      int32_t PC_OFFSET9 = (label->addr - *PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_OFF) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (TEST || DEBUG)
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
      int32_t PC_OFFSET9 = (label->addr - *PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_ON) | SET_CC_P(P_OFF) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (TEST || DEBUG)
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
      int32_t PC_OFFSET9 = (label->addr - *PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_OFF) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (TEST || DEBUG)
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
      int32_t PC_OFFSET9 = (label->addr - *PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_OFF) | SET_CC_P(P_OFF) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (TEST || DEBUG)
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
      int32_t PC_OFFSET9 = (label->addr - *PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_OFF) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (TEST || DEBUG)
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
      int32_t PC_OFFSET9 = (label->addr - *PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_OFF) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (TEST || DEBUG)
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
      int32_t PC_OFFSET9 = (label->addr - *PC) / 2;
      CHECK_PCOFFSET9(PC_OFFSET9);
      instruction = SET_OPCODE(BR) | SET_CC_N(N_ON) | SET_CC_Z(Z_ON) | SET_CC_P(P_ON) | SET_PCOFFSET9(PC_OFFSET9);
    } else {
      #if (TEST || DEBUG)
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
      #if (TEST || DEBUG)
      printf("Bad register input: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
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
      int32_t PC_OFFSET11 = (label->addr - *PC) / 2;
      CHECK_PCOFFSET11(PC_OFFSET11);
      instruction = SET_OPCODE(JSR) | SET_IMM_OR_REG_B11(IMMEDIATE) | SET_PCOFFSET11(PC_OFFSET11);
    } else {
      #if (TEST || DEBUG)
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
      #if (TEST || DEBUG)
      printf("Bad register input: %s!\n", arg1);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  else if(string_equal(Opcode, "ldb")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      int16_t BOFFSET6 = toNum(arg3);
      CHECK_BOFFSET6(BOFFSET6);
      instruction = SET_OPCODE(LDB) | SET_DR(reg_value1) | SET_BASE_R(reg_value2) | SET_BOFFSET6(BOFFSET6);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (TEST || DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  else if(string_equal(Opcode, "ldw")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      int16_t OFFSET6 = toNum(arg3);
      CHECK_OFFSET6(OFFSET6);
      instruction = SET_OPCODE(LDW) | SET_DR(reg_value1) | SET_BASE_R(reg_value2) | SET_OFFSET6(OFFSET6);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (TEST || DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
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
        printf("These are the addresses: 0x%x 0x%x\n", label->addr, *PC);
        #endif

        int32_t PC_OFFSET9 = (label->addr - *PC) / 2;
        CHECK_PCOFFSET9(PC_OFFSET9);
        instruction = SET_OPCODE(LEA) | SET_DR(reg_value1) | SET_PCOFFSET9(PC_OFFSET9);
      } else {
        #if (TEST || DEBUG)
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
      int16_t AMOUNT4 = toNum(arg3);
      CHECK_AMOUNT4(AMOUNT4);
      instruction = SET_OPCODE(LSHF) | SET_DR(reg_value1) | SET_SR(reg_value2) | SET_SHF_TYPE(LSHF_TYPE) | SET_AMOUNT4(AMOUNT4);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (TEST || DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  else if(string_equal(Opcode, "rshfl")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      int16_t AMOUNT4 = toNum(arg3);
      CHECK_AMOUNT4(AMOUNT4);
      instruction = SET_OPCODE(RSHFL) | SET_DR(reg_value1) | SET_SR(reg_value2) | SET_SHF_TYPE(RSHFL_TYPE) | SET_AMOUNT4(AMOUNT4);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (TEST || DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  else if(string_equal(Opcode, "rshfa")) {
    check_arguments(Opcode, arg4);
    
    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      int16_t AMOUNT4 = toNum(arg3);
      CHECK_AMOUNT4(AMOUNT4);
      instruction = SET_OPCODE(RSHFA) | SET_DR(reg_value1) | SET_SR(reg_value2) | SET_SHF_TYPE(RSHFA_TYPE) | SET_AMOUNT4(AMOUNT4);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (TEST || DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  else if(string_equal(Opcode, "stb")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      int16_t BOFFSET6 = toNum(arg3);
      CHECK_BOFFSET6(BOFFSET6);
      instruction = SET_OPCODE(STB) | SET_ST_SR(reg_value1) | SET_BASE_R(reg_value2)| SET_BOFFSET6(BOFFSET6);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (TEST || DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  else if(string_equal(Opcode, "stw")) {
    check_arguments(Opcode, arg4);

    int reg_value1 = check_register(arg1);
    int reg_value2 = check_register(arg2);

    if (!((reg_value1 < 0) || (reg_value2 < 0))) {
      int16_t OFFSET6 = toNum(arg3);
      CHECK_OFFSET6(OFFSET6);
      instruction = SET_OPCODE(STW) | SET_ST_SR(reg_value1) | SET_BASE_R(reg_value2)| SET_OFFSET6(OFFSET6);

      #if (DEBUG)
      printf("0x%4X\n", instruction);
      #endif

      fprintf(outfile, "0x%04X\n", instruction);
    } else {
      #if (TEST || DEBUG)
      printf("Bad input: %s %s %d!\n", arg1, arg2, toNum(arg3));
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
    }
  }

  else if(string_equal(Opcode, "trap")) {
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    if (arg1[0] != 'x') {
      #if (TEST || DEBUG)
      printf("Invalid syntax for a TRAP instruction: '%c'\n", arg1[0]);
      printf("Exit code: 4\n");
      #endif
      
      exit(4);
    }

    int16_t TRAPVECT_8 = toNum(arg1);
    CHECK_TRAPVECT8(TRAPVECT_8);

    instruction = SET_OPCODE(TRAP) | SET_TRAPVECT8(TRAPVECT_8);

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
      #if (TEST || DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4); 
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
      #if (TEST || DEBUG)
      printf("Bad register input: %s %s!\n", arg1, arg2);
      printf("Exit code: %d\n", 4);
      #endif

      exit(4);
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

    int32_t FILL = toNum(arg1);
    CHECK_FILL(FILL);
    instruction = FILL;

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", instruction);
  }

  else if(string_equal(Opcode, "nop")) {
    check_arguments(Opcode, arg1);
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    #if (DEBUG)
    printf("0x%4X\n", instruction);
    #endif

    fprintf(outfile, "0x%04X\n", 0x0000);
  }

  else if(string_equal(Opcode, ".end")) {
    check_arguments(Opcode, arg1);
    check_arguments(Opcode, arg2);
    check_arguments(Opcode, arg3);
    check_arguments(Opcode, arg4);

    return FINISH;
  }

  else {
    #if (TEST || DEBUG)
    printf("Invalid Opcode Found: %s!\n", Opcode);
    printf("Error Code: %d\n", 2);
    #endif

    exit(2);
  }

  return NOT_FINISH;
}

void
assembler_init(FILE** infile, FILE** outfile, char* prgName, char* iFileName, char* oFileName) {
  if (prgName == NULL ||
      iFileName == NULL ||
      oFileName == NULL) {
    #if (TEST || DEBUG)
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

    #if (TEST || DEBUG)
    printf("Error Code: %d\n", 4);
    #endif

    exit(4);
  }

  if (!(*outfile)) {
    printf("Error: Cannot open file %s\n", oFileName);
    #if (TEST || DEBUG)
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

  assembler_init(&infile, &outfile, prgName, iFileName, oFileName);

  create_all_labels(argv[1]);
  first_pass(argv[1]);
  second_pass(argv[1], argv[2]);

  exit(0);
}