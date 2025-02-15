---
title: The MTest Python API
author: Thomas Helfer
date: 16/08/2021
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This page describes the `mtest` module

# The `MaterialProperty` class

## Description

The `MaterialProperty` class handles a material property.

This class has the following core interface:

- `getVariablesNames`, which returns the names of the arguments of
  material property.
- `setVariableValue` which allows to set the value of an argument of the
  material property.
- `getValue` which evaluates the material property for the values of the
  arguments set by the `setVariableValue` method.
- `getParametersNames`, which returns the names of the parameters of
  material property.
- `setParameter` which allows to change the value of a parameter.

As described in the next paragraph, several convenient methods and
operators are also provided to simplify the usage of this class.

## Usage

Here is an example of the usage of the `MaterialProperty` class.

~~~~{.python}
import mtest
young_modulus = mtest.MaterialProperty(
    'src/libCastemVanadium.so', 'VanadiumAlloy_YoungModulus_SRMA')
~~~~

Note that the `MaterialProperty` constructor automatically detects the
interface used to generate the material property and instantiates the
proper concrete implementation internally. In this case, an instance of
the `CastemMaterialProperty` class is instanciated.

The arguments of the material property can then be set and the material
property can be evaluated:

~~~~{.python}
young_modulus.setVariableValue('Temperature', 562)
E = young_modulus.getValue()
~~~~

Setting the variables' values and evaluating the material property can
be tedious. To avoid this, overloaded versions of the `getValue` are
available:

~~~~{.python}
# using a dictionary
E = young_modulus.getValue({'Temperature': 562})
# for material properties with only one argument
E = young_modulus.getValue(562)
~~~~

To make the code even more explicit, the call operator can also be used,
as follows:

~~~~{.python}
# using a dictionary
E = young_modulus({'Temperature': 562})
# for material properties with only one argument
E = young_modulus(562)
~~~~

# The `Behaviour` class

## Rationale

The `Behaviour` class retrieves information in a shared library about
a behaviour specific implementation.

Contrary the `tfel.system.ExternalBehaviourDescription` class, the
information given by the `Behaviour` class takes into account the
variables that are implicitly declared by the interface to match its
specific (internal) requirements.

For example, if a behaviour requires the stiffness tensor to be given
by the calling code (see the `@RequireStiffnessTensor` `MFront`'
keyword for details), additional material properties are provided to
the behaviour. The number of those additional material properties may
depend on the modelling hypothesis (mostly for orthotropic
behaviours): the calling code generally only not provides the minimal
number of material properties needed, in order to reduce the cost of
handling those extra material properties.

One can give some other examples:

- If the `@AsterSaveTangentOperator` is used in `MFront`, the
  consistent tangent operator will be saved as additional state
  variables.
- If the `@AbaqusOrthotropyManagementPolicy` is used to set the
  orthotropy management policy to "`MFront`" in `Abaqus/Standard` and
  `Abaqus/Explicit`, the definition of the orthotropic axes must be
  given in additional state variables.
- Most of time, support for the plane stress hypothesis defines an
  additional state variable, the axial strain (see for example the
  `StandardElasticity` brick). This variable is only defined for this
  specific modelling hypothesis.

## Usage

~~~~{.python}
import mtest
b = mtest.Behaviour('aster','libAsterBehaviour.so','asterplasticity','PlaneStrain')
~~~~~

## Methods available

The following methods are available:

- `getBehaviourType`: Return the behaviour type. The value returned
  has the following meaning:
    - `0`: general behaviour.
    - `1`: small strain behaviour.
    - `2`: finite strain behaviour.
    - `3`: cohesive zone model.
- `getBehaviourKinematic`: Return the behaviour kinematic. The value
  returned has the following meaning:
    - `0`: undefined kinematic.
    - `1`: standard small strain behaviour kinematic.
    - `2`: cohesive zone model kinematic.
    - `3`: standard finite strain kinematic (F-Cauchy).
    - `4`: `PTest` finite strain kinematic (eto-pk1) (see
      @helfer_extension_2015).
