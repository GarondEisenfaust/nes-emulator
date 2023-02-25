#! /bin/python

file1 = open('../scripts/opcodes.txt', 'r')
Lines = file1.readlines()

h_template = """#pragma once
#include "IOpcode.h"

class {name} : public IOpcode {{
 public:
  {name}(Cpu* cpu);
  virtual bool operator()();
    virtual const char* Name();
}};
"""

cpp_template = """#include "opcodes/{name}.h"
#include "Cpu.h"

{name}::{name}(Cpu* cpu) : IOpcode(cpu) {{}}

bool {name}::operator()() {{
  return false;
}}
"""

get_name = """
const char* {name}::Name() {{
  return "{name}";
}}
"""

for line in Lines:
  name = line.strip()
  h = h_template.format(name=name)
  cpp = cpp_template.format(name=name)

  with open("include/opcodes/" + name + ".h", 'w') as h_file:
    h_file.writelines(h)

  with open("src/opcodes/" + name + ".cpp", 'r+') as cpp_file:
    current_cpp_content = cpp_file.read()
    new_cpp_content = get_name.format(name=name)
    cpp_file.writelines(new_cpp_content)
