% MFront usage in [Code-Aster](http://www.code-aster.org/)
% Helfer Thomas
% October 13, 2014

This document shows how to use `MFront` mechanical behaviour in EDF
[Code_Aster](http://www.code-aster.org) finite element solver. It is
extracted from the
[`MFront` tutorial](documents/tutoriel/tutoriel.pdf). A full
description is available in the Code_Aster reference documentation
(see @code_notice_2014).

# Compilation of the behaviour

The following instruction will compile the `MFront` behaviour using
the `aster` interface :

~~~~ {.bash}
$ mfront --obuild --interface=aster Chaboche.mfront
~~~~~~~~~~~~~~~~~~~

This will generate a shared library usually called
`libAsterBehabviour.so` in the `src` directory.

# Usage in Code_Aster

First, the shared library `libAsterBehabviour.so` must be declared in
the study under the type "nom" in `astk`.

Starting from an existing input file, two things must be declared.

In the `DEFI_MATERIAU` block, one must add:

~~~~ {.python}
.......=DEFI_MATERIAU( UMAT=_F( C1 = ... , C2 = ... , C3 = ... , )
~~~~~~~~~~~~~~~~

where `C1`, ..., `CN` are the material properties declared by the
`MFront` behaviour.

In the `COMPORTEMENT` part of the main computation instructions
(`STAT_NON_LINE`, `DYNA_NON_LINE`, `SIMU_POINT_MAT`, ...), the
behaviour has the name `MFRONT`. Here is an example of such
declaration:

~~~~ {.python}
COMPORTEMENT=_F ( RELATION    = 'MFRONT',
	              LIBRAIRIE   = 'LibAsterBehaviour.so',
			      NOM_ROUTINE = 'asterchaboche',
	              NB_VARI     = 19,
			      DEFORMATION = 'GDEF_LOG', )
~~~~~~~~~~~~~~~~

#References
