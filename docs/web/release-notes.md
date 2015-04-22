% Release notes
% Helfer Thomas
% April 21, 2015

# Versions 2.0.x

## Version 2.0.0

First open-source version.

## Version 2.0.1



## Version 2.0.2

- the multiplication of two symmetric tensors has been corrected. In
  previous versions, the result was though to be symmetric, which is
  obviously wrong. The static methods `tpld` et `tprd` were removed
  from the `st2tost2` class and added to the `st2tot2` class. Such
  modification leads to small changes in the results of finite strain
  single crystal behaviours.
- the parallel compilation could fail on the documentation generation
  [`cmake`](http://www.cmake.org/): this is now fixed.
- in the results files of [`mtest`](mtest.html), the name of the
  driving variables and the thermodynamic forces are appropriately
  changed for more clarity.

## Versions 2.1.x

# Versions 3.0.x

<!-- Local IspellDict: english -->
