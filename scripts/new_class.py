import os, sys, datetime

# Default Settings
DEFAULT_NS  = ""
DEFAULT_DIR = ""

# Read in the defaults
with open("class.config", 'r') as f:
    for line in f:
        if len(line.split("=")) > 1 and line.split("=")[0] == "namespace":
            DEFAULT_NS = line.split("=")[1]
            break

# Read input
CLASS_INFORMATION = {}
if len(sys.argv) == 1:
    CLASS_INFORMATION = {
        "CLASS_NAME": input("Class Name: "),
        "NAMESPACE":  input("Namespace (default: '" + DEFAULT_NS + "'): "),
        "DIRECTORY":  input("Directory (default: '" + DEFAULT_DIR + "'): ")
    }
else:
    args = []
    for c, arg in enumerate(sys.argv):
        if c == 0: continue

        args.append(arg)

    for i in range(3 - len(args)):
        args.append("")

    CLASS_INFORMATION = {
        "CLASS_NAME": args[0],
        "NAMESPACE": "",
        "DIRECTORY": args[1]
    }

# If there's input, include a slash
if len(CLASS_INFORMATION["DIRECTORY"]) > 0:
    CLASS_INFORMATION["DIRECTORY"] += "/"

# Set the namespace as default if empty
if (CLASS_INFORMATION["NAMESPACE"] == ""):
    CLASS_INFORMATION["NAMESPACE"] = DEFAULT_NS

# Location Variables
INCLUDE_LOCATION = "../include/SGAL"
SOURCE_LOCATION  = "../src"

CLASS_FILE_REL   = CLASS_INFORMATION["DIRECTORY"] + CLASS_INFORMATION["CLASS_NAME"]
HEADER_LOCATION = INCLUDE_LOCATION + "/" + CLASS_FILE_REL + ".h"
SOURCE_FILE_LOCATION = SOURCE_LOCATION + "/" + CLASS_FILE_REL + ".cpp"

if os.path.isfile(HEADER_LOCATION):
    input ("Class already exists!")
    sys.exit(1)

directories = CLASS_INFORMATION["DIRECTORY"].split("/")

# Go through each directory and create a subdirectory with an associated
# header file
if len(directories) > 0:
    current_dir = ""
    for c, directory in enumerate(directories):
        if (directory == ""): break

        # This is the file that includes the folder
        FOLDER_INCLUDE_FILE = INCLUDE_LOCATION
        if current_dir != "":
            FOLDER_INCLUDE_FILE += "/" 
        FOLDER_INCLUDE_FILE += current_dir + "/" + directory + ".h"

        # This is the source file that includes the folder
        FOLDER_SOURCE_FILE = SOURCE_LOCATION
        if current_dir != "":
            FOLDER_SOURCE_FILE += "/"
        FOLDER_SOURCE_FILE += current_dir + "/" + directory + ".cpp"

        # Check if the include file exists
        if not os.path.isfile(FOLDER_INCLUDE_FILE):
            with open(FOLDER_INCLUDE_FILE, 'w') as f:
                f.write("#pragma once\n\n")
                
            if c == 0:
                with open(INCLUDE_LOCATION + "/SGAL.h", 'a') as f:
                    f.write("#include \"./" + directory + ".h\"\n")
        
        # Check if the source file exists
        if not os.path.isfile(FOLDER_SOURCE_FILE):
            with open(FOLDER_SOURCE_FILE, 'w') as f:
                f.write("\n")

        # Append the include
        with open(FOLDER_INCLUDE_FILE, 'a') as f:
            if c == len(directories) - 2:
                f.write("#include \"./" + directory + "/" + CLASS_INFORMATION["CLASS_NAME"] + ".h\"\n")
            else:
                f.write("#include \"./" + directory + "/" + directories[c + 1] + ".h\"\n")
        
        # Append the include to source
        with open(FOLDER_SOURCE_FILE, 'a') as f:
            if c == len(directories) - 2:
                f.write("#include \"./" + directory + "/" + CLASS_INFORMATION["CLASS_NAME"] + ".cpp\"\n")
            else:
                f.write("#include \"./" + directory + "/" + directories[c + 1] + ".cpp\"\n")

        current_dir += directory + "/"
        THIS_INCLUDE_DIRECTORY = INCLUDE_LOCATION + "/" + current_dir
        THIS_SOURCE_DIRECTORY = SOURCE_LOCATION + "/" + current_dir

        if not os.path.exists(THIS_INCLUDE_DIRECTORY):
            os.mkdir(THIS_INCLUDE_DIRECTORY)

        if not os.path.exists(THIS_SOURCE_DIRECTORY):
            os.mkdir(THIS_SOURCE_DIRECTORY)
            

with open(HEADER_LOCATION, 'w') as f:
    # Documentation
    f.write("/**\n")
    f.write(" * @file   " + CLASS_INFORMATION["CLASS_NAME"] + ".h\n")
    f.write(" * @author Max Ortner\n")
    f.write(" * @date   " + str(datetime.datetime.now().month) + "-" + str(datetime.datetime.now().day) + "-" + str(datetime.datetime.now().year) +  "\n")
    f.write(" * \n")
    f.write(" * Description...\n")
    f.write(" * \n")
    f.write(" * @copyright Copyright (c) " + str(datetime.datetime.now().year) + "\n")
    f.write("**/\n\n")

    f.write("#pragma once\n\n")
    f.write("#include <SGAL/decl.h>\n\n")
    f.write("namespace " + CLASS_INFORMATION["NAMESPACE"] + "\n{\n\tclass SGAL_API " + CLASS_INFORMATION["CLASS_NAME"])
    f.write("\n\t{\n")
    f.write("\n\n")
    f.write("\t};\n}")

with open(SOURCE_FILE_LOCATION, 'w') as f:
    f.write("#include <SGAL/SGAL.h>\n\n")
    f.write("namespace " + CLASS_INFORMATION["NAMESPACE"] + "\n{\n\n\n\n}")