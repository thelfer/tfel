% The TFEL Python API
% Thomas Helfer
% 19/12/2017

This page describes the `python` modules based on the `TFEL`
libraries.

# The `tfel.math` module

## Bindings related to the `tvector` class

Three classes standing for vectors are available: `TVector1D`, `TVector2D` and `TVector3D`.
These `class` can be initialized/modified as follows:

~~~~{.py}
import tfel.math as tm
n1=tm.TVector3D([1.,0.,0.])
n2=tm.TVector3D()
n2[0]=1.
n3_=np.array([1.,0.,0.])
n3=tm.TVector3D(n3_)
~~~~

## Bindings related to the `stensor` class

Three classes standing for symmetric tensors are available: `Stensor1D`, `Stensor2D` and `Stensor3D`.
These `class` can be initialized/modified as follows:

~~~~{.py}
import tfel.math as tm
s1=tm.Stensor3D([1.,0.,0.,0.,0.,0.])
s2=tm.Stensor2D([1.,0.,0.,0.])
sig=tm.Stensor3D()
sig[2]=1.e9
epsilon=np.zeros((6,))
epsilon[0]=0.001
eps=tm.Stensor3D(epsilon)
~~~~

The standard mathematical operations are defined:

- addition of two symmetric tensors.
- subtraction of two symmetric tensors.
- multiplication by scalar.
- in-place addition by a symmetric tensor.
- in-place subtraction by a symmetric tensor.
- in-place multiplication by scalar.
- in-place division by scalar.

The following functions are available:

- `sigmaeq`: computes the von Mises norm of a symmetric tensor.
- `tresca`: computes the Tresca norm of a symmetric tensor.

## Bindings related to the `st2tost2` class

Three classes standing for fourth-order tensors with minor symmetries
are available:
`ST2toST21D`, `ST2toST22D`, `ST2toST23D`.
These `class` can be initialized/modified as follows:

~~~~{.py}
import tfel.math as tm
s1=tm.ST2toST22D([[1.,0.,0.,0.],[0.,1.,0.,0.],[0.,0.,1.,0.],[0.,0.,0.,1.]])
C=tm.ST2toST23D()
C[2,2]=1.e9
A=np.zeros((6,6))
A[0,1]=0.001
A_=tm.ST2toST23D(A)
~~~~
# The `tfel.material` module

## Bindings related to the \(\pi\)-plane

The following functions are available:

- `buildFromPiPlane`: returns a tuple containing the three eigenvalues
  of the stress corresponding to the given point in the \(\pi\)-plane.
- `projectOnPiPlane`: projects a stress state, defined its three
  eigenvalues or by a symmetric tensor, on the \(\pi\)-plane.

## Bindings related to the Hosford equivalent stress

The `computeHosfordStress` function, which compute the Hosford
equivalent stress, is available.

## Bindings related to the Barlat equivalent stress

The following functions are available:

- `makeBarlatLinearTransformation1D`: builds a \(1D\) linear
  transformation of the stress tensor.
- `makeBarlatLinearTransformation2D`: builds a \(2D\) linear
  transformation of the stress tensor.
- `makeBarlatLinearTransformation3D`: builds a \(3D\) linear
  transformation of the stress tensor.
- `computeBarlatStress`: computes the Barlat equivalent Barlat stress.

## Bindings related to the `IsotropicModuli`

The three following `class` are available: `KGModuli`, `YoungNuModuli`
and `LambdaMuModuli`.
They can be constructed as: 

~~~~{.py}
import tfel.material as tmat
K=1e9
G=0.2e9
kg=tmat.KGModuli(K,G)
kg2=tmat.KGModuli(kg)
~~~~

their attributes are accessible and their methods permit to convert them:

~~~~{.py}
kg=tmat.KGModuli(3e6,2e6)
print(kg.kappa,kg.mu)

Enu=tmat.YoungNuModuli(1e6,0.2)
print(Enu.young,Enu.nu)

lg=tmat.LambdaMuModuli(0.5e6,1e6)
print(lg.lamb,lg.mu)

Enu2=kg.ToYoungNu()
print(Enu2.young,Enu2.nu)
~~~~
 
Note that Lamé coefficient is `lamb` in `Python` and `lambda` in `C++`.

## The `tfel.material.homogenization` module

