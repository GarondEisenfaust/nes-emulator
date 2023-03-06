#! /bin/python
from pathlib import Path
from sys import argv

shader_path = Path(argv[1])
output_path = Path(argv[2])

suffix = ".h"
template = """#pragma once
const char* {name}Code = R"======(
{code}
)======";
"""

with open(shader_path, "r") as shader_file:
  file_name = shader_path.stem
  code = shader_file.read()
  content = template.format(name=file_name, code=code)

with open(output_path, "w") as header_file:
  header_file.write(content)
