#!/bin/bash
gcc -std=c99 -o ../assemble ../assembler.c

#

../assemble input/add1-py.asm output/add1-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with add1!\n"
fi


../assemble input/add2-py.asm output/add2-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with add2!\n"
fi

#

../assemble input/and1-py.asm output/and1-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with and1!\n"
fi

../assemble input/and2-py.asm output/and2-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with and2!\n"
fi

#

../assemble input/br-py.asm output/br-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with br!\n"
fi

#

../assemble input/brp-py.asm output/brp-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with brp!\n"
fi

#

../assemble input/brz-py.asm output/brz-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with brz!\n"
fi

#

../assemble input/brzp-py.asm output/brzp-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with brzp!\n"
fi

#

../assemble input/brn-py.asm output/brn-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with brn!\n"
fi

#

../assemble input/brnp-py.asm output/brnp-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with brnp!\n"
fi

#

../assemble input/brnz-py.asm output/brnz-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with brnz!\n"
fi

#

../assemble input/brnzp-py.asm output/brnzp-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with brnzp!\n"
fi

#

../assemble input/jmp-py.asm output/jmp-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with jmp!\n"
fi

#

../assemble input/ret-py.asm output/ret-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with ret!\n"
fi

#

../assemble input/jsr-py.asm output/jsr-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with jsr!\n"
fi

#

../assemble input/jsrr-py.asm output/jsrr-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with jsrr!\n"
fi

#

../assemble input/ldb1-py.asm output/ldb1-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with ldb1!\n"
fi

../assemble input/ldb2-py.asm output/ldb2-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with ldb2!\n"
fi

#

../assemble input/ldw1-py.asm output/ldw1-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with ldw1!\n"
fi

../assemble input/ldw2-py.asm output/ldw2-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with ldw2!\n"
fi

#

../assemble input/lea-py.asm output/lea-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with lea!\n"
fi

#

../assemble input/not-py.asm output/not-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with not!\n"
fi

#

../assemble input/rti-py.asm output/rti-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with rti!\n"
fi

#

../assemble input/lshf-py.asm output/lshf-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with lshf!\n"
fi

#

../assemble input/rshfl-py.asm output/rshfl-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with rshfl!\n"
fi

#

../assemble input/rshfa-py.asm output/rshfa-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with rshfa!\n"
fi

#

../assemble input/stb1-py.asm output/stb1-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with stb1!\n"
fi

../assemble input/stb2-py.asm output/stb2-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with stb2!\n"
fi

#

../assemble input/stw1-py.asm output/stw1-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with stw1!\n"
fi

../assemble input/stw2-py.asm output/stw2-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with stw2!\n"
fi

#

../assemble input/trap-py.asm output/trap-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with trap!\n"
fi

#

../assemble input/xor1-py.asm output/xor1-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with xor1!\n"
fi


../assemble input/xor2-py.asm output/xor2-c.obj
if [[ $(echo $?) != "0" ]]; then
  printf "There was an issue with xor2!\n"
fi