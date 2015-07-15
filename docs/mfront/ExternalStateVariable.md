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

### Arrays

One may declare an array of external state variables by specifying the
array size after the external state variable name.

### External names

It is recommended to associate to a external state variable a glossary
or an entry name through the methods `setGlossaryName` or
`setEntryName` respectively.

### Example

~~~~{.cpp}
// scalar external state variable
@ExternalStateVariable strain s;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
