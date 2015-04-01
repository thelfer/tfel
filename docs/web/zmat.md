% MFront usage in [ZeBuLoN](http://www.zset-software.com/products/zebulon)
% Helfer Thomas
% October 13, 2014

This document shows how to use `MFront` mechanical behaviour in the
[ZeBuLoN finite element solver](http://www.zset-software.com/products/zebulon). It
is extracted from the
[`zmat` documentation](documents/mfront/zmat.pdf).

This interface has been tested with the version \(8.5\) of
[ZeBuLoN](http://www.zset-software.com/products/zebulon). The files
used can be downloaded [here](downloads/example-zset8.5.tar.bz2).

# Compilation of the behaviour

The following instruction will compile the `MFront` behaviour using
the `zmat` interface :

~~~~ {.bash}
$ mfront --obuild --interface=zmat norton.mfront
~~~~~~~~~~~~~~~~~~~

# Usage in [ZeBuLoN](http://www.zset-software.com/products/zebulon)

The generated library must be placed in a directory registered by the
`ZEBUPATH` environment variable. The generated library is then
automatically loaded at `ZeBuLoN` start-up and all the behaviours are
available.

The behaviour can then be used as standard `ZMAT` behaviour:

~~~~~~~~~ {.bash}
***behavior Norton
 **material_properties
   YoungModulus 150e9
   PoissonRatio   0.33
   A[0] 8.e-67
   A[1] 1.
   E 8.2
 **out_of_bounds_policy Strict
 **parameters
   epsilon 1.e-12
***return
~~~~~~~~~
