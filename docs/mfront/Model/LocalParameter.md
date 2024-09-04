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
~~~~

