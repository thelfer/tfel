The `@Model` keyword is used to call an external model from a
behaviour. This model is called before any code block defining the
behaviour.

This model is meant to make evolve one or more state variables of the
material.

From the behaviour point of view, those state variables are declared
as additional auxiliary state variables, but their values and their
increments over the time step are known. *Those variables are meant to
be used like external state variables*.

## Note

In the behaviour, the names of the variables described by the model
are the same than in the model.
 
The entry/glossary names of those variables shall be defined in the
model.

## Example

~~~~ {#Model .cpp}
// Evolution of the \(\beta\) phase
@Model "M5PhaseTransformation_EdgarV3_SRMA2017.mfront";
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

