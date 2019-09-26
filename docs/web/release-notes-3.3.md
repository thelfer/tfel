% New functionalities of the 3.3 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2019

The page declares the new functionalities of the 3.3 version of the
`TFEL` project.

This version was released along with Version `3.2.2` and inherits from
all the fixes of this version.

# Improvements to `TFEL`' core libraries

## Improvements and new features of `TFEL/System` library

### Improvements to `ExternalLibraryManager`

`ExternalLibraryManager`' new features includes:

- retrieving the build identifier of a material knowledge (see Section
  @sec:build_identifiers).
- retrieving a material property implemented using the `Cyrano`
  interface (see Section @sec:cyrano:material_properties).

### Improvements to the `ExternalBehaviourData` structure

The `ExternalBehaviourData` class now contains three new fields
associated respectively with the build identifier see Section
@sec:build_identifiers), the interface used to generate the behaviour,
and the exported elastic material properties.

The later is empty if the behaviour does not export elastic material
properties.

See the [doxygen
documentation](doxygen/structtfel_1_1system_1_1_external_behaviour_data.html)
for details.

### The `ExternalMaterialPropertyData` and `ExternalMaterialPropertyDescription` structures

The `ExternalMaterialPropertyData` and
`ExternalMaterialPropertyDescription` structures provides metadata about
a material property. See the [doxygen
documentation](doxygen/structtfel_1_1system_1_1_external_material_property_data.html)
for details.

# Improvements to `MFront`

## Specifying a build identifier{#sec:build_identifiers}

As defined by Wikipedia, Quality assurance (QA) is a way of preventing
mistakes and defects in manufactured products and avoiding problems when
delivering products.

Fuel performance codes are developed under stringent Quality Assurance
requirements that make their developers reluctant to rely on shared
libraries to deliver material knowledge they are responsible for. A risk
that must be mitigated as much as possible is the replacement of a
shared library by another one, either intentionally to modify the
results of the code or by accident (conflict with a previous version for
example).

Specifying a build identifier proposes a solution for this issue. During
building the shared libraries, developers may now specify a build
identifier using the TFEL_BUILD_ID environment variable. This identifier
is meant to be only known by the developers of the code and is hidden to
end users. At runtime, when calling a shared library, the code can
retrieve its build identifier, if any, and can check if it matches the
identifier defined by the developers. If not, it may indicate a conflict
exists with a previous versions of the shared library or that the end
user wants to test a new material property or a new mechanical
behaviour. In both cases, the code will report that results are
unreliable and not coverered by the code specifications.

## Parameters can now be declared with a type

In previous versions, parameters had the implicit type `real`. The user
can now specify any scalar type. This makes the declaration of
parameters consistent with the declaration of the other variables.

~~~~
@Parameter strainrate de0 = 1e-4;
~~~~

## New material property interface for the `Cyrano` fuel performance code{#sec:cyrano:material_properties}

For the `Cyrano` fuel performance code to call external material
properties, a dedicated interface has been built.

The prototype of the generated functions are as follows:

~~~~{.c++}
CyranoRealType (*)(CyranoOutputStatus* const,
				   const CyranoRealType* const,
				   const CyranoIntegerType,
				   const CyranoOutOfBoundsPolicy);
~~~~

This new interface is fully documented [here](cyrano.html).

## Improvements to the `Cast3M`' behaviour interface

`Cast3M` users have to deal with two *a priori* incompatible
requirements:

- On one hand, `Cast3M`' default algorithm relies on the definition of
  the elastic stiffness.
- On the other hand, Quality Assurance requirements urges `MFront`
  implementations to define the elastic properties internally[^1].

[^1]: Elastic properties may be defined in various ways. One way is to
  use either the `@ElasticMaterialProperties` keyword or the
  `@ComputeStiffnessTensor` keyword. Another way is to define them
  inside the options of bricks.

Possible workarounds so far include:

- Giving approximate values to `Cast3M` as its elastic stiffness is just
  a numerical parameter.
- Implementing the material properties in their own `MFront` files and
  compiling both the behaviour and the elastic properties.

Both workarounds have their drawbacks:

- In the first case, one may degrade the performances if the elastic
  stiffness is not close enough to the real one. Another drawback of
  this approach is that it may sometimes be convenient to have the real
  values of the elastic properties for post-processings.
- The second solution is error-prone as it requires the user to take
  care that the definition of the elastic properties is consistent with
  the ones used in the behaviour.

This release introduces a better solution. When elastic properties are
defined internally, the `Cast3M` interface for behaviours now
automatically generates the elastic material properties using the
`Cast3M` interface for material properties.

The generated functions: have a name built with the name of the
behaviour, followed by the underscore, and the name of the material
property and are built in the same shared library than the behaviour.

At this stage, one gets something very similar with the second solution
we just discussed. However, we now have the following advantages:

