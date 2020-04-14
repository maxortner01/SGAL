import os

files = []

file_types = ["cpp", "h"]
SOURCE_FOLDER  = "../src"
INCLUDE_FOLDER = "../include"

def addFilesInFolder(foldername):
    for f in os.listdir(foldername):
        if not '.' in f:
            addFilesInFolder(foldername + "/" + f)
        else:
            if f.split(".")[len(f.split(".")) - 1] not in file_types:
                continue
            
            files.append(foldername + "/" + f)

addFilesInFolder(SOURCE_FOLDER)
addFilesInFolder(INCLUDE_FOLDER)

total = 0
for f in files:
    lines = 0
    with open(f, "r") as fl:
        for line in fl:
            lines += 1
    
    total += lines
    filename = f.split("/")[len(f.split("/")) - 1]
    for s in range(20 - len(filename)):
        filename += " "

    print(filename + str(lines))

print("Total:              " + str(total))

input("")