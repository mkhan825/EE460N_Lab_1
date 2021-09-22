#!/bin/bash
gcc -std=c99 -o ../assemble ../assembler.c

python3 br_label_far_top.py
../assemble ../tests/br_label_far_top.asm output/br_label_far_top.obj
if [[ $(echo $?) != "4" ]]; then
  printf "There was an issue with br_label_far_top!\n"
fi

python3 br_label_far_bot.py
../assemble ../tests/br_label_far_bot.asm output/br_label_far_bot.obj
if [[ $(echo $?) != "4" ]]; then
  printf "There was an issue with br_label_far_bot!\n"
fi

python3 lea_label_far_top.py
../assemble ../tests/lea_label_far_top.asm output/lea_label_far_top.obj
if [[ $(echo $?) != "4" ]]; then
  printf "There was an issue with lea_label_far_top!\n"
fi

python3 lea_label_far_bot.py
../assemble ../tests/lea_label_far_bot.asm output/lea_label_far_bot.obj
if [[ $(echo $?) != "4" ]]; then
  printf "There was an issue with lea_label_far_bot!\n"
fi

python3 jsr_label_far_top.py
../assemble ../tests/jsr_label_far_top.asm output/jsr_label_far_top.obj
if [[ $(echo $?) != "4" ]]; then
  printf "There was an issue with jsr_label_far_top!\n"
fi

python3 jsr_label_far_bot.py
../assemble ../tests/jsr_label_far_bot.asm output/jsr_label_far_bot.obj
if [[ $(echo $?) != "4" ]]; then
  printf "There was an issue with jsr_label_far_bot!\n"
fi