% The `mfm` tool
% Thomas Helfer
% 30/08/2017

`mfm` is a tool that allow querying a library about the entry points
defined by `MFront`. Depending on the interface, an entry point can be
a class name, a function, a name of an entity that will be registered
in an abstract factory when the library is loaded, etc...

~~~~{.bash}
$ mfront --obuild --interface=aster ImplicitNorton.mfront 
Treating target : all
The following library has been built :
- libAsterBehaviour.so :  asterimplicitnorton
th202608@pleiades098:/tmp$ mfm src/libAsterBehaviour.so 
- asterimplicitnorton
~~~~

# Filters

The entry points can be filtered. The following filters are available:

- `--filter-by-interface`.
- `--filter-by-material`
- `--filter-by-name`.
- `--filter-by-type`. This option can be followed by
  `material-property`, `behaviour` or `model`

Filters are based on case-insensitive regular expressions. 

# Other command line options

Apart from filters, `mfm` also have the following options:

- `--verbose`: set the verbosity level. The following values are
  accepted: `quiet`, `level0`, `level1`, `level2`, `debug`, `full`. If
  no value is given, `level1` is selected.
- `--show-libs`: show library name in front of entry points.

# Usage

~~~~{.bash}
$ mfm --filter-by-material='M5' --filter-by-type=material_property --filter-by-name='.*YoungModulus.*' --filter-by-interface=castem --show-libs  $(find . -type f)
- ./lib/libM5MaterialProperties-castem.so: M5_YoungModulus
- ./lib/libM5MaterialProperties-castem.so: M5_YoungModulus_Crocodile2015
- ./lib/libM5MaterialProperties-castem.so: M5_YoungModulus_MATPRO2001
~~~~
