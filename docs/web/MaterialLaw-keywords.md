% `MaterialLaw` keywords


# The `;` keyword

The keyword `;` is not documented yet

# The `@Author` keyword

The `@Author` keyword is used to give the name of the person who wrote
the `mfront` file.

All the following words are appended to the author's name up to a
final semi-colon.

Note: The name of the person who formulated the material property,
behaviour or model shall be given in the description section (see the
`@Description` keyword).

## Example

~~~~ {#Author .cpp}
@Author Éric Brunon;
~~~~



# The `@Bounds` keyword

The `@Bounds` keyword let the user define the domain of validity of
the law.

The `@Bounds` keyword is followed by a variable name, the keyword in
and an interval. The interval may contain the infinity, represented by
the '*' character.

## Effect

What happens if a variable if found to be out of its bounds depends on
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


# The `@Constant` keyword

The `@Constant` keyword let the user define a constant value.  This
keyword is a synonym of `@StaticVariable` for a real variable.

This keyword must be followed by the name of the constant and its
value.

## Example

~~~~{.cpp}
@Constant A 1.234e56;
~~~~



# The `@DSL` keyword

The `@DSL` keyword specify the domain specific language (dsl) used.

The list of available dsl's is returned by the `--list-dsl` option of
`mfront`:

~~~~{.bash}
$ mfront --list-dsl
~~~~

As the time of writting this notice, the following dsl's are available:

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
  based on a theta method..
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

A DSL' behaviour may be changed using options defined either in the
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
  parameters can be modified from a external state file. This feature is
  only implemented by a few interfaces and is enabled by default.
- `default_out_of_bounds_policy` (string), which selects the default out
  of bounds policy. Allowed values ar `None` (the default), `Warning` or
  `Strict`.
- `out_of_bounds_policy_runtime_modification` (boolean), which states if
  the out of bounds policy can be changed at runtime. By default, this
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
semi-colon.

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

# The `@Function` keyword

The `@Function` keyword introduces the C++ code computing the value a
the material property.

Note: the default output is called `res`. This is changed by the
`@Output` keyword.

## Note

Although most interfaces use double for floatting point operations,
this may not be always the case. A special `typedef` named `real` is
thus introduced by the interfaces. We advice the user to use this
`typedef` to define their local variables.

## Example

~~~~{.cpp}
@Function
{
  const real C0 =    575.57;
  const real C1 = -21094.;
  Cp = C0+C1/T;
}
~~~~

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

# The `@Input` keyword

The `@Input` keyword specifies one or several inputs of a material
law. This keyword is followed by the names of the inputs, separated by
commas.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## External name

It is recommended to associate to an input a glossary or an entry
name through the methods `setGlossaryName` or `setEntryName`
respectively.

## Example

~~~~{.cpp}
@Input T,p;
T.setGlossaryName("Temperature");
p.setGlossaryName("Porosity");
~~~~

# The `@Interface` keyword

The `@Interface` keyword let the user specify interfaces to be
used. The keyword is followed by a list of interface name, separated
by commas.

Using this keyword is considered a bad pratice. The user shall use the
`--interface` command line argument to specify which interface shall
be used.

## Example

~~~~{.cpp}
@Interface castem;
~~~~


# The `@Law` keyword

The `@Law` keyword allows the user to associate a name to the material
law being treated. This keyword is followed by a name.

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

## Name of the generated functions or classes

The names of the generated functions or classes depend on the law name
as specifified with the `@Law` keyword but may also include the
material name, as specified by the `@MaterialLaw` keyword.

## Example

~~~~{.cpp}
@Law ThermalExansionCoefficient_SRMA;
~~~~


# The `@Library` keyword

The `@Library` keyword let the user specify part of the generated
libary name. This keyword is followed by the name of library.

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


# The `@MFront` keyword

The `@MFront` keyword leads to the generation of sources from some
mfront files as a subprocess of the current file treatment. This
keyword is followed by a pair of strings or array of strings. The
first member of the pair describes which a list of files to be
treated. The second member of the pair describes a list of interfaces.

## Example

~~~~{.cpp}
@MFront {"YoungModulus.mfront",{"c","excel"}};
~~~~


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
an function using the `mfront` interface. This function is available
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

# The `@Output` keyword

The `Output` keyword changes the name of the output variable. By
default, this name is `res`. This keyword is followed by the new name
of the output variable.

## Example

~~~~{.cpp}
@Output b ;
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

# The `@PhysicalBounds` keyword

The `@PhysicalBounds` keyword let the use define the physical domain
of a variable.

The `@PhysicalBounds` keyword is followed by a variable name, the
keyword in and an interval. The interval may contain the infinity,
represented by the '*' character.

## Effect

In implicit schemes, if physical bounds are set on a integration
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

# The `@UnitSystem` keyword

The `@UnitSystem` keyword declares that the state variables, external
state variables an parameters are expressed in a given unit system. In
the current version of `MFront`, the only supported unit system is the
international system of units, denoted `SI`.

One advantage of declaring an unit system is that physical bounds of
variables associated with a glossary entry can automatically be declared
by `MFront`.

For instance the declaration of the physical bounds for the temperature
and the porosity is automatic if the `SI` unit system is used.

## Example of usage

~~~~{.cxx}
@UnitSystem SI;
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
