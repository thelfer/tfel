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

### External name

It is recommended to associate to a constant material property a
glossary or an entry name through the methods `setGlossaryName` or
`setEntryName` respectively.

### Example

~~~~{.cpp}
@ConstantMaterialProperty E;
E.setEntryName("EnergyReleasedByFission");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
