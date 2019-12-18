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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
