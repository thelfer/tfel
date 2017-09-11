% Description of the slip systems, orientation tensors and of the interaction matrices of single crystal and their usage
% Thomas Helfer
% 11/09/2017

This page' purpose is twofold:

- it shows how to define:
    - the crystal structure.
    - the slip systems. Once the slip systems are defined, the
      orientations tensors and their symmetric parts are automatically
      defined.
    - the interaction matrix (used to describe hardening).
    - the interaction matrix used to evaluate the effect of all the
      dislocation on the mean free path of dislocations of a specific
      system.
- how to use to information in pratice.

# Description of the slip systems, orientation tensors and of the interaction matrices

## Crystal structure

The crystal structure must be defined through the `@CrystalStructure`
keyword. This keyword is followed by one of the following value:

- `Cubic`: cubic structure.
- `BCC`: body centered cubic structure.
- `FCC`: face centered cubic structure.
- `HCP`: hexagonal closed-packed structures.

~~~~{.cpp}
@CrystalStructure FCC;
~~~~

## Definition of the slip systems

A single slip systems family can be defined by one of the following
keywords: `@SlidingSystem`, `@GlidingSystem` or `@SlipSystem`. Those
keywords are followed by the definition of one family of slip systems.

Several slip systems families can be defined by one of the following
keywords: `@SlidingSystems`, `@GlidingSystems` or
`@SlipSystems`. Those keywords are followed by an array giving the
definition of the families of slip systems.

A slip system is defined by its Burgers'vector and its plane. In the
case of a cubic system, the syntax is as follows:

~~~~{.cpp}
<1,-1,0>{1,1,1}
~~~~

> **Hexagonal systems** For an hexagonal system, a fourth indices
> notation is used.

Thus, an example of the usage of the `@SlidingSystem` keyword is:

~~~~{.cpp}
@SlidingSystem <1,-1,0>{1,1,1};
~~~~

For a given slip systems family, a set of slip systems are generated
by symmetry using code extracted from the `NUMODIS` code (see
@numodis_site_2017). The previous example defines \(12\) slip systems.

### Getting information about the generated slip systems

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

### Getting information about the orientation tensors

Concerning the orientation tensors, the following queries are
available:

- `--orientation-tensors`: list all the orientation tensors, sorted by
  family".
- `--orientation-tensors-by-index`: list all the orientation tensors.
- `--orientation-tensors-by-slip-system`: list all the orientation
  tensors.

### Getting information about the Schmid factors

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

## Definition of the interaction matrix

### Structure of the interaction matrix

Since many interactions between slip systems are equivalent, the
interaction matrix only contains a few independent coefficients.

Using `mfront-query`, one may have access to the whole matrix using
the `--interaction-matrix` query:

~~~~{.sh}
$ mfront-query --interaction-matrix SlipSystemGenerationTest.mfront 
| 0 1 2 2 3 4 5 6 5 6 4 3 |
| 1 0 2 2 6 5 4 3 4 3 5 6 |
| 2 2 0 1 5 6 3 4 6 5 3 4 |
| 2 2 1 0 4 3 6 5 3 4 6 5 |
| 3 4 5 6 0 1 2 2 6 5 4 3 |
| 6 5 4 3 1 0 2 2 3 4 5 6 |
| 5 6 3 4 2 2 0 1 5 6 3 4 |
| 4 3 6 5 2 2 1 0 4 3 6 5 |
| 5 6 4 3 4 3 5 6 0 1 2 2 |
| 4 3 5 6 5 6 4 3 1 0 2 2 |
| 6 5 3 4 6 5 3 4 2 2 0 1 |
| 3 4 6 5 3 4 6 5 2 2 1 0 |
~~~~

Here only \(7\) coefficients are needed. The number corresponding to a
pair of slip systems defines the rank of the interaction.

The `mfront-query` also provides the `--interaction-matrix-structure`
query which displays the number of independent coefficients and sorts
the pair of slip systems by rank:

