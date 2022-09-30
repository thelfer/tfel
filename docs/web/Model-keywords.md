% `Model` keywords


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


# The `@ConstantMaterialProperty` keyword

The `@ConstantMaterialPropery` let the user define a specific kind of
input to a model which is meant to be constant and uniform during the
computations. This allows the interfaces to optimize the generated
code. This keyword is followed by the names of the material
properties. Those names must be valid `C++` identifiers.

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

It is recommended to associate to a constant material property a
glossary or an entry name through the methods `setGlossaryName` or
`setEntryName` respectively.

## Example

~~~~{.cpp}
@ConstantMaterialProperty E;
E.setEntryName("EnergyReleasedByFission");
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

# The `@Domain` keyword

The `@Domain` let the user define the default domain on which a model
shall be applied. This keyword is followed by the name of the domain.

This keyword is deprecated as domains shall be explicitely specified
by specialisation.

## Example

~~~~{.cpp}
@Domain "Fuel";
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Domains` keyword

The `@Domains` let the user define the default domains on which a model
shall be applied. This keyword is followed by an array of strings.

This keyword is deprecated as domains shall be explicitely specified
by specialisation.

## Example

~~~~{.cpp}
@Domains {"MATRIX","CLADDING"};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Function` keyword

The `@Function` keyword defines a computational part of the
model. This keyword is followed by a string defining the name of this
part of the computation.

## Example

~~~~{.cpp}
@Function compute
{
  const real coef1 = 8.e-3;
  const real coef2 = 4.e-2;
  const real p_    = 0.5*(p+p_1);
  s = s_1 + coef1*exp(coef2-p_)*(Bu-Bu_1);
} // end of function compute
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@GlobalParameter` keyword

The `@GlobalParameter` keyword let the user defines model
parameters. This keyword is followed by the type and the names of the
parameters, separated by commas. The names of the variables must be
valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Default values

The user may attribute a default value to a global parameter through
the `setDefaultValue` method.

## Example

~~~~{.cpp}
@GlobalParameter real a;
a.setDefaultValue(1.23);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


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
law. This keyword is followed by the names of the inputs, separated
by commas.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Variable depth

The depth of a variable is the number of preceedings values of this
variable needed to perform the computations. A depth of \(0\) means
that only the value at the end of the time step is required. A depth
of \(1\) means that we need the value of the variable a the end of the
time step and the value at the beginning of the time step.

The depth of a variable is set using the `setDepth` method.

If `p` is the variable of interest, `p` is its value at the end of the
time step, `p_1` is the value at the beginning of the time step.

## External name

It is recommended to associate to an input a glossary or an entry name
through the methods `setGlossaryName` or `setEntryName` respectively.

## Example

~~~~{.cpp}
@Input T,p;
T.setGlossaryName("Temperature");
p.setGlossaryName("Porosity");
// we need the value at the end of the time step and the value at the
// beginning at the time step
p.setGlossaryName("Porosity");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@LocalParameter` keyword

The `@LocalParameter` keyword let the user defines specialisation
parameters. This keyword is followed by the type and the names of the
parameters, separated by commas. The names of the variables must be
valid `C++` identifiers.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Default values

The user may attribute a default value to a local parameter through
the `setDefaultValue` method.

## Example

~~~~{.cpp}
@LocalParameter real a;
a.setDefaultValue(1.23);
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

# The `@Model` keyword

The `@Model` defines the name of the model. The name must be a
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
@Model SolidSwelling;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# The `@Output` keyword

The `@Output` keyword specifies one or several outputs of a material
law. This keyword is followed by the names of the outputs, separated
by commas.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

## Variable depth

The depth of a variable is the number of preceedings values of this
variable needed to perform the computations. A depth of \(0\) means
that only the value at the end of the time step is required. A depth
of \(1\) means that we need the value of the variable a the end of the
time step and the value at the beginning of the time step.

The depth of a variable is set using the `setDepth` method.

If `p` is the variable of interest, `p` is its value at the end of the
time step, `p_1` is the value at the beginning of the time step.

## External name

It is recommended to associate to an output a glossary or an entry
name through the methods `setGlossaryName` or `setEntryName`
respectively.

## Example

~~~~{.cpp}
@Output T,p;
T.setGlossaryName("Temperature");
p.setGlossaryName("Porosity");
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
