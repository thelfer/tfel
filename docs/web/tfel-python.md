% The TFEL Python API
% Thomas Helfer
% 19/12/2017

This page describes the `python` modules based on the `TFEL`
libraries.

# The `tfel.math` module

## Bindings related to the `stensor` class

Three classes standing for symmetric tensors are available:

- `Stensor1D`: symmetric tensor in \(1D\).
- `Stensor2D`: symmetric tensor in \(2D\).
- `Stensor3D`: symmetric tensor in \(3D\).

The standard mathematical operations are defined:

- addition of two symmetric tensors.
- subtraction of two symmetric tensors.
- multiplication by scalar.
- in-place addition by a symmetric tensor.
- in-place subtraction by a symmetric tensor.
- in-place multiplication by scalar.
- in-place division by scalar.

The following functions are available:

- `sigmaeq`: computes the von Mises norm of a symmetric tensor.
- `tresca`: computes the Tresca norm of a symmetric tensor.

## Bindings related to the `st2tost2` class

Three classes standing for linear transformation of symmetric tensors
to symmetric tensors:

- `ST2toST1D`: linear transformation of symmetric tensors to symmetric
tensors in \(1D\).
- `ST2toST2D`: linear transformation of symmetric tensors to symmetric
tensors in \(2D\).
- `ST2toST3D`: linear transformation of symmetric tensors to symmetric
tensors in \(3D\).

# The `tfel.material` module

## Bindings related to the \(\pi\)-plane

The following functions are available:

- `buildFromPiPlane`: returns a tuple containing the three eigenvalues
  of the stress corresponding to the given point in the \(\pi\)-plane.
- `projectOnPiPlane`: projects a stress state, defined its three
  eigenvalues or by a symmetric tensor, on the \(\pi\)-plane.

## Bindings related to the Hosford equivalent stress

The `computeHosfordStress` function, which compute the Hosford
equivalent stress, is available.

## Bindings related to the Barlat equivalent stress

The following functions are available:

- `makeBarlatLinearTransformation1D`: builds a \(1D\) linear
  transformation of the stress tensor.
- `makeBarlatLinearTransformation2D`: builds a \(2D\) linear
  transformation of the stress tensor.
- `makeBarlatLinearTransformation3D`: builds a \(3D\) linear
  transformation of the stress tensor.
- `computeBarlatStress`: computes the Barlat equivalent Barlat stress.

<!-- Local IspellDict: english -->
