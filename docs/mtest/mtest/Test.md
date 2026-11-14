The `@Test` keyword allows the user to add a test on the results. Two
types of tests are supported, `function` or `file`. The tests are
executed at the end of each time step.

This keyword is followed by the type of the test.

If the type of the test is `function`, two syntaxes are allowed. In the
first one, three arguments are expected: the name of the tested
variable, a function of time given the expected value, a criterion
value used for the comparison. In the second syntax, two arguments are
expected: a map which associates the name of a tested variable to a
function of time given the expected value of this variable and a
criterion value used for the comparison. The functions may explicitly
depend on time through the variable `t`.

If the type of the test is `file`, the expected values are read in the
columns of a text file. The name of the text file shall be given and
then two syntaxes are allowed. In the first one, three arguments are
expected: the name of the tested variable, a column number and a
criterion value used for the comparison. In the second syntax, two
arguments are expected: a map which associates the name of a tested
variable to a column number and a criterion value used for the
comparison. In each case, the values given by the n+1 line shall
correspond to the values expected after the n period.

The comparisons are made using an absolute criterion.  The names of
the components of the strains begins with E. The name of the
components of the stresses begins with S. The name of the components
of symmetric tensor internal state variables begins with the name of
the internal state variables. The name of the components of symmetric
tensors are obtained by suffixing their name according to the
modelling hypothesis used (see the `@ModellingHypothesis`
keyword). For example, the components of the strains are:

- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT
- Axisymmetrical                       : ERR EZZ ETT ERZ
- PlaneStress                          : EXX EYY EZZ EXY
- PlaneStrain                          : EXX EYY EZZ EXY
- GeneralisedPlaneStrain               : EXX EYY EZZ EXY
- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ

Off-diagonals terms are affected by a factor \(\sqrt(2)\).

# Example (function)

~~~~ {.cpp}
@Test<function> 'EXY' '0.' 1.e-12;
~~~~~~~~

# Example (function)

~~~~ {.cpp}
@Test<function> {'EXX':'SXX/YoungModulus',
                 'EYY':'-PoissonRatio*SXX/YoungModulus',
                 'EZZ':'-PoissonRatio*SXX/YoungModulus',
                 'EXY':'0.','EXZ':'0.','EYZ':'0.'} 1.e-12;
~~~~~~~~

# Example (file)

~~~~ {.cpp}
@Test<file> 'reference.txt' 'EXY' 1 1.e-12;
~~~~~~~~
