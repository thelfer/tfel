% `MTest` keywords


# The `@AccelerationAlgorithm` keyword

The `@AccelerationAlgorithm` keyword is used to give the name of the
acceeleration algorithm used to accelerate the convergence toward
equilibrium.

The follwing algorithm are available:

- `Cast3M`
- `Secant`
- `Steffensen`
- `Irons-Tuck`

## Example:

~~~~~{.cpp}
@AccelerationAlgorithm 'Secant';
~~~~~

# The `@AccelerationAlgorithmParameter` keyword

The `@AccelerationAlgorithmParameter` keyword allows the user to
fine-tune the acceleration algorithm used to reach equilibrium. The
parameter that can be defined depends on the acceleration algorithm
used.

This keyword is followed by a string giving the name of the parameter
and a *string* giving the value of this parameter.

## Example

~~~~~{.cpp}
@AccelerationAlgorithmParameter 'AccelerationTrigger' '3'; 
~~~~~



# The `@Author` keyword

The `@Author` keyword is used give the name of the person who wrote
the mtest file.

All the following words are appended to the author's name up to a
final semi-colon.

## Example:

~~~~ {.cpp}
@Author Éric Brunon;
~~~~~~~~

# The `@Behaviour` keyword

The `@Behaviour` keyword declares the behaviour used for the
test. This keyword must be followed by an option specifying the
interface used by the behaviour. The `castem`, `cyrano` and `aster`
interfaces are supported.

Two strings are then expected:

- the library in which the behaviour is implemented;
- the name of the function.

## Example

~~~~ {.cpp}
@Behaviour<castem> 'libMFrontCastemBehaviours.so' 'umatnorton';
~~~~~~~~

# The `@CastemAccelerationPeriod` keyword

The keyword `@CastemAccelerationPeriod` specifies the number of
iterations between two calls to the acceleration algorithm.

This keyword is followed by a positive number.

## Example

~~~~ {.cpp}
@CastemAccelerationPeriod 3;
~~~~~~~~

# The `@CastemAccelerationTrigger` keyword

The keyword `@CastemAccelerationTrigger` specifies the number of
iterations before the first call to the acceleration algorithm.

This keyword is followed by a positive number.

## Example

~~~~ {.cpp}
@CastemAccelerationTrigger 4;
~~~~~~~~

# The `@CohesiveForce` keyword

The `@CohesiveForce` let the user specifiy the inital value of the
cohesive forces.

This keyword is followed by an array. The size of this array must be
equal to the number of components of symmetric tensors for the
modelling hypothesis considered (see the `@ModellingHypothesis`
keyword).

## Example