- The generation of those properties is automatic.
- The existence of those properties can be check as part of the metadata
  associated with the behaviour. Though not useful for raw `Cast3M`
  usage, this is very useful for more advanced solvers built on top of
  `Cast3M`, such as the ones available in the `PLEIADES` platform.
- The generated example of usage shows how the material properties must
  be declared (see the example below).

### Example

Consider the following minimal example:

~~~~{.cxx}
@DSL Implicit;
@Behaviour Test;
@ElasticMaterialProperties{"127.8e9*(1-7.825e-5*(T-293.15))", 0.3};
@Integrator {
  fεᵉˡ += Δεᵗᵒ;
}
~~~~

Compiling this behaviour with the `Cast3M` interface generates the
following output:

~~~~{.bash}
Treating target : all
The following library has been built :
- libUmatBehaviour.so :  umattest Test_YoungModulus Test_PoissonRatio
~~~~

In addition of the behaviour, this output highlights that two functions
associated respectively with the `YoungModulus` and the `PoissonRatio`
were generated.

In the generated example of usage, the material properties are declared
as follows:

~~~~{.python}
xyoun = 'TABLE';
xyoun . 'MODELE' = 'Test_YoungModulus';
xyoun . 'LIBRAIRIE' = '';
xyoun . 'VARIABLES' = 'MOTS'  'T';

xnu = 0.3;
~~~~

## Improvements to the `Cyrano`' behaviour interface

### Generation of elastic material properties from the behaviour

For post-processing reasons, the `Cyrano` fuel performance code must
access the elastic material properties used by the behaviour. To ensure
consistency, the behaviour now automatically exports the elastic
material properties when they are defined internally.

# Improvements to `MTest`

## Ability to choose  the stress measure used in input/output and the consistent tangent operator returned for finite strain behaviours through the `generic` interface

The following options can be passed to the behaviour:

- `stress_measure`. The value of this option must be either
  `CauchyStress` (or `CAUCHY`), `SecondPiolaKirchoffStress` (or `PK2`)
  or `FirstPiolaKirchoffStress` (or `PK1`).
- `tangent_operator`. The value of this option must be either:
    - `DCAUCHY_DF` (or `DSIG_DF`): the derivative of the Cauchy stress
	  with respect to the deformation gradient
    - `DS_DEGL`: the derivative of the second Piola-Kirchoff stress
      with respect to the Green-Lagrange strain
	- `DPK1_DF`: the derivative of the first Piola-Kirchoff
      stress with respect to the deformation gradient is returned

~~~~{.cxx}
@Behaviour<generic> 'src/libBehaviour.so' 'LogarithmicStrainElasticity2' {
  stress_measure : PK2,
  tangent_operator : DPK1_DF
};
~~~~

The output of the behaviour is still the Cauchy stress. The stress
measure on input and input are converted from or to the Cauchy
stress.

The non linear algorithm still relies the derivative of the Cauchy
stress with respect to the deformation gradient: the tangent operator
returned by the behaviour is automatically converted to this
derivative.

# Known incompatibilities

## API changes in the class `TargetsDescription`

In previous versions, the class `TargetsDescription` was mainly designed
to handle libraries. Version 3.3 provides a better support for
executables.

The following changes were made:

- a `TargetsDescription` is not more iterable. One shall use the
  `getLibraries()` methods to get an iterable object over libraries.
- a `LibraryDescrition` can't be retrieved via the subscript operator
  `[]`, one shall use the `getLibrary` method instead.


# Tickets solved during the development of this version

## Ticket #172: Support for dynamic choice of the consistent tangent operator of finite strain behaviours using the `generic` interface

The `generic` interface now allows to specify, for finite strain
behaviours (including the behaviours based on the Green-Lagrange and
the Hencky strain measure):

- the stress measure used on input/output
- the consistent tangent operator expected.

If `K` is the array used to store the consistent tangent operator,
`K[0]` still represent the kind of integration to be performed.

For finite strain behaviours `K[1]` must now hold the stress measure used:

- if `K[1]` < 0.5, the Cauchy stress is used
- if 0.5 < `K[1]` < 1.5, the second Piola-Kirchoff stress is used
- if 1.5 < `K[1]` < 2.5, the first Piola-Kirchoff stress is used

`K[2]` must hold the consitent tangent operator returned by the behaviour:

- if `K[2]`<0.5, the derivative of the Cauchy stress with respect
  to the deformation gradient is returned
- if 0.5<`K[2]`<1.5, the derivative of the second Piola-Kirchoff
  stress with respect to the Green-Lagrange strain
  is returned
- if 1.5<`K[2]`<2.5, the derivative of the first Piola-Kirchoff
  stress with respect to the deformation gradient is returned

For more details, see: <https://sourceforge.net/p/tfel/tickets/172/>

## Ticket #171: Add the derivative of the first Piola-Kirchoff stress as a possible consistent tangentor for finite strain behaviours

For more details, see: <https://sourceforge.net/p/tfel/tickets/171/>
