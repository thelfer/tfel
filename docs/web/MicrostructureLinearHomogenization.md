% Implementation of mean-field homogenization schemes for general microstructures
% Martin Antoine
% March 7, 2025

\newcommand{\D}{\mathrm{d}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\dbldot}{\mathbin{\mathord{:}}}
\newcommand{\reals}{\mathbb R}

\newcommand{\tsigma}{\underline{\sigma}}

\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilon}{\underline{\epsilon}}

\newcommand{\Frac}[2]{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}

This tutorial shows how to implement in `MFront` mean-field homogenization schemes
within the framework of linear elasticity, for complex microstructures.

The implementation begins with a description of the microstructure, and continues
with the computation of the homogenization scheme.
Both step are part of the `namespace` `tfel::material::homogenization::elasticity`.
The implementation thus provides a `@MaterialLaw`.

# Microstructure description

Two types of microstructures are considered:

 - Matrix-inclusion microstructures: `MatrixInclusionMicrostructure<d, real, length, stress>` (d is the dimension)
 - Polycrystal microstructures: `Polycrystal<d, real, length, stress>`

The first one consists of a matrix, within which the user can add several
inclusion phases (he can add as much as he wants).
The matrix is given only by its elasticity that can be general (included anisotropic).
Each inclusion phase has also a particular description:
 
 - it has a volume fraction of inclusions
 - the inclusions have an isotropic elasticity 
 - the shape of inclusions can be spherical, spheroidal, ellipsoidal, in 3d, or circular, elliptical, in 2d
 - for spheroids and ellipsoids, we consider 3 possibilities of orientation distribution: isotropic, transverse isotropic and oriented
 - for ellipses, we consider 2 possibilities of orientation distribution: isotropic, and oriented
 - for spheres or disks, there is no orientation to provide
 
The `Polycrystal` microstructure is composed of grain phases,
that can be added by the user (he can add as much as he wants).
Each grain phase:

 - has a determined volume fraction
 - has a general elasticity (included anisotropic)
 - has a shape that can be spherical, spheroidal, ellipsoidal, in 3d, or circular, elliptical, in 2d
 - is necessarily oriented (included for spheres)

## Example 1: construction of a `MatrixInclusionMicrostructure`

We here see how to construct the `MatrixInclusionMicrostructure`.
It is important not to forget the header file that must be included
for the construction:

