# Description

`TFEL` is a collaborative development of CEA and EDF.

`MFront` is a code generator which translates a set of closely related
domain specific languages into plain C++ on top of the `TFEL`
library. Those languages covers three kind of material knowledge:

- material properties (for instance the
  Young modulus, the thermal conductivity, etc.)
- mechanical behaviours. Numerical performances of generated
  mechanical behaviours was given a particular attention. MFront
  offers a variety of interfaces to finite element solvers `Cast3M`,
  `Code-Aster`, `EUROPLEXUS`, `Abaqus-Standard`, `Abaqus-Explicit`,
  `Zebulon`, etc.. or various FFT solvers such as
  `AMITEX_FFTP`. Various benchmarks shows that `MFront`
  implementations are competitive with native implementations
  available in the `Cast3M`, `Code-Aster` and `Cyrano3` solvers.
- simple point-wise models, such as material swelling
  used in fuel performance codes.

`MFront` comes with an handy easy-to-use tool called `MTest` that can
test the local behaviour of a material, by imposing independent
constraints on each component of the strain or the stress. This tool
has been much faster (from ten to several hundred times depending on
the test case) than using a full-fledged finite element solver.

# Licences

TFEL version prior to 0.1 were released under both the LGPL and the
CECILL-B licences. A copy of those licences are included in the
distributions of TFEL.

TFEL versions 1.x were developped by CEA within the PLEIADES
project. Since svn revision 584, TFEL was part of the pleiades
architecture projet.

Starting from versions 2.x, TFEL has been publicly released under
either the GPL or the CECILL-A licence. A copy of thoses licences are
delivered with the sources of TFEL. CEA or EDF may also distribute
this project under specific licensing conditions.

Copyright (C) 2006-2013 CEA/DEN. All rights reserved. 
Copyright (C) 2014-2015 CEA/DEN, EDF R&D. All rights reserved. 
