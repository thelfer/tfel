% Release notes of the 2.0.4 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

# Overview of `TFEL`, `MFront` and `MTest`

# Tickets fixed

## Ticket 38:  Bad normalisation of the tensor when computing eigenvalues

Tensors are normalised when computing the eigenvalues. There was a
mistake in the normalisation which can lead to severe numerical
instabilities in some (rare) case.

For more details, see: <https://sourceforge.net/p/tfel/tickets/38/>

## Ticket 39: Handling of Lagrange Multiplier in the ImposedDrivingVariable class

The terms related to the Lagrange multipliers are badly handled in the
ImposedDrivingVariable:

- The sign is wrong (Thus the lagrange multipliers can't be identified
  to the stress).
- The residual is not updated appropriatly.

For the second point, one must do this

~~~~{.cpp}
- r(this->c)  = a*u1(pos);
+ r(this->c) -= a*u1(pos);
~~~~

Those mistakes don't have any impact on the results, but changes the
values of the residual: with the correct handling of the Langrange
multiplier, some unit-tests had to be fixed by introducting a looser
criterion value on the stress.

For more details, see: <https://sourceforge.net/p/tfel/tickets/39/>

## Ticket 44: `mtest` `@OutputFile` error: output file name already defined

With `TFEL` version `2.0.4`, when assigning a name for the output of
the mtest analysis rather than using the default name, the following
error is reported:

~~~~{.sh}
No hypothesis defined, using default
terminate called after throwing an instance of 'std::runtime_error'
what(): MTest::setOutputFileName : output file name already defined
Abandon
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/44/>
