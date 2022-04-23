#!/bin/env python

#
# This basic script uses Host GCC compiler for compilation.
# It doesn't produce any object file nor cache files
#

import sys

if sys.version_info < (3,7):
    print("Please upgrade your Python interpreter to 3.7 or higher")
    sys.exit(1)

import platform
import os
from pathlib import Path
import subprocess

__scriptdir__ = Path(__file__).parent

sources = Path(__scriptdir__, "core").rglob("*.cpp")
includedirs = [
    Path(__scriptdir__, "include"),
    Path(__scriptdir__, "lua", "src")
]

def _build_lua():
    luadir = Path(__scriptdir__, "lua")
    incdir = Path(luadir, "src")
    if Path(luadir, "src", "liblua.a").exists():
        return

    args = ["-O2", "-DLUA_COMPAT_ALL", f"-I{incdir}"]
    largs = ["-lm"]
    cc = "gcc"
    plat = platform.system()
    objs = {
        "lapi.o": "lapi.c",
        "lauxlib.o": "lauxlib.c",
        "lbaselib.o": "lbaselib.c",
        "lbitlib.o": "lbitlib.c",
        "lcode.o": "lcode.c", 
        "lcorolib.o": "lcorolib.c",
        "lctype.o": "lctype.c",
        "ldblib.o": "ldblib.c",
        "ldebug.o": "ldebug.c", 
        "ldo.o": "ldo.c", 
        "ldump.o": "ldump.c", 
        "lfunc.o": "lfunc.c", 
        "lgc.o": "lgc.c", 
        "linit.o": "linit.c",
        "liolib.o": "liolib.c",
        "llex.o": "llex.c", 
        "lmathlib.o": "lmathlib.c",
        "lmem.o": "lmem.c", 
        "loadlib.o": "loadlib.c",
        "lobject.o": "lobject.c", 
        "lopcodes.o": "lopcodes.c",
        "loslib.o": "loslib.c",
        "lparser.o": "lparser.c", 
        "lstate.o": "lstate.c", 
        "lstring.o": "lstring.c", 
        "lstrlib.o": "lstrlib.c",
        "ltable.o": "ltable.c", 
        "ltablib.o": "ltablib.c",
        "ltm.o": "ltm.c", 
        "lua.o": "lua.c",
        "luac.o": "luac.c", 
        "lundump.o": "lundump.c", 
        "lvm.o": "lvm.c",
        "lzio.o": "lzio.c"
    }
    if plat == "FreeBSD":
        args = args + ["-DLUA_USE_LINUX"]
        largs = largs + ["-Wl,-E", "-lreadline"]
    elif plat == "Linux":
        args = args + ["-DLUA_USE_LINUX"]
        largs = args + ["-Wl,-E", "-ldl", "-lreadline"]
    elif plat == "Darwin":
        args = args + ["-DLUA_USE_MACOSX"]
        largs = largs + ["-lreadline"]
        cc = "cc"
    elif plat != "Windows":
        args = args + ["-DLUA_USE_POSIX"]
    os.chdir(Path(luadir, "src"))

    for obj, src in objs.items():
        subprocess.run(f"{cc} -c {' '.join(args)} {src} -o {obj}", check=True, shell=True)
    ar_cmd = f"ar rcu liblua.a {' '.join(list(objs.keys()))}"
    ranlib_cmd = "ranlib liblua.a"

    subprocess.run(ar_cmd, shell=True, check=True)
    subprocess.run(ranlib_cmd, shell=True, check=True)
    
    os.chdir(__scriptdir__)
    

_build_lua()

cmd_args = []

for src in sources:
    cmd_args.append(str(src))

cmd_args += ["-o", "zake"]

for dir in includedirs:
    cmd_args.append(f"-I{dir}")

cmd_args += [
    f"-L{Path(__scriptdir__, 'lua', 'src')} "
    "-llua"
]

cmd = f"g++ {' '.join(cmd_args)} -g"
print(cmd)

subprocess.run(cmd, shell=True, check=True)