The `@AuxiliaryModel` keyword is used to call an external point-wise
model from a behaviour. Those models can be implemented using the
following DSLs: `Model`, `DefaultModel`, `RungeKuttaModel` and
`ImplicitModel`.

This model is called after updating the auxiliary state variables of the
behaviour.

From the behaviour point of view, the state variables of an auxiliary
model are declared as additional auxiliary state variables.

## Example

~~~~ {#AuxiliaryModel .cpp}
@AuxiliaryModel "DuctileDamageIndicator_RiceTracey1969.mfront";
~~~~

