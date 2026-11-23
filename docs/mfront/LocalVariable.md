The `@LocalVariable` keyword introduces one or several new local
variables. It is followed by a type name and the name(s) of the
variable(s) declared, separated by commas.

Local variables are data members of the behaviour class. They are
typically used to store values computed in a code block, such as
`@InitializeLocalVariables`, and used in others. Their values are not
saved from one time step to the other.

## Arrays

One may declare an array of local variables by specifying the array
size after the local variable name.

## Uninitialized local variables

The values of the arrays of local variables and of the local variables
whose types are fixed size mathematical objects (symmetric tensors,
tensors, tiny vectors, tiny matrices, fourth order tensors, etc.) are
set to zero when the behaviour is built, i.e. at each integration point
and at each time step.

The `setUninitialized` method states that the values of a local variable
shall be left uninitialized. This spares writing values which are never
read when the variable is assigned before being used, which is
significant for large objects, in particular when the behaviour is
integrated on a GPU.

~~~~{.cpp}
@LocalVariable Stensor4 De;
De.setUninitialized();
~~~~

Reading a value of such a variable before its assignment is an undefined
behaviour: it is up to the user to guarantee that the variable is
assigned before being used.

The `setUninitialized` method:

- is only available for local variables and for integration variables
  which are not state variables (see the `@IntegrationVariable`
  keyword),
- is not available for scalar variables,
- is meant for the fixed size mathematical objects of the `TFEL/Math`
  library: other types lead to a compilation error,
- does not affect the values of class type, such as quantities, which
  are still built by their default constructors,
- has no effect on dynamically allocated arrays.

## Example

~~~~{.cpp}
// scalar local variable
@LocalVariable stress mu;
// array of local variables
@LocalVariable real fv[2];
// symmetric tensor local variable, left uninitialized
@LocalVariable Stensor n;
n.setUninitialized();
~~~~
