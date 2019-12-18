% Release notes of the 3.0.1 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

# Overview of `TFEL`, `MFront` and `MTest`

# Tickets fixed

## Ticket 38:  Bad normalisation of the tensor when computing eigenvalues

Tensors are normalised when computing the eigenvalues. There was a
mistake in the normalisation which can lead to severe numerical
instabilities in some (rare) case.

For more details, see: <https://sourceforge.net/p/tfel/tickets/38/>

## Ticket #35: regression when using `@CastemFiniteStrainStrategy` or `@CastemFiniteStrainStrategies` in the `castem` interface

When definining finite strain strategies in the `castem` interface, a
regression was introduced in version \(3.0.0\), if only one finite
strain strategy is defined: in this specific case, one define two
behaviours (in fact the second is barely an alias for the first one):

- A first one having the base name of the behaviour (e.g. `umatnorton`).
- A second name having the base name of the behaviour following by the
  finite strain strategy (e.g. `umatnorton_ss`).

The regression was that the `ModellingHypotheses` symbols was only
defined for the second behaviour.

For more details, see: <https://sourceforge.net/p/tfel/tickets/35/>
