import os, sys

# Go to SGAL directory
os.chdir("../../")

shader_files = []
for f in os.listdir("cgen/shaders"):
    shader_files.append("cgen/shaders/" + f)

dependency_files = []
for f in os.listdir("build"):
    if f.split(".")[len(f.split(".")) - 1] == "sdep":
        dependency_files.append("build/" + f)

if len(shader_files) == len(dependency_files): 
    newer = False
    for c, dep in enumerate(dependency_files):
        shader_file_name = ""
        for shader in shader_files:
            shader_name = shader.split("/")[len(shader.split("/")) - 1].split(".")[0].lower()
            dep_name = dep.split("/")[len(dep.split("/")) - 1].split(".")[0].lower()

            if shader_name == dep_name: 
                shader_file_name = shader
                break

        if shader_file_name == "":
            newer = True
            break

        if round(os.stat(dep).st_mtime) != round(os.stat(shader_file_name).st_mtime):
            newer = True
            break

    if not newer:
        print("Shader files haven't been updated.")
        sys.exit(0)
        
    print("Shader files have been updated.")

for shader in shader_files:
    name = shader.split("/")[len(shader.split("/")) - 1].split(".")[0].lower()

    content = ""
    with open(shader, "r") as f:
        content = f.read()

    f = open(shader, "w")
    f.write(content)
    f.close()

    f = open("build/" + name + ".sdep", "w")
    f.write("Built")
    f.close()

before_header = ""
after_header  = ""
before_source = ""
after_source  = ""

header_file = "include/SGAL/Graphics/Shader.h"
source_file = "src/Graphics/Shader.cpp"

# Get the parts of the header file
with open(header_file, 'r') as f:
    before = True
    for line in f:
        if "[SHADER]" in line:
            before = False
            continue

        if before:  before_header += line
        else:       after_header += line
    
# Get the parts of the source file
with open(source_file, 'r') as f:
    before = True
    for line in f:
        if "[SHADER]" in line:
            before = False
            continue

        if before:  before_source += line
        else:       after_source += line

# Always rewrite the source file
with open(source_file, 'w') as f:
    f.write(before_source)
    f.write("\t// [SHADER] generated-code\n")

    for shader in shader_files:
        name = shader.split("/")[len(shader.split("/")) - 1].split(".")[0].lower()
        f.write("\tstatic Shader* " + name + " = nullptr;\n")

    f.write("\n")

    for shader in shader_files:
        has_vertex   = False
        has_fragment = False
        scope = ""

        name = shader.split("/")[len(shader.split("/")) - 1].split(".")[0]
        f.write("\tShader& Shader::" + name + "()\n")
        f.write("\t{\n")
        f.write("\t\tif (!" + name.lower() + ")")
        f.write("\t\t{\n")
        
        with open(shader, 'r') as shader_file:
            for line in shader_file:
                line = line.strip("\n")
                if "[VERTEX]" in line: 
                    has_vertex = True
                    scope = "vertex"
                    f.write("\n\t\t\tstd::string vertex_contents;\n")
                    continue

                if "[FRAGMENT]" in line:
                    has_fragment = True
                    scope = "fragment"
                    f.write("\n\t\t\tstd::string fragment_contents;\n")
                    continue

                if scope == "vertex":
                    f.write('\t\t\tvertex_contents')
                if scope == "fragment":
                    f.write('\t\t\tfragment_contents')

                f.write(' += "' + line + '\\n";\n')
        
        f.write("\n")

        f.write("\t\t\t" + name.lower() + " = new Shader();\n")
        if has_vertex:
            f.write("\t\t\t" + name.lower() + "->fromString(vertex_contents,     Shader::Vertex);\n")
        if has_fragment:
            f.write("\t\t\t" + name.lower() + "->fromString(fragment_contents,   Shader::Fragment);\n")
        f.write("\t\t\t" + name.lower() + "->link();\n")
        f.write("\n")
        f.write("\t\t}\n")

        f.write("\t\treturn *" + name.lower() + ";\n")

        f.write("\t}\n\n")

    f.write("}\n")

with open(header_file, 'w') as f:
    f.write(before_header)
    
    f.write("\t\t// [SHADER] insert-decl\n")
    for shader in shader_files:
        name = shader.split("/")[len(shader.split("/")) - 1].split(".")[0]
        f.write("\t\tstatic Shader& " + name + "();\n")

    f.write("\t};\n}")