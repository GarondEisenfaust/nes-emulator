#! /bin/python
from pathlib import Path
from sys import argv, exit

shader_path = Path(argv[1])
output_path = Path(argv[2])

suffix = ".h"
template = """#pragma once
const char* {name}Code = R"======(
{code}
)======";
"""

if shader_path.is_dir():
  print("The shader path {shader_path} leads to a directory".format(
      shader_path=shader_path))
  exit(1)

if output_path.is_dir():
  print("The output path {output_path} leads to a directory".format(
      output_path=output_path))
  exit(1)

Path(output_path.parent).mkdir(parents=True, exist_ok=True)

with open(shader_path, "r") as shader_file:
  file_name = shader_path.stem
  code = shader_file.read()
  content = template.format(name=file_name, code=code)

with open(output_path, "w") as header_file:
  header_file.write(content)
