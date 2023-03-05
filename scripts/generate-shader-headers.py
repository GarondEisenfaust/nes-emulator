#! /bin/python
from pathlib import Path
from sys import argv

shader_path = argv[1]
output_path = argv[2]

suffix = ".h"
template = """#pragma once
const char* {name}Code = R"======(
{code}
)======";
"""

Path(output_path).mkdir(parents=True, exist_ok=True)
for path in Path(shader_path).glob("*"):
  with open(path, "r") as shader_file:
    file_name = path.stem
    code = shader_file.read()
    content = template.format(name=file_name, code=code)

  header_output_path = Path(output_path).joinpath(file_name + suffix)
  with open(header_output_path, "w") as header_file:
    message = "Generating {out} from {source}".format(
        out=header_output_path, source=path)
    print(message)
    header_file.write(content)
