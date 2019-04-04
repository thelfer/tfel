The `@Event` keyword allows the user to define event during the
computations. Defining an event is a way to active/desactivate a
constraint, (see `@ImposedStrain`, `@ImposedCohesiveForce`,
`@ImposedOpeningDisplacement`, `@ImposedStrain`,
`@ImposedDeformationGradient`, `@NonLinearConstraint`).

The `@Event` keyword is followed by the name of the event and a time or
a list of times (given as an array of values).

## Example

~~~~ {.cpp}
@Event 'Stop' '1';
~~~~~~~~

## Note

The times defining an event are not recquired to match a time of the
discretisation of the The times defining an event are not required to
match a time of the discretisation of the computation, but this is
highly recommended. An event is triggered at the the end of a time step
when the final time is greater or equal that the time associated with
the event. **This does not take into account sub-steppings**.