#! /bin/python

file1 = open('../scripts/opcodes.txt', 'r')
Lines = file1.readlines()

h_template = """#pragma once
#include "IOpcode.h"

class {name} : public IOpcode {{
 public:
  {name}(Cpu* cpu);
  virtual void operator()();
}};
"""

cpp_template = """#include "opcodes/{name}.h"
#include "Cpu.h"

{name}::{name}(Cpu* cpu) : IOpcode(cpu) {{}}

void {name}::operator()() {{
  return false;
}}
"""

for line in Lines:
  name = line.strip()
  h = h_template.format(name=name)
  cpp = cpp_template.format(name=name)

  with open("include/opcodes/" + name + ".h", 'w') as h_file:
    h_file.writelines(h)

with open("src/opcodes/" + name + ".cpp", 'w') as cpp_file:
  cpp_file.writelines(cpp)
