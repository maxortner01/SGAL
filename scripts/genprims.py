import os, sys

header_file_top = """#pragma once

#include <SGAL/decl.h>

namespace sgal
{
    class SGAL_API Primitives
    {
"""

header_file_bottom = """    };
}

"""

source_file_top = """#include <SGAL/SGAL.h>

namespace sgal
{
"""

source_file_bottom = """
}

"""

# Go to SGAL directory
os.chdir("../../")

if (os.path.exists("src/Graphics/Primitives/Primitives.cpp") and os.path.exists("include/SGAL/Graphics/Primitives/Primitives.h")): sys.exit(0)

source = open("src/Graphics/Primitives/Primitives.cpp", "w")
header = open("include/SGAL/Graphics/Primitives/Primitives.h", "w")
header.write(header_file_top)
source.write(source_file_top)

for rmFileName in os.listdir("prims"):
    rmFile = open("prims/" + rmFileName, "rb")
    data = rmFile.read()
    rmFile.close()

    header.write("        static unsigned char " + rmFileName.split(".")[0] + "[" + str(len(data)) + "];\n")

    source.write("\tunsigned char Primitives::" + rmFileName.split(".")[0] + "[" + str(len(data)) + "] = {")
    for c, b in enumerate(data):
        if (c % 10 == 0): source.write("\n\t\t")
        source.write(hex(b))
        if (c != len(data) - 1): source.write(", ")

    source.write("\n\t};\n\n")

header.write("\n\tpublic:\n")
for rmFileName in os.listdir("prims"):
    rmFile = open("prims/" + rmFileName, "rb")
    data = rmFile.read()
    rmFile.close()

    methodName = "get"
    methodNameL = rmFileName.split(".")[0].split("_")
    for c, name in enumerate(methodNameL):
        methodName += name.capitalize()
    
    header.write("\t\tstatic const VertexArray& " + methodName + "();\n")

    source.write("\tconst VertexArray& Primitives::" + methodName + "()\n\t{\n")
    source.write("\t\tstatic VertexArray* array = nullptr;\n")
    source.write("\t\tif (!array) { array = new VertexArray(); Serializer::loadFromArray(" + rmFileName.split(".")[0] + ", " + str(len(data)) + ", *array); }\n")
    source.write("\t\treturn *array;\n")
    source.write("\t}\n\n")

header.write(header_file_bottom)
source.write(source_file_bottom)
header.close()
source.close()

print("Finished generating header.")