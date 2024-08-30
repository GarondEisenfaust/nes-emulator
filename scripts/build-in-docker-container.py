#! /bin/python

import subprocess
import os
import argparse
import shutil
import sys


def main():

  parser = argparse.ArgumentParser("build-in-docker-container.py")
  parser.add_argument(
      "--source_directory", help="the source directory.", type=str)
  parser.add_argument("--configure_preset", help="the configure preset", type=str)
  parser.add_argument("--build_preset", help="the configure preset", type=str, required=False)

  args = parser.parse_args()

  source_dir = str(args.source_directory)
  configure_preset = str(args.configure_preset)
  build_preset = str(args.build_preset if args.build_preset is not None else configure_preset)

  gid = os.getegid()
  uid = os.geteuid()

  docker_executable = shutil.which("docker")

  if docker_executable is None:
    print("Docker executable could not be found!")
    sys.exit(1)

  id = build_container(docker_executable, uid, gid, source_dir)
  run_command_in_build_container(
      docker_executable, uid, gid, source_dir, id,
      "cmake --preset {configure_preset} && cmake --build --preset {build_preset}"
      .format(configure_preset=configure_preset, build_preset=build_preset))


def run_command_in_build_container(docker_executable: str, uid: int, gid: int,
                                   source_dir: str, container_id: str,
                                   command: str):
  volume_mapping = "{source_dir}:/home/builder/build/".format(
      source_dir=source_dir)
  user_and_group = "{uid}:{gid}".format(uid=uid, gid=gid)
  subprocess.run([
      docker_executable, "run", "--rm", "-it", "-v", volume_mapping,
      container_id, "sh", "-c", command
  ],
                 check=True,
                 text=True)


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