~~~~ {.cpp}
@CohesiveForce {'T0,,0.,0.};
~~~~

# The `@CohesiveForceEpsilon` keyword

The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on opening displacements, the other on
cohesive forces.

This criterium on cohesive forces checks that the residual of the
Newton algorithm is low enought. By default, this value is equal to
1.e-3, which is a convenient value if cohesive force are expressed in
Newton.

The `@CohesiveEpsilon` let the user specifiy the criterium value used
for the criterium on cohesive force.

It is followed by the criterium value.

## Example

~~~~~ {.cpp}
@CohesiveForce 1.e2;
~~~~~~~~~~

# The `@CompareToNumericalTangentOperator` keyword

If the `@CompareToNumericalTangentOperator` is followed by `true`, a
comparison of the tangent operator given by the behaviour to a
numerical approximation computed by a centered finite difference is
performed.

The value of the perturbation used to compute this numerical
approximation can be adjusted by the
`@NumericalTangentOperatorPerturbationValue`.

The value used for to compare the tangent operator given by the behaviour to a
numerical approximation can be adjusted by the
`@TangentOperatorComparisonCriterium`.

## Example

~~~~ {.cpp}
@CompareToNumericalTangentOperator true;
~~~~~~~~

# The `@Date` keyword

The `@Date` keyword allows the user to precise when the mtest file was
written.

All the following words are appended to the date up to a final
semi-colon.

## Example

~~~~ {.cpp}
@Date 2008-11-17;
~~~~~~~~

# The `@DeformationGradient` keyword

The `@DeformationGradient` let the user specifiy the inital value of
the deformation gradient.

This keyword is followed by an array. The size of this array must be
equal to the number of components of (non symmetric) tensors for the
modelling hypothesis considered (see the `@ModellingHypothesis`
keyword).

## Example

~~~~~ {.cpp}
@DeformationGradient {1,1,1,0,0,0,0,0,0};
~~~~~~~~~~

# The `@DeformationGradientEpsilon` keyword

The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on deformation gradient, the other on
stresses.

This criterium on the deformation gradient checks if two successives
estimate of the strains are close enough.  By default, the criterium
value used is 1.e-12;

The `@SDeformationGradient` let the user specifiy the criterium value used
for the criterium on strains.

It is followed by the criterium value.

## Example

~~~~ {.cpp}
@DeformationGradient 1.e-12;
~~~~~~~~

# The `@Description` keyword

The `@Description` keywords describes the test which is implemented in
a mtest file.

This keyword is followed by a block containing all the relevant piece
of information.

## Example

~~~~ {.cpp}
@Description
{
  "Modélisation d'un tube sous pression"
}
~~~~~~~~

# The `@DrivingVariable` keyword

The keyword `@DrivingVariable` is not documented yet

# The `@DrivingVariableEpsilon` keyword

The keyword `@DrivingVariableEpsilon` is not documented yet

# The `@Evolution` keyword

The `@Evolution` specifies a function of time.

This keyword may have one option, which is the way the evolution will
be defined. Two values are accepted: `evolution` and `function`. If no
option is specified, the `evolution` option is choosen.

After the option, the name of evolution, which shall be given as
string, is expected.

If the `evolution` option has been selected, the user may specify a
constant evolution by simply giving its value. Otherwise, complex
evolutions can be build using a associative array where the key is the
time and the value the value of the evolution. Between two times, the
values will be interpolated linearly. Prior the first declared time
given, the value corresponding the this first time is used. After the
last time given, the value corresponding this last time is used.

If the `function` option has been selected, a string is expected which
wil be interpreted as a function of time. The time is represented by
the variable `t`.

## Example (constant evolution):

~~~~ {.cpp}
@Evolution 'Pressure' 1.e5;
~~~~~~~~

## Example (linear evolution):

~~~~ {.cpp}
@Evolution 'Pressure' {0:0.,1.:1.e5};
~~~~~~~~

## Example (function):

~~~~ {.cpp}
@Real P0 2.e5
@Real P1 4.e5
@Evolution<function> 'Pressure' 'P0+(P1-P0)*t**2';
~~~~~~~~


# The `@ExternalStateVariable` keyword

The `@ExternalStateVariable` keyword let the user specify the
evolution of an external state variables, including the temperature
which is generally defined by default by behaviour interfaces.

This keyword may have one option, which is the way the evolution will
be defined. Two values are accepted: `evolution` and `function`. If no
option is specified, the `evolution` option is choosen.

After the option, the name of the external state variable, which shall
be given as string, is expected.

If the `evolution` option has been selected, the user may specify a
constant evolution by simply giving its value. Otherwise, complex
evolutions can be build using a associative array where the key is the
time and the value the value of the evolution. Between two times, the
values will be interpolated linearly. Prior the first declared time
given, the value corresponding the this first time is used. After the
last time given, the value corresponding this last time is used.

If the `function` option has been selected, a string is expected which
wil be interpreted as a function of time. The time is represented by
the variable `t`.

## Example (constant evolution)

~~~~ {.cpp}
@ExternalStateVariable 'Temperature' 293.15;
~~~~~~~~

## Example (linear evolution)

~~~~ {.cpp}
@ExternalStateVariable 'Temperature' {0:293.15,1.:800};
~~~~~~~~

## Example (function)

~~~~ {.cpp}
@Real T0 293.15
@Real T1 400.15
@ExternalStateVariable<function> 'Temperature' 'T0+(T1-T0)*t**2';
~~~~~~~~


# The `@HandleThermalExpansion` keyword

The keyword `@HandleThermalExpansion` is not documented yet

# The `@ImposedCohesiveForce` keyword

The keyword `@ImposedCohesiveForce` is not documented yet

# The `@ImposedDeformationGradient` keyword

The keyword `@ImposedDeformationGradient` is not documented yet

# The `@ImposedDrivingVariable` keyword

The keyword `@ImposedDrivingVariable` is not documented yet

# The `@ImposedOpeningDisplacement` keyword

The keyword `@ImposedOpeningDisplacement` is not documented yet

# The `@ImposedStrain` keyword

The `@ImposedStrain` keyword allows the user to impose the evolution
of a component of the strains.

This keyword may have one option, which is the way the evolution will
be defined. Two values are accepted: `evolution` and `function`. If no
option is specified, the `evolution` option is choosen.

After the option, the name of the component of the strains, which
shall be given as string, is expected.

According to the modelling hypothesis selected (see the
`@ModellingHypothesis` keyword), the following components names are
accepted:

- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT
- Axisymmetrical                       : ERR EZZ ETT ERZ
- PlaneStress                          : EXX EYY EZZ EXY
- PlaneStrain                          : EXX EYY     EXY
- GeneralisedPlaneStrain               : EXX EYY EZZ EXY
- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ

If the `evolution` option has been selected, the user may specify a
constant evolution by simply giving its value. Otherwise, complex
evolutions can be build using a associative array where the key is the
time and the value the value of the evolution. Between two times, the
values will be interpolated linearly. Prior the first declared time
given, the value corresponding the this first time is used. After the
last time given, the value corresponding this last time is used.

If the `function` option has been selected, a string is expected which
is interpreted as a function of time. The current time is represented
by the variable `t`.

## Example (constant evolution)

~~~~ {.cpp}
@ImposedStrain<evolution> 'EXX' 1e-3;
~~~~~~~~

## Example (linear evolution)

~~~~ {.cpp}
@ImposedStrain<evolution> 'EXX' {0.:0.,1:1e-3};
~~~~~~~~

## Example (function)

~~~~ {.cpp}
@ImposedStrain<function> 'EXX' 'e0*sin(t/900.)';
~~~~~~~~

## Note

The off-diagonal terms shall be multiplied by a factor \(\sqrt(2)\)
with respect to standard conventions.

# The `@ImposedStress` keyword

The `@ImposedStress` keyword allows the user to impose the evolution
of a component of the stresses.

This keyword may have one option, which is the way the evolution will
be defined. Two values are accepted: `evolution` and `function`. If no
option is specified, the `evolution` option is chosen.

After the option, the name of the component of the stresses, which
shall be given as string, is expected.

According to the modelling hypothesis selected (see the
`@ModellingHypothesis` keyword), the following components names are
accepted:

- AxisymmetricalGeneralisedPlaneStrain : SRR SZZ STT
- Axisymmetrical                       : SRR SZZ STT SRZ
- PlaneStress                          : SXX SYY     SXY
- PlaneStrain                          : SXX SYY SZZ SXY
- GeneralisedPlaneStrain               : SXX SYY SZZ SXY
- Tridimensional                       : SXX SYY SZZ SXY SXZ SYZ

If the `evolution` option has been selected, the user may specify a
constant evolution by simply giving its value. Otherwise, complex
evolutions can be build using a associative array where the key is the
time and the value the value of the evolution. Between two times, the
values will be interpolated linearly. Prior the first declared time
given, the value corresponding the this first time is used. After the
last time given, the value corresponding this last time is used.

If the `function` option has been selected, a string is expected which
is interpreted as a function of time. The current time is represented
by the variable `t`.

## Example (constant evolution)

~~~~ {.cpp}
@ImposedStress 'SXX' 50.e6;
~~~~~~~~

## Example (linear evolution)

~~~~ {.cpp}
@ImposedStress<evolution> 'SXX' {0.:0.,1:50e6};
~~~~~~~~

## Example (function)

~~~~ {.cpp}
@ImposedStress<function>  'SXX' 's0*sin(t/900.)';
~~~~~~~~

## Note

The off-diagonal terms shall be multiplied by a factor \(\sqrt(2)\).

# The `@ImposedThermodynamicForce` keyword

The keyword `@ImposedThermodynamicForce` is not documented yet

# The `@IntegerParameter` keyword

The `@IntegerParameter` keyword specifies the value of an integer
parameter of the behaviour.

## Example

~~~~ {.cpp}
@IntegerParameter 'iter' 12;
~~~~~~~~


# The `@InternalStateVariable` keyword

The `@InternalStateVariable` keyword defines the inital value of a
state variable.

This keyword must be followed by the name of the internal state
variable considered, given as a string.

If this internal state variable is scalar, a real value is then
expected.

If this internal state variable is a symmetric tensor, an array of
real values of the appropriate size is expected. A symmetric tensor
has 3 components in 1D, 4 components in 2D and 6 components in 3D (see
the `@ModellingHypothesis` keyword). The off-diagonals components
shall be declared with a \(\sqrt(2)\) factor.

## Example

~~~~ {.cpp}
@InternalStateVariable 'ElasticStrain' {'EELRR0','EELZZ0','EELZZ0',0.};
~~~~~~~~

# The `@MaterialProperty` keyword

The `@MaterialProperty` keyword defines a material property.  Only the
mechanical properties defined by the behaviour for the modelling
hypothesis considered may be used with the noticeable exception of the
thermal expansions:

- isotropic thermal expansion is defined through the
  `ThermalExpansion` property
- orthotropic thermal expansion is defined through the
  `ThermalExpansion1`, `ThermalExpansion2` and `ThermalExpansion3`
  material properties.

This keyword takes the type of the material property as an option.
Three types of material properties are supported, `constant` or
`castem` or function.

The name of the material property shall then be given.

If the material property is constant, its value is expected.

If the material property is of the `castem` type, the name of the
library and the name of the function implementing the material
property are expected.

If the material property is a function, the function definition is
expected.

## Example (constant)

~~~~{.cpp}
@MaterialProperty<constant> 'YoungModulus' 150.e9;
~~~~~~~~

## Example (castem)

~~~~{.cpp}
@MaterialProperty<castem> 'YoungModulus' 'libInconel600MaterialProperties.so' 'Inconel600_YoungModulus';
~~~~~~~~

# The `@MaximumNumberOfIterations` keyword

The `@MaximumNumberOfIterations` keyword let the user specify the
maximum number of iterations of the global algorithm to reach the
equilibrium.

This keyword is followed by an integer.

If the number of iterations reaches the maximal authorized value, the
time step is divided by two. The maximum number of sub-steps can be
specified using the `@MaximumNumberOfSubSteps` keyword.

## Example

~~~~ {.cpp}
@MaximumNumberOfIterations 10;
~~~~~~~~

# The `@MaximumNumberOfSubSteps` keyword

The `@MaximumNumberOfSubSteps` keyword let the user specify the
maximum number of sub steps allowed.

This keyword is followed by an integer.

When the global algorithm fails to reach equilibrium (see the
`@MaximumNumberOfIterations` keyword), the time step can be divided by
two. The maximal number of times the time step is reduced if given by
maximum number of sub steps.

## Example

~~~~ {.cpp}
@MaximumNumberOfSubSteps 10;
~~~~~~~~

# The `@ModellingHypothesis` keyword

The `@ModellingHypothesis` keyword allows the user to choose the
modelling hypothesis to use. The keywords shall be one of the first
keyword of the file: if not, some keywords, which requires the
modelling hypothesis to be defined, may force the default modelling
hypothesis to be used (`Tridimensional`).

This keyword is followed by the desired modelling hypothesis given as
a string. The following values are currently supported:

- AxisymmetricalGeneralisedPlaneStrain (1D)
- Axisymmetrical         (2D)
- PlaneStress            (2D)
- PlaneStrain            (2D)
- GeneralisedPlaneStrain (2D)
- Tridimensional         (3D)

The modelling hypothesis changes the name of the components of
symmetric tensors and their numbers. For example, the components of
the strains are:

- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT
- Axisymmetrical                       : ERR EZZ ETT ERZ
- PlaneStress                          : EXX EYY EZZ EXY
- PlaneStrain                          : EXX EYY EZZ EXY
- GeneralisedPlaneStrain               : EXX EYY EZZ EXY
- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ

# The `@NumericalTangentOperatorPerturbationValue` keyword

The `@NumericalTangentOperatorPerturbationValue` keyword can be used
to adjust the value use to perform a numerical approximation of the
tangent operator. This keyword followed by the value to be used.

## Example

~~~~ {.cpp}
@NumericalTangentOperatorPerturbationValue 1.e-9;
~~~~~~~~

# The `@OpeningDisplacement` keyword

The keyword `@OpeningDisplacement` is not documented yet

# The `@OpeningDisplacementEpsilon` keyword

The keyword `@OpeningDisplacementEpsilon` is not documented yet

# The `@OutputFile` keyword

The `@OutputFile` keywords specifies the name of the output file. By
default, the name of output file is egal to the name of the input
file, minus the `.mtest` extension if present, plus `.res`.

## Example

~~~~ {.cpp}
@OutputFile 'results.txt';
~~~~~~~~

# The `@OutputFilePrecision` keyword

The `@OutputFilePrecision` specify the number of digits used to print
the results in the output file.

This keyword is followed by the number of digits wanted.

## Example

~~~~ {.cpp}
@OutputFilePrecision 15;
~~~~~~~~

# The `@Parameter` keyword

The `@Parameter` keyword specifies the value of a parameter of the
behaviour.

## Example

~~~~ {.cpp}
@Parameter 'espilon' 1.e-8;
~~~~~~~~


# The `@PredictionPolicy` keyword

The `@PredictionPolicy` keyword let the user how the initial estimate
of the solution shall be obtained.

This keyword is followed by a string. The following values are
allowed:

- NoPrediction
- LinearPrediction
- ElasticPrediction
- ElasticPredictionFromMaterialProperties
- SecantPrediction
- TangentPrediction

## Example

~~~~ {.cpp}
@PredictionPolicy 'ElasticPrediction';
~~~~~~~~

## Note

The `ElasticPredictionFromMaterialProperties` policy is only supported
by the `castem` interface as the `Cast3M` finite element solver
requires the stiffness matrix to be defined.


# The `@Real` keyword

The `@Real` keyword let the user define a constant.

This keyword is followed by the name of the constant, as a string and
by the value of the constant.

## Exemple

~~~~ {.cpp}
@Real 'SXX0' 20.6;
~~~~

# The `@ResidualFile` keyword

The keyword `@ResidualFile` is not documented yet

# The `@ResidualFilePrecision` keyword

The keyword `@ResidualFilePrecision` is not documented yet

# The `@RotationMatrix` keyword

The `@RotationMatrix` let the user specifiy a rotation matrix, so that
the principal directions of the material are different from those used
for the resolution and the boundary conditions.

This keyword is followed by an array of array reprensentig a 3x3
matrix.

## Example

~~~~ {.cpp}
@RotationMatrix {{0,1,0},
                 {1,0,0},
                 {0,0,1}};
~~~~~~~~

# The `@StiffnessMatrixType` keyword

The `@StiffnessMatrixType` keyword let the user specifiy choose type
of stiffness matrix that shall be given by the mechanical behaviour
and that will be used by the resolution algorithm.

This keyword is followed by a string. The following values are
allowed:

- Elastic (undammaged)
- SecantOperator (damaged)
- TangentOperator
- ConsistentTangentOperator

## Example

~~~~ {.cpp}
@StiffnessMatrixType 'Elastic';
~~~~~~~~



# The `@StiffnessUpdatePolicy` keyword

This keyword is not yet implemented.


# The `@Strain` keyword

The `@Strain` let the user specifiy the inital value of the strains.

This keyword is followed by an array. The size of this array must be
equal to the number of components of symmetric tensors for the
modelling hypothesis considered (see the `@ModellingHypothesis`
keyword).

## Note

The values must follow the `TFEL` conventions. In \(3D\), strains are
stored in the following order:
\[
\left(
varepsilon_{xx},\varepsilon_{yy},\varepsilon_{zz},\sqrt{2}\,\varepsilon_{xy},\sqrt{2}\,\varepsilon_{xz},\sqrt{2}\,\varepsilon_{yz}
\right)
\]

## Example

~~~~ {.cpp}
@Strain {0.000239466253465591,
         -7.18398760396772e-05,
         -7.18398760396772e-05,
         0.,0.,0.};
~~~~~~~~~~~

# The `@StrainEpsilon` keyword

The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on strains, the other on stresses.

This criterium on strains checks if two successives estimate of the
strains are close enough.  By default, the criterium value used is
1.e-12;

The `@StrainEpsilon` let the user specifiy the criterium value used
for the criterium on strains.

It is followed by the criterium value.

## Example

~~~~ {.cpp}
@StrainEpsilon 1.e-12;
~~~~~~~~

# The `@Stress` keyword

The `@Stress` let the user specifiy the inital value of the stresses.

This keyword is followed by an array. The size of this array must be
equal to the number of components of symmetric tensors for the
modelling hypothesis considered (see the `@ModellingHypothesis`
keyword).

## Example

~~~~ {.cpp}
@Stress {'YoungModulus*EXX0',
         '-PoissonRatio*YoungModulus*EXX0',
         '-PoissonRatio*YoungModulus*EXX0',
         0.,0.,0.};
~~~~

# The `@StressEpsilon` keyword

The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on strains, the other on stresses.

This criterium on stresses checks that the residual of the Newton
Algorithm is low enought. By default, this value is equal to 1.e-3,
which is a convenient value if stresses are expressed in Pa.

The `@StressEpsilon` keyword let the user specifiy the criterium value
used for the criterium on stressess.

It is followed by the criterium value.

## Example

~~~~ {.cpp}
@StressEpsilon 1.e2;
~~~~~~~~

# The `@TangentOperatorComparisonCriterium` keyword

The keyword `@TangentOperatorComparisonCriterium` is not documented yet

# The `@Test` keyword

The `@Test` keyword allows the user to add a test on the results. Two
types of tests are supported, `function` or `file`. The tests are
executed at the end of each time step.

This keyword is followed by the type of the test.

If the type of the test is `function`, two syntaxs are allowed. In the
first one, three arguments are expected: the name of the tested
variable, a function of time given the exepected value, a criterium
value used for the comparison. In the second syntax, two arguments are
exepected: an map which associates the name of a tested variable to a
function of time given the expected value of this variable and a
criterium value used for the comparison. The functions may explicitly
depend of time through the variable `t`.

If the type of the test is `file`, the expected values are read in the
columns of a text file. The name of the text file shall be given and
then two syntaxs are allowed. In the first one, three arguments are
expected: the name of the tested variable, a column number and a
criterium value used for the comparison. In the second syntax, two
arguments are exepected: an map which associates the name of a tested
variable to a column number and a criterium value used for the
comparison. In each cases, the values given by the n+1 line shall
corresponds the values expected after the n period.

The comparisons are made using an absolute criterium.  The names of
the componenents of the strains begins with E. The name of the
components of the stresses begins with S. The name of the components
of symmetric tensor internal state variables begins with the name of
the internal state variables. The name of the components of symmetric
tensors are obtained by suffixing their name according to the
modelling hypothesis used (see the `@ModellingHypothesis`
keyword). For example, the components of the strains are:

- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT
- Axisymmetrical                       : ERR EZZ ETT ERZ
- PlaneStress                          : EXX EYY EZZ EXY
- PlaneStrain                          : EXX EYY EZZ EXY
- GeneralisedPlaneStrain               : EXX EYY EZZ EXY
- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ

Off-diagonals terms are affected by a factor \(\sqrt(2)\).

# Example (function)

~~~~ {.cpp}
@Test<function> 'EXY' '0.' 1.e-12;
~~~~~~~~

# Example (function)

~~~~ {.cpp}
@Test<function> {'EXX':'SXX/YoungModulus',
                 'EYY':'-PoissonRatio*SXX/YoungModulus',
                 'EZZ':'-PoissonRatio*SXX/YoungModulus',
                 'EXY':'0.','EXZ':'0.','EYZ':'0.'} 1.e-12;
~~~~~~~~

# Example (file)

~~~~ {.cpp}
@Test<file> 'reference.txt' 'EXY' 1 1.e-12;
~~~~~~~~

# The `@ThermodynamicForce` keyword

The keyword `@ThermodynamicForce` is not documented yet

# The `@ThermodynamicForceEpsilon` keyword

The keyword `@ThermodynamicForceEpsilon` is not documented yet

# The `@Times` keyword

The `@Times` keywords let the user specify a list of times used for
the computations.

This keywords is followed by an array describing intervals of
times. By default, one time step is used to go from a given time to
the next one. The keyword 'in' allows the user to divide an interval
into a given number of smaller intervals.

## Note

The loadings and the external state variables evolutions are
described in a totally independent manner (see the `@ImposedStrain`,
the `@ImposedStress` and the `@ExternalStateVariable` keywords).

## Note

In case of non convergence, the algorithm handles time sub
steppings. See the `@MaximumNumberOfSubSteps` for details.

## Example

~~~~ {.cpp}
@Times {0.,3600.};         // 1  time step of  3600. seconds
~~~~~~~~

## Exemple

~~~~ {.cpp}
@Times {0.,3600. in 10};   // 10 time steps of 360   seconds
~~~~~~~~

# The `@UnsignedIntegerParameter` keyword

The `@UnsignedIntegerParameter` keyword specifies the value of a
parameter of the behaviour.

## Example

~~~~ {.cpp}
@UnsignedIntegerParameter 'iterMax' 12;
~~~~~~~~

# The `@UseCastemAccelerationAlgorithm` keyword

The `@UseCastemAccelerationAlgorithm` specifies if the acceleration
algorithm introduced by the resolution procedures of the castem finite
element shall be used.

This keyword is followed by `true` or `false`.

## Example

~~~~ {.cpp}
@UseCastemAccelerationAlgorithm true;
~~~~~~~~
