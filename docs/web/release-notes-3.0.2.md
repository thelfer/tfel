% Release notes of the 3.0.2 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

Version 3.0.2 of `TFEL`, `MFront` and `MTest` has been released on the
25th October, 2017.

This is mainly a bug fix version of the `3.0` series. All tickets
solved are described below. There are no known regressions.

\newcommand{\mol}{\mathrm{mol}}

# Tickets fixed

## Ticket #93: Error declaring local variable

In certain circumstances, parsing a variable type was not consistent
with `C++` rules. For example, the following statement was not
properly parsed:

~~~~{.cpp}
@LocalVariable tfel::math::tvector<Nss,tfel::math::tensor<3u,real> >  mu;
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/93/>

## Ticket #92: Reading boolean values in `Data::read_value` is broken

For more details, see: <https://sourceforge.net/p/tfel/tickets/92/>

## Ticket #91: Parts of the implicit systems and jacobian matrix blocks by the `StandardElasticity` brick shall be inserted at the beginning of the `@Integrator` block, not at the end

For more details, see: <https://sourceforge.net/p/tfel/tickets/91/>

## Ticket #90: Wrong test on `BOOST_VERSION` version in `bindings/python/include/TFEL/Python/SharedPtr.hxx`

For more details, see: <https://sourceforge.net/p/tfel/tickets/90/>

## Ticket #89: StandardElasticity: bug when only one the plane stress modelling hypotheses is selected

For more details, see: <https://sourceforge.net/p/tfel/tickets/89/>

## Ticket #88: Error when treating axial growth: the axis along the growth is wrong

The main axis for the axial growth shall be the second direction of
orthotropy, as described in the documentation, not the third one
as implemented in versions \(3.0\) and \(3.0.1\).

For more details, see: <https://sourceforge.net/p/tfel/tickets/88/>

## Ticket #87: The correction of the axial growth modifies all the stress free expansion treated so far

For more details, see: <https://sourceforge.net/p/tfel/tickets/87/>

## Ticket #86: The material laws are not available in the `ComputeStress` block in the `RungeKutta` DSL

The material laws (declared for example via the `@MaterialLaw`
keyword) were not available without extra qualification in the
`ComputeStress` block when using the `RungeKutta` DSL: one have to add
the `mfront::` qualifier in front of the material law.

The problem is a missing call to `writeMaterialLaws` in the
`writeBehaviourParserSpecificMembers` member function of the
`RungeKuttaDSLBase` class.

For more details, see: <https://sourceforge.net/p/tfel/tickets/86/>

## Ticket #85 Add support for an in plane "relocalisation strain" in behaviours laws

The `@Relocation` keyword can be used to impose a boundary condition
specific the fuel performances describing the rigid body translation
of fuel pellet fragment as an additional strain.

- an external state variable name.
- a string giving an mfront file implementing a relocation model.

### Modelling hypotheses affected

This keyword is only effective in:

- axisymmetrical generalised plane strain.
- axisymmetrical generalised plane stress.
- generalised plane strain.
- plane strain.
- plane stress.

No expansion are added in the tridimensional and axisymmetrical
modelling hypotheses.

### Physical description

The value of the relocation expansion \(r\) is converted in an
additional expansion in the radial and orthoradial directions:

