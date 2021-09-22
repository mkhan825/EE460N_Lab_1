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

test = "add1"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("ADD R7, R7, #-16\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x1FF0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

#

test = "add2"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("ADD R7, R7, #15\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x1FEF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# Second file

test = "and1"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("AND R7, R7, #-16\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x5FF0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

#

test = "and2"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("AND R7, R7, #15\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x5FEF\n")
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
file2.write("0x0FFF\n")
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
file2.write("0x03FF\n")
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
file2.write("0x05FF\n")
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
file2.write("0x07FF\n")
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
file2.write("0x09FF\n")
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
file2.write("0x0BFF\n")
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
file2.write("0x0DFF\n")
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
file2.write("0x0FFF\n")
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
file2.write("0x4FFF\n")
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

test = "ldb1"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("LDB R7, R7, #-32\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x2FE0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

#

test = "ldb2"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("LDB R7, R7, #31\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x2FDF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 16 file

test = "ldw1"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("LDW R7, R7, #-32\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x6FE0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

#

test = "ldw2"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("LDW R7, R7, #31\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x6FDF\n")
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
file2.write("0xE3FF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 18 file

test = "not"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("NOT R7, R7\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x9FFF\n")
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
file1.write("LSHF R7, R7, #15\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xDFCF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 20 file

test = "rshfl"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("RSHFL R7, R7, #15\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xDFDF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 21 file

test = "rshfa"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("RSHFA R7, R7, #15\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xDFFF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 21 file

test = "stb1"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("STB R7, R7, #-32\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x3FE0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

#

test = "stb2"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("STB R7, R7, #31\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x3FDF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 21 file

test = "stw1"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("STW R7, R7, #-32\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x7FE0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

#

test = "stw2"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("STW R7, R7, #31\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x7FDF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 21 file

test = "trap"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("TRAP xFF\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0xF0FF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# 22 file

test = "xor1"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("XOR R7, R7, #-16\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x9FF0\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

#

test = "xor2"

file1 = open("input/"+test+"-py.asm", "w")
file1.write(".ORIG x3000\n")
file1.write("XOR R7, R7, #15\n")
file1.write(".END")
file1.close()

os.system("../assemble input/"+test+"-py.asm output/"+test+"-c.obj")

file2 = open("output/"+test+"-py.obj", "w")
file2.write("0x3000\n")
file2.write("0x9FEF\n")
file2.close()

output1 = open("output/"+test+"-c.obj", "r")
output2 = open("output/"+test+"-py.obj", "r")

files_equal(test, output1, output2)

# os.system("rm input/* output/*")