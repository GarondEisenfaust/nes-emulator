#! /bin/python

import os.path
import subprocess
import argparse

parser = argparse.ArgumentParser("prepare.py")
parser.add_argument(
    "--build-type", help="the build type.", type=str, required=False)
args = parser.parse_args()
build_type = str(args.build_type) if args.build_type is not None else "Release"

prepare_dir = os.path.dirname(os.path.abspath(__file__)) + "/"
virtual_env = prepare_dir + "venv/"
conan_dir = prepare_dir + ".conan2/"
activate_this_file = virtual_env + "bin/activate_this.py"
requirements_txt = prepare_dir + "requirements.txt"

if not os.path.exists(virtual_env):
  print("Creating virtualenv at {}".format(virtual_env))
  subprocess.Popen(["python", "-m", "virtualenv", virtual_env]).wait()

exec(open(activate_this_file).read(), {'__file__': activate_this_file})
subprocess.Popen(["pip", "install", "-r", requirements_txt]).wait()

env = os.environ.copy()
env["CONAN_HOME"] = conan_dir

subprocess.Popen([
    "conan", "install", prepare_dir, "--update", "--build=missing", "-s",
    "build_type={}".format(build_type.title())
],
                 env=env).wait()
