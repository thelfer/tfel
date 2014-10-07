% MFront: a code generation tool dedicated to material knowledge
% Helfer Thomas; Jean-Michel Proix
% August 15, 2014

<div id="slideshow">
  <ul class="slides">
    <li><img src="img/FuelRod-ddif2.png" width="620" height="320" alt="Fissuration d'une pastille combustible" /></li>
    <li><img src="img/172_5pc.png" width="620" height="320" alt="Calculs sur polycristal" /></li>
	<li><img src="img/VoidsGrowth.png" width="620" height="320" alt="Croissance de vides" /></li>
  </ul>
  <span class="arrow previous"></span>
  <span class="arrow next"></span>
</div>
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js"></script>
<script src="js/slideshow.js"></script>

# Overview

MFront is a code generator which translates a set of closely related
domain specific languages into plain [C++](http://isocpp.org/). Those
languages covers three kind of material knowledge:

- [material properties](material-properties.html) (for instance the Young modulus, the thermal
conductivity, etc.
- mechanical behaviours
- [simple point-wise models](models.html), such as material swelling
used in fuel performance codes.

MFront is a collaborative development of [CEA](http://www.cea.fr/english-portal "Commissariat à l'énergie atomique") and [EDF](http://www.edf.com/ "Électricité de France").

# History

- Versions prior to 0.1 were developed by T. Helfer as an independent project.
- Starting with version 1.0, TFEL was developed as a sub-project of
  the PLEIADES platform.
- Versions higher than 2.0 are open-source and distributed under the
  [GPL](https://gnu.org/licenses/gpl.html "GNU General Public
  License") license.