The `tfel.material.homogenization` module mirrors the functionalities defined
in the namespace `tfel::material::homogenization::elasticity`. Hence,
the reader may be interested by the [details](tfel-material.html#homogenization)
of the documentation of this namespace.
The `Python` modules can be imported as follows:

~~~~{.py}
import tfel.material.homogenization as hm
import tfel.material as tmat
import tfel.math as tm
~~~~

### Hill tensors

The Hill tensors are available:

~~~~{.py}
young=1e9
nu=0.2
P0=hm.computeSphereHillTensor(young,nu)

e=10.
n=tm.TVector3D([0.,0.,1.])
P1=hm.computeAxisymmetricalHillTensor(young,nu,n,e)

n_a=tm.TVector3D([0.,0.,1.])
n_b=tm.TVector3D([0.,1.,0.])
a=10.
b=1.
c=3.
P2=hm.computeHillTensor(young,nu,n_a,a,n_b,b,c)
~~~~

Note that the above functions have slightly different names from the `C++` version:
`computeHillPolarisationTensor` becomes `computeHillTensor`.
Moreover, as in `C++`, it is possible to pass isotropic moduli
objects instead of `young` and `nu`:

~~~~{.py}
IM0=tmat.YoungNuModuli(young,nu)
P0 = hm.computeSphereHillTensor(IM0)
P0_axi = hm.computeAxisymmetricalHillTensor(IM0,n_a,e)
P0_ellipsoid = hm.computeHillTensor(IM0,n_a,a,n_b,b,c)
~~~~

The computation in the anisotropic reference medium is given by:

~~~~{.py}
C0=tm.ST2toST23D(1e9*np.eye(6))
C0[0,2]=0.1e9
C0[2,0]=0.1e9

max_it=14 #optional

P=hm.computeAnisotropicHillTensor(C0,n_a,a,n_b,b,c,max_it)
~~~~

Note that the integer `max_it` is related to the number of
iterations in the integration process (see the [documentation](tfel-material.html#homogenization)
of the namespace).

### Localisation tensors

The computation of the strain localisation tensors are given by:

~~~~{.py}
young=1e9
nu=0.2
young_i=100e9
nu_i=0.3

# Spherical inclusion
A_S=hm.computeSphereLocalisationTensor(young,nu,young_i,nu_i)

# Axisymmetric ellipsoidal inclusion (or spheroid)
e=20.
n=tm.TVector3D([0.,0.,1.])
A_AE=hm.computeAxisymmetricalLocalisationTensor(young,nu,young_i,nu_i,n,e)

# General ellipsoidal inclusion
a=10.
b=1.
c=3.
n_a=tm.TVector3D([0.,0.,1.])
n_b=tm.TVector3D([0.,1.,0.])

A_GE=hm.computeLocalisationTensor(young,nu,young_i,nu_i,n_a,a,n_b,b,c)
~~~~

Some isotropic moduli can also be passed for the elasticities, as follows:

~~~~{.py}
IM0=tmat.YoungNuModuli(young,nu)
IMi=tmat.YoungNuModuli(young_i,nu_i)
A0 = hm.computeSphereLocalisationTensor(IM0,IMi)
A0_axi = hm.computeAxisymmetricalLocalisationTensor(IM0,IMi,n_a,e)
A0_ellipsoid = hm.computeLocalisationTensor(IM0,IMi,n_a,a,n_b,b,c)
~~~~

Note that if the elasticity of the inclusion
is not isotropic, an anisotropic elasticity `C_i` can be provided, assuming that this elasticiy
is expressed in the same basis as the one defined by `n_a,n_b` (the local basis of the inclusion):

~~~~{.cpp}
A_aniso = hm.computeLocalisationTensor(IM0,C_i,n_a,a,n_b,b,c)
~~~~

The case of an anisotropic reference medium is detailed below:

~~~~{.py}
# Anisotropic matrix
max_it=12 #optional
C0_glob=tm.ST2toST23D(1e9*np.eye(6)) # C0_glob is defined in the basis in which the localisation tensor is returned
C0_glob[0,2]=0.1e9
C0_glob[2,0]=0.1e9

Ci_loc=tm.ST2toST23D(1e9*np.eye(6)) # Ci_loc is defined in the basis defined by 'n_a' and 'n_b'

A_AN=hm.computeAnisotropicLocalisationTensor(C0_glob,Ci_loc,n_a,a,n_b,b,c,max_it)
~~~~

Note that in this case, the elasticity of the inclusion
is always passed as a `ST2toST2` object `C_i_loc`. Moreover, if this elasticity is not isotropic,
`C_i_loc` is expressed in the same basis as the one defined by `n_a,n_b`
(the local basis of the inclusion, see the [documentation](tfel-material.html#homogenization)
of the namespace).

### Homogenization schemes in biphasic media

The following schemes are available for biphasic media with
2 isotropic phases:
 
 - Mori-Tanaka scheme
 - dilute scheme
 - Ponte Castaneda and Willis scheme

Here are some examples of computation:
 
~~~~{.py}
young=1e9
nu=0.2
young_i=100e9
nu_i=0.3
f=0.2

# Spherical inclusions
pairDS=hm.computeSphereDiluteScheme(young,nu,f,young_i,nu_i)
pairMT=hm.computeSphereMoriTanakaScheme(young,nu,f,young_i,nu_i)
young_hom=pairDS[0]
nu_hom=pairDS[1]

# Ellipsoidal inclusions
a=10.
b=1.
c=3.

## Isotropic distribution of orientations
pair_I_DS=hm.computeIsotropicDiluteScheme(young,nu,f,young_i,nu_i,a,b,c)
pair_I_MT=hm.computeIsotropicMoriTanakaScheme(young,nu,f,young_i,nu_i,a,b,c)
young_I=pair_I_DS[0]
nu_I=pair_I_DS[1]

n_a=tm.TVector3D([0.,0.,1.])
n_b=tm.TVector3D([0.,1.,0.])

## Ellipsoids which turn around their axis 'a'
C_TI_DS=hm.computeTransverseIsotropicDiluteScheme(young,nu,f,young_i,nu_i,n_a,a,b,c)
C_TI_MT=hm.computeTransverseIsotropicMoriTanakaScheme(young,nu,f,young_i,nu_i,n_a,a,b,c)

## Oriented ellipsoids
C_O_DS=hm.computeOrientedDiluteScheme(young,nu,f,young_i,nu_i,n_a,a,n_b,b,c)
C_O_MT=hm.computeOrientedMoriTanakaScheme(young,nu,f,young_i,nu_i,n_a,a,n_b,b,c)
~~~~


### Homogenization bounds

The available bounds are:

 - Voigt bound
 - Reuss bound
 - Hashin-Shtrikman bounds


### Homogenization of general microstructures


<!-- Local IspellDict: english -->
