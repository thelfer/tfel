% `ImplicitII` keywords


# The `;` keyword

The keyword `;` is not documented yet

# The `@APosterioriTimeStepScalingFactor` keyword

The keyword `@APosterioriTimeStepScalingFactor` is not documented yet

# The `@APrioriTimeStepScalingFactor` keyword

The keyword `@APrioriTimeStepScalingFactor` is not documented yet

# The `@AdditionalConvergenceChecks` keyword

The `@AdditionalConvergenceChecks` keyword is meant to introduce a
code block returning stating if convergence has been reached. More
precisely, this code block is meant to modify a boolean variable
called `converged`. This boolean is `true` if the standard convergence
criterion has been reached, `false` otherwise.

One possible usage of this code block is multi-surfaces plasticity
treated by activating or deactivating statuses, as described by Simo
and Hughes.

# Example

Consider a two surfaces plastic behaviour. We will use an array of
booleans called `statuses`.

~~~~{.cpp}
@Brick StandardElasticity; // to have computeElasticPrediction

@LocalVariable bool statuses[2];

@Prediction{
  // initial status based of the elastic prediction
  auto sigel = computeElasticPrediction();
  for(unsigned short i=0;i!=2;++i){
	statuses[i] = ...
  }
} // end of @Prediction

@Integrator{
  for(unsigned short i=0;i!=2;++i){
    if(statuses[i]){
      ...
    }
  }
} // end of @Integrator
~~~~

# The `@Algorithm` keyword

The `@Algorithm` keyword is used to select a numerical algorithm.

The set of available algorithms depends on the domain specific
language. As the time of writing this notice, the following
algorithms are available:

- `euler`, `rk2`, `rk4`, `rk42` , `rk54` and `rkCastem` for the
  `Runge-Kutta` dsl.
- `NewtonRaphson`, `NewtonRaphson_NumericalJacobian`,
  `PowellDogLeg_NewtonRaphson`,
  `PowellDogLeg_NewtonRaphson_NumericalJacobian`, `Broyden`,
  `PowellDogLeg_Broyden`, `Broyden2`, `LevenbergMarquardt`,
  `LevenbergMarquardt_NumericalJacobian` for implicit dsls.

## Example (Runge-Kutta dsl)

~~~~{.cpp}
@Algorithm rk54;
~~~~

## Example (Implicit dsl)

~~~~{.cpp}
@Algorithm NewtonRaphson;
~~~~


# The `@Author` keyword

The `@Author` keyword is used to give the name of the person who wrote
the `mfront` file.

All the following words are appended to the author's name up to a
final semicolon.

Note: The name of the person who formulated the material property,
behaviour or model shall be given in the description section (see the
`@Description` keyword).

## Example