~~~~{.sh}
$ mfront-query --interaction-matrix-structure SlipSystemGenerationTest.mfront 
- number of independent coefficients: 7
- rank 0: ([0,1,-1](1,1,1):[0,1,-1](1,1,1)) ([1,0,-1](1,1,1):[1,0,-1](1,1,1)) ([1,-1,0](1,1,1):[1,-1,0](1,1,1)) ([0,1,1](1,1,-1):[0,1,1](1,1,-1)) ([1,0,1](1,1,-1):[1,0,1](1,1,-1)) ([1,-1,0](1,1,-1):[1,-1,0](1,1,-1)) ([0,1,-1](1,-1,-1):[0,1,-1](1,-1,-1)) ([1,0,1](1,-1,-1):[1,0,1](1,-1,-1)) ([1,1,0](1,-1,-1):[1,1,0](1,-1,-1)) ([0,1,1](1,-1,1):[0,1,1](1,-1,1)) ([1,0,-1](1,-1,1):[1,0,-1](1,-1,1)) ([1,1,0](1,-1,1):[1,1,0](1,-1,1))
- rank 1: ([0,1,-1](1,1,1):[1,0,-1](1,1,1)) ([0,1,-1](1,1,1):[1,-1,0](1,1,1)) ([1,0,-1](1,1,1):[0,1,-1](1,1,1)) ([1,0,-1](1,1,1):[1,-1,0](1,1,1)) ([1,-1,0](1,1,1):[0,1,-1](1,1,1)) ([1,-1,0](1,1,1):[1,0,-1](1,1,1)) ([0,1,1](1,1,-1):[1,0,1](1,1,-1)) ([0,1,1](1,1,-1):[1,-1,0](1,1,-1)) ([1,0,1](1,1,-1):[0,1,1](1,1,-1)) ([1,0,1](1,1,-1):[1,-1,0](1,1,-1)) ([1,-1,0](1,1,-1):[0,1,1](1,1,-1)) ([1,-1,0](1,1,-1):[1,0,1](1,1,-1)) ([0,1,-1](1,-1,-1):[1,0,1](1,-1,-1)) ([0,1,-1](1,-1,-1):[1,1,0](1,-1,-1)) ([1,0,1](1,-1,-1):[0,1,-1](1,-1,-1)) ([1,0,1](1,-1,-1):[1,1,0](1,-1,-1)) ([1,1,0](1,-1,-1):[0,1,-1](1,-1,-1)) ([1,1,0](1,-1,-1):[1,0,1](1,-1,-1)) ([0,1,1](1,-1,1):[1,0,-1](1,-1,1)) ([0,1,1](1,-1,1):[1,1,0](1,-1,1)) ([1,0,-1](1,-1,1):[0,1,1](1,-1,1)) ([1,0,-1](1,-1,1):[1,1,0](1,-1,1)) ([1,1,0](1,-1,1):[0,1,1](1,-1,1)) ([1,1,0](1,-1,1):[1,0,-1](1,-1,1))
.....
~~~~

In this example, the rank \(0\) contains all the interactions of a
slip system with itself.

#### `FCC` case

In the case of `FCC`, the interaction matrix has \(7\) independent
coefficients and the matrix is non symmetric (see
@madec_dislocation_2017).

### Definition of the interaction matrix

The interaction matrix is defined through the `@InteractionMatrix`
keyword. This keyword is followed by an array of values, ordered by
growing rank.

## Definition of the interaction matrix used to evaluate the effect of all the dislocation on the mean free path of dislocations of a specific system

A special keyword has been introduced for the user to have access to a
second interaction matrix. By convention this matrix is meant to be
used to evaluate the effect of all the dislocation on the mean free
path of dislocations of a specific system.

This interaction has the same structure than the interaction matrix
used to describe hardening
effects. Please refer to the previous paragraph for details.

This second matrix can be defined using the
`@DislocationsMeanFreePathInteractionMatrix` keywords.

# Usage in pratice

## Number of slip systems

Once the sliding system families has been defined, a static integer
variable called `Nss` is available which contains the total number of
slip systems defined. In the previous example, `Nss` value is \(12\).

