% The MTest Python API
% Thomas Helfer
% 10/06/2016

This page describes the `mtest` module

# Using the MTest class

Here is a first example of the `MTest` class usage:

~~~~ {.python}
import std
import tfel.tests
from mtest import *

m = MTest()

umax = 3.e-6

setVerboseMode(VerboseLevel.VERBOSE_LEVEL2)
m.setPredictionPolicy(PredictionPolicy.LINEARPREDICTION)

m.setBehaviour("umat","src/libUmatBehaviour.so","umattvergaard")
m.setMaterialProperty('NormalStiffness',2.e16)
m.setMaterialProperty('TangentialStiffness',2.e16)
m.setMaterialProperty('MassDensity',0.)
m.setMaterialProperty('NormalThermalExpansion',0.)
m.setMaterialProperty('MaximumStress',200.e6)
m.setMaterialProperty('MaximumOpeningDisplacement',3.e-6)

m.setExternalStateVariable("Temperature",293.15)
m.setImposedOpeningDisplacement('Un',{0.:0.,1800.:umax/2,
				      2400.:0.,2600:-1.e-8,3000:0.,
				      3600.:umax})

m.setTimes([3.6*i for i in range(0,1001)])
m.setOutputFileName("castemtvergaard.res")
m.execute()
~~~~

This example shows that there is almost a one to one mapping between
the keywords used in `mtest` input file and the methods of the `MTest`
class.

The last instruction will execute all the time steps defined the time
discretization defined using the `setTimes` method.

This is not convenient and the `mtest` module allows you to precisely
define and manage how the computations are performed.

The three last line are indeed equivalent to:

~~~~ {.python}
s  = MTestCurrentState()
wk = MTestWorkSpace()

m.setOutputFileName("castemtvergaard.res")
m.completeInitialisation()
m.initializeCurrentState(s)
m.initializeWorkSpace(wk)

t = [3.6*i for i in range(0,1001)]
# values at 0
m.printOutput(t[0],s)

# do the job
for i in range(0,len(t)-1):
    m.setEvolutionValue('Un',t[i+1],umax*t[i+1]/t[-1])
    m.execute(s,wk,t[i],t[i+1])
    m.printOutput(t[i+1],s)
~~~~

The previous example introduces two classes: the `MTestWorkSpace`
class and the `MTestCurrentState` class.

The `MTestWorkSpace` is of no interest for the end-user. It only
manages to memory allocated to perform the computation: this memory is
allocated once when calling the `initializeWorkSpace` of the MTest
class.

The `MTestCurrentState` class will be discussed in depth in the next
section.

## The `MTestCurrentState` class

The `MTestCurrentState` is a wrapper class around the
`StudyCurrentState` class discussed later.

It has been introduced for backward compatibility with previous
versions of `TFEL` and have a simpler interface for studies handled by
the `MTest` class (one structure, one integration point..).

This structure contains the state of the (unique) integration point at
the beginning and at the end of the time step.

### Fields of the `MTestCurrentState` class

The `MTestCurrentState` class provides the following fields:

- `u_1`: values of the unknows (driving variables and lagrange
  multipliers) at the beginning of the previous time step. This
  information is used for extrapolation the unknowns evolution over
  the time step.
- `u0`: values of the unknows (driving variables and lagrange
  multipliers) at the beginning of the current time step.
- `u1`: values of the unknows (driving variables and lagrange
  multipliers) at the end of the current time step.
- `s_1`: values of the forces (thermodynamic forces and dual of the
  lagrange multipliers) at the beginning of the previous time step.
- `s1`: values of the forces (thermodynamic forces and dual of the
  lagrange multipliers) at the beginning of the end time step.
- `s0`: values of the forces (thermodynamic forces and dual of the
  lagrange multipliers) at the beginning of the current time step.
- `e0`: values of the driving variables at the beginning of the time
  step.
- `e1`: values of the driving variables at the end of the time step.
- `e_th0`: values of the thermal strain for small strain behaviours at
  the beginning of the current time step. Those values are not null
  only if the thermal strain is handled by `MTest` and not by the
  behaviour.
- `e_th1`: values of the thermal strain for small strain behaviours at
  the end of the time step. Those values are not null only if the
  thermal strain is handled by `MTest` and not by the behaviour.
- `mprops1`: values of the material properties at the end of the time
  step.
- `iv_1`: values of the internal state variables at the beginning of
  the previous time step.
- `iv0`: values of the internal state variables at the beginning of
  the current time step.
- `iv1`: values of the internal state variables at the end of the
  current time step.
- `evs0`: values of the external state variables at the beginning of
  the current time step.
- `desv`: values of the increment of the external state variables over
  the current time step.
- `dt_1`: value of the previous time increment.
- `Tref`: value of the reference temperature for the thermal strain
  computation.

The meaning of the driving variables depends on the type of behaviour
treated:

- for small strain behaviours, they are the components of the
  linearized strain tensor.
- for finite strain behaviours, they are the components of the
  deformation gradient tensor.
- for cohesive zone models, they are the components of the opening
  displacement vector.

The meaning of the thermodynamic forces also depends on the type of
behaviour treated:

- for small strain behaviours, they are the components of the
  stress tensor.
- for finite strain behaviours, they are the components of the
  Cauchy stress tensor gradient.
- for cohesive zone models, they are the components of the traction
  force vector.

### Methods of the `MTestCurrentState` class

The `MTestCurrentState` class provide the `copy` method to get a deep
copy of the structure. It is mandatory to use such a copy in some
advanced usage of the `MTest` class. This includes the following
cases:

- make appropriate time substepping in case of integration failure
- loading control based of the behaviour feed-back

# Using the `PipeTest` class

Here is an example:

~~~~ {.python}
import std
import tfel.tests
from mtest import PipeTest,StudyCurrentState,SolverWorkSpace, \
    setVerboseMode, VerboseLevel

setVerboseMode(VerboseLevel.VERBOSE_QUIET)

t = PipeTest()
# geometry and meshing
t.setInnerRadius(4.2e-3)
t.setOuterRadius(4.7e-3)
t.setNumberOfElements(10)
t.setElementType('Linear')

# modelling hypothesis
t.setPipeModellingHypothesis('None')
t.performSmallStrainAnalysis()

t.setTimes([0,1])
t.setInnerPressureEvolution(1.5e6)
t.setOuterPressureEvolution({0:1.5e6,1:10e6})

t.setBehaviour('castem','../../src/libUmatBehaviour.so','umatelasticity')
t.setMaterialProperty('YoungModulus',150e9)
t.setMaterialProperty('PoissonRatio',0.3)
t.setExternalStateVariable('Temperature',293.15)

t.execute()
~~~~

## The `StudyCurrentState` class

<!-- Local IspellDict: english -->
