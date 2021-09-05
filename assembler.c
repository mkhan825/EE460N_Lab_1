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

  printf("String: %s\n", pLine);
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
  if(isOpcode(lPtr) == -1 && lPtr[0] != '.') {
    *pLabel = lPtr;
    if(!(lPtr = strtok( NULL, "\t\n ,"))) {
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

int
isOpcode(char* str) {
  printf("TODO: isOpcode\n");
  if (str == "Add") {

  } else if (str == "And") {

  }
  return false;
}

int
main(int argc, char* argv[]) {
  char *prgName   = argv[0];
  char *iFileName = argv[1];
  char *oFileName = argv[2];

  if (prgName == NULL ||
      iFileName == NULL ||
      oFileName == NULL) {
    printf("Missing command line argument(s)!\n");
    exit(4);
  }

  printf("program name = '%s'\n", prgName);
  printf("input file name = '%s'\n", iFileName);
  printf("output file name = '%s'\n", oFileName);

  /* open the source file */
  FILE* infile = fopen(argv[1], "r");
  FILE* outfile = fopen(argv[2], "w");
    
  if (!infile) {
    printf("Error: Cannot open file %s\n", argv[1]);
    exit(4);
  }
  if (!outfile) {
    printf("Error: Cannot open file %s\n", argv[2]);
    exit(4);
  }

  /* Do stuff with files */
  fprintf(outfile, "Hello World!\n");
  // maybe we change tonum function
  printf("-1 toNum: %d\n", toNum("x-1"));

  char pLine[MAX_LINE_LENGTH + 1];
  char* pLabel[MAX_LINE_LENGTH + 1];
  char* pOpcode[MAX_LINE_LENGTH + 1];
  char* pArg1[MAX_LINE_LENGTH + 1];
  char* pArg2[MAX_LINE_LENGTH + 1];
  char* pArg3[MAX_LINE_LENGTH + 1];
  char* pArg4[MAX_LINE_LENGTH + 1];

  int lRet;

  do {
    lRet = readAndParse(infile, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);

    if (lRet != DONE && lRet != EMPTY_LINE) {
      // do stuff and write to out file
    }
  } while(lRet != DONE);

  fclose(infile);
  fclose(outfile);
}