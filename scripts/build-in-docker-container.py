#! /bin/python

import subprocess
import os
import argparse
import shutil
import sys


def main():

  parser = argparse.ArgumentParser("build-in-docker-container.py")
  parser.add_argument(
      "source_directory", help="the source directory.", type=str)
  parser.add_argument("build_directory", help="the build directory", type=str)
  parser.add_argument("build_type", help="the build type", type=str)
  args = parser.parse_args()

  source_dir = str(args.source_directory)
  build_dir = str(args.build_directory)
  build_type = str(args.build_type)

  gid = os.getegid()
  uid = os.geteuid()

  docker_executable = shutil.which("docker")

  if docker_executable is None:
    print("Docker executable could not be found!")
    sys.exit(1)

  id = build_container(docker_executable, uid, gid, source_dir)
  run_command_in_build_container(
      docker_executable, uid, gid, source_dir, id,
      "cmake -B {build_dir} -G Ninja -DCMAKE_BUILD_TYPE={build_type} && cmake --build {build_dir} --config {build_type}"
      .format(build_type=build_type, build_dir=build_dir))


def run_command_in_build_container(docker_executable: str, uid: int, gid: int,
                                   source_dir: str, container_id: str,
                                   command: str):
  volume_mapping = "{source_dir}:/home/builder/build/".format(
      source_dir=source_dir)
  user_and_group = "{uid}:{gid}".format(uid=uid, gid=gid)
  subprocess.run([
      docker_executable, "run", "--rm", "-it", "-v", volume_mapping,
      container_id, "sh", "-c", command
  ])


def build_container(docker_executable: str, uid: int, gid: int,
                    source_dir: str):
  uid_arg = "--build-arg=UID={uid}".format(uid=uid)
  gid_arg = "--build-arg=GID={gid}".format(gid=gid)
  return subprocess.run(
      [docker_executable, "build", "-q", uid_arg, gid_arg, source_dir],
      check=True,
      capture_output=True,
      text=True).stdout.strip()


if __name__ == "__main__":
  main()
