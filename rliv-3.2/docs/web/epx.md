% The Europlexus interface

<div id="slideshow">
  <ul class="slides">
  <li>
  <video style="display:block; margin: 0 auto;" width="640" height="320" controls>
  <source src="media/epx1.mp4" type="video/mp4">
  Your browser does not support the video tag.
  </video>
  </li>
  <li>
  <video style="display:block; margin: 0 auto;" width="640" height="320" controls>
  <source src="media/epx2.mp4" type="video/mp4">
  Your browser does not support the video tag.
  </video>
  </li>
  </ul>
  <span class="arrow previous"></span>
  <span class="arrow next"></span>
</div>
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js"></script>
<script src="js/slideshow.js"></script>

`Europlexus` (`EPX`) is a simulation software dedicated to the
analysis of fast transient phenomena involving structures and fluids
in interaction. The program is co-owned by the Commissariat à
l’Energie Atomique et aux Energies Alternatives (CEA) and the Joint
Research Centre of the European Commission (EC/JRC). Its development
is carried out through a Consortium involving the co-owners and
so-called major partners who are granted a complete access to the
source code and development tools (see @cea_epx_2016).

Time integration is achieved through a conditionally stable explicit
scheme.

# Description of the interface

The following features are currently available:

- Support for finite strain behaviours
  + Small strain behaviours can be embedded though finite strain
    strategies: currently, `MFront` provides the
    `FiniteRotationSmallStrain` and the
    `MieheApelLambrechtLogarithmicStrain` strategies (see
    @edf_loi_2013;@miehe_anisotropic_2002)
- `Europlexus` is able to analyse the generated library to get
  helpfull information about the mechanical behaviour:
  + number and name of material properties
  + number, names and type of internal state variables
  + number and names external variables
  + symmetry of the behaviour (isotropy, orthotropy)
  + etc...
- An example of input file is automatically generated to help the user
  in assigning a `MFront` behaviour to a material.

## Example of generated input file fragment

The following listing gives an example of generated input file
fragment for an axisymmetric modelling hypothesis:

~~~~{.bash}
MFRONT
LIB 'libEuroplexusBehaviour.so'
LAW 'LogarithmicStrainNortonCreep'
!! material properties
MATP 'YoungModulus' ???
MATP 'PoissonRatio' ???
MATP 'A' ???
MATP 'E' ???
!! internal state variables
!! by default, internal state variables are set to zero
!! IVAR 'ElasticStrain'  ??? ??? ??? ???
!! IVAR 'EquivalentViscoplasticStrain'  ???
!! external state variables
EVAR 'Temperature' 293.15
LECT MESH TERM
~~~~

The user must replace each occurrences of '??' by appropriate values.

# Qualification

<!-- Local IspellDict: english -->