- `getDrivingVariablesSize`: Return the size of a vector able to
	 contain all the components of the driving variables.
- `getThermodynamicForcesSize`: Return the size of a vector able to
	 contain all the components of the thermodynamic forces.
- `getStensorComponentsSuffixes`: Return the components suffixes of a
  symmetric tensor.
- `getVectorComponentsSuffixes`: Return the components suffixes of a
  vector.
- `getTensorComponentsSuffixes`: Return the components suffixes of a
  tensor.
- `getDrivingVariablesComponents`: Return the components of the
  driving variables.
- `getThermodynamicForcesComponents`: Return the components of the
  thermodynamic forces.
- `getDrivingVariableComponentPosition`: Return the position of the
  component of a driving variable.
- `getThermodynamicForceComponentPosition`: Return the position of the
  component of a thermodynamic force.
- `getSymmetryType`: Return the symmetry of the behaviour:
    - `0` means that the behaviour is isotropic.
    - `1` means that the behaviour is orthotropic.
- `getMaterialPropertiesNames`: return the names of the material
  properties.
- `getInternalStateVariablesNames`: Return the names of the internal
  state variables.
- `expandInternalStateVariablesNames`: Return the names of the
  internal state variables, taking into account the suffixes for
  vectors, symmetric tensors and tensors.
- `getInternalStateVariablesSize`: Return the the size of the array of
  internal variables.
- `getInternalStateVariablesDescriptions`: Return the descriptions the
  internal variables.
- `getInternalStateVariableType`: Return the type of an internal
  variable:
    - `0` means that the internal state variable is a scalar.
    - `1` means that the internal state variable is a symmetric tensor.
    - `3` means that the internal state variable is a tensor.
- `getInternalStateVariablePosition`: Return the internal state
  variable position.
- `getExternalStateVariablesNames`: Return the names of the external
  state variables.
- `getParametersNames`: Return the names of the floating point
  parameters.
- `getIntegerParametersNames`: Return the names of the integer
  parameters.
- `getUnsignedShortParametersNames`: Return the names of the unsigned
  short parameters.

# The `MTest` class

Here is a first example of the `MTest` class usage:

~~~~ {.python}
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

- `u_1`: values of the unknowns (driving variables and lagrange
  multipliers) at the beginning of the previous time step. This
  information is used for extrapolation the unknowns evolution over
  the time step.
- `u0`: values of the unknowns (driving variables and lagrange
  multipliers) at the beginning of the current time step.
