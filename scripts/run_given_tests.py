import os

os.system("gcc -std=c99 -o ../assemble ../assembler.c")

os.system("../assemble ../tests/inv_constant.asm ./output/inv_constant.obj")
print("Expected Error code 3\n")
os.system("../assemble ../tests/inv_constant2.asm ./output/inv_constant2.obj")
print("Expected Error code 3\n")

os.system("../assemble ../tests/inv_num_ops.asm ./output/inv_num_ops.obj")
print("Expected Error code 4\n")
os.system("../assemble ../tests/inv_num_ops2.asm ./output/inv_num_ops2.obj")
print("Expected Error code 4\n")
os.system("../assemble ../tests/inv_num_ops3.asm ./output/inv_num_ops3.obj")
print("Expected Error code 4\n")
os.system("../assemble ../tests/inv_num_ops4.asm ./output/inv_num_ops4.obj")
print("Expected Error code 4\n")
os.system("../assemble ../tests/inv_num_ops5.asm ./output/inv_num_ops5.obj")
print("Expected Error code 4\n")

os.system("../assemble ../tests/inv_opcode.asm ./output/inv_opcode.obj")
print("Expected Error code 2\n")
os.system("../assemble ../tests/inv_opcode2.asm ./output/inv_opcode2.obj")
print("Expected Error code 2\n")

os.system("../assemble ../tests/ten.asm ./output/ten.obj")
print("Expected Error code 0\n")
os.system("../assemble ../tests/test1.asm ./output/test1.obj")
print("Expected Error code 0\n")
os.system("../assemble ../tests/test2.asm ./output/test2.obj")
print("Expected Error code 0\n")

os.system("../assemble ../tests/und_label.asm ./output/und_label.obj")
print("Expected Error code 1\n")
os.system("../assemble ../tests/und_label2.asm ./output/und_label2.obj")
print("Expected Error code 4\n")