\[
\left\{
\begin{aligned}
\frac{\Delta\,l_{r}}{l_{r}}           &= \frac{r}{2} \\
\frac{\Delta\,l_{\theta}}{l_{\theta}} &= \frac{r}{2} 
\end{aligned}
\right.
\].

This additional expansion is such that it does not create any stress
in the fuel pellet for an isotropic elastic material with constant
material properties.

The axial axis is the second direction in \(1D\) hypotheses and the
third direction in \(2D\) hypotheses.

### Examples

~~~~ {#Relocation .cpp}
// relocation defined by an external model
@Relocation 'UO2Relocation.mfront';
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~ {#Relocation2 .cpp}
// relocation defined by an external state variable
@ExternalStateVariable real r;
r.setEntryName("Relocation");
@Relocation r;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/85/>

## Ticket #84: Check the `NTENS` value in the `Abaqus` interface

The value of the `NTENS` value was not used to check that the function
called was consistent with the modelling hypothesis used.

For more details, see: <https://sourceforge.net/p/tfel/tickets/84/>

## Ticket #82: Python interface is broken if no material name if not defined

For more details, see: <https://sourceforge.net/p/tfel/tickets/82/>

## Ticket #81: Have two versions of MFront in the same environment

In `cmake`, the `TFEL_APPEND_SUFFIX` option will append the version
number to the names of:

- The executables.
- The libraries.
- The python modules. Note that, to comply with `python` restriction
  on module' names, the characters `.` and `-` are replace by `_` and
  that only the first level modules are affected.
- The directories in the `share` folder.

The headers are installed in a subforder named `TFEL-${TVEL_VERSION}`.

For example, if the `TFEL` version is `3.0.2-dev`, using
`TFEL_APPEND_SUFFIX` opion will generate:

- The `mfront-3.0.2-dev` executable.
- The `libTFELMaterial-3.0.2-dev.so` library.
- The `mtest_3_0_2_dev` `python` module.
- The `tfel_3_0_2_dev.material` `python` module. In this case, the
  second level (`material`) is not affected.

This allows multiple executables to be installed in the same
directory.

### The `tfel-config` tool

The `tfel-config` tool has been modified to take this
option appropriately. For example:

~~~~{.bash}
$ tfel-config-3.0.2-dev --includes
-I/tmp/install/include/TFEL-3.0.2-dev
$ tfel-config-3.0.2-dev --libs --material
-L/tmp/install/lib -lTFELMaterial-3.0.2-dev -lTFELMath-3.0.2-dev -lTFELUtilities-3.0.2-dev -lTFELException-3.0.2-dev
~~~~

### Modifications to `mfront`

`mfront` has been modified to call the appropriate `tfel-config`
executable.

For more details, see: <https://sourceforge.net/p/tfel/tickets/81/>

### An helper function for importing `TFEL` modules

~~~~{.python}
import os, importlib

def python_module_version(v):
    """Return the python module extension from the TFEL version"""
    return v.replace('.','_').replace('-','_')

def import_tfel_module(n,*v):
    """Try to import a tfel module, taking into account the TFEL_VERSION environment variable."""
    if not n:
        ImportError('Empty module name')
    m   = n.split('.')
    if not v:
        env = os.environ;
        if('TFEL_VERSION' in env):
            v = env.get('TFEL_VERSION')
            m[0] = m[0]+'_'+python_module_version(v)
    elif len(v)==1:
        m[0] = m[0]+'_'+python_module_version(v[0])
    else:
        ImportError('Invalid number of arguments')
    return importlib.import_module('.'.join(m))
~~~~

## Ticket #80: Adding the possiblity to compile a law using python module

`MFront` relies on external tools, called generators, for this step
(this semantic is the same than the one used by `cmake`).

The only available generator is currently the `Makefile` generator,
handled by the `MakefileGenerator` class. This generator can be used
as follows:

~~~~{.python}
import mfront
dsl = mfront.getDSL("Chaboche.mfront")

dsl.setInterfaces(['castem'])
dsl.analyseFile("Chaboche.mfront")
dsl.generateOutputFiles()

m = mfront.MakefileGenerator()
m.exe(dsl.getTargetsDescription())
~~~~

The `exe` methods provides several overloads and may accept an
additional argument of type `GeneratorOptions` which let the user
specifiy the optimisation level to be used.

For more details, see: <https://sourceforge.net/p/tfel/tickets/80/>

## Ticket #79: `Cyrano` interface: bug in `CyranoBehaviourHandler` class. Missing test.

The problem appeared when local substepping is enabled.

This feature was not tested for the `Cyrano` interface, which
explains why this regression has not been detected so far.

The problem was fixed and appropriate test were added.

For more details, see: <https://sourceforge.net/p/tfel/tickets/79/>

## Ticket #78: Fix transpose of `st2tost2` when using rvalue reference

The definition of the transpose of `st2tost2` was broken when the
argument was an rvalue reference.

For more details, see: <https://sourceforge.net/p/tfel/tickets/78/>

## Ticket #77: operators `*=` and `/=` are broken for fourth order tensors

The operators `*=` and `/=` are broken for fourth order tensors: the
`std::is_same` metafunction return value must be accessed with the
`value` member, not the `cond` member. This was a reminiscence an old
remove class called `tfel::meta::IsSame` which was removed in
`TFEL-3.0`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/77/>

## Ticket #75: Support for `python3` in the python interface

The code generated by mfront with the python interface is not
compatible with `python3`. The trouble is linked to changes in the
module declaration. See
<https://docs.python.org/3/howto/cporting.html> for details.

For more details, see: <https://sourceforge.net/p/tfel/tickets/75/>

## Ticket #73 AbaqusExplicit: wait for tasks termination when the `ThreadPool` threading model is used

The `ThreadPool` class now has a `wait` method that stops the main
thread up to tasks completion. This method is now called at the end of
the integration in the `AbaqusExplicit` interface when the
`ThreadPool` threading model is used.

For more details, see: <https://sourceforge.net/p/tfel/tickets/73/>

## Ticket #72: Error at compilation of documentation

The latex documents declared a unnecessary dependency to the
`stmaryrd` package. This package became outdated in recent `LaTeX`
distributions causing some portability issues. This dependency has
been removed.

The inclusion of the `babel` package was missing in the
`docs/presentations/mfront-formation-tp/tp.tex.in` file.

For more details, see: <https://sourceforge.net/p/tfel/tickets/72/>

## Ticket #69: `@Profiling` true; Error in cygwin

Under Windows, the `__declspec(dllexport)` attribute does not
propagate to inner structures, wheres the visibility flags used under
`POSIX` systems does. This attribute is now set also on the
`BehaviourProfiler::Timer` structure.

For more details, see: <https://sourceforge.net/p/tfel/tickets/69/>

## Ticket #70: `PLASTIC` + `MFRONT` in `CalculiX`

`CalculiX` provides two interfaces to `ABAQUS` `umat` behaviours. In
versions prior to 2.12, those interfaces are not fully compatible with
`ABAQUS` concerning various checks. The issue has been reported to
`CalculiX` developers.

To circumvent those issues, the `MFRONT_ABAQUS_NORUNTIMECHECKS` flag
has been added. This flag desactivates:

- checks related to the number of material properties (value of
  `NPROPS`)
- checks related to the number of state variables (value of `NSTATV`)
- checks related to usage of the behaviour in finite strain analysis
  (`KSTEP`)

All those checks may fail with the `ABAQUS` and `ABAQUSNL` interfaces
in `CalculiX` for reasons described in the previous posts. This flag
can be set as follows:

~~~~{.bash}
mfront --obuild --interface=abaqus -D MFRONT_ABAQUS_NORUNTIMECHECKS SaintVenantKirchhoffElasticityTotalLagrangian.mfront
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/70/>

## Ticket #67 A suffix to the material name is required (`abaqus` interface)

The material name in `Abaqus/Standard` and `Abaqus/Explicit` are now
expected to have the following form:
`LIBRARY_FUNCTION_HYPOTHESIS_IDENTIFIER`.

The last part (`_IDENTIFIER`) is optional.

For more details, see: <https://sourceforge.net/p/tfel/tickets/67/>

## Ticket #64: Change the library prefix to "cyg" under cygwin

The default library prefix under cygwin has been changed to `cyg`
(rather than `lib`) to follow the standard conventions of this
environment.

For more details, see: <https://sourceforge.net/p/tfel/tickets/84/>

## Ticket #63: Error cygwin (make)

Compiling `MFront` under `Cygwin` was broken because the `_GNU_SOURCE`
macro was not defined in the following headers:

- `mfront\tests\properties\test-cxx.cxx`
- `src\System\ProcessManager.cxx`
- `src\System\SignalManager.cxx`

For more details, see: <https://sourceforge.net/p/tfel/tickets/83/>

## Ticket #58: Fix initialisation of an array of parameters

For more details, see: <https://sourceforge.net/p/tfel/tickets/58/>

## Ticket #57: More robust detection of python versions: avoid mismatch between the python libraries, the python interpreter and the boost python library

For more details, see: <https://sourceforge.net/p/tfel/tickets/57/>

## Ticket #54: Bug in StensorFromTinyVectorView: standard assignement fails

For more details, see: <https://sourceforge.net/p/tfel/tickets/54/>

## Ticket #53: Add comments handling in parameters txt files

For more details, see: <https://sourceforge.net/p/tfel/tickets/53/>

## Ticket #52: More robust handling of parameters txt file

For more details, see: <https://sourceforge.net/p/tfel/tickets/52/>

## Ticket #20 Allow `MFront` to work under `Windows` without requiring `MSYS`

The documentation describing how to install `TFEL` on `Windows` has
been updated: The main difficulty associated with building
`TFEL/MFront` on [Windows plateform](http://windows.microsoft.com) is
the lack of an unified build environment which must be set up by the
user.

The installation on [Windows plateform](http://windows.microsoft.com)
is thus described in the following pages:

- [Installation based on `cmake` and `Visual Studio`](http://tfel.sourceforge.net/install-windows-VisualStudio.html).
- [Installation based on `cmake` and `MinGW` (as packaged with `Cast3M` `2017`)](http://tfel.sourceforge.net/install-windows-Cast3M2017.html). This
  tutorial can easily be adapted to other versions of `MinGW`, without
  requiring `Cast3M` to be installed.
- [Installation based on `MSYS`](http://tfel.sourceforge.net/install-windows-msys.html).

The two first solutions does not depend on `MSYS`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/20/>

## Ticket #17: Add physical constants from CODATA

Various physical constants are now available. The values are extracted from: <http://physics.nist.gov/cuu/Constants/index.htm>

|   Symbol  |       Description         |          Value               |        Unit            |
|-----------+---------------------------+------------------------------+------------------------|
| \(\mu\)   | Atomic mass constant      | \(1.660539040\,.10^{-27}\)   | \(kg\)                 |
| \(N_{a}\) | Avogadro constant         | \(6.022140857\,.10^{23}\)    | \(\mol^{-1}\)          |
| \(k_{b}\) | Boltzmann constant        | \(1.38064852\,.10^{-23} \)   | \(J.K^{-1}\)           |
| \(G_{0}\) | Conductance quantum       | \(7.7480917310\,.10^{-5}\)   | \(S\)                  |
| \(e_{0}\) | Electric constant         | \(8.854187817\,.10^{-12}\)   | \(F.m^{-1}\)           |
| \(m_{e}\) | Electron mass             | \(9.10938356\,.10^{-31} \)   | \(kg\)                 |
| \(eV\)    | Electron volt             | \(1.6021766208\,.10^{-19}\)  | \(J\)                  |
| \(e\)     | Elementary charge         | \(1.6021766208\,.10^{-19}\)  | \(C\)                  |
| \(F\)     | Faraday constant          | \(96485.33289\)              | \(C.\mol^{-1}\)        |
| \(a\)     | Fine-structure constant   | \(7.2973525664\,.10^{-3}\)   |                        |
| \(R\)     | Molar gas constant        | \(8.3144598\)                | \(J.\mol^{-1}.K^{-1}\) |
| \(s\)     | Stefan-Boltzmann constant | \(5.670367\,.10^{-8}\)       | \(W.m^{-2}.K^{-4}\)    |

For more details, see: <https://sourceforge.net/p/tfel/tickets/17/>

### `C++`

The physical constants are available as constexpr static members of
`tfel::PhysicalConstants` class defined in the
`TFEL/PhysicalConstants.hxx` header.

~~~~{.cpp}
using Cste   = tfel::PhysicalConstants<real>;
const auto R = Cste::R;
~~~~

### Python

The physical constants are available as members of the
`tfel.PhysicalConstants` class, as follows:

~~~~{.python}
from tfel import PhysicalConstants as Cste
print(Cste.Na)
~~~~

<!-- Local IspellDict: english -->
