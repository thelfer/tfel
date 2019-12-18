The `@InteractionMatrix` keyword let the user specify an interaction
matrix. By convention, this matrix is associated with harderning.

The interaction matrix associated with the effects of the dislocations
on the mean free path of the dislocations of a specific system can be
defined though the `@DislocationsMeanFreePathInteractionMatrix`
keyword.

## Structure of the interaction matrix

Since many interactions between slip systems are equivalent, the
interaction matrix only contains a few independent coefficients.

Consider the following example:

~~~~{.cpp}
@DSL       ImplicitFiniteStrain;
@Behaviour SlipSystemGenerationTest;
@Brick     FiniteStrainSingleCrystal;
@CrystalStructure FCC;
@SlidingSystem <1,-1,0>{1,1,1};
~~~~

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

## Definition of the interaction matrix

The interaction matrix is defined through the `@InteractionMatrix`
keyword.

This keyword is followed by an array of values, ordered by growing
rank.


