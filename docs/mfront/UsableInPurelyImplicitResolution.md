The `@UsableInPurelyImplicitResolution` is a specifier which states
that the behaviour is usable in a purely implicit resolution, i. e. if
we can call the behaviour by only providing the values of external
state variables at the end of the time step and by setting their
increments to zero.

This behaviour characteristic may or may not have any influence
depending on the calling code. As the time of writting this notice,
only the `Licos` fuel performance code take advantage of this
characteristic. If true, the `Licos` code will simplify the definition
of the external state variables (called material loadings in this
context) which may result in some gain in time and memory space.

## Example

~~~~{.cpp}
@UsableInPurelyImplicitResolution true;
~~~~

