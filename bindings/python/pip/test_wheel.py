"""Test of an installed tfel-mfront wheel, run by cibuildwheel.

On Linux and macOS, two behaviours are compiled with mfront, the
second one using the TDLS linear system solver. They are integrated
with mtest and loaded in python, which checks that the executables, the
headers and the libraries of the package are found. On Windows, building
behaviours requires Visual Studio and is not tested.
"""

import os
import shutil
import subprocess
import sys
import tempfile

import tfel
import tfel.math
import tfel.material
import mfront
import mtest
import mfm_test_generator

tfelhome = os.path.dirname(os.path.abspath(tfel.__file__))


def tfel_config(option):
    output = subprocess.run([shutil.which("tfel-config"), option], check=True,
                            stdout=subprocess.PIPE, text=True).stdout.strip()
    print("tfel-config " + option + ":", output)
    return output


# the headers of TFEL and TDLS are installed in tfel/include
expected_includes = os.path.realpath(os.path.join(tfelhome, "include"))
for include in tfel_config("--includes").split():
    if os.path.realpath(include[2:]) != expected_includes:
        sys.exit("tfel-config does not point to " + expected_includes)
if os.path.realpath(tfel_config("--tdls-include-path")) != expected_includes:
    sys.exit("tfel-config does not point to " + expected_includes)

if os.name == "nt":
    sys.exit(0)

behaviours = {
    "Elasticity": "",
    "TDLSElasticity": "@LinearSystemSolver TDLS;\n",
}

library = "libBehaviour.dylib" if sys.platform == "darwin" else "libBehaviour.so"

with tempfile.TemporaryDirectory() as directory:
    os.chdir(directory)
    for name, solver in behaviours.items():
        with open(name + ".mfront", "w") as f:
            f.write(f"""\
@DSL Implicit;
@Behaviour {name};
{solver}@Brick StandardElasticity;
@ElasticMaterialProperties {{150e9, 0.3}};
""")
    subprocess.run([shutil.which("mfront"), "--obuild", "--interface=generic"]
                   + [name + ".mfront" for name in behaviours], check=True)
    for name in behaviours:
        with open(name + ".mtest", "w") as f:
            f.write(f"""\
@Behaviour<generic> 'src/{library}' '{name}';
@ExternalStateVariable 'Temperature' 293.15;
@ImposedStrain 'EXX' {{0 : 0, 1 : 1e-3}};
@Times {{0, 1 in 1}};
""")
        subprocess.run([shutil.which("mtest"), name + ".mtest"], check=True)
        # loading the behaviour in python
        m = mtest.MTest()
        m.setBehaviour("generic", os.path.join("src", library), name)
        with open(name + ".res") as f:
            lines = f.readlines()
        column = next(int(line.split()[1]) for line in lines if "(SXX)" in line)
        sxx = float(lines[-1].split()[column - 1])
        print(name, "SXX:", sxx)
        if abs(sxx - 150e6) > 1:
            sys.exit("unexpected stress value")
