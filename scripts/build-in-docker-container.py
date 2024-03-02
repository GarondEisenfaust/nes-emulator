#! /bin/python

import subprocess
import os
import argparse


def main():

  parser = argparse.ArgumentParser("build-in-docker-container.py")
  parser.add_argument(
      "source_directory", help="the source directory.", type=str)
  parser.add_argument("build_type", help="the build type", type=str)
  args = parser.parse_args()

  source_dir = str(args.source_directory)
  build_type = str(args.build_type)

  id = build_container(source_dir)

  run_command_in_build_container(
      source_dir, id,
      "cmake -B build/{build_type}Docker -G Ninja -DCMAKE_BUILD_TYPE={build_type} && cmake --build ./build/{build_type}Docker/ --config {build_type}"
      .format(build_type=build_type))


def run_command_in_build_container(source_dir: str, container_id: str,
                                   command: str):
  subprocess.run([
      "docker", "run", "-it", "-v", "{}:/root/build".format(source_dir),
      container_id, "sh", "-c", command
  ])


def build_container(source_dir: str):
  return subprocess.run(["docker", "build", "-q", source_dir],
                        check=True,
                        capture_output=True,
                        text=True).stdout.strip()


if __name__ == "__main__":
  main()
