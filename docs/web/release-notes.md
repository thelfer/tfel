% Release notes
% Helfer Thomas
% April 21, 2015

# Versions 2.0.x

## Version 2.0.0

First open-source version.

## Version 2.0.1

Minor fixes.

## Version 2.0.2

- The multiplication of two symmetric tensors has been corrected. In
  previous versions, the result was though to be symmetric, which is
  obviously wrong. The static methods `tpld` et `tprd` were removed
  from the `st2tost2` class and added to the `st2tot2` class. Such
  modification leads to small changes in the results of finite strain
  single crystal behaviours.
- The parallel compilation could fail on the documentation generation
  [`cmake`](http://www.cmake.org/): this is now fixed.
- In the results files of [`mtest`](mtest.html), the name of the
  driving variables and the thermodynamic forces are appropriately
  changed for more clarity.
- A new finite strain strategy named `log1D` was added to the `umat`
  interface. This interface allow the use of the logarithmic strain
  formalism in mono-dimensional fuel performance codes. See
  @helfer_extension_2015 for details.

## Version 2.0.3 (9/09/2015)

- Fix of a minor bug in the `CxxTokenizer` class which was appears
  when using the [`clang` `libc++`](http://libcxx.llvm.org/) standard
  library. This prevented many `MTest` tests to work on
  [Mac OS X](http://www.apple.com/fr/osx/).
- Fix of [Ticket #9](https://sourceforge.net/p/tfel/tickets/9/)
- Fix of [Ticket #10](https://sourceforge.net/p/tfel/tickets/10/)
- Fix of [Ticket #11](https://sourceforge.net/p/tfel/tickets/11/)
- The class `ExternalBehaviourDescription` was introduced
- The `AxialGrowth` entry was added to the glossary

A full description of the 2.0.3 release can be found
[here](documents/tfel/tfel-2.0.3.pdf) (in french).

## Version 2.0.4

- Fix a typo in the `zmat` interface which may prevent the use of
  external state variables

# Versions 2.1.x

The 2.1.x versions will be the last versions based build on C++98.

# Versions 3.0.x

The version 3.0.x will be based on the C++11 standard. This implied a
major code refactoring. In particular, the expression template engine
was greatly simplified and is now much more liable and maintainable.

## New features

- behaviour bricks
- support of thermal expansion computations in behaviours
- support of calling models from behaviours
- add the '--help-commands' command to `MFront`. This option displays
  the documentation of all the keywords of a given domain specific
  language. This option was used to generate the following pages:
    + [DefaultDSL](DefaultDSL-keywords.html) 
    + [DefaultCZMDSL](DefaultCZMDSL-keywords.html) 
    + [DefaultFiniteStrainDSL](DefaultFiniteStrainDSL-keywords.html) 
    + [Implicit](Implicit-keywords.html) 
    + [ImplicitFiniteStrain](ImplicitFiniteStrain-keywords.html) 
    + [ImplicitII](ImplicitII-keywords.html) 
    + [IsotropicMisesCreep](IsotropicMisesCreep-keywords.html) 
    + [IsotropicPlasticMisesFlow](IsotropicPlasticMisesFlow-keywords.html) 
    + [IsotropicStrainHardeningMisesCreep](IsotropicStrainHardeningMisesCreep-keywords.html) 
    + [MaterialLaw](MaterialLaw-keywords.html) 
    + [Model](Model-keywords.html) 
    + [MultipleIsotropicMisesFlows](MultipleIsotropicMisesFlows-keywords.html) 
    + [RungeKutta](RungeKutta-keywords.html) 
- add the '--help-commands' command to `MTest`. This option displays
  the documentation of all the keywords. This option was used to
  generate this [page](MTest-keywords.html).
- new applications:
    + `mfront-query` can be used to extract various information from
      a `MFront` file
	+ `mfront-doc` can be used to generate a documentations from a
      mfront file. `mfront-doc` generates files in `pandoc`
      [markdown format](http://pandoc.org/demo/example9/pandocs-markdown.html). Those
      files can be processed using [`pandoc`](http://pandoc.org/) and
      be converted to one of the many file format supported by
      [`pandoc`](http://pandoc.org/), including
      [`LaTeX`](www.latex-project.org), `html` or various Word
      processor formats: Microsoft Word
      [docx](http://www.microsoft.com/interop/openup/openxml/default.aspx),
      OpenOffice/LibreOffice
      [ODT](http://en.wikipedia.org/wiki/OpenDocument).
- new material properties interfaces:
	+ `java` designed for the
      [`java` language](http://www.oracle.com/fr/java/overview/index.html).
  	+ `octave` designed for
	  [`GNU Octave`](https://www.gnu.org/software/octave/) which is
      a high-level interpreted language, primarily intended for
      numerical computations.

## Systems supported

Version 3.0.x of `TFEL/MFront` will be available on the following
systems:

- `LiNuX`
- `Mac Os X`
- `Windows`. The port to `Visual Studio 2015` is the first step toward
  a industrial strength support of this platform. However, du to the
  very nature of this platform, a direct use of `MFront` is
  discouraged. One shall consider creating a material knowledge
  management project based on `cmake` to build material librairies.

Various Unix-based systems have been tested and shall work out of the
box :

- `FreeBSD`
- `PCBSD`

## Compiler support

Version 3.0.x were tested using:

- [`gcc`](https://gcc.gnu.org/)
- [`clang`](http://clang.llvm.org/)
- [`Visual Studio`](https://www.visualstudio.com/)
- [`mingw`](http://www.mingw.org/)
- [`cygwin`](https://www.visualstudio.com)

[Intel compilers 15](https://software.intel.com/en-us/intel-compilers)
are known not to work due to a bug in the [EDG](https://www.edg.com)
front-end that can't parse a syntax mandatory for the expression
template engine. The same bug affects the
[Blitz++](http://sourceforge.net/projects/blitz/) library:

<http://dsec.pku.edu.cn/~mendl/blitz/manual/blitz11.html>

# References

<!-- Local IspellDict: english -->
