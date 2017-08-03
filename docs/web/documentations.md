% General documentation 
% Helfer Thomas; Jean-Michel Proix
% August 2, 2017

This page provides several links to available documentations regarding
`TFEL` and `MFront`, which are fairly detailed. Unfortunately, most of
these materials are written in French. They also mostly refer to
versions `2.0.x`, and thus do not describe .

For a new (English speaking) user, we would highly recommend:

- Read the **reference paper** @helfer_introducing_2015. Altough this
  paper covers versions `2.0.x`, it gives an good general overiew of
  `TFEL` and `MFront`'s abilities.
- Read the page dedicated to second and fourth order tensors,
  [here](tensors.html).
- Have a look at the [**gallery**](gallery.html) which details the
  implementations of various classical mechanical behaviours. Those
  implementations are kept up-to-date to use the latest developments
  and techniques introduced in `TFEL` and `MFront`.
- If none of implementation described in the gallery match the user
  need, one may have a look at the implementations provided in the
  `mfront/tests/behaviours` directory in the sources. Beware that
  those implementations are not updated to ensure back-ward
  compatibility. Most of them can be simplified by using
- Finally, you may want to have a look at the documentation of the
  interface of the solver that you plan to use (see below for links).

For spanish users, Jordi Alberich have written "A spanish introduction
to `MFront` and `Salome-MECA`", which is available
[here](tutorial-spanish.html).

# References

- General introduction. Material properties and Models: [mfront.pdf](documents/mfront/mfront.pdf)
- Writing mechanical behaviours: [behaviours.pdf](documents/mfront/behaviours.pdf)
- How to handle plane stress in implicit schemes: [planestress.pdf](documents/mfront/planestress.pdf)
- Finite strain behaviours [finitestrain.pdf](documents/mfront/finitestrain.pdf)
- `MTest` user guide: [mtest.pdf](documents/mtest/mtest.pdf)

# Tutorials

- A fairly complete tutorial is available
  [here](documents/tutoriel/tutoriel.pdf) (in French).
- For spanish users, Jordi Alberich have written "A spanish
  introduction to `MFront` and `Salome-MECA`", which is available
  [here](tutorial-spanish.html).

# Courses

- General introduction: [mfront-presentation.pdf](documents/mfront-presentation/mfront-presentation.pdf)
- Writing mechanical behaviours [mfront-behaviours.pdf](documents/mfront-behaviours/mfront-behaviours.pdf)
- General introduction [mfront-formation.pdf](documents/mfront-formation/mfront-formation.pdf)
    - [tp.pdf](documents/tp/tp.pdf)

# Talks

- Finite strain: [mfront-finitestrain.pdf](documents/mfront-finitestrain/mfront-finitestrain.pdf)
- [Writing portable behaviours](documents/portable-behaviour/portability.html)

A `github` repository has been set up to store various documents
describing TFEL and MFront usage. The talks of the first and second
MFront Users are available there:

<https://github.com/thelfer/tfel-doc>

# Solver interfaces

- The description of the interface to the
  [Cast3M](http://www-cast3m.cea.fr) finite element solver is
  available [here](castem.html).
- The description of the interface to the
  [Code-Aster](http://www.code-aster.org) finite
  element solver is available [here](aster.html).
- The description of the interface to the
  [Abaqus/Standard](https://www.3ds.com/fr/produits-et-services/simulia/produits/abaqus/abaqusstandard) and
  [Abaqus/Explicit](https://www.3ds.com/products-services/simulia/products/abaqus/abaqusexplicit)
  finite element solvers is available [here](abaqus.html).
- The description of the interface to the
  [ZeBuLoN](http://www.zset-software.com/products/zebulon) finite
  element solver is available [here](zmat.html).
- The description of the interface to the
  [Europlexus](http://www-epx.cea.fr) finite
  element solver is available [here](epx.html).
- The description of the interface to the [CalculiX](calculix.de)
  finite element solver is available [here](calculix.html).

# Bibliography

<!-- Local IspellDict: english -->
