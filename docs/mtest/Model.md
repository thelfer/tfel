The `@Model` keyword is mostly a synomymous of the `@Behaviour`. The
only difference is that a check is done to see if the behaviour does not
declare any gradient, thermodynamic force nor any tangent operator
block.

## Example

~~~~{.cxx}
@Model<generic> 'src/libBehaviour.so' 'ode_rk54';
~~~~
