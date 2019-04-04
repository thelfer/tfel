The `@MFront` keyword leads to the generation of sources from some
mfront files as a subprocess of the current file treatment. This
keyword is followed by a pair of strings or array of strings. The
first member of the pair describes which a list of files to be
treated. The second member of the pair describes a list of interfaces.

## Example

~~~~{.cpp}
@MFront {"YoungModulus.mfront",{"c","excel"}};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

