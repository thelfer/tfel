The `@AuxiliaryModel` keyword is used to call an external point-wise
model from a behaviour. Those models can be implemented using the
following DSLs: `Model`, `DefaultModel`, `RungeKuttaModel` and
`ImplicitModel`.

This model is called after updating the auxiliary state variables of the
behaviour.


From the behaviour point of view, those state variables are declared as
additional auxiliary state variables.

## Note

In the behaviour, the names of the variables described by the model
are the same than in the model.
 
The entry/glossary names of those variables shall be defined in the
model.

## Example

~~~~ {#AuxiliaryModel .cpp}
@AuxiliaryModel "DuctileDamageIndicator_RiceTracey1969.mfront";
~~~~

