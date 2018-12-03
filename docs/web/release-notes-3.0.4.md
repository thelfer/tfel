% Release notes of the 3.0.4 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

This is mainly a bug fix version of the `3.0` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #151: Correct the python modules install path

In `tfel.cmake`, the python modules install path was defined as follows:

~~~~
install(TARGETS py_${package}_${name}
        DESTINATION lib${LIB_SUFFIX}/${PYTHON_LIBRARY}/sitepackages/${package}_${TFEL_SUFFIX_FOR_PYTHON_MODULES}
        COMPONENT python_bindings)
~~~~

In Python 3.x under linux, this led to install the module in
`python3.xm`. The `m` suffix is not standard and had to be
removed. The install path definition has been corrected as follows:

~~~~
install(TARGETS py_${package}_${name}
        DESTINATION lib${LIB_SUFFIX}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/sitepackages/${package}_${TFEL_SUFFIX_FOR_PYTHON_MODULES}
        COMPONENT python_bindings)
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/151/>

## Ticket #147: Improving material properties support with Visual Studio

Material properties could not be compiled with Visual Studio because
of an explicit link with the "m" library.

For more details, see: <https://sourceforge.net/p/tfel/tickets/147/>
