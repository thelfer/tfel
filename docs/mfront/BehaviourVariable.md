The `@BehaviourVariable` defines a variable associated with another
behaviour. This keyword is followed by the name of the variable and a
set of options describing how this variable interacts with the
current behaviour.

## Example of usage

### Declaration

The following snippet declares a variable named `b1` associated with a
behaviour `Plasticity` defined in the file
`BehaviourVariablePlasticity.mfront`.

~~~~{.cpp}
@BehaviourVariable b1 {
  file: "Plasticity.mfront",
  variables_suffix: "1",
  external_names_prefix: "FirstPhase",
  store_gradients: true,
  store_thermodynamic_forces: true,
  shared_external_state_variables: {".+"}
};
~~~~

This example illustrates the complexity of the declaration of a
behaviour variable. The main reason for this complexity is due to the
fact that the calling behaviour is responsible for declaring and
managing the variables associated with `b1` (material properties,
internal state variables, external state variable), etc.

In particular, the behaviour variable and the calling behaviour may
share some variables (material properties, external state variables).

In this example, since no material property is declared as shared, the
calling behaviour will create one material property for each material
property declared in the behaviour `Plasticity` (see the
`shared_material_properties option below for details on how to declare
shared material properties). To avoid conflicts, the names and external
names of this new material properties are modified using the options
`variables_prefix`, `variables_suffix`, `external_names_prefix` or
`external_names_suffix`. For example, if the behaviour `Plasticity`
declares a material property named `young` associated with the glossary
name `YoungModulus`, the calling behaviour will declare a material
property named `young1` (due to the `variables_suffix` option) with the
external name `FirstPhaseYoungModulus` (due to the
`external_names_prefix` option).

In this example, the `shared_external_state_variables` option specifies
that all the external state variables declared by the behaviour
`Plasticity` are shared with the calling behaviour. For example, if the
`Plasticity` declares the temperature, no additional external state
variable will be declared if the calling behaviour also declares the
temperature as an external state variable.

### Integration of a behaviour variable

A behaviour variable can be integrated using the `integrate` method as
follows:

~~~~{.cxx}
const auto r = b1.integrate(smflag, smt);
~~~~

where `smflag` and `smt` are parameters associated with the computation
of the consistent tangent operator (see below).

### Retrieveing the tangent operator

The tangent operator can be retrieved using the `getTangentOperator`
method as follows:

~~~~{.cxx}
Dt = b1.getTangentOperator();
~~~~

## Options of the `@BehaviourVariable` keyword

- `file`: name of the `MFront` file in which the behaviour is defined.
  This entry is required.
- `variables_prefix`: prefix added in front of the variable names
  associated with the behaviour variable in the calling behaviour.
- `variables_prefix`: prefix added to variables that are not shared with
  the calling behaviour.
- `variables_suffix`: suffix added to variables that are not shared with
  the calling behaviour.
- `external_names_prefix`: prefix added to external names of variables
  that are not shared with the calling behaviour.
- `external_names_suffix`: suffix added to external names of variables
  that are not shared with the calling behaviour.
- `store_gradients`: flag stating if the gradients of the behaviour
  variable must be stored in a dedicated auxiliary state variable. This
  flag is `true` by default.
- `store_thermodynamic_forces`: flag stating if the thermodynamic forces
  of the behaviour variable must be stored in a dedicated auxiliary
  state variable. This flag is `true` by default.
- `shared_material_properties`: list of regular expressions used to
  select shared material properties. This list is empty by default.
- `shared_external_state_variables`: list of regular expressions used to
  select shared external state variables. This list is empty by default.
- `automatically_save_associated_auxiliary_state_variables`: flag
  stating if the persistent variables, gradients (if `stored_gradients`
  is true) or thermodynamic forces (if `stored_thermodynamic_forces` is
  true) of the behaviour variable shall automatically be saved in the
  `updateAuxiliaryStateVariables` method of the calling behaviour. This
  flag is `true` by default.

## Treatment of the gradients or thermodynamic variables of the behaviour variable

If the `store_gradients` option is set to `true`, an associated
auxiliary state variable is automatically declared by the calling
behaviour for each gradient of the behaviour variable. These auxiliary
state variables are obtained by applying the transformation rules on
names and external names.

If the `store_gradients` option is set to `true`, the gradients of the
behaviour variables are automatically initialized using the values of
the associated auxiliary state variable.

If the `store_gradients` option is set to `true`, the values of the
associated auxiliary state variable are automatically updated using the
values of the gradient of the behaviour variable plus its increment in
the `updateAuxiliaryStateVariables` method of the calling behaviour.

The `store_thermodynamic_forces` option has a similar role for
thermodynamic forces.

If the `automatically_save_associated_auxiliary_state_variables` option
is set to true, gradients (if `stored_gradients` is true) or
thermodynamic forces (if `stored_thermodynamic_forces` is true) of the
behaviour variable are automatically saved in the associated auxiliary
state variables at the beginning of the `updateAuxiliaryStateVariables`
method of the calling behaviour.

## Treatment of the persistent variables of the behaviour variable

A behaviour variable may declares persistent variables, i.e. state
variables or auxiliary state variables. Those persistent variables are
automatically declared as auxiliary state variables by the calling
behaviour. The transformation rules on the names and external names are
applied to define those new variables (see the `variables_prefix`,
`variables_suffix`, `external_names_prefix` or `external_names_suffix`
options).

The persistent variables of the behaviour variable are automatically
initialized by copying the values of the associated auxiliary state
variables of the calling behaviour.

If the `automatically_save_associated_auxiliary_state_variables` option
is set to true, the persistent variables of the behaviour variable are
automatically saved in the associated auxiliary state variables at the
beginning of the `updateAuxiliaryStateVariables` method of the calling
behaviour.

## Treatment of the material properties and external state variables of the behaviour variable

When a material property (or an external state variable) of the
behaviour variable is declared as shared (see the
`shared_material_properties` and `shared_external_state_variables`), the
calling behaviour will check if a matching material property (or an
external state variable) is declared. If not, it will declare exactly
the same material property (or external state variable), i.e. the
transformation rules on the names and external names are
**not** applied.

When a material property (or an external state variable) of the
behaviour variable is **not** declared as shared (the default), a new
material property (or an external state variable) is automatically
declaring applying the transformation rules on names.

## Access to the variables of the behaviour variable

The calling behaviour has access to all the variables of the behaviour
variable (material properties, parameters, external state variables,
gradients, local variables, etc..).

## Initialization of the behaviour variable

The `initialize` method of the behaviour variable is automatically
called in the `initialize` method of the calling behaviour after
initializing the material properties, external state variables,
persistent variables, gradients and thermodynamic forces.

## Reinitializing a behaviour variable

A behaviour variable can be initialized again by a specialized version
of the `initialize` method which takes the behaviour variable as its
only argument, as follows:

~~~~{.cxx}
initialize(b1);
~~~~

This method initializes the persistent variables to the values of the
associated auxiliary state variables, and does the same thing for
gradients and thermodynamic forces depending on the values of the
`store_gradients` and `store_thermodynamic_forces` options.

However, this does not guarantee that calling the integration of the
behaviour variable will results in the same result as some internal
variables may have evolved.

In particular, the increment of the integration variables will not
zeroed. Not zeroing those increment may accelerate the integration when
the behaviour integration is called multiple times as the solution of
the previous call may be a good estimate for the new resolution.

To initialize the behaviour and zero the increment of the integration
variables , one may call the `reset` method instead:

~~~~{.cxx}
reset(b1); // calls initialize and zeros integration variables increments
~~~~

> Note that even the `reset` method does not the absolute guarantee that
> calling the integration of the behaviour variable will results in the
> same result as some internal variables may have evolved. This will probably
> the case assuming that the implementation of the called behaviour does not
> make any strange things internally.

## Updating the auxiliary state variables associated with a behaviour variable

As stated in the previous paragraph, the
`automatically_save_associated_auxiliary_state_variables` option
controls if the auxiliary state variables associated with a behaviour
variable are automatically saved.

If this option is `false`, the author of the calling behaviour is
responible for updating thos state variables by hand. This task can
deleguated to a specialized version of the
`updateAuxiliaryStateVariables` method which takes the behaviour
variable as its only argument, as follows:

~~~~{.cxx}
updateAuxiliaryStateVariables(b1);
~~~~

The last solution is to cherry-pick the variables to be saved by hand.

## Name of the behaviour variable

The name of behaviour variable must be a valid `C++` variable name.

The following characters are legal as the first character of an
identifier, or any subsequent character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`
