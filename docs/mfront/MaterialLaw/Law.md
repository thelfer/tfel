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

