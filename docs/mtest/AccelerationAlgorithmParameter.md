The `@AccelerationAlgorithmParameter` keyword allows the user to
fine-tune the acceleration algorithm used to reach equilibrium. The
parameter that can be defined depends on the acceleration algorithm
used.

This keyword is followed by a string giving the name of the parameter
and the value of this parameter, given without quotes.

## Example

~~~~~{.cpp}
@AccelerationAlgorithmParameter 'AccelerationTrigger' 3;
~~~~~


