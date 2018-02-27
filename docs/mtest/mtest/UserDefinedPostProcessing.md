The `@UserDefinedPostProcessing` lets the user define its own
post-processings.

This keywords is followed by:

- the name of the output file
- the list of post-processings given by a string or and an array of
  strings. Those strings defines formulae which are evaluated at the end
  of the time step. Those formulae may depend on:
    - the behaviour' driving variable
    - the behaviour' thermodynamic forces
    - the behaviour' internal state variables
    - the behaviour' external state variables
    - any evolution defined in the input file

## Example 

~~~~{.cpp}
@UserDefinedPostprocessing 'test.txt' '(SXX+SYY+SZZ)/3';
~~~~

~~~~{.cpp}
@UserDefinedPostProcessing 'myoutput.txt' {'SXX','EquivalentPlasticStrain'};
~~~~