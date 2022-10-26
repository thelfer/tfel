% `MaterialLaw` keywords


# The `@Author` keyword

The `@Author` keyword is used give the name of the person who wrote
the `mfront` file.

All the following words are appended to the author's name up to a
final semi-colon.

Note: The name of the person who formulated the material property,
behaviour or model shall be given in the description section (see the
`@Description` keyword).

## Example

~~~~ {#Author .cpp}
@Author Éric Brunon;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@Constant` keyword

The `@Constant` keyword let the user define a constant value.  This
keyword is a synonym of `@StaticVariable` for a real variable.

This keyword must be followed by the name of the constant and its
value.

## Example

~~~~{.cpp}
@Constant A 1.234e56;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



# The `@Date` keyword

The `@Date` keyword allows the user to precise when the mfront file
was written.

All the following words are appended to the date up to a final
semi-colon.

## Example

~~~~{.cpp}
@Date 2008-11-17;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Includes` keyword

The `@Includes` introduces a block were the user may define some
preprocessor directives, typically #include directives (hence the
name).

## Example

~~~~{.cpp}
@Includes{
#include<fstream>
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# The `@MFront` keyword

The `@MFront` keyword leads to the generation of sources from some
mfront files as a subprocess of the current file treatment. This
keyword is followed by a pair of strings or array of strings. The
first member of the pair describes which a list of files to be
treated. The second member of the pair describes a list of interfaces.

## Example

~~~~{.cpp}
@MFront {"YoungModulus.mfront",{"c","excel"}};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Output` keyword

The `Output` keyword changes the name of the output variable. By
default, this name is `res`. This keyword is followed by the new name
of the output variable.

## Example

~~~~{.cpp}
@Output b ;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
