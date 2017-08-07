% MFront: a code generation tool dedicated to material knowledge
% Helfer Thomas; Jean-Michel Proix
% August 15, 2014

<div id="slideshow">
  <ul class="slides">
	<li><img src="img/CalculiX_MFRONT_WSL.png" width="620" height="320" alt="CalculiX and MFront in the bConverged environment" /></li>
    <li><img src="img/VeRCoRs.png" width="620" height="320" alt="Simulation of the VeRCoRs mock-up of a nuclear building" /></li>
	<li><img src="img/AbaqusFLD.png" width="620" height="320" alt="Simulation of a Punching test" /></li>
    <li><img src="img/BrittleFracturePhaseField.png" width="620" height="320" alt="Phase field modelling of brittle fracture" /></li>
    <li><img src="img/FuelRod-ddif2.png" width="620" height="320" alt="Fuel pellet cracking" /></li>
    <li><img src="img/172_5pc.png" width="620" height="320" alt="Polycrystals computation" /></li>
	<li><img src="img/VoidsGrowth.png" width="620" height="320" alt="Void growth" /></li>
	<li><img src="img/AMITEX_FFTP_sig1.png" width="620" height="320" alt="Polycrystals computation based on FFT" /></li>
	<li><img src="img/hsnv125.png" width="620" height="320" alt="Unit testing of a Chaboche-like viscoplastic beahviour" /></li>
  </ul>
  <span class="arrow previous"></span>
  <span class="arrow next"></span>
</div>
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js"></script>
<script src="js/slideshow.js"></script>

# Overview

`TFEL` is a collaborative development of
[CEA](http://www.cea.fr/english-portal "Commissariat à l'énergie
atomique") and [EDF](http://www.edf.com/ "Électricité de France").

`MFront` is a code generator which translates a set of closely related
domain specific languages into plain [C++](http://isocpp.org/) on top
of the `TFEL` library. Those languages cover three kinds of material
knowledge:

- [material properties](material-properties.html) (for instance the
  Young modulus, the thermal conductivity, etc.)
- [mechanical behaviours](behaviours.html). Numerical performances of
  generated mechanical behaviours were given a particular
  attention. [Various benchmarks](benchmarks.html) show that `MFront`
  implementations are competitive with native implementations
  available in the `Cast3M`, `Code-Aster`, `Abaqus Standard` and
  `Cyrano3` solvers.
- [simple point-wise models](models.html), such as material swelling
  used in fuel performance codes.

A set of interfaces are provided for various solvers:

- the [`Cast3M`](http://www-cast3m.cea.fr) finite element solver. This
  interface is described [here](castem.html).
- the [`Code_Aster`](http://www.code-aster.org) finite element
  solver. This interface is described [here](aster.html)
- the [`Europlexus`](http://europlexus.jrc.ec.europa.eu/) finite
  element solver. This interface is described [here](epx.html)
- the
  [`Abaqus Standard`](http://www.3ds.com/products-services/simulia/products/abaqus/abaqusstandard/)
  finite element solver. This interface is described
  [here](abaqus.html).
- the
  [`Abaqus Explicit`](http://www.3ds.com/products-services/simulia/products/abaqus/abaqusexplicit/)
  finite element solver. This interface is described
  [here](abaqus.html).
- the
  [`CalculiX`](http://www.calculix.de/)
  finite element solver. This interface is described
  [here](calculix.html).
- the [`ZeBuLoN`](http://www.zset-software.com/products/zebulon)
  finite element solver. This interface is described [here](zmat.html).
- the `TMFFT` and the
  [`AMITEX_FFTP`](http://www.maisondelasimulation.fr/projects/amitex/html/)
  Fast Fourier Transform solvers developed internally at
  [CEA](http://www.cea.fr/english-portal "Commissariat à l'énergie
  atomique"). Note that
  [`AMITEX_FFTP`](http://www.maisondelasimulation.fr/projects/amitex/html/)
  is distributed under a free license for research and education
  purpose. Those solvers reuse the interface developed for the
  [`Cast3M`](http://www-cast3m.cea.fr) finite element solver.

For material properties, various interfaces are also provided and
covers the following languages `C`, `C++`, `python`, `fortran`, etc.

# History

- Versions prior to 0.1 were developed by T. Helfer as an independent project.
- Starting with version 1.0, TFEL was developed as a sub-project of
  the PLEIADES platform.
- Versions higher than 2.0 are open-source and distributed under the
  [GPL](https://gnu.org/licenses/gpl.html "GNU General Public
  License") license.

<!-- Local IspellDict: english -->
