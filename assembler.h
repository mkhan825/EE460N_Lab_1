/***************************************************************************************/
/*                                                                                     */
/*                               DEFINES/MACROS/ENUMS                                  */
/*                                                                                     */
/***************************************************************************************/

enum
{
  DONE, OK, EMPTY_LINE
};

/* Note: MAX_LINE_LENGTH, OK, EMPTY_LINE, and DONE are defined values */
#define MAX_LINE_LENGTH 255

// Opcodes
#define ADD 0x1
#define AND 0xA
// #define 

// Functions
#define GET_DR(x) (((uint16_t)x & 0x0e00) >> 9)

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

int
isOpcode(char* str);