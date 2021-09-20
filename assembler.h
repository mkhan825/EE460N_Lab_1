#include <string.h>
#include <stdint.h>

/***************************************************************************************/
/*                                                                                     */
/*                               DEFINES/MACROS/ENUMS                                  */
/*                                                                                     */
/***************************************************************************************/

#define DEBUG 1

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
#define SET_BOFFSET6(boffset6) ((((uint16_t)boffset6) & 0x2f) << 0)
#define SET_OFFSET6(offset6) ((((uint16_t)offset6) & 0x2f) << 0)

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
