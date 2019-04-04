A single slip systems family can be defined by one of the following
keywords: `@SlidingSystem`, `@GlidingSystem` or `@SlipSystem`. Those
keywords are followed by the definition of one family of slip systems.

A slip system is defined by its plane and its Burgers'vector as
follows, for a `Cubic` and `FCC` crystal structures:

~~~~{.cpp}
<1,-1,0>{1,1,1}
~~~~

For hexagonal structures, the slip systems are defined through a four
indices notation.

## Example

Thus, an example of the usage of the `@SlidingSystem` keyword is:

~~~~{.cpp}
@SlidingSystem <1,-1,0>{1,1,1};
~~~~

For a given slip systems family, a set of slip systems are generated
by symmetry using code extracted from the `NUMODIS` code. The previous
example defines \(12\) slip systems.

Once the sliding system families has been defined, a static integer
variable called `Nss` is available which contains the total number of
slip systems defined. In the previous example, `Nss` value is \(12\).

This value can be used to define additional state variables:

~~~~{.cpp}
//! equivalent plastic strain on each slip system
@AuxiliaryStateVariable real p[Nss];
~~~~

For post-processing reasons, one needs to know in which order the slip
systems are defined. This question is treated in the next paragraph.

Also, note that for the \(i^{\text{th}}\) slip systems family, a
static integer variable beginning by `Nss` and followed by the value
of \(i\) is defined. In the previous example, a unique slip systems
family is defined and a variable `Nss0` is made available. Of course,
in this case `Nss0` is equal to `Nss`. Those variables may be useful
to define variables specific to a slip system family.

## Getting information about the generated slip systems

The list of the generated slip systems can be retrieved using
`mfront-query`.

Consider the following example:

~~~~{.cpp}
@DSL       ImplicitFiniteStrain;
@Behaviour SlipSystemGenerationTest;
@Brick     FiniteStrainSingleCrystal;
@CrystalStructure FCC;
@SlidingSystem <1,-1,0>{1,1,1};
~~~~

If the previous code is saved a file called
`SlipSystemGenerationTest.mfront`, one may use `mfront-query` as
follows:

~~~~{.sh}
$ mfront-query --slip-systems SlipSystemGenerationTest.mfront
- <1,-1,0>{1,1,1}: [0,1,-1](1,1,1) [1,0,-1](1,1,1) [1,-1,0](1,1,1) [0,1,1](1,1,-1) [1,0,1](1,1,-1) [1,-1,0](1,1,-1) [0,1,-1](1,-1,-1) [1,0,1](1,-1,-1) [1,1,0](1,-1,-1) [0,1,1](1,-1,1) [1,0,-1](1,-1,1) [1,1,0](1,-1,1)
~~~~

The output shows that \(12\) slip systems were generated. All those
systems are equivalent by symmetry.

Concerning slip systems, the following queries are available:

~~~~{.sh}
$ mfront-query --help-behaviour-queries-list |grep slip
--slip-systems          : list all the slip systems, sorted by family
--slip-systems-by-index : list all the slip systems
~~~~

The `--slip-systems-by-index` query gives the index associated to a
given slip system, which is helpfull for postprocessing purposes. For
this example:

~~~~{.sh}
$ mfront-query --slip-systems-by-index SlipSystemGenerationTest.mfront
- 0: [0,1,-1](1,1,1)
- 1: [1,0,-1](1,1,1)
- 2: [1,-1,0](1,1,1)
- 3: [0,1,1](1,1,-1)
- 4: [1,0,1](1,1,-1)
- 5: [1,-1,0](1,1,-1)
- 6: [0,1,-1](1,-1,-1)
- 7: [1,0,1](1,-1,-1)
- 8: [1,1,0](1,-1,-1)
- 9: [0,1,1](1,-1,1)
- 10: [1,0,-1](1,-1,1)
- 11: [1,1,0](1,-1,1)
~~~~

## Getting information about the orientation tensors

Concerning the orientation tensors, the following queries are
available:

- `--orientation-tensors`: list all the orientation tensors, sorted by
  family".
- `--orientation-tensors-by-index`: list all the orientation tensors.
- `--orientation-tensors-by-slip-system`: list all the orientation
  tensors.

#### Getting information about the Schmid factors

Concerning the Schmid factors, the following queries are
available:

- `--schmid-factors`: list all the Schmid factors, sorted by
  family".
- `--schmid-factors-by-index`: list all the Schmid factors.

~~~~{.sh}
$ mfront-query --schmid-factors-by-index='<1,1,1>' SlipSystemGenerationTest.mfront
~~~~

**Note** In this example, the direction must be surrounded by simple
quote to avoid interpretation by the shell.
