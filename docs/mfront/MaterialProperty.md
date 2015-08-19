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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