~~~~ {#Author .cpp}
@Author Éric Brunon;
~~~~



# The `@AuxiliaryStateVar` keyword

The `@AuxiliaryStateVar` keyword is a deprecated synonymous of
`@AuxiliaryStateVariable`.

# The `@AuxiliaryStateVariable` keyword

The `AuxiliaryStateVariable` keyword introduces one or several new
auxiliary state variables. It is followed by a type name and the
name(s) of the variable(s) declared, separated by commas.

## Example

~~~~{.cpp}
// scalar auxiliary state variable
@AuxiliaryStateVariables strain p;
// symmetric tensors auxiliary state variable
@AuxiliaryStateVariables StrainStensor evp,evp2;
~~~~

# The `@AxialGrowth` keyword

The `@AxialGrowth` keyword let the user specify a linear stress-free
expansion:

- along the second direction of orthotropy if the default orthotropic axes
  convention has been selected.
- along the 'z' direction if the pipe orthotropic axes has been
  defined is defined. The 'z' direction is the second direction of
  orthotropy for all hypotheses except Generalised Plane Strain, Plane
  Strain and Plane Stress hypotheses.

This keyword is followed by:

- an external state variable name.
- a string giving an mfront file implementing a model of the axial growth.

## Physical description

The value of the axial growth gives the change in length in the axial
direction \(\frac{\Delta\,l_{z}}{l_{z}}\).

The axial growth is assumed to be isochoric, so the change of length
in each direction of the plane perpendicular to the axial direction is
given by:
\[
\frac{\Delta\,l_{\parallel}}{l_{\parallel}}=\frac{1}{\sqrt{1+\frac{\Delta\,l_{z}}{l_{z}}}}-1
\]

## Notes

- The behaviour must be orthotropic (see `@OrthotropicBehaviour`).
- An orthotropic axes convention *shall* have been set (see
  `@OrthotropicBehaviour`).

## Examples

~~~~ {#AxialGrowth .cpp}
// axial growth defined by an external model
@AxialGrowth 'M5AxialGrowth.mfront';
~~~~

~~~~ {#AxialGrowth2 .cpp}
// axial growth defined by an external state variable
@ExternalStateVariable real ag;
ag.setGlossaryName("AxialGrowth");
@AxialGrowth ag;
~~~~

# The `@Behaviour` keyword

The `@Behaviour` defines the name of the behaviour. The name must be a
valid `C++` class name.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Example

~~~~{.cpp}
@Behaviour Norton;
~~~~

# The `@Bounds` keyword

The `@Bounds` keyword let the user define the domain of validity of
the law.

The `@Bounds` keyword is followed by a variable name, the keyword in
and an interval. The interval may contain the infinity, represented by
the '*' character.

## Effect

What happens if a variable is found to be out of its bounds depends on
the interface used. Most interfaces let the user choose one of three
following policies:

- `None`, which means that nothing is done.
- `Warning`, which means that a message is displayed, but computations
  are not stopped.
- `Strict`, which means that computations are stopped with an error
  message.

## Example

~~~~{.cpp}
@Bounds T in [293.15:873.15];
~~~~


# The `@Brick` keyword

The `@Brick` keyword introduces a behaviour brick.

## Example

~~~~{.cpp}
@Brick "StandardElasticity";
~~~~

# The `@Coef` keyword

The `@Coef` keyword is a deprecated synonymous of
`@MaterialProperty`.

# The `@CompareToNumericalJacobian` keyword

The `@CompareToNumericalJacobian` keyword enables or disables the
comparison of the user defined jacobian to a numerically computed
jacobian. This keyword is followed by a boolean value (`true` or
`false`).

Jacobians are compared by blocks. Blocks for which the comparison
failed are reported on the standard output.

The comparison criterion value can be changed used the
`@JacobianComparisonCriterion` keyword.

## Example

~~~~{.cpp}
@CompareToNumericalJacobian true;
~~~~

# The `@ComputeFinalStress` keyword

The `@ComputeFinalStress` keyword introduces a code block meant to
compute the stress symmetric tensor after the integration.

The code block is called after the update of the state variables. The
auxiliary state variable and the external state variables are not
updated yet.

## Example

~~~~{.cpp}
@ComputeFinalStress{
  const SlidingSystems& ss = SlidingSystems::getSlidingSystems();
  // approximation de l'inverse de \(\Delta\,F_p\)
  inv_dFp = Tensor::Id();
  for(unsigned short i=0;i!=12;++i){
    inv_dFp -= dg[i]*ss.mu[i];
  }
  real J_inv_dFp = det(inv_dFp);
  inv_dFp /= CubicRoots::cbrt(J_inv_dFp);
  // Fe en fin de pas de temps
  Fe  = Fe_tr*inv_dFp;
  // Piola-Kirchhoff II
  S = D*eel;
  // Cauchy
  sig = convertSecondPiolaKirchhoffStressToCauchyStress(S,Fe);
}
~~~~

# The `@ComputeStiffnessTensor` keyword

The `ComputeStiffnessTensor` keyword is used to define the elastic
stiffness tensor based on the elastic material properties given as an
array of entries. After this array, a semicolon is expected.

This array is used to automatically used to declare the elastic
material properties of the behaviour (see the
`@ElasticMaterialProperties` keyword for details).

An entry can be either a string referring to an external `MFront`
file, a formula or a numerical value.

If an entry refers to an external `MFront` file or the formula, all
the inputs of this material property must be either:

- a material property
- a parameter
- a state variable
- an external state variable

of the behaviour.

## Isotropic case

In the isotropic case, two entries are expected in the array, in that
order:

- the Young Modulus
- the Poisson ratio

## Orthotropic case

In the orthotropic case, 9 entries are expected in the array, in that
order:

- three Young modulus \((E_{1},E_{2},E_{3})\)
- three Poisson ratio \((nu_{12},nu_{23},nu_{13})\)
- three shear modulus \((G_{12},G_{23},G_{13})\)

In the orthoropic case, computation of the stiffness tensor rely on the
definition of an orthotropic convention. For example, the `Pipe`
orthotropic convention will lead to automatically exchange the second
and first axes when computing the stiffness tensor for the plane strain,
plane stress and generalised plane strain hypotheses. See the
`OrthotropicBehaviour` keyword for details.

## Tensors computed

In an implicit scheme, this keyword leads to the definition and the
automatic computation of the tensors `D` and `D_tdt` which
respectively refer to the stiffness tensor at \(t+\theta\,dt\) and
\(t+dt\).

In an explicit scheme, this keyword leads to the definition of
the stiffness tensor which is automatically updated if this tensor
evolves during the time step.

## Example

~~~~ {#ComputeStiffnessTensor .cpp}
@ComputeStiffnessTensor{7.8e+4,2.64233e+5,3.32e+5,
    0.13,0.24,0.18,
    4.8e+4,1.16418e+5,7.8e+4};
~~~~

# The `@ComputeStress` keyword

The `@ComputeStress` keyword introduces a code block meant to compute
the stress symmetric tensor.

This keyword interprets the code block to generate two methods:

- The first one is used before the integration step, using updated
  values for the state variables and external state variables.
- The second one is a candidate for the computation of the stress at
  the end of the integration. This candidate is used if the user does
  not provide an appropriate way of computing the stress at the end of
  the time step using the `@ComputeFinalStress` keyword.

## Note

If the user provide a way of computing the stress at the end of the
time step through the `@ComputeFinalStress` keyword, we consider that
the use of `@ComputeStress` is meaningless and advice the user to
rather compute explicitly the stress as part of the integration step.

## Example

~~~~{.cpp}
@ComputeStress{
  sig = (1-d)*(lambda*trace(eel)*Stensor::Id()+2*mu*eel);
}
~~~~


# The `@ComputeStressFreeExpansion` keyword

The `ComputeStressFreeExansion` keyword introduces a code block which
allows the user to define an arbitrary expansion.

The code block is meant to **update** two symmetric tensors `dl0_l0`
and `dl1_l0` which respectively stands for the value of the expansion
at the beginning and at the end of the time step.

## Note

- How `dl0_l0` and `dl1_l0` are treated depends on the behaviour type
  (small or finite strain, CZM) and on the finite strain strategies
  used, if any (in this case, the user have to refer the interface
  documentation).
- Local variables can't be used in a `@ComputeStressFreeExpansion`
  block, because stress-free expansion are computed before the
  initialisation of those variables.

## Example

~~~~ {#ComputeStressFreeExpansion .cpp}
@ExternalStateVariable real tau;
tau.setGlossaryName("BurnUp (at.%)");

@Parameter Ctau = 0.6e-2;
Ctau.setEntryName("SolidSwellingCoefficient");

@ComputeStressFreeExpansion{
  dl0_l0 += Ctau*tau*StrainStensor::Id();
  dl1_l0 += Ctau*(tau+dtau)*StrainStensor::Id();
}
~~~~



# The `@ComputeThermalExpansion` keyword

The `ComputeThermalExansion` keyword is followed either by a material
property description or an array of material descriptions (othotropic
behaviours) giving the mean linear thermal expansion coefficient.

A material description is either a floating point number, a formula or
the name of an external MFront file.

The thermal expansion is computed as follows:
\[
\frac{\Delta\,l}{l_{T^{i}}} = \frac{1}{1+\alpha\left(T^{i}\right)\,\left(T^{i}-T^{\alpha}\right)}\,\left[\alpha\left(T\right)\,\left(T-T^{\alpha}\right)-\alpha\left(T^{i}\right)\,\left(T^{i}-T^{\alpha}\right)\right]
\]

where:

- \(T^{\alpha}\) is the reference temperature for the thermal
  expansion.
- \(T^{i}\) is the reference temperature at which of the geometry has
  been measured which is generally assumed to be equal to the
  temperature at the beginning of the computations, in the undeformed
  state.

## Definition of the reference temperature for the thermal expansion

The reference temperature for the thermal expansion \(T^{\alpha}\) is
intrinsically linked to the definition of the thermal expansion
coefficient.

One may use the following syntax to define  \(T^{\alpha}\):

~~~~ {#ComputeThermalExpansion .cpp}
@Parameter a0 = 2.e-5;
@Parameter a1 = 4.e-8;
@ComputeThermalExpansion "a0+a1*(T-273.15)"{
 reference_temperature : 273.15
};
~~~~

However, if the mean linear thermal expansion coefficient is given by
an external `MFront` file, it is recommended to define a static
variable called `ReferenceTemperature` in this file.

Whatever the way used to defined \(T^{\alpha}\), a parameter called
`ThermalExpansionReferenceTemperature` is automatically defined.

> **Note**
>
> If \(T^{\alpha}\) is not explicitly defined, a default value of
> \(293.15\,K\) is used.

## Parameter associated with the reference temperature for the geometry

The reference temperature at which of the geometry has been measured,
\(T^{i}\), has a default value of \(293.15\,K\). This can be changed
using the automatically defined
`ReferenceTemperatureForInitialGeometry` parameter.

## Orthotropic axis convention

For orthotropic behaviours, the orthotropic axes convention is taken
into account (see `OrthotropicBehaviour`).

## Example

~~~~ {#ComputeThermalExpansion .cpp}
@ComputeThermalExpansion 1.e-5;
~~~~

~~~~ {#ComputeThermalExpansion2 .cpp}
@ComputeThermalExpansion "UO2_ThermalExpansion.mfront"
~~~~

~~~~ {#ComputeThermalExpansion3 .cpp}
@ComputeThermalExpansion {1.e-5,0.2e-5,1.2e-5};
~~~~

# The `@ComputedVar` keyword

The keyword `@ComputedVar` is not documented yet

# The `@CrystalStructure` keyword

The crystal structure must be defined through the `@CrystalStructure`
keyword. This keyword is followed by one of the following value:

- `Cubic`: cubic structure.
- `BCC`: body centered cubic structure.
- `FCC`: face centered cubic structure.
- `HCP`: hexagonal closed-packed structures.

## Example

~~~~{.cpp}
@CrystalStructure FCC;
~~~~

# The `@DSL` keyword

The `@DSL` keyword specify the domain specific language (dsl) used.

The list of available dsl's is returned by the `--list-dsl` option of
`mfront`:

~~~~{.bash}
$ mfront --list-dsl
~~~~

As the time of writing this notice, the following dsl's are available:

- `DefaultDSL`: this parser is the most generic one as it does not
  make any restriction on the behaviour or the integration method that
  may be used.
- `DefaultCZMDSL`: this parser is the most generic one as it does not
  make any restriction on the behaviour or the integration method that
  may be used.
- `DefaultFiniteStrainDSL`: this parser is the most generic one as it
  does not make any restriction on the behaviour or the integration
  method that may be used.
- `Implicit`: this parser provides a generic integrator based on an
  implicit scheme. The elastic strain is automatically defined as a
  state variable
- `ImplicitII`: this parser provides a generic integrator based on a
  theta method. Unlike `Implicit`, the elastic strain is not
  automatically defined as a state variable.
- `ImplicitFiniteStrain`: this parser provides a generic integrator
  based on a theta method.
- `IsotropicMisesCreep`: this parser is used for standard creep
  behaviours of the form \(\dot{p}=f(s)\) where \(p\) is the equivalent
  creep strain and \(s\) the equivalent mises stress.
- `IsotropicPlasticMisesFlow`: this parser is used for standard
  plastics behaviours with yield surface of the form \(f(s,p)=0\)
  where \(p\) is the equivalent creep strain and \(s\) the equivalent
  mises stress.
- `IsotropicStrainHardeningMisesCreep`: this parser is used for
  standard strain hardening creep behaviours of the form
  \(\dot{p}=f(s,p)\) where \(p\) is the equivalent creep strain and
  \(s\) the equivalent mises stress.
- `MaterialLaw`: this parser is used to define material properties.
- `Model`: this parser is used to define simple material models.
- `MultipleIsotropicMisesFlows `: this parser is used to define
  behaviours combining several isotropic flows. Supported flow type
  are 'Creep' (\(\dot{p}=f(s)\)) 'StrainHardeningCreep'
  (\(\dot{p}=f(s,p)\)) and 'Plasticity' (\(f(p,s)=0\)) where \(p\) is
  the equivalent plastic strain and \(s\) the equivalent mises stress.
- `RungeKutta`: this parser provides a generic integrator based on one
  of the many Runge-Kutta algorithm.

## DSL options

A DSL's behaviour may be changed using options defined either in the
`MFront` file using a JSON-like syntax or as a command line
argument.

The options related to a specific `DSL` can be retrieved using the
`--list-dsl-options` command line argument, as follows:

~~~~{.bash}
$ mfront --list-dsl-options=Model
- parameters_as_static_variables: boolean stating if the parameter shall be treated as static variables.
- build_identifier              : string specifying a build identifier. This option shall only be specified on the command line.
~~~~

### Defining `DSL` options in the MFront file

Options can be passed to a DSL as follows:

~~~~{.cxx}
@DSL Default{
  parameters_as_static_variables : true
};
~~~~

### Defining `DSL` options on the command line:

`MFront` have various command line arguments to define options passed to DSLs:

- `--dsl-option`, which allows to define options passed to domain
  specific languages.
- `--behaviour-dsl-option`, which allows to define options passed to
  domain specific languages related to behaviours.
- `--material-property-dsl-option`, which allows to define options
  passed to domain specific languages related to material properties.
- `--model-dsl-option`, which allows to define options passed to domain
  specific languages related to models.

For example, the `--dsl-option` can be used as follows:

~~~~{.bash}
$ mfront --obuild --interface=cyrano --dsl-option=build_identifier:\"Cyrano-3.2\" Elasticity.mfront
~~~~

This example illustrates that special care must be taken when defining
an option expecting a string value.

### Options common to all DSLs

The following options are available for all DSLs:

- `build_identifier` (string), which must be associated to a string
  value. However, the `build_identifier` is not meant to be directly
  specified in the `MFront` source file. It shall rather be defined on
  the command line.
- `parameters_as_static_variables` (boolean), which states if parameters
  shall be treated as static variables.
- `parameters_initialization_from_file` (boolean), which states if
  parameters can be modified from an external state file. This feature is
  only implemented by a few interfaces and is enabled by default.
- `default_out_of_bounds_policy` (string), which selects the default out
  of bounds policy. Allowed values ar `None` (the default), `Warning` or
  `Strict`.
- `out_of_bounds_policy_runtime_modification` (boolean), which states if
  the out-of-bounds policy can be changed at runtime. By default, this
  option is `true`.


### Options common to all DSLs related to behaviours

- `automatic_declaration_of_the_temperature_as_first_external_state_variable`
  (boolean), which states if the temperature shall be automatically
  declared as an external state variable.
- `overriding_parameters`, which allows to specify overriding
  parameters. This parameters must be a map associating variables names
  and default values of the overriding parameters.

## Example

~~~~{.cpp}
@DSL Implicit;
~~~~


# The `@Date` keyword

The `@Date` keyword allows the user to precise when the mfront file
was written.

All the following words are appended to the date up to a final
semicolon.

## Example

~~~~{.cpp}
@Date 2008-11-17;
~~~~


# The `@Description` keyword

The `@Description` describes the material property, behaviour or model
which is implemented in a mfront file.

This keyword is followed by a block containing all the relevant piece
of information including:

- the references of the article or technical report from which the
  material property, behaviour or model is extracted
- the name of the authors
- the modifications made

## Example

~~~~{.cpp}
@Description
{
  Corrélation établie sur la nuance V-4Cr-4Ti.

  Propriétés extraites du document :

  Propriétés et comportement mécanique d alliages
   de Vanadium avant, après et sous irradiation

  Marion Le Flem, Jean-Luc Bechade, Annick Bougault,
  Aurore Michaux, Lionel Gosmain, Jean-Louis Seran
  DMN/SRMA/LA2M/NT/2008-2967/A
}
~~~~

# The `@DislocationsMeanFreePathInteractionMatrix` keyword

The interaction matrix associated with the effects of the dislocations
on the mean free path of the dislocations of a specific system can be
defined though the `@DislocationsMeanFreePathInteractionMatrix`
keyword.

This keyword follows the same conventions as the
`@InteractionMatrix` keyword. In particular, the structure of the
matrix is fully described in the description of `@InteractionMatrix`.

See the `@InteractionMatrix` keyword for details.

# The `@DissipatedEnergy` keyword

The `@DissipatedEnergy` keyword allows the computation of the internal
energy per unit of volume in the reference configuration.

The code provided must update a variable called `Psi_d`.

This update is called at the end of the behaviour integration, if
convergence was reached, after that:

- Internal state variables have been updated.
- The stress at the end of the time step have been computed.

## Example

~~~~ {#DissipatedEnergy .cpp}
@DissipatedEnergy{
	Psi_d += sig|(deto-deel)
	// If would have been better to store the initial value of the
    // stress in a local variable sig0 and then used 

}
~~~~

~~~~ {#DissipatedEnergy2 .cpp}
@DissipatedEnergy{
	// Here, we have stored the initial value of the
    // stress in a local variable sig0 
	Psi_d += (sig0+sig)|(deto-deel)/2
}
~~~~

# The `@ElasticMaterialProperties` keyword

The `@ElasticMaterialProperties` keyword is used give the material
properties for standard mechanical behaviours.

This keyword is followed by an array which values can be either a
string referring to a formula, an external `MFront` file or a
numerical value. After this array, a semicolon is expected.

If an entry refers to an external `MFront` file or a formula, all the
inputs of this material property must be either:

- a material property
- a parameter
- a state variable
- an external state variable

of the behaviour.

Elastic material properties are used by behaviours bricks.

## Isotropic case

In the isotropic case, two entries are expected in the array, in that
order:

- the Young Modulus
- the Poisson ratio

In domain specific languages providing an implicit scheme
(`Implicit`,`ImplicitII`, `ImplicitFiniteStrain`), the following local
variables are automatically defined and computed:

- `young`, `young_tdt` which respectively stand for the Young modulus
  at \(t+\theta\,dt\) and \(t+\,dt\).
- `nu`, `nu_tdt` which respectively stand for the Poisson ratio at
  \(t+\theta\,dt\) and \(t+\,dt\).

## Orthotropic case

In the orthotropic case, 9 entries are expected in the array, in that
order:

- three Young modulus \((E_{1},E_{2},E_{3})\)
- three Poisson ratio \((nu_{12},nu_{23},nu_{13})\)
- three shear modulus \((G_{12},G_{23},G_{13})\)

In the orthoropic case, behaviours bricks will rely on the definition
of an orthotropic convention to compute the stiffness tensor. For
example, the `Pipe` orthotropic convention will lead to automatically
exchange the second and first axes when computing the stiffness
tensor for the plane strain, plane stress and generalised plane strain
hypotheses.

## Example

~~~~ {#ElasticMaterialProperties .cpp}
@ElasticMaterialProperties {"AISI348_YoungModulus.mfront",0.3};
~~~~

# The `@Epsilon` keyword

The `@Epsilon` keyword let the user define the convergence criterion
value. It is followed by a positive floating-point number.

## The epsilon parameter

The `@Epsilon` keyword defines the default value for the `epsilon`
parameter. This parameter can be changed at runtime.

## Example

~~~~{.cpp}
@Epsilon 1.e-12;
~~~~


# The `@ExternalStateVar` keyword

The `@ExternalStateVar` keyword is a deprecated synonymous of
`@ExternalStateVariable`.

# The `@ExternalStateVariable` keyword

The `ExternalStateVariable` keyword introduces one or several new
external state variables. It is followed by a type name and the
name(s) of the variable(s) declared, separated by commas.

The external state variables names must be valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Arrays

One may declare an array of external state variables by specifying the
array size after the external state variable name.

## External names

It is recommended to associate to an external state variable a glossary
or an entry name through the methods `setGlossaryName` or
`setEntryName` respectively.

## Example

~~~~{.cpp}
// scalar external state variable
@ExternalStateVariable strain s;
~~~~

# The `@GlidingSystem` keyword

A synonym of `@SlipSystem`.
See `@SlipSystem` for details.

# The `@GlidingSystems` keyword

A synonym of `@SlipSystems`.
See `@SlipSystems` for details.

# The `@HillTensor` keyword

The keyword `@HillTensor` is not documented yet

# The `@Import` keyword

The `@Import` keyword allows the inclusion of one or several
(external) mfront files. This keyword is followed by a string or an
array of strings. Each string stands for a mfront file name that is
meant to be imported.

This instruction interrupts the treatment of the current file and
starts the sequential treatment of each file to be imported.

## Search paths

Files to be imported are searched, in that order:

- starting from the current directory.
- starting from directories specified with one of the `--search-path`
  or the `--include` (`-I`) `mfront`' command line options.
- staring from directories specified through the `MFRONT_INCLUDE_PATH`
  environment variable.

## Example

~~~~{.cpp}
@Import "SlidingSystemsCC.mfront";
~~~~

# The `@Includes` keyword

The `@Includes` introduces a block were the user may define some
preprocessor directives, typically #include directives (hence the
name).

## Example

~~~~{.cpp}
@Includes{
#include<fstream>
}
~~~~

# The `@InitJacobian` keyword

The `@InitJacobian` keyword is a deprecated synonymous of
`@InitializeJacobian`.

# The `@InitJacobianInvert` keyword

The `@InitJacobianInvert` keyword is a deprecated synonymous of
`@InitializeJacobianInvert`.

# The `@InitLocalVariables` keyword

The `@InitLocalVariables` keyword is a deprecated synonymous of
`@InitializeLocalVariables`.

# The `@InitLocalVars` keyword

The `@InitLocalVars` keyword is a deprecated synonymous of
`@InitializeLocalVariables`.

# The `@Initialize` keyword

The keyword `@Initialize` is not documented yet

# The `@InitializeFunction` keyword

The `@InitializeFunction` keyword introduces a code block that can be
used to initialize internal state variables at the very beginning of the
computation. Initialize functions may have user so-called
*initialize function variables*.

In this version, only the `generic` interface generates functions
associated with initialize functions (See Section
@sec::tfel_4.1:system:elm:initialize_functions to see how to retrieve
the initialize functions generated by the `generic` interfaces).

Because initialize functions are called before any behaviour
integration, special care to the meaning of the variables must be taken:

- The gradients and external state variable will have their values at
  the beginning of the time step and their increments will be null.
- The thermodynamic forces will have their values at the beginning of
  the time step.
- The state variables and auxiliary state variables will have undefined
  values. The associated increments, if defined, will be null.

Concerning material properties, they have their values at the beginning
of the time step.

> **About initialisation of local variables**
>
> The code block defined by the `@InitLocalVariables` code block
> shall be called before the execution of an initialize function.

### Example of usage

The following code defines an initializer function which initializes the
elastic strain from the value of stress:

~~~~{.cxx}
@InitializeFunction ElasticStrainFromInitialStress{
  const auto K = 2 / (3 * (1 - 2 * nu));
  const auto pr = trace(sig) / 3;
  const auto s = deviator(sig);
  eel = eval((pr / K) * Stensor::Id() + s / mu);
}
~~~~

# The `@InitializeFunctionVariable` keyword

Initialize function variables are introduced by the
`@InitializeFunctionVariable` keyword.

Initialize function variables are only defined in initialize functions,
and can't be used in the other code blocks.

Contrary most variables (internal state variables, external state
variables, etc.), initialize function variables can be defined after the
first code block. However, care must be taken to declare initialize
function variables **before** their use in an initialize function.

Note that an initialize function variable can be used in different
initialize function.

# The `@InitializeJacobian` keyword

The `@InitializeJacobian` keyword let the user introduce an initial
estimate for the jacobian matrix used by quasi-newton methods, such as
the first Broyden method.

## Example

~~~~{.cpp}
@InitializeJacobian{
  // setting the jacobian to identity (which is the default)
  fill(this->jacobian.begin(),this->jacobian.end(),real(0));
  for(unsigned short i=0;i!=this->jacobian.size();++i){
    this->jacobian(i,i)=real(1);
  }
}
~~~~

# The `@InitializeJacobianInvert` keyword

The `@InitializeJacobianInvert` keyword let the user introduce an
initial estimate for the jacobian matrix used by quasi-newton methods,
such as the second Broyden method.

## Example

~~~~{.cpp}
@InitializeJacobianInvert{
  // setting the inverse of the jacobian (still called jacobian by the
  // way) to identity (which is the default)
  fill(this->jacobian.begin(),this->jacobian.end(),real(0));
  for(unsigned short i=0;i!=this->jacobian.size();++i){
    this->jacobian(i,i)=real(1);
  }
}
~~~~

# The `@InitializeLocalVariables` keyword

The `@InitializeLocalVariables` introduces a code block meant to
initialize the local variables (see the `@LocalVariable` keyword).

## Example

~~~~{.cpp}
//! volumic fraction of each phase
@LocalVariable real  fv[Np];
//! interaction matrix
@LocalVariable tfel::math::tmatrix<Nss,Nss,real> mh;

@InitLocalVariables<Append>{
  // volumic fraction of each phase
  for(unsigned short i=0;i!=Np;++i){
    fv[i]=1.0/static_cast<real>(Np) ; 
  }
  // interaction matrix
  const real h1 = 1.;
  const real h2 = 1.;
  const real h3 = 1.;
  const real h4 = 1.;
  const real h5 = 1.;
  const real h6 = 1.;
  mh = InteractionMatrix::getInteractionMatrix(h1,h2,h3,
					       h4,h5,h6);
}
~~~~



# The `@IntegerConstant` keyword

The keyword `@IntegerConstant` is not documented yet

# The `@IntegrationVariable` keyword

The `IntegrationVariable` keyword introduces one or several new
integration variables. It is followed by a type name and the name(s)
of the variable(s) declared, separated by commas.

The integration variables names must be valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Arrays

One may declare an array of integration variables by specifying the array
size after the integration variable name.

## Example

~~~~{.cpp}
// scalar integration variable
@IntegrationVariables strain p;
// symmetric tensors integration variable
@IntegrationVariables StrainStensor evp,evp2;
~~~~

# The `@Integrator` keyword

The keyword `@Integrator` is not documented yet

# The `@InteractionMatrix` keyword

The `@InteractionMatrix` keyword let the user specify an interaction
matrix. By convention, this matrix is associated with hardening.

The interaction matrix associated with the effects of the dislocations
on the mean free path of the dislocations of a specific system can be
defined though the `@DislocationsMeanFreePathInteractionMatrix`
keyword.

## Structure of the interaction matrix

Since many interactions between slip systems are equivalent, the
interaction matrix only contains a few independent coefficients.

Consider the following example:

~~~~{.cpp}
@DSL       ImplicitFiniteStrain;
@Behaviour SlipSystemGenerationTest;
@Brick     FiniteStrainSingleCrystal;
@CrystalStructure FCC;
@SlidingSystem <1,-1,0>{1,1,1};
~~~~

Using `mfront-query`, one may have access to the whole matrix using
the `--interaction-matrix` query:

~~~~{.sh}
$ mfront-query --interaction-matrix SlipSystemGenerationTest.mfront 
| 0 1 2 2 3 4 5 6 5 6 4 3 |
| 1 0 2 2 6 5 4 3 4 3 5 6 |
| 2 2 0 1 5 6 3 4 6 5 3 4 |
| 2 2 1 0 4 3 6 5 3 4 6 5 |
| 3 4 5 6 0 1 2 2 6 5 4 3 |
| 6 5 4 3 1 0 2 2 3 4 5 6 |
| 5 6 3 4 2 2 0 1 5 6 3 4 |
| 4 3 6 5 2 2 1 0 4 3 6 5 |
| 5 6 4 3 4 3 5 6 0 1 2 2 |
| 4 3 5 6 5 6 4 3 1 0 2 2 |
| 6 5 3 4 6 5 3 4 2 2 0 1 |
| 3 4 6 5 3 4 6 5 2 2 1 0 |
~~~~

Here only \(7\) coefficients are needed. The number corresponding to a
pair of slip systems defines the rank of the interaction.

The `mfront-query` also provides the `--interaction-matrix-structure`
query which displays the number of independent coefficients and sorts
the pair of slip systems by rank:

~~~~{.sh}
$ mfront-query --interaction-matrix-structure SlipSystemGenerationTest.mfront 
- number of independent coefficients: 7
- rank 0: ([0,1,-1](1,1,1):[0,1,-1](1,1,1)) ([1,0,-1](1,1,1):[1,0,-1](1,1,1)) ([1,-1,0](1,1,1):[1,-1,0](1,1,1)) ([0,1,1](1,1,-1):[0,1,1](1,1,-1)) ([1,0,1](1,1,-1):[1,0,1](1,1,-1)) ([1,-1,0](1,1,-1):[1,-1,0](1,1,-1)) ([0,1,-1](1,-1,-1):[0,1,-1](1,-1,-1)) ([1,0,1](1,-1,-1):[1,0,1](1,-1,-1)) ([1,1,0](1,-1,-1):[1,1,0](1,-1,-1)) ([0,1,1](1,-1,1):[0,1,1](1,-1,1)) ([1,0,-1](1,-1,1):[1,0,-1](1,-1,1)) ([1,1,0](1,-1,1):[1,1,0](1,-1,1))
- rank 1: ([0,1,-1](1,1,1):[1,0,-1](1,1,1)) ([0,1,-1](1,1,1):[1,-1,0](1,1,1)) ([1,0,-1](1,1,1):[0,1,-1](1,1,1)) ([1,0,-1](1,1,1):[1,-1,0](1,1,1)) ([1,-1,0](1,1,1):[0,1,-1](1,1,1)) ([1,-1,0](1,1,1):[1,0,-1](1,1,1)) ([0,1,1](1,1,-1):[1,0,1](1,1,-1)) ([0,1,1](1,1,-1):[1,-1,0](1,1,-1)) ([1,0,1](1,1,-1):[0,1,1](1,1,-1)) ([1,0,1](1,1,-1):[1,-1,0](1,1,-1)) ([1,-1,0](1,1,-1):[0,1,1](1,1,-1)) ([1,-1,0](1,1,-1):[1,0,1](1,1,-1)) ([0,1,-1](1,-1,-1):[1,0,1](1,-1,-1)) ([0,1,-1](1,-1,-1):[1,1,0](1,-1,-1)) ([1,0,1](1,-1,-1):[0,1,-1](1,-1,-1)) ([1,0,1](1,-1,-1):[1,1,0](1,-1,-1)) ([1,1,0](1,-1,-1):[0,1,-1](1,-1,-1)) ([1,1,0](1,-1,-1):[1,0,1](1,-1,-1)) ([0,1,1](1,-1,1):[1,0,-1](1,-1,1)) ([0,1,1](1,-1,1):[1,1,0](1,-1,1)) ([1,0,-1](1,-1,1):[0,1,1](1,-1,1)) ([1,0,-1](1,-1,1):[1,1,0](1,-1,1)) ([1,1,0](1,-1,1):[0,1,1](1,-1,1)) ([1,1,0](1,-1,1):[1,0,-1](1,-1,1))
.....
~~~~

In this example, the rank \(0\) contains all the interactions of a
slip system with itself.

## Definition of the interaction matrix

The interaction matrix is defined through the `@InteractionMatrix`
keyword.

This keyword is followed by an array of values, ordered by growing
rank.



# The `@Interface` keyword

The `@Interface` keyword let the user specify interfaces to be
used. The keyword is followed by a list of interface name, separated
by commas.

Using this keyword is considered a bad practice. The user shall use the
`--interface` command line argument to specify which interface shall
be used.

## Example

~~~~{.cpp}
@Interface castem;
~~~~


# The `@InternalEnergy` keyword

The `@InternalEnergy` keyword allows the computation of the internal
energy density per unit of volume in the reference configuration.

The code provided must update a variable called `Psi_s`.

This update is called at the end of the behaviour integration, if
convergence was reached, after that:

- Internal state variables have been updated.
- The stress at the end of the time step have been computed.

## Example

~~~~ {#InternalEnergy .cpp}
@InternalEnergy{
  // updating the previous value
  // of the stored energy
  Psi_s += (sig|deel)/2;
}
~~~~

~~~~ {#InternalEnergy2 .cpp}
@InternalEnergy{
	Psi_s = (sig|eel)/2;
}
~~~~

~~~~ {#InternalEnergy2 .cpp}
@InternalEnergy{
	const strain tr = trace(eel);
	Psi_s = lambda*tr*tr/2+mu*(eel|eel);
}
~~~~

# The `@IsTangentOperatorSymmetric` keyword

The keyword `@IsTangentOperatorSymmetric` is not documented yet

# The `@IsotropicBehaviour` keyword

The `@IsotropicBehaviour` declares the behaviour to be isotropic. As
this symmetry is the default one, this keyword is seldom used.

## Example

~~~~{.cpp}
@IsotropicBehaviour;
~~~~

# The `@IsotropicElasticBehaviour` keyword

The `@IsotropicElasticBehaviour` is used to declare that the elastic
behaviour is isotropic even though the material as been declared
orthotropic. This declaration affects the `@RequireStiffnessTensor`
behaviour.

## Example

~~~~{.cpp}
@IsotropicElasticBehaviour;
~~~~

# The `@IterMax` keyword

The `@IterMax` keyword is a deprecated synonymous of
`@MaximumNumberOfIterations`.

# The `@JacobianComparisonCriterion` keyword

The `@JacobianComparisonCriterion` keyword defines the criterion value
used for the comparison of the user-defined jacobian to a numerically
computed one. This keyword must be followed by a positive
floating-point number.

## The jacobianComparisonCriterion parameter

The `@JacobianComparisonCriterion` keyword defines the default value
for the `jacobianComparisonCriterion` parameter. This parameter can be
changed at runtime.

## Example

~~~~{.cpp}
@JacobianComparisonCriterion 1.e-6;
~~~~



# The `@JacobianComparisonCriterium` keyword

The `@JacobianComparisonCriterium` keyword is a deprecated synonymous
of `@JacobianComparisonCriterion`.

# The `@Library` keyword

The `@Library` keyword let the user specify part of the generated
library name. This keyword is followed by the name of library.

This name must be a valid C++ identifier. The following characters are
legal as the first character of an identifier, or any subsequent
character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Example

~~~~{.cpp}
@Library AlcyoneLibrary;
~~~~

# The `@Link` keyword

The `@Link` keyword let the user specify additional linker flags used
to build their shared library/executables. This keyword must be
followed by a string or an array of strings.

Usage of the `@Link` keyword is deprecated a linker flags are not
portable. The user may prefer using the `LDFLAGS` environment
variable.

## Example

~~~~{.cpp}
// explicit link with libm.so
// (not necessary in pratice)
@Link "-lm";
~~~~


# The `@LocalVar` keyword

The `@LocalVar` keyword is a deprecated synonymous of
`@LocalVariable`.

# The `@LocalVariable` keyword

The keyword `@LocalVariable` is not documented yet

# The `@MFront` keyword

The keyword `@MFront` is not documented yet

# The `@Material` keyword

The `@Material` keyword let the user specify which material is treated
by the current file. This keyword is followed by the name of the
material.

This name must be a valid `C++` identifier. The following characters
are legal as the first character of an identifier, or any subsequent
character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Example

~~~~{.cpp}
@Material UO2;
~~~~

# The `@MaterialLaw` keyword

The `@MaterialLaw` keyword imports the definition of a material law
defined in a mfront file and compiles, as part of the current library,
a function using the `mfront` interface. This function is available
in every standard code blocks.

The `@MaterialLaw` keyword is followed by a string or an array of
string identifying `mfront` files. 

## Search paths

Files to be imported are searched, in that order:

- starting from the current directory.
- starting from directories specified with one of the `--search-path`
  or the `--include` (`-I`) `mfront`' command line options.
- staring from directories specified through the `MFRONT_INCLUDE_PATH`
  environment variable.

## Note

The `mfront` interface has been created to avoid names conflict.

For internal reasons, `mfront` reports the creation of an auxiliary
library which is of no use.

## Example

~~~~{.cpp}
@MaterialLaw "UO2_YoungModulus.mfront";
~~~~

# The `@MaterialProperty` keyword

The `@MaterialProperty` keyword let the user define one or several
material properties. This keyword is followed by the type of the
material property and a list of material properties names separated by
commas.

The material properties names must be valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Arrays

One may declare an array of material properties by specifying the
array size after the material property name.

## External names

It is recommended to associate to a material property a glossary or an
entry name through the methods `setGlossaryName` or `setEntryName`
respectively.

## Example

~~~~{.cpp}
// scalar material property
@MaterialProperty stress young;
young.setGlossaryName("YoungModulus");
~~~~

# The `@MaximalTimeStepScalingFactor` keyword

The `@MaximalTimeStepScalingFactor` let the user define the default
value of the `maximal_time_step_scaling_factor` parameter.

This parameter defines the maximal value by which the time step will
be increase in case of convergence. This parameter acts as a lower
bound for the value returned by the
`@APosterioriTimeStepScalingFactor` code block.

This keyword is followed by a number which must be strictly greater
than one.

## Note

By default (i.e. if the `@MaximalTimeStepScalingFactor` is not used),
the default value for `maximal_time_step_scaling_factor` parameter
will be very large so that the value returned by the
`@APosterioriTimeStepScalingFactor` code block will not be bounded.

## Example

~~~~ {#MaximalTimeStepScalingFactor .cpp}
@MaximalTimeStepScalingFactor 1.2;
~~~~

# The `@MaximumIncrementValuePerIteration` keyword

The keyword `@MaximumIncrementValuePerIteration` is not documented yet

# The `@MaximumNumberOfIterations` keyword

The `@MaximumNumberOfIterations` keyword let the user define the
maximum number of iterations allowed. It is followed by a positive
integer.

## The iterMax parameter

The `@MaximumNumberOfIterations` keyword defines the default value for
the `itermax` parameter. This parameter can be changed at runtime.

## Example

~~~~{.cpp}
@MaximumNumberOfIterations 200;
~~~~

# The `@Members` keyword

The keyword `@Members` is not documented yet

# The `@MinimalTimeStepScalingFactor` keyword

The `@MinimalTimeStepScalingFactor` let the user define the default
value of the `minimal_time_step_scaling_factor` parameter.

This parameter defines the minimal value by which the time step will
be reduced in case of non convergence or when the time step is rejected
by one of the `@APrioriTimeStepScalingFactor` or
`@APosterioriTimeStepScalingFactor` code blocks.

This keyword is followed by a number which must be strictly positive
and strictly lower than one.

## Note

By default (i.e. if the `@MinimalTimeStepScalingFactor` is not used),
the default value for `minimal_time_step_scaling_factor` parameter
will be 0.1.

## Example

~~~~ {#MinimalTimeStepScalingFactor .cpp}
@MinimalTimeStepScalingFactor 0.5;
~~~~

# The `@Model` keyword

The `@Model` keyword is used to call an external model from a
behaviour. This model is called before any code block defining the
behaviour.

This model is meant to make evolve one or more state variables of the
material.

From the behaviour point of view, those state variables are declared
as additional auxiliary state variables, but their values and their
increments over the time step are known. *Those variables are meant to
be used like external state variables*.

## Note

In the behaviour, the names of the variables described by the model
are the same as in the model.
 
The entry/glossary names of those variables shall be defined in the
model.

## Example

~~~~ {#Model .cpp}
// Evolution of the \(\beta\) phase
@Model "M5PhaseTransformation_EdgarV3_SRMA2017.mfront";
~~~~


# The `@ModellingHypotheses` keyword

The keyword `@ModellingHypotheses` is not documented yet

# The `@ModellingHypothesis` keyword

The keyword `@ModellingHypothesis` is not documented yet

# The `@NumericallyComputedJacobianBlocks` keyword

The `@NumericallyComputedJacobianBlocks` keyword is used to give a
list of jacobian blocks that have to be computed numerically.

This keyword can optionally be followed by a list of modelling
hypotheses. The list of jacobian blocks is given as an array.

## Notes

- This keyword is only valid for implicit dsl and an analytical
  jacobian.
- This keyword can be used multiple times. The newly declared jacobian
  blocks are added to the existing ones.

## Example

~~~~ {#NumericallyComputedJacobianBlocks .cpp}
@NumericallyComputedJacobianBlocks {dfp_ddeel,dfeel_ddeel};
~~~~

# The `@OrthotropicBehaviour` keyword

The `@OrthotropicBehaviour` declares the behaviour to be orthotropic.

As an option, the orthotropic behaviour can be followed by an
orthotropic axes convention. Three orthotropic axes convention are
currently supported:

- `Default`: no specific treatment. This can lead to serious
  difficulties as there is no way of ensuring the consistency of the
  definition of the orthotropic axes in most solvers.
- `Pipe`: the behaviour has been written using the Pipe convention
  described in the MFront behaviour manual. With this convention, the
  results of various keywords depends on the modelling hypothesis. For
  example, this option allows a consistent definition of the elastic
  material properties (see `@StiffnessTensor`, `@ElasticProperties`),
  thermal expansion coefficients (see `@ComputeThermalExpansion`) and
  the proper application of swelling (see `@AxialGrowth`, `Swelling`).
- `Plate`: when modelling plates, an appropriate convention is the following:
    - The first material axis is the rolling direction
    - The second material axis is the in plane direction perpendicular to
      the rolling direction (transverse direction).
    - The third material axis is the normal to the plate.
  By definition, this convention is only valid for \(3D\), \(2D\) plane
  stress, strain and generalized plane strain modelling hypotheses.

## Examples

~~~~{.cpp}
@OrthotropicBehaviour;
~~~~

~~~~{.cpp}
@OrthotropicBehaviour<Pipe>;
~~~~

# The `@Parameter` keyword

The `@Parameter` keyword declares a new parameter or a list of new
parameters. Optionally, the default value of the declared parameters
may also be given following various C++ standard assignment syntaxes.

The default value of a parameter can also be declared after its
declaration using the `setDefaultValue` method.

## Example

~~~~{.cpp}
@Parameter  R0 = 500;
@Parameter  Q1{1000000000},b1{0.000001};
@Parameter  Q2(0),b2(0);
@Parameter  fc;
fc.setDefaultValue(1.e-2);
~~~~


# The `@Parser` keyword

The `@Paser` keyword is a deprecated synonymous of `@DSL`.

# The `@PerturbationValueForNumericalJacobianComputation` keyword

The `@PerturbationValueForNumericalJacobianComputation` keyword let the
user defines the perturbation value used to compute the numerical
jacobian.

If \(Y\) is the vector of integration variables and \(F\) the function
defining the implicit system, the \(j^{\text{th}}\) column of the
numerical jacobian \(J^{n}\) is defined by the centered finite difference
formulae:
\[
J^{n}(i,j)=\frac{F(Y_{i}^{+\epsilon})-F(Y_{i}^{-\epsilon})}{2\,\epsilon}
\]
where:
\[
Y_{i}^{+\epsilon}(j)=
\left\{
  \begin{aligned}
    Y(j)          & \text{ si } &j \neq i \\
    Y(i)+\epsilon & \text{ si } &j =    i
  \end{aligned}
\right.
\]

## Example

~~~~{.cpp}
@PerturbationValueForNumericalJacobianComputation 1.e-7;
~~~~

# The `@PhysicalBounds` keyword

The `@PhysicalBounds` keyword let the use define the physical domain
of a variable.

The `@PhysicalBounds` keyword is followed by a variable name, the
keyword in and an interval. The interval may contain the infinity,
represented by the '*' character.

## Effect

In implicit schemes, if physical bounds are set on an integration
variable, this variable is bounded to satisfy them during the internal
iterations.

If a variable is found to be out of its physical bounds, the
computations are stopped. The tests are performed at different stages
of the integration depending on the nature of the variable.

## Example

~~~~{.cpp}
// a temperature (in Kelvin) can't be negative
@PhysicalBounds T in [0:*[;
~~~~


# The `@PostProcessing` keyword

The `@PostProcessing` keyword introduces a code block that can be used
to perform computations independently of the behaviour integration.

The outputs of post-processing are stored in so-called *post-processing
variables*. See the `@PostProcessingVariable` for details.

Post-processing is typically meant to be called at the end of a time
step, when the equilibrium has been reached.

Because post-processing is called independently of the behaviour
integration step, special care to the meaning of the variables must be
taken:

- The thermodynamic forces will have their values at the end of the time
  step.
- The state variables and auxiliary state variables will have their
  values at the end of the time step. The associated increments, if
  defined, will be null.

The values of the thermodynamic forces, state variables, auxiliary state
variables at the beginning of the time step are available in a special
data structure named `initial_state`.

Concerning material properties, they have their values at the end of the
time step as usual.

For the gradients and external state variables have their values at the
end of the time step. Their values at the beginning of the time step are
available in the `initial_state` data structure (if defined). Their
increments have their usual values.

> **About initialisation of local variables**
>
> The code block defined by the `@InitLocalVariables` code block
> shall be called before the execution of the post-processing.
> However, this code block will be called with the thermodynamic forces, 
> state variables, auxiliary state variables at the end of the time step.

## Example of usage

The following code defines a post-processing computing the principal
strain at the end of the time step:

~~~~{.cxx}
//! principal strains
@PostProcessingVariable tvector<3u,strain> εᵖ;
εᵖ.setEntryName("PrincipalStrain");
//! compute the principal strain
@PostProcessing PrincipalStrain {
  εᵖ = eto.computeEigenValues();
}
~~~~
# The `@PostProcessingVariable` keyword

Post-processing variables are introduced by the
`@PostProcessingVariable` keyword.

Post-processing variables are only defined in post-processing, and
can't be used in the other code blocks.

Contrary most variables (internal state variables, external state
variables, etc.), post-processing variables can be defined after the
first code block. However, care must be taken to declare post-processing
variables **before** their use in a post-processing.

Note that a post-processing variable can be used in different
post-processing. Typically, one may compute the principal strains in a
dedicated post-processing and in a post-processing computing the
principal strains and the strain eigen vectors.

## Exemple of usage

~~~~{.cxx}
//! principal strains
@PostProcessingVariable tvector<3u,strain> εᵖ;
~~~~
# The `@PredictionOperator` keyword

The keyword `@PredictionOperator` is not documented yet

# The `@Predictor` keyword

The keyword `@Predictor` is not documented yet

# The `@Private` keyword

The `@Private` keyword let the user define private methods or members
of in the generated behaviour class.

## Example

~~~~{.cpp}
@Private{
  void display(void) const{
    std::cout << "eto " << this->eto << std::endl;
  }
} // end of @Private
~~~~

# The `@ProcessNewCorrection` keyword

The `@ProcessNewCorrection` keyword introduces a code block called when
a new correction of the increments of the integration variables is
available.

This method can be used to:

- Limit the amplitude of the correction (see also the
  `setMaximumIncrementValuePerIteration` method).
- Implement a line-search algorithm.
- Implement the decondensation step when some integration variables were
  eliminated by static condensation.

This increment is stored in an array called `delta_zeros`. The
`delta_zeros` variable is not meant to be used directly by the users and
views to the corrections of the increments of the integration variables
are automatically declared in this code block.

Let `v` be an integration variable, then the variable `delta_dv` is a
view to the correction of the increment of this variable. If Unicode
notations are used, let `υ` be the symbolic name of `v`, then `δΔv` is
an alias for `delta_dv`.

The increments of the integration variables are not updated at this
stage of the resolution algorithm.

## Example

The following code limits the amplitude of the correction given to the
increment of the elastic strain:

~~~~{.cpp}
@ProcessNewCorrection{
 constexpr const real δΔεᵉˡ_m = 1.e-4;
 const auto e = abs(δΔεᵉˡ);
 if(e > δΔεᵉˡ_m){
   δΔεᵉˡ *= e / δΔεᵉˡ_m;
 }
}
~~~~
# The `@ProcessNewEstimate` keyword

The `@ProcessNewEstimate` keyword introduces a code block called after
the update of the increments of the integration variables.

This method may be used to compute local variables dependent on the
updated value of the integration variables.

For example, `MFront` may define or update this code block to evaluate
material properties dependent on the value of the state variable (for
example, a Young modulus depending of the porosity), if any.

# The `@Profiling` keyword

The `@Profiling` keyword is followed by a boolean. If true, several
high resolutions clocks will be introduced in the generated code to
profile performance bottlenecks. The total time spend in various
portions of the generated code will be stored and displayed when the
calling process exits.

## Example

~~~~{.cpp}
@Profiling true;
~~~~

# The `@RejectCurrentCorrection` keyword

The `@RejectCurrentCorrection` keyword introduces a code block called
when the current correction is rejected.

This method can be used to reject the decondensation step when some
integration variables were eliminated by static condensation.

This increment is stored in an array called `delta_zeros`. The
`delta_zeros` variable is not meant to be used directly by the users and
views to the corrections of the increments of the integration variables
are automatically declared in this code block.

# The `@Relocation` keyword

The `@Relocation` keyword can be used to impose a boundary condition
specific the fuel performances describing the rigid body translation
of fuel pellet fragment as an additional strain.

- an external state variable name.
- a string giving an mfront file implementing a relocation model.

## Modelling hypotheses affected

This keyword is only effective in:

- axisymmetrical generalised plane strain.
- axisymmetrical generalised plane stress.
- generalised plane strain.
- plane strain.
- plane stress.

No expansion are added in the tridimensional and axisymmetrical
modelling hypotheses.

## Physical description

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

## Examples

~~~~ {#Relocation .cpp}
// relocation defined by an external model
@Relocation 'UO2Relocation.mfront';
~~~~

~~~~ {#Relocation2 .cpp}
// relocation defined by an external state variable
@ExternalStateVariable real r;
r.setEntryName("Relocation");
@Relocation r;
~~~~

# The `@RequireStiffnessOperator` keyword

The `@RequireStiffnessOperator` keyword is a deprecated synonymous of
`@RequireStiffnessTensor`.

# The `@RequireStiffnessTensor` keyword

The `@RequireStiffnessTensor` keyword requires the stiffness tensor to
be computed by the calling code. This generally means that some extra
material properties will be introduced and handled by the interface
before the behaviour integration.

By default, the stiffness tensor will have the same symmetry as the
behaviour (see `@IsotropicElasticBehaviour` to change this in the case
of an orthotropic behaviour).

~~~~{.cpp}
@RequireStiffnessTensor true;
~~~~



# The `@RequireThermalExpansionCoefficientTensor` keyword

The `@RequireThermalExpansionCoefficientTensor` keyword requires the
thermal expansion coefficient tensor to be computed by the calling
code. This generally means that some extra material properties will be
introduced and handled by the interface before the behaviour
integration.

The thermal expansion tensor will have the same symmetry as the
behaviour. For example, for an isotropic behaviour, the thermal
expansion tensor will be proportional to the identity. For an
orthotropic behaviour, the thermal expansion tensor is diagonal and
expressed in the material frame.

~~~~{.cpp}
@RequireThermalExpansionCoefficientTensor true;
~~~~

# The `@SlidingSystem` keyword

A synonym of `@SlipSystem`.
See `@SlipSystem` for details.

# The `@SlidingSystems` keyword

A synonym of `@SlipSystems`.
See `@SlipSystems` for details.

# The `@SlipSystem` keyword

A single slip systems family can be defined by one of the following
keywords: `@SlidingSystem`, `@GlidingSystem` or `@SlipSystem`. Those
keywords are followed by the definition of one family of slip systems.

A slip system is defined by its plane and its Burgers'vector as
follows, for a `Cubic` and `FCC` crystal structures:

~~~~{.cpp}
<1,-1,0>{1,1,1}
~~~~

For hexagonal structures, the slip systems are defined through a four
indices notation.

## Example

Thus, an example of the usage of the `@SlidingSystem` keyword is:

~~~~{.cpp}
@SlidingSystem <1,-1,0>{1,1,1};
~~~~

For a given slip systems family, a set of slip systems are generated
by symmetry using code extracted from the `NUMODIS` code. The previous
example defines \(12\) slip systems.

Once the sliding system families has been defined, a static integer
variable called `Nss` is available which contains the total number of
slip systems defined. In the previous example, `Nss` value is \(12\).

This value can be used to define additional state variables:

~~~~{.cpp}
//! equivalent plastic strain on each slip system
@AuxiliaryStateVariable real p[Nss];
~~~~

For post-processing reasons, one needs to know in which order the slip
systems are defined. This question is treated in the next paragraph.

Also, note that for the \(i^{\text{th}}\) slip systems family, a
static integer variable beginning by `Nss` and followed by the value
of \(i\) is defined. In the previous example, a unique slip systems
family is defined and a variable `Nss0` is made available. Of course,
in this case `Nss0` is equal to `Nss`. Those variables may be useful
to define variables specific to a slip system family.

## Getting information about the generated slip systems

The list of the generated slip systems can be retrieved using
`mfront-query`.

Consider the following example:

~~~~{.cpp}
@DSL       ImplicitFiniteStrain;
@Behaviour SlipSystemGenerationTest;
@Brick     FiniteStrainSingleCrystal;
@CrystalStructure FCC;
@SlidingSystem <1,-1,0>{1,1,1};
~~~~

If the previous code is saved a file called
`SlipSystemGenerationTest.mfront`, one may use `mfront-query` as
follows:

~~~~{.sh}
$ mfront-query --slip-systems SlipSystemGenerationTest.mfront
- <1,-1,0>{1,1,1}: [0,1,-1](1,1,1) [1,0,-1](1,1,1) [1,-1,0](1,1,1) [0,1,1](1,1,-1) [1,0,1](1,1,-1) [1,-1,0](1,1,-1) [0,1,-1](1,-1,-1) [1,0,1](1,-1,-1) [1,1,0](1,-1,-1) [0,1,1](1,-1,1) [1,0,-1](1,-1,1) [1,1,0](1,-1,1)
~~~~

The output shows that \(12\) slip systems were generated. All those
systems are equivalent by symmetry.

Concerning slip systems, the following queries are available:

~~~~{.sh}
$ mfront-query --help-behaviour-queries-list |grep slip
--slip-systems          : list all the slip systems, sorted by family
--slip-systems-by-index : list all the slip systems
~~~~

The `--slip-systems-by-index` query gives the index associated to a
given slip system, which is helpful for postprocessing purposes. For
this example:

~~~~{.sh}
$ mfront-query --slip-systems-by-index SlipSystemGenerationTest.mfront
- 0: [0,1,-1](1,1,1)
- 1: [1,0,-1](1,1,1)
- 2: [1,-1,0](1,1,1)
- 3: [0,1,1](1,1,-1)
- 4: [1,0,1](1,1,-1)
- 5: [1,-1,0](1,1,-1)
- 6: [0,1,-1](1,-1,-1)
- 7: [1,0,1](1,-1,-1)
- 8: [1,1,0](1,-1,-1)
- 9: [0,1,1](1,-1,1)
- 10: [1,0,-1](1,-1,1)
- 11: [1,1,0](1,-1,1)
~~~~

## Getting information about the orientation tensors

Concerning the orientation tensors, the following queries are
available:

- `--orientation-tensors`: list all the orientation tensors, sorted by
  family.
- `--orientation-tensors-by-index`: list all the orientation tensors.
- `--orientation-tensors-by-slip-system`: list all the orientation
  tensors.

#### Getting information about the Schmid factors

Concerning the Schmid factors, the following queries are
available:

- `--schmid-factors`: list all the Schmid factors, sorted by
  family.
- `--schmid-factors-by-index`: list all the Schmid factors.

~~~~{.sh}
$ mfront-query --schmid-factors-by-index='<1,1,1>' SlipSystemGenerationTest.mfront
~~~~

**Note** In this example, the direction must be surrounded by simple
quote to avoid interpretation by the shell.

# The `@SlipSystems` keyword

The `@SlipSystems` can be used to define several families of slip
systems. This keyword is followed by an array defining the various
slip systems. The user can refer to the description of the
`@SlipSystem` keyword for a detailed description.

# The `@Sources` keyword

The `@Sources` keyword let the user define a code block that will be
integrated in the generated sources of a behaviour. This allows the
user to implement their own classes or functions. These declarations of
such classes or functions can be made in a code block introduced by
the `@Includes` keyword.

~~~~{.cpp}
@Includes{
  //! declaration of the function f
  void f(void);
}

@Sources{
  // source of the function
  void f(void){
    std::cout << "Example of a function " << std::endl;
  } // end of f
}
~~~~


# The `@SpeedOfSound` keyword

The `@SpeedOfSound` keyword introduces the computation of an upper bound
of the speed of sound in a material. This quantity is mostly meant to be
used by explicit mechanical solvers to evaluate an upper bound of the
critical time step.

The code block must compute a variable called `v_sound` (or `vₛ` if
`UTF-8` encoding is used).

The mass density in the initial configuration is available in a variable
called `rho_m0` (or `ρₘ₀` if `UTF-8` encoding is used).

## Example

~~~~ {#SpeedOfSound .cpp}
@SpeedOfSound{
 v_sound = sqrt(young / rho_m0);
} 
~~~~

~~~~ {#SpeedOfSound_utf8 .cpp}
@SpeedOfSound{
 vₛ = sqrt(young / ρₘ₀);
} 
~~~~

# The `@StateVar` keyword

The `@StateVar` keyword is a deprecated synonymous of
`@StateVariable`.

# The `@StateVariable` keyword

The `StateVariable` keyword introduces one or several new state
variables. It is followed by a type name and the name(s) of the
variable(s) declared, separated by commas.

The state variables names must be valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Arrays

One may declare an array of state variables by specifying the array
size after the state variable name.

## External names

It is recommended to associate to a state variable a glossary or an
entry name through the methods `setGlossaryName` or `setEntryName`
respectively.

## Example

~~~~{.cpp}
// scalar state variable
@StateVariables strain p;
// symmetric tensors state variable
@StateVariables StrainStensor evp,evp2;
~~~~

# The `@StaticVar` keyword

The `@StaticVar` keyword is a deprecated synonymous of
`@StaticVariable`.

# The `@StaticVariable` keyword

The `@StaticVariable` keyword let the user define a constant
value. Unlike parameters (see the `@Parameter` keyword), static
variables' values can't be changed after compilation.

This keyword must be followed by the type of the constant, its name,
an equal sign and its value.

## Example

~~~~{.cpp}
@StaticVariable real A = 1.234e56;
~~~~

# The `@StrainMeasure` keyword

This keyword let the user choose to the strain measure. This choice
also defines the meaning of the stress tensor by energetic duality.

The following values are supported:

- `LINEARISED` (or `LINEARISED`, or `HPP`): the behaviour is meant to
  be used in a small strain analysis.
- `GreenLagrange`: the behaviour is meant to be used in a finite
  strain analysis. The dual stress is the second Piola-Kirchhoff
  stress.
- `Hencky`: the behaviour is meant to be used in a finite
  strain analysis. The dual stress has now specific name.

Two options can be specified:

- `save_strain`: save the strain in a dedicated auxiliary state variable
  called `DualStrain`. Internally, the name of this auxiliary state
  variable is `saved_strain_measure`.
- `save_stress`: save the stress in a dedicated auxiliary state variable
  called `DualStress`. Internally, the name of this auxiliary state
  variable is `saved_dual_stress`.

If requested, the auxiliary state variables `saved_strain_measure` and
`saved_dual_stress` are updated at the beginning of the
`@UpdateAuxiliaryStateVariables` code block. For orthotropic behaviours,
it is worth mentioning that those variables are expressed in the
material frame.

## Note

1. The computation of the strain measure and its dual and the convertion
  of the tangent operator is delegated to the interface, because several
  finite element solver already provides those operations natively
  (`Code_Aster`, `ZebuloN`, etc...)

## Example

~~~~{#StrainMeasure .cpp}
@StrainMeasure Hencky;
~~~~

~~~~{#StrainMeasure2 .cpp}
@StrainMeasure{
  save_strain: true,
  save_stress: true
};
~~~~

# The `@Swelling` keyword

The `@Swelling` keyword allow the user to specify that an additional
stress-free expansion must be taken into account.

This keyword has the following options:

- `Volume`, stating that the swelling is given as a volume change
  (\(\displaystyle\frac{\Delta\,V}{V_{0}}\))
- `Linear` (the default), stating that the swelling is given as a
  linear change (\(\displaystyle\frac{\Delta\,l}{l_{0}}\))

The `@Swelling` keyword is then followed by one or three definition of
stress-free expansion.

If one stress-free expansion is defined, the swelling is computed as
an isotropic second order tensor.

If three stress-free expansions are defined, the swelling is computed
as a diagonal second order tensor:
\[
s =
\left(
\begin{array}{ccc}
	\left.\frac{\Delta\,l}{l_{0}}\right|_{0} & 0 & 0 \\
	0 & \left.\frac{\Delta\,l}{l_{0}}\right|_{1} & 0 \\
	0 & 0 & \left.\frac{\Delta\,l}{l_{0}}\right|_{2} \\
\end{array}
\right)
\]

A stress-free expansion is either defined by:

- A string pointing to a mfront model.
- `0`, which allow the definition of a null swelling.
- The name of an external state variable.

## Notes

A null swelling is not allowed when only one stress-free expansion

The definition of three stress-free expansion is only valid for
orthotropic behaviours and is not compatible with the `Volume` option.

## Examples

~~~~ {#Swelling .cpp}
@ExternalStateVariable strain s;
s.setGlossaryName("SolidSwelling");
@Swelling<Volume> s;
~~~~

# The `@TangentOperator` keyword

The `TangentOperator` keyword introduces a code block used to define
the tangent operator. This code is called once the integration
variables, the stresses and the auxiliary state variables (see the
`@UpdateAuxiliaryStateVariables` keyword) have been updated.

The kind of tangent operator requested is given by variable named
`smt` (stiffness matrix type). As the time of writing this notice,
the possible values for `smt` are the following:

- `ELASTIC`: the elastic operator is requested (undamaged).
- `SECANT`: the secant operator is requested. This operator is only
  meaningful for behaviours describing brittle damage. The expected
  operator is then the elastic operator modified by the damage
  variables.
- `TANGENTOPERATOR`: the tangent operator is requested. This operator
  is seldom implemented as the consistent tangent operator is more
  interesting (and easier to compute).
- `CONSISTENTTANGENTOPERATOR`: the consistent tangent operator is
  requested. If an implicit scheme is used in small strain, the
  consistent tangent operator can be computed using the jacobian of
  the implicit system, see the mfront behaviours documentation for
  details.

## Example

~~~~{.cpp}
@TangentOperator{
  using namespace tfel::material::lame;
  if((smt==ELASTIC)||(smt==SECANTOPERATOR)){
    computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt,lambda,mu);
  } else if (smt==CONSISTENTTANGENTOPERATOR){
    StiffnessTensor De;
    Stensor4 Je;
    computeElasticStiffness<N,Type>::exe(De,lambda,mu);
    getPartialJacobianInvert(Je);
    Dt = De*Je;
  } else {
    return false;
  }
}
~~~~

# The `@Theta` keyword

The `Theta` keyword is used to define the default value of \(\theta\)
parameter used by implicit schemes. If the `Theta` keyword is not
used, implicit domain specific languages provide their own specific
default value (either \(0.5\) or \(1\)).

The value given to \(\theta\) must be in the range \(]0:1]).

The value of \(\theta\) can be changed at runtime by modifying the
`theta` parameter (see the `@Parameter` keyword).

## Example

~~~~{.cpp}
@Theta 0.5;
~~~~

# The `@UnitSystem` keyword

The `@UnitSystem` keyword declares that the state variables, external
state variables and parameters are expressed in a given unit system. In
the current version of `MFront`, the only supported unit system is the
international system of units, denoted `SI`.

One advantage of declaring a unit system is that physical bounds of
variables associated with a glossary entry can automatically be declared
by `MFront`.

For instance the declaration of the physical bounds for the temperature
and the porosity is automatic if the `SI` unit system is used.

## Example of usage

~~~~{.cxx}
@UnitSystem SI;
~~~~

# The `@UpdateAuxiliaryStateVariables` keyword

The `UpdateAuxiliaryStateVariables` introduces a code block meant to
update the auxiliary state variables after integration.

In implicit domain specific languages, the code declared by
`UpdateAuxiliaryStateVariables` is called once the integration
variables (including state variables) and stresses (see the
`@ComputeFinalStress` keyword) have been updated. The external state
variables are not updated.

In Runge-Kutta domain specific languages, the code declared by
`UpdateAuxiliaryStateVariables` is called after each successful time
step. Keep in mind that most Runge-Kutta algorithms performs internal
substepping: in this case, the code declared by
`UpdateAuxiliaryStateVariables` may be called several time during the
behaviour integration. An additional variable called `dt_`, which is
lower than the total time step increment `dt` if substepping is
performed, gives the current time increment. The external state
variables are set to their values at the current date.

## Example (Implicit dsl)

~~~~{.cpp}
@UpdateAuxiliaryStateVariables{
  // valeur de milieu de pas, q ayant
  // déjà été mis à jour
  const real q_ = q-(1-theta)*dq;
  const real Q = Q0 + (Qm - Q0) * (1 - exp(-2 * Mu * q_) );
  R+=b*(Q-R)*dp;
}
~~~~

## Example (Runge-Kutta dsl)

~~~~{.cpp}
@UpdateAuxiliaryStateVariables{
  sigeq = sqrt(sig|sig);
}
~~~~

# The `@UpdateAuxiliaryStateVars` keyword

The `@UpdateAuxiliaryStateVars` keyword is a deprecated synonymous of
`@UpdateAuxiliaryStateVariables`.

# The `@UsableInPurelyImplicitResolution` keyword

The `@UsableInPurelyImplicitResolution` is a specifier which states
that the behaviour is usable in a purely implicit resolution, i.e. if
we can call the behaviour by only providing the values of external
state variables at the end of the time step and by setting their
increments to zero.

This behaviour characteristic may or may not have any influence
depending on the calling code. As the time of writing this notice,
only the `Licos` fuel performance code take advantage of this
characteristic. If true, the `Licos` code will simplify the definition
of the external state variables (called material loadings in this
context) which may result in some gain in time and memory space.

## Example

~~~~{.cpp}
@UsableInPurelyImplicitResolution true;
~~~~


# The `@UseQt` keyword

The `UseQt` keyword (use quantities) specify if the behaviour
compilation should perform compile-time units checks. It is followed
by a boolean.

## Note

This feature is still experimental and is disabled in most cases.

## Example

~~~~{.cpp}
@UseQt true;
~~~~
