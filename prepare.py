#! /bin/python

import os.path
import subprocess
import argparse

parser = argparse.ArgumentParser("prepare.py")
parser.add_argument("--build-type", help="the build type.", type=str, required=False)
parser.add_argument("--profile", help="the conan profile.", type=str, required=False)
args = parser.parse_args()
build_type = str(args.build_type) if args.build_type is not None else "Release"
profile = str(args.profile) if args.profile is not None else "default"

prepare_dir = os.path.dirname(os.path.abspath(__file__)) + "/"
virtual_env = prepare_dir + "venv/"
virtual_env_bin = virtual_env + "bin/"
conan_dir = prepare_dir + ".conan2/"
requirements_txt = prepare_dir + "requirements.txt"
pip_path = virtual_env_bin + "pip"
conan_path = virtual_env_bin + "conan"

def run_command(command: str, env=os.environ.copy()):
  subprocess.Popen(command.split(" "), env=env).wait()

if not os.path.exists(virtual_env):
  print("Creating virtualenv at {}".format(virtual_env))
  run_command("python -m virtualenv {}".format(virtual_env))

run_command("{} install -r {}".format(pip_path,requirements_txt))

env = os.environ.copy()
env["CONAN_HOME"] = conan_dir
run_command(
    "{} install {} --update --build=missing -s build_type={} --profile={}".format(conan_path,
        prepare_dir, build_type, profile), env)
