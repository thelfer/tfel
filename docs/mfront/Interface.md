The `@Interface` keyword let the user specify interfaces to be
used. The keyword is followed by a list of interface name, separated
by commas.

Using this keyword is considered a bad pratice. The user shall use the
`--interface` command line argument to specify which interface shall
be used.

Some interfaces can't be used together for the same behaviour, because
they add to the generated classes constructors taking the same
arguments (for example, the `aster` and `abaqus` interfaces, or the
`aster` and `cyrano` interfaces). `MFront` reports such combinations:
the behaviour must then be generated for each of those interfaces by a
separate call to `mfront`, in its own directory.

## Example

~~~~{.cpp}
@Interface castem;
~~~~

