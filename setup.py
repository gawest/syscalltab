from distutils.core import setup, Extension

import subprocess
import os

curpwd = os.environ['PWD']

#subprocess.check_call("{0}/syscalltab_gen.sh -t -e".format(curpwd))

setup(
    ext_modules=[Extension("syscalltab", ["_syscalltab.c", "syscalltab.c"])],
    include_dirs="/usr/include/asm",
)
