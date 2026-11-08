# Description

`TFEL` is a collaborative development of CEA and EDF.

`MFront` is a code generator which translates a set of closely related
domain specific languages into plain C++ on top of the `TFEL`
library. Those languages cover three kind of material knowledge:

- material properties (for instance the
  Young modulus, the thermal conductivity, etc.)
- mechanical behaviours. Numerical performances of generated
  mechanical behaviours was given a particular attention. MFront
  offers a variety of interfaces to finite element solvers `Cast3M`,
  `Code-Aster`, `Abaqus-Standard`, `Abaqus-Explicit`,
  `Zebulon`, etc. or various FFT solvers such as
  `AMITEX_FFTP`. Various benchmarks shows that `MFront`
  implementations are competitive with native implementations
  available in the `Cast3M`, `Code-Aster` and `Cyrano3` solvers.
- simple point-wise models, such as material swelling
  used in fuel performance codes.

`MFront` comes with a handy easy-to-use tool called `MTest` that can
test the local behaviour of a material, by imposing independent
constraints on each component of the strain or the stress. This tool
has been much faster (from ten to several hundred times depending on
the test case) than using a full-fledged finite element solver.

# Licences

`TFEL` version prior to 0.1 were released under both the LGPL and the
CECILL-B licences. A copy of those licences are included in the
distributions of TFEL.

`TFEL` versions 1.x were developed by CEA within the PLEIADES
project. Since svn revision 584, TFEL was part of the `PLEIADES`
project.

Starting from versions 2.x, TFEL has been publicly released under either
the GPL or the CECILL-A licence. A copy of those licences are delivered
with the sources of TFEL. CEA or EDF may also distribute this project
under specific licensing conditions. Since 2023, the linking exception
clause was added to the GPL licence.

Copyright (C) 2006-2013 CEA/DEN. All rights reserved. 
Copyright (C) 2014-2025 CEA/DEN, EDF R&D. All rights reserved. 

# How to cite `TFEL/MFront`

If you use `TFEL/MFront` in your research, please cite the following
paper:

~~~~~~~~~
@article{helfer_introducing_2015,
	title = {Introducing the open-source mfront code generator: Application to mechanical behaviours and material knowledge management within the {PLEIADES} fuel element modelling platform},
	volume = {70},
	issn = {0898-1221},
	url = {http://www.sciencedirect.com/science/article/pii/S0898122115003132},
	doi = {10.1016/j.camwa.2015.06.027},
	shorttitle = {Introducing the open-source mfront code generator},
	pages = {994--1023},
	number = {5},
	journaltitle = {Computers \& Mathematics with Applications},
	shortjournal = {Computers \& Mathematics with Applications},
	author = {Helfer, Thomas and Michel, Bruno and Proix, Jean-Michel and Salvo, Maxime and Sercombe, Jérôme and Casella, Michel},
	urldate = {2016-09-22},
	date = {2015-09},
	keywords = {Domain specific languages, Implicit integration schemes, Material knowledge management, Mechanical behaviour integration, Single crystal plasticity},
}
~~~~~~~~~

# Current versions

The latest official version is `TFEL-4.2.0`.  It is built upon the
`C++-17` standard.

The stable development branch is called `rliv-4.2`. It is the
recommended branch for most users.

The current `master` branch will become `TFEL-5.0`.
