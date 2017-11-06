% Installing `TFEL/MFront` on `FreeBSD`
% Thomas Helfer
% 5/11/2017

<div id="slideshow">
  <ul class="slides">
	<li><img src="img/FreeBSD.jpg" width="620" height="320" alt="TFEL/MFront on FreeBSD 11.1" /></li>
    <li><img src="img/TrueOS.jpg" width="620" height="320" alt="TFEL/MFront on TrueOS 2017" /></li>
	</ul>
  <span class="arrow previous"></span>
  <span class="arrow next"></span>
</div>
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js"></script>
<script src="js/slideshow.js"></script>

This page covers some specific issues arising when compiling
`TFEL/MFront` on `FreeBSD`.

The recommended compiler on `FreeBSD` is `clang`.

# `python` bindings

The `python` bindings shall work as expected. Beware to use a version
of the `boost` python library build for the `python` library that you
intend to use.

## `FreeBSD 11.1` and `boost` version `1.65.1`

On `FreeBSD 11.1`, `boost` version `1.65.1` has a bug that prevents
the `init` function of modules from being exported.  See
https://github.com/boostorg/python/issues/173 for details.

You can workaround this issue by setting the following flag when
 invoking `cmake`:
 `-Denable-broken-boost-python-module-visibility-handling=ON`.
