import pyperclip
import os

os.system("g++ tmp1.cpp -o e")
os.system("./e")

file = open("out", "r", encoding="UTF-8")
content = file.read()

print(type(content))

pyperclip.copy(content)
