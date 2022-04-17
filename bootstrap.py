#!/bin/env python

#
# This basic script uses Host GCC compiler for compilation.
# It doesn't produce any object file nor cache files
#

import sys

if sys.version_info < (3,7):
    print("Please upgrade your Python interpreter to 3.7 or higher")
    sys.exit(1)

import glob
from pathlib import Path
import subprocess

__scriptdir__ = Path(__file__).parent

sources = glob.glob("core/**.cpp", root_dir=__scriptdir__)
includedirs = [
    Path(__scriptdir__, "include")
]

cmd_args = []

for src in sources:
    cmd_args.append(src)

cmd_args.append("-o")
cmd_args.append("zake")

for dir in includedirs:
    cmd_args.append("-I{}".format(str(dir)))

cmd = "g++ {}".format(" ".join(cmd_args))
print(cmd)

subprocess.run(cmd, shell=True, check=True)