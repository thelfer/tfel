"""Test of an installed tfel-mfront wheel, run by cibuildwheel.

On Linux and macOS, a behaviour is compiled with mfront, integrated
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

includes = subprocess.run([shutil.which("tfel-config"), "--includes"],
                          check=True, stdout=subprocess.PIPE,
                          text=True).stdout.strip()
print("tfel-config --includes:", includes)
expected_includes = os.path.realpath(os.path.join(tfelhome, "include"))
if os.path.realpath(includes[2:]) != expected_includes:
    sys.exit("tfel-config does not point to " + expected_includes)

if os.name == "nt":
    sys.exit(0)

behaviour = """\
@DSL Implicit;
@Behaviour Elasticity;
@Brick StandardElasticity;
@ElasticMaterialProperties {150e9, 0.3};
"""

library = "libBehaviour.dylib" if sys.platform == "darwin" else "libBehaviour.so"
test = f"""\
@Behaviour<generic> 'src/{library}' 'Elasticity';
@ExternalStateVariable 'Temperature' 293.15;
@ImposedStrain 'EXX' {{0 : 0, 1 : 1e-3}};
@Times {{0, 1 in 1}};
"""

with tempfile.TemporaryDirectory() as directory:
    os.chdir(directory)
    with open("Elasticity.mfront", "w") as f:
        f.write(behaviour)
    with open("elasticity.mtest", "w") as f:
        f.write(test)
    subprocess.run([shutil.which("mfront"), "--obuild", "--interface=generic",
                    "Elasticity.mfront"], check=True)
    subprocess.run([shutil.which("mtest"), "elasticity.mtest"], check=True)
    # loading the behaviour in python
    m = mtest.MTest()
    m.setBehaviour("generic", os.path.join("src", library), "Elasticity")
    with open("elasticity.res") as f:
        lines = f.readlines()
    column = next(int(line.split()[1]) for line in lines if "(SXX)" in line)
    sxx = float(lines[-1].split()[column - 1])
    print("SXX:", sxx)
    if abs(sxx - 150e6) > 1:
        sys.exit("unexpected stress value")
