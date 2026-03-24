#!/usr/bin/env python
from pathlib import Path
from sys import argv, exit

eglHeader= """#version 300 es
precision highp float;
precision highp int;
{code}
"""
glHeader= """#version 330 core
{code}
"""

template = """#pragma once
struct {name}Shader {{
  const char* source = R"======(
{code}
)======";
}};
"""

def fixHeader(code: str, is_egl: bool):
  if is_egl:
    without_header = code.split("\n", 1)[1]
    return eglHeader.format(code=without_header)
  return code

shader_path = Path(argv[1])
output_path = Path(argv[2])
is_egl = argv[3] in ["true", "True", "1"]

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
  code = fixHeader(shader_file.read(), is_egl)
  content = template.format(name=file_name, code=code)

with open(output_path, "w") as header_file:
  header_file.write(content)
