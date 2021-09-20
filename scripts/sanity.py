import os

def files_equal(test, input1, input2):
  content1 = input1.readlines()
  content2 = input2.readlines()

  count = 0
  for i in content2:
    count += 1

  try:
    for i in range(count):
      if (content1[i] == content2[i]):
        pass
      else:
        print(test + " has missmatches!")
        break
  except IndexError:
    print("Maybe something wrong with " + test)
    pass

print("Compiling assembler source!")
os.system("gcc -std=c99 -o ../assemble ../assembler.c")
print("Running Sanity Tests!")
print("Generating input and output files\n")

# First File

test = "add"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("ADD R1, R2, R3\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x1283\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Second file

test = "and"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("AND R1, R2, R3\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xA283\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Third file

test = "br"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  BR A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x01FE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Fourth file

test = "brp"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  BRP A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x03FE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Fifth file

test = "brz"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  BRZ A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x05FE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Sixth file

test = "brzp"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  BRZP A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x07FE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Seventh file

test = "brn"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  BRN A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x09FE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Eigth file

test = "brnp"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  BRNP A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x0BFE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Ninth file

test = "brnz"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  BRNZ A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x0DFE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Tenth file

test = "brnzp"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  BRNZP A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x0FFE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Eleventh file

test = "jmp"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("JMP R7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xC1C0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 12

test = "ret"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("RET\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xC1C0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 13 file

test = "jsr"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  JSR A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x4FFE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 14 file

test = "jsrr"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("JSRR R7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x41C0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 15 file

test = "ldb"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("LDB R1, R2, #7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x2287\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 16 file

test = "ldw"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("LDW R1, R2, #7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x6287\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 17 file

test = "lea"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("A  LEA R1, A\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xE3FE\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 18 file

test = "not"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("NOT R1, R2\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x92BF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 19 file

test = "rti"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("RTI\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x8000\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 20 file

test = "lshf"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("LSHF R1, R2, #7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xD287\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 20 file

test = "rshfl"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("RSHFL R1, R2, #7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xD297\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 21 file

test = "rshfa"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("RSHFA R1, R2, #7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xD2B7\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 21 file

test = "stb"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("STB R1, R2, #7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x3287\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 21 file

test = "stw"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("STW R1, R2, #7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x7287\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 21 file

test = "trap"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("TRAP x25\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xF025\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 22 file

test = "xor"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("XOR R1, R2, R3\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x9283\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

os.system("rm input/* output/*")