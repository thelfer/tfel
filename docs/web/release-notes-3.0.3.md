% Release notes of the 3.0.3 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

This is mainly a bug fix version of the `3.0` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #97: Problem building the `python` bindings with a recent version of `boost`

Building the python bindings with a recent version of `boost` libs and
`clang` led to the following error:

~~~~{.sh}
--- vector.lo ---
In file included from vector.cxx:22:
.../../../bindings/python/include/TFEL/Python/VectorConverter.hxx:41:2:
error: reference to 'list' is ambiguous
         list l;
         ^
/usr/include/c++/v1/list:805:28: note: candidate found by name lookup is
'std::__1::list'
class _LIBCPP_TEMPLATE_VIS list
                            ^
/usr/local/include/boost/python/list.hpp:57:7: note: candidate found by
name lookup is 'boost::python::list'
class list : public detail::list_base
       ^
In file included from vector.cxx:22:
.../../../bindings/python/include/TFEL/Python/VectorConverter.hxx:43:4:
error: use of undeclared identifier 'l'
           l.append(*p);
           ^
.../../../bindings/python/include/TFEL/Python/VectorConverter.hxx:45:16:
error: use of undeclared identifier 'l'
         return incref(l.ptr());
~~~~

The error is du to an ambiguity between `std::list` and
`boost::python::list`, probably du to an implicit inclusion of the
`<list>` header file with `libc++`. This ambiguity has been eliminated.

For more details, see: <https://sourceforge.net/p/tfel/tickets/97/>

## Ticket #96: Building with the `musl` `c` library

The `Alpine` `Linux` distribution is based on the `musl` `c` library
and not `glibc`. Thus, the `feenableexcept` and `fedisableexcept`
functions are not available.

Appropriate tests, based on the definition of the `__GLIBC__` flags
were added to enable the compilation of `TFEL/MFront` with the `musl`
`c` library.

For more details, see: <https://sourceforge.net/p/tfel/tickets/96/>

## Ticket #94: The `pymfronttest1` test shall not be executed if the `Aster` interface is not enabled

For more details, see: <https://sourceforge.net/p/tfel/tickets/94/>