~~~~ {#micro .cpp .numberLines}
@TFELLibraries {"Material"};

@Includes{
#include "TFEL/Material/MicrostructureDescription.hxx"
}
~~~~

Here is the construction of the microstructure:

~~~~ {#micro .cpp .numberLines}
using namespace tfel::math;

tvector<3u, real> n_a = {1, 0, 0};
st2tost2<3u, stress> C_0;
st2tost2<3u, stress> C_i1;
st2tost2<3u, stress> C_i2;
stensor<3u,real> E={1,0,0,0,0,0};

using namespace tfel::material;
static constexpr auto value =
StiffnessTensorAlterationCharacteristic::UNALTERED;
computeIsotropicStiffnessTensorII<3u, value, stress, real>(
C_0, E0, nu0);
computeIsotropicStiffnessTensorII<3u, value, stress, real>(
C_i1, Ei1, nui1);
computeIsotropicStiffnessTensorII<3u, value, stress, real>(
C_i2, Ei2, nui2);

using namespace tfel::material::homogenization::elasticity;

Ellipsoid<length> ellipsoid1(a,b,c);
Spheroid<length> spheroid1(a,c);
IsotropicDistribution<3u,real,stress,length> distrib1(ellipsoid1,f1,C_i1);
unsigned short int index = 0;
TransverseIsotropicDistribution<real,stress,length> distrib2(spheroid1,f2,C_i2,n_a,index);                 
MatrixInclusionMicrostructure<3u, real, length, stress> micro1(C_0);
micro1.addInclusionPhase(distrib1);
micro1.addInclusionPhase(distrib2);
~~~~

## Example 1: construction of a `Polycrystal`

We here see how to construct the `Polycrystal`.
It is important not to forget the header file that must be included
for the construction:

~~~~ {#micro .cpp .numberLines}
@TFELLibraries {"Material"};

@Includes{
#include "TFEL/Material/MicrostructureDescription.hxx"
}
~~~~

Here is the construction of the microstructure:

~~~~ {#micro .cpp .numberLines}
using namespace tfel::math;
tvector<3u, real> n_a = {1, 0, 0};
tvector<3u, real> n_b = {0, 1, 0};
st2tost2<3u, stress> C_i1;
st2tost2<3u, stress> C_i2;
stensor<3u,real> E={1,0,0,0,0,0};

using namespace tfel::material;
static constexpr auto value =
StiffnessTensorAlterationCharacteristic::UNALTERED;
computeIsotropicStiffnessTensorII<3u, value, stress, real>(
C_i1, Ei1, nui1);
computeIsotropicStiffnessTensorII<3u, value, stress, real>(
C_i2, Ei2, nui2);

using namespace tfel::material::homogenization::elasticity;

Ellipsoid<length> ellipsoid1(a,b,c);

GrainPhase<3u, real, stress,length> grain1(f1, C_i1, ellipsoid1, n_a, n_b);
GrainPhase<3u, real, stress,length> grain2(f2, C_i2, ellipsoid1, n_a, n_b);

Polycrystal<3u, real, length, stress> crystal;
crystal.addGrain(grain1);
crystal.addGrain(grain2);
~~~~


# Implementation of the homogenization schemes

~~~~ {#hs .cpp .numberLines}
template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct HomogenizationScheme {
    tfel::math::st2tost2<N, StressType> homogenized_stiffness;
    tfel::math::stensor<N, StressType> effective_polarisation;
    std::vector<tfel::math::st2tost2<N, real>> mean_strain_localisation_tensors;
    std::vector<tfel::math::stensor<N, real>> mean_free_strains;
    std::vector<tfel::math::st2tost2<N, real>> strain_second_moments;
  };
~~~~

## Implementation in `MFront`

~~~~ {#biphasic .cpp .numberLines}
@StateVariable real distrib;
distrib.setEntryName("TypeOfDistribution");
@StateVariable stress E0;
E0.setEntryName("MatrixYoungModulus");
@StateVariable real nu0;
nu0.setEntryName("MatrixPoissonRatio");
@StateVariable stress Ei;
Ei.setEntryName("InclusionYoungModulus");
@StateVariable real nui;
nui.setEntryName("InclusionPoissonRatio");
@StateVariable real f;
f.setEntryName("InclusionVolumeFraction");
@StateVariable length a;
a.setEntryName("FirstSemiLength");
@StateVariable length b;
b.setEntryName("SecondSemiLength");
@StateVariable length c;
c.setEntryName("ThirdSemiLength");
~~~~

Note that the orientation for the oriented case will be fixed in the `mfront` file,
and it will be the same for the transverse isotropic case which necessitates to
gives the direction of one of the axes of the ellipsoid. But those orientations
could also be inputs of the law.

We choose the Young homogenized modulus `E` to be the output of our
`Function`.

~~~~ {#micro .cpp .numberLines}
@Output stress E;
~~~~

For `MatrixInclusionMicrostructure`:

~~~~ {#biphasic .cpp .numberLines}
auto zs=stress(0);
stensor<3u,stress> P={zs,zs,zs,zs,zs,zs};
std::vector<stensor<3u,stress>> polarizations = {P,P,P};
HomogenizationScheme<3u, real, length, stress> h_DS=computeMoriTanaka<3u, real, length, stress>(micro1,polarizations,false,E,true);
Chom_11=h_DS.homogenized_stiffness(0,0);
}
~~~~

For `Polycrystal`:

~~~~ {#biphasic .cpp .numberLines}
auto zs=stress(0);
stensor<3u,stress> P={zs,zs,zs,zs,zs,zs};
std::vector<stensor<3u,stress>> polarizations = {P,P};
auto h_s=computeSelfConsistent<3u, real, length, stress>(crystal,polarizations,false,E,false,20,6);
Chom_11=h_s.homogenized_stiffness(0,0);
~~~~

## Test in `python`

We choose `interface` `python` to compute the schemes for a volume fraction
of ellipsoids which goes from 0 to 1.

Here is the `python` file :

~~~~ {#biphasic .py .numberLines}
import src.biphasic as bip
from matplotlib import pyplot as plt
import numpy as np


frac=np.linspace(0,1,200)
E_I=np.array([bip.biphasic_Ellipsoidal(0,1e9,0.2,100e9,0.3,f,30,1,1)/1e11 for f in frac])
E_TI=np.array([bip.biphasic_Ellipsoidal(1,1e9,0.2,100e9,0.3,f,1,30,1)/1e11 for f in frac])
E_O=np.array([bip.biphasic_Ellipsoidal(2,1e9,0.2,100e9,0.3,f,30,1,1)/1e11 for f in frac])
plt.figure()
g1,=plt.plot(frac,E_I,color="blue")
g2,=plt.plot(frac,E_TI,color="red")
g3,=plt.plot(frac,E_O,color="green")
plt.legend([g1,g2,g3],['Isotropic','Transverse Isotropic','Oriented'])
plt.xlabel(r'$f$')
plt.ylabel(r'$E^{1}/E^{1}_0$',rotation='horizontal')
plt.show()
plt.close()
~~~~

We see here that we chose prolate spheroids for the ellipsoids, with aspect ratio of 30.
The first case is the isotropic distribution, the second case is the transverse isotropic distribution where
the biggest axis rotates in the `y-z` plane, and in the last case, the biggest axis is oriented in the `x` direction.
The results are given by the following figure.

![Homogenized young modulus (direction 1) of a composite made of a matrix and ellipsoidal inclusions as a function of the volume fraction of ellipsoids](./img/Ellipsoidal.png)

<!-- Local IspellDict: english -->
