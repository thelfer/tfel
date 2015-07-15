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

### Variable depth

The depth of a variable is the number of preceedings values of this
variable needed to perform the computations. A depth of \(0\) means
that only the value at the end of the time step is required. A depth
of \(1\) means that we need the value of the variable a the end of the
time step and the value at the beginning of the time step.

The depth of a variable is set using the `setDepth` method.

If `p` is the variable of interest, `p` is its value at the end of the
time step, `p_1` is the value at the beginning of the time step.

### External name

It is recommended to associate to an input a glossary or an entry name
through the methods `setGlossaryName` or `setEntryName` respectively.

### Example

~~~~{.cpp}
@Input T,p;
T.setGlossaryName("Temperature");
p.setGlossaryName("Porosity");
// we need the value at the end of the time step and the value at the
// beginning at the time step
p.setGlossaryName("Porosity");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