- `u1`: values of the unknowns (driving variables and lagrange
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
- `period`: total number of time steps done so far.
- `iterations`: total number of iterations done so far.
- `subSteps`: total number of sub steps done so far.
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

The `MTestCurrentState` also provides `getInternalStateVariableValue`
and `setInternalStateVariableValue` methods, which are described more
in depth below.

# The `PipeTest` class

Here is an example:

~~~~ {.python}
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
t.setAxialLoading('None')
t.performSmallStrainAnalysis()

t.setTimes([0,1])
t.setInnerPressureEvolution(1.5e6)
t.setOuterPressureEvolution({0:1.5e6,1:10e6})

t.setBehaviour('castem','../../src/libUmatBehaviour.so','umatelasticity')
t.setMaterialProperty('YoungModulus',150e9)
t.setMaterialProperty('PoissonRatio',0.3)
t.setExternalStateVariable('Temperature',293.15)

t.setOutputFileName("pipe.res")

t.execute()
~~~~

## The `StudyCurrentState`, `StructureCurrentState` and `CurrentState` classes

### The `StudyCurrentState` class

As with the `MTest` class, the user can have precise control of how
the computations are performed. The last instruction of the previous
example can be changed by the following code:

~~~~ {.python}
s  = StudyCurrentState()
wk = SolverWorkSpace()

t.completeInitialisation()
t.initializeCurrentState(s)
t.initializeWorkSpace(wk)

# values at 0
t.printOutput(0,s)
# resolution from 0 to 1
t.execute(s,wk,0.,1.)
# values at 1
t.printOutput(1.,s)
~~~~

This code introduces two new classes: `SolverWorkSpace` and
`StudyCurrentState`. Only the latter is interesting for the end user.

The `StudyCurrentState` has been designed to encapsulate all the test
cases handled by `MTest`, in the current and future versions of
`TFEL`.

The `StudyCurrentState` provides the following fields:

- `u_1`: values of the unknowns (driving variables and lagrange
  multipliers) at the beginning of the previous time step. This
  information is used for extrapolation the unknowns evolution over
  the time step.
- `u0`: values of the unknowns (driving variables and lagrange
  multipliers) at the beginning of the current time step.
- `u1`: values of the unknowns (driving variables and lagrange
  multipliers) at the end of the current time step.
- `u10`: values of the unknowns (driving variables and lagrange
  multipliers) at the previous iteration.
- `period`: total number of time steps done so far.
- `iterations`: total number of iterations done so far.
- `subSteps`: total number of sub steps done so far.
- `dt_1`: value of the previous time increment.

Some of those fields are the same than those described earlier when
discussing the `MTestCurrentState` class. Indeed, the
`MTestCurrentState` is just a wrapper around the `StudyCurrentState`.

In the case of pipes, the unknowns are the radial displacements of the
nodes of the pipe mesh and the axial displacement. There are no
Lagrange multipliers.

### The `StructureCurrentState` class

A study may contains one or several structures. The current state of
those structures are handled by the `StructureCurrentState` class
which can be retrieved from a `StudyCurrentState` object using the
`getStructureCurrentState` method. This latter method takes the name
of the structure as argument.

In the case of pipes, only one *unnamed* structure is defined (See the
example below).

The `StructureCurrentState` has mostly one field: `istates` which
contains `CurrentState` objects describing each integration
points.

For a pipe, integration points in the `istates` array are ordered
using their position from the closest to the inner radius of the pipe
to the closest to the outer radius of the pipe.

### The `CurrentState` class

The `CurrentState` class contains the values at integration points. It
contains the following fields:

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

Those fields are the same than those of the `MTestCurrentState` which
indeed acts as a proxy for the unique integration point it handles.

Here is an example of use of the `StudyCurrentState` class:

~~~~ {.python}
# get the structure current state
scs = s.getStructureCurrentState("")
# iterate of over the integration points
for cs in scs.istates:
    #print the radial stress, the axial stress, the hoop stress
    print(str(cs.s1[0])+" "+str(cs.s1[1])+" "+str(cs .s1[1]))
~~~~

#### Accessing the values of the internal state variables

The internal state variables are stored in the `iv_1`, `iv0` and `iv1`
fields. Beware that access to those fields creates a copy of the
underlying array of values.

The `CurrentState` also provides a method called
`getInternalStateVariableValue` which has two overloads.

The first one takes the name of a variable and returns its value at
the end of the time step.

The second one has one more argument: the depth of the values to be
accessed:

- `-1` means that we are modifying the internal state variable
	value at the beginning of the previous time step
-  `0` means that we are modifying the internal state variable
       value at the beginning of the current time step
-  `1` means that we are modifying the internal state variable
      value at the end of the current time step

#### Initial values of the internal state variables

The initial values of the internal state variables can be specified by
the `setInternalStateVariableInitialValue` method of the `MTest` or
`PipeTest` class. This method takes the name of internal state
variable and a `list` of `floats` (a `float` is accepted for
scalar internal state variables).

The values are stored in the `MTest` or `PipeTest` object and will be
used every time the `initializeCurrentState` method is called.

Here is an example of use of the `getInternalStateVariableValue` method:

~~~~ {.python}
scs = s.getStructureCurrentState("")
# get the maximum of the equivalent plastic strain over the integration points
p = max([cs.getInternalStateVariableValue('EquivalentPlasticStrain') for cs in scs.istates])
~~~~

#### Modifying the values of the internal state variables

Sometimes, it can be useful to modify the values of the internal
state variables during the computation. This can

The internal state variables are stored in the `iv_1`, `iv0` and `iv1`
fields. Access to those variables creates a copy of the underlying
array of values. Thus, modifying those values has no effect of the
`CurrentState` object.

To modify the values of a `CurrentState`, one may of the two overloads
of the `setInternalStateVariableValue` method:

- the first one takes the name of the state variable to be modified,
  the new values as a `list` of `floats` (a `float` is accepted for
  scalar internal state variables). The value given is affected to
  internal state variable values at the beginning of the previous time
  step (`iv_1` field), the beginning of the current time step (`iv0`
  field) and the end of the current time step (`iv1` field).
- the second one as one more argument than the first overload: the
  depth of the values to be modified:
    - `-1` means that we are modifying the internal state variable
      value at the beginning of the previous time step
    -  `0` means that we are modifying the internal state variable
      value at the beginning of the current time step
    -  `1` means that we are modifying the internal state variable
      value at the end of the current time step

Here is an example of use of the `setInternalStateVariableValue`
method:

~~~~ {.python}
scs = s.getStructureCurrentState("")
# iterate of over the integration points
for cs in scs.istates:
    # set the value of the equivalent plastic strain
    cs.setInternalStateVariableValue('EquivalentPlasticStrain',2.e-2)
~~~~

### Example of use of the `PipeTest` class

~~~~ {.python}
from mtest import PipeTest,StudyCurrentState,SolverWorkSpace, \
    setVerboseMode, VerboseLevel

# no output
setVerboseMode(VerboseLevel.VERBOSE_QUIET)

Re = 4.18e-3
Fz = 3.1415926*Re*Re*20e6

t = PipeTest()

# geometric and meshing options
t.setInnerRadius(0)
t.setOuterRadius(Re)
t.setNumberOfElements(1)
t.setElementType('Quadratic')

# Type of loading
t.setAxialLoading('ImposedAxialForce')

# no geometrical update
t.performSmallStrainAnalysis()

# by default, with the 'castem' interface, one does not use the
# consistent tangent operator. Using the acceleration algorithm
# is mandatory for convergence.
t.setUseCastemAccelerationAlgorithm(True)

# axial loading
t.setAxialForceEvolution({0.:0.,1.e-6 :Fz,3600:Fz})

# definition of the mechanical behaviour and the material properties
t.setBehaviour('castem','../../src/libUmatBehaviour.so','umatnorton')
t.setMaterialProperty('YoungModulus',150e9)
t.setMaterialProperty('PoissonRatio',0.3)
t.setMaterialProperty('A',8.e-67)
t.setMaterialProperty('E',8.2)

# temperature (mandatory)
t.setExternalStateVariable('Temperature',293.15)

# results file
t.setOutputFileName('pipe4.res')

s  = StudyCurrentState()
wk = SolverWorkSpace()

# complete initialisation of the problem
t.completeInitialisation()
t.initializeCurrentState(s)
t.initializeWorkSpace(wk)

# time discretization
times = [0]+[1.e-6+((3600.-1.e-6)/20.)*i for i in range(21)];

# get the state of the pipe (unique structure defined)
scs = s.getStructureCurrentState("")
p   = max([cs.getInternalStateVariableValue("EquivalentViscoplasticStrain")
           for cs in scs.istates])
print(str(times[0])+" "+str(p))

# loop over the time steps
for i in range(0,len(times)-1):
	# search the mechanical equilibrium at the end of the time step
	t.execute(s,wk,times[i],times[i+1])
	# write in the output file
    t.printOutput(times[i+1],s)
    # iterate of over the integration points
    p = max([cs.getInternalStateVariableValue("EquivalentViscoplasticStrain")
             for cs in scs.istates])
    print(str(times[i+1])+" "+str(p))
~~~~

# References

<!-- Local IspellDict: english -->