This value can be used to define additional state variables:

~~~~{.cpp}
//! equivalent plastic strain on each slip system
@AuxiliaryStateVariable real p[Nss];
~~~~

Also, note that for the \(i^{\text{th}}\) slip systems family, a
static integer variable beginning by `Nss` and followed by the value
of \(i\) is defined. In the previous example, a unique slip systems
family is defined and a variable `Nss0` is made available. Of course,
in this case `Nss0` is equal to `Nss`. Those variables may be useful
to define variables specific to a slip system family.

## Data structure generated

For performances reasons, it would be too costly to build the
information about slip systems and interaction matrices every time the
behaviour is integrated.

For this reason, an auxiliary data structure is generated. The name of
the data structure is the name of the behaviour, followed by
`SlipSystems`. For example, if the behaviour name is
`FCCSingleCrystal`, the name of this auxiliary data structure is
`FCCSingleCrystalSlipSystems`.

This data structure is
build once for the whole duration of the execution following a
technique called `singleton`.

When using the `FiniteStrainSingleCrystal` brick, this uniq instance
is available in `@Integrator` block through a (reference) variable
called `ss`.

If this brick is not used or if this variable is needed in other blocks
of the code, one may declare the `ss` variable as follows:

~~~~{.cpp}
const auto& ss = FCCSingleCrystalSlipSystems<real>::getSlipSystems();
~~~~

Of course, the name `ss` (for slips systems) can be changed by the
user to match his/her own conventions.

### Access to the interaction matrix defined through `@InteractionMatrix`

The interaction matrix is accessible through the `him` (hardening
interaction matrix) member.

~~~~{.cpp}
//! access to the interaction matrix
const auto& m = ss.him;
~~~~

For backward compatibility and ease the transition from previous
implementations, this matrix is also accessible through the `mh`
member.

### Access to the interaction matrix defined though `@DislocationsMeanFreePathInteractionMatrix`

If defined, the interaction matrix meant to evaluate the effect of all
the dislocation on the mean free path of dislocations of a specific
system can be accessed through the `dim` (dislocation interaction
matrix) member.

~~~~{.cpp}
//! access to the interaction matrix
const auto& m = ss.dim;
~~~~

## Knowing if two systems are coplanar

To know if two systems are coplanar, the data structure described in
the previous paragraph provides the `areCoplanar` method. For example:

~~~~{.cpp}
for(unsigned short i=0;i!=Nss;++i){
  ....
  for(unsigned short j=0;j!=Nss;++j){
    if (ss.areCoplanar(i,j)){
	  hcopla[i]+=sqrt(ss.him(i,j)*ppomega[j]);
    } else {
      hforest[i]+=sqrt(ss.him(i,j))*ppomega[j];
    }
    ...
~~~~

## Handling of multiple slip systems families

The `FiniteStrainSingleCrystal` brick creates a huge vector of
internal state variable describing the plastic slips along the slips
systems. It is sometimes useful to be able to distinguish each slip
systems' family: for example to prescribe different constitutive
equations for each family.

The slips systems are sorted by family. So the `Nss0` first plastic
slips correspond to the first family. One can thus use the `Nss.+`
variables to compute the offset of each family. For example, the
plastic slip of the second system of the third family can be accessed
as follows (remember that `C++` indices starts at `0`, so the index of
the second system is `1`):

~~~~{.cpp}
const auto g2_3 = this->g[Nss0+Nss1+1];
~~~~

To define constitutive equations for the third family, one may use a
loop, as follows:

~~~~{.cpp}
for(const unsigned short i=0;i!=Nss2;++i){
  fg[Nss0+Nss1+i]=...
}
~~~~

However, this approach is error-prone, the data structure describing
the slip systems provides methods to compute the offset of each
family. Those methods are called `offset` plus the number of the
family.

Hence, the previous loop can be written:

~~~~{.cpp}
for(const unsigned short i=0;i!=Nss2;++i){
  fg[ss.offset2(i)]=...
}
~~~~

# References

<!-- Local IspellDict: english -->
