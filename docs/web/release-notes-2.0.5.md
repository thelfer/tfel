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
