The `@Model` keyword is used to call an external point-wise model from a
behaviour. Those models can be implemented using the following DSLs:
`Model`, `DefaultModel`, `RungeKuttaModel` and `ImplicitModel`.

This model is called before any code block defining the behaviour.

This model is meant to make evolve one or more state variables of the
material.

From the behaviour point of view, those state variables are declared as
additional auxiliary state variables, but their values and their
increments over the time step are known. *Those variables are meant to
be used like external state variables*. However, those variables are
updated after computing the final thermodynamic forces and updating the
(standard) state variables of the behaviour **before** any code defined
by the user through the `@UpdateAuxiliaryStateVariables`.

## Note

In the behaviour, the names of the variables described by the model
are the same than in the model.
 
The entry/glossary names of those variables shall be defined in the
model.

## Example

~~~~ {#Model .cpp}
// Evolution of the \(\beta\) phase
@Model "M5PhaseTransformation_EdgarV3_SRMA2017.mfront";
~~~~

