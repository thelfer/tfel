% MTest: unit mechanical behaviour testing
% Helfer Thomas; Jean-Michel Proix
% August 19, 2014

# Overview

`MFront` comes with an handy easy-to-use tool called `MTest`. This
tool handles two types of computations:

- The description of a single material point.
- The description of a pipe, submitted to variaous kinds of loadings.

From various benchmarcks, this tool has been found much faster (from
ten to several hundred times depending on the test case) than using a
full-fledged finite element solver such as `Cast3M` or `Code_Aster`.

The `mtest` python module, described [here](mtest-python.html), allows
much finer control of the computations.

# Usage

MTest can be used from the command line as follows:

~~~~{.bash}
$ mtest [options] inputfile
~~~~

If the input file has the `ptest` extension, `MTest` will assume that
the input files describes a test on a pipe. Otherwise, a test on a
material point is assumed.

This can be changed by using the `--scheme` option that currently
accept two values:

- `--scheme=mtest` to specify that the simulation of a single material
  point is intended.
- `--scheme=ptest` to specify that the simulation of a pipe is
  intended.

## Getting help

### The `--help-keywords-list` command line option

The list of available keywords are available thanks to the
`--help-keywords-list` option. By default, this command describes the
list of keywords associated with the simulation of a single material
point.  To see the keywords associated with the simulation of a pipe,
use `--scheme=ptest` option *before* the `--help-keywords-list`
option.

### The `--help-keyword` option

The `--help-keyword` displays the help associated with a keyword. If
this keyword is specific to the simulation of a pipe, use
`--scheme=ptest` option *before* the `--help-keyword` option.

# Material point

`MTest` can test the local behaviour of a material, by imposing
independent constraints on each component of the driving variables (or
deformation gradient or or the stress. It equivalent to the
`SIMU_POINT_MAT` operator available within the `Code-Aster` finite
element solver [@edf_macro-commande_2013] or to the `SiDoLo` software
[@pilvin_sidolo_2003].

`MTest` can be used to model various experiments, as far as a stage
implying strain localisation is not reached: tensile, compressive or
shear tests driven by stresses or deformations, pipe loaded by
internal or external pressure, \nom{Satoh} test, etc.

`MTest` generates a text file containing the evolution of the strains
(for small strains behaviours), the stresses an the state variables
during the loading history. Other `MTest` functionalities include:

- the ability to test all the behaviours handled by `MFront` (small
  strain and finite strain behaviours, cohesive zone models);
-  the ability to test isotropic and orthotropic behaviours;
-  the support of various modelling hypotheses, notably the plane
  stress and axisymmetric generalised imposed plane stress
  hypotheses;
-  many features to evaluate the numerical performances of
  mechanical behaviours. For example, user can compare the computed
  tangent consistent operator to a numerical approximation;
- a C++ library and a Python interface
  [@von_rossum_python_2007]. `MTest` can be embedded in general purpose
  scientific environment to fit behaviour parameters against
  experimental data. In particular, `MTest` can be used in ADAO
  [@salome_adao_2014], a module for Data Assimilation and Optimization
  of the Salome platform [@salome_open_2014];
- comparison of the results (strains, stresses, internal state
  variables) to reference or analytical results. `MTest` automatically
  generates XML file using the [JUnit](http://junit.org) format. Those
  files can be used for reporting using the
  [Jenkins continuous integration application](http://jenkins-ci.org/)
  This functionality is central in the assurance quality procedure of
  `MFront`.

Through an appropriate option, a behaviour implementation generated
through `MFront` may create an `MTest` file in case of integration
failure: this `MTest` file only describe the failed time step with
the appropriate initial conditions. This feature is particularly
useful to analyse the failure of large simulations which may happen
after several hours of computations.

## Example

~~~~ {#MTestPlasticity .cpp .numberLines}
@Behaviour<aster> 'src/libAsterBehaviour.so' 'asterplasticity';
@MaterialProperty<constant> 'YoungModulus'     150.e9;
@MaterialProperty<constant> 'PoissonRatio'       0.3;
@MaterialProperty<constant> 'H'                100.e9;
@MaterialProperty<constant> 's0'               100.e6;
@ExternalStateVariable 'Temperature' {0:293.15,3600.:800};
@ImposedStrain<function> 'EXX' '1.e-3*t';
@Times {0.,1 in 20};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Simulation of a pipe

`MTest` 

# References

<!-- Local IspellDict: english -->
