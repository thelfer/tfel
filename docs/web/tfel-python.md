% The TFEL Python API
% Thomas Helfer
% 19/12/2017

This page describes the `python` modules based on the `TFEL`
libraries.

# The `tfel.math` module

## Bindings related to the `stensor` class

Three classes standing for symmetric tensors are available:

- `Stensor1D`: symmetric tensor in \(1D\).
- `Stensor2D`: symmetric tensor in \(2D\).
- `Stensor3D`: symmetric tensor in \(3D\).

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

Three classes standing for linear transformation of symmetric tensors
to symmetric tensors:

- `ST2toST1D`: linear transformation of symmetric tensors to symmetric
tensors in \(1D\).
- `ST2toST2D`: linear transformation of symmetric tensors to symmetric
tensors in \(2D\).
- `ST2toST3D`: linear transformation of symmetric tensors to symmetric
tensors in \(3D\).

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

## Bindings related to the IsotropicModuli

The objects available are the following:

- KGModuli
- YoungNuModuli
- LambdaMuModuli

They can be constructed as: 

~~~~{.py}
from tfel.material import KGModuli
K=1e9
G=0.2e9
kg=KGModuli(K,G)
kg2=KGModuli(kg)
~~~~

and their methods permit to convert them:

- `ToYoungNu()`
- `ToLambdaMu()`
- `ToKG()`
 

## The `tfel.material.homogenization` module

### Hill tensors

The following functions are available:

- `computeSphereHillTensor` corresponds to the function `computeSphereHillPolarisationTensor` of
`tfel::material::homogenization::elasticity` in 3d, where the arguments are Young modulus and Poisson ratio:

~~~~{.py}
young=1e9
nu=0.2
P=tfel.material.homogenization.computeSphereHillTensor(young,nu)
~~~~

- `computeAxisymmetricalHillTensor` corresponds to the function `computeAxisymmetricalHillPolarisationTensor` of
`tfel::material::homogenization::elasticity` in 3d:

~~~~{.py}
young=1e9
nu=0.2
e=10.
n=tfel.math.TVector3D()
n[0]=0.
n[1]=0.
n[2]=1.
P=tfel.material.homogenization.computeAxisymmetricalHillTensor(young,nu,n,e)
~~~~

- `computeHillTensor` corresponds to the function `computeHillPolarisationTensor` of
`tfel::material::homogenization::elasticity` in 3d:

~~~~{.py}
young=1e9
nu=0.2
n_a=tfel.math.TVector3D()
n_b=tfel.math.TVector3D()
n_a[0]=0.
n_a[1]=0.
n_a[2]=1.
n_b[0]=0.
n_b[1]=1.
n_b[2]=0.
a=10.
b=1.
c=3.
P=tfel.material.homogenization.computeHillPolarisationTensor(young,nu,n_a,a,n_b,b,c)
~~~~

- `computeAnisotropicHillTensor` corresponds to the function `computeAnisotropicHillTensor` of
`tfel::material::homogenization::elasticity` in 3d (where the stiffness tensor, a `tfel.math.ST2toST23D` object, is an argument):

~~~~{.py}
C0=tfel.math.ST2toST23D()
for i in range(6):
    C0.__setitem__(i,i,1e9)
C0.__setitem__(0,2,0.1e9)
C0.__setitem__(2,0,0.1e9)


n_a=tfel.math.TVector3D()
n_b=tfel.math.TVector3D()
n_a[0]=0.
n_a[1]=0.
n_a[2]=1.
n_b[0]=0.
n_b[1]=1.
n_b[2]=0.

a=10.
b=1.
c=3.
max_it=14 #optional

P=tfel.material.homogenization.computeAnisotropicHillTensor(C0,n_a,a,n_b,b,c,max_it)
~~~~

### Localisation tensors

The following functions are available:

 - `computeSphereLocalisationTensor`
 - `computeAxisymmetricalEllipsoidLocalisationTensor`
 - `computeEllipsoidLocalisationTensor`
 - `computeAnisotropicLocalisationTensor`
 
which are, in 3d, the same as those defined in `tfel::material::homogenization::elasticity`.

Here are some examples of computation:

~~~~{.py}
young=1e9
nu=0.2
young_i=100e9
nu_i=0.3

# Spherical inclusion
A_S=tfel.material.homogenization.computeSphereLocalisationTensor(young,nu,young_i,nu_i)

# Axisymmetric ellipsoidal inclusion (or spheroid)
e=20.
n=tfel.math.TVector3D()
n[0]=0.
n[1]=0.
n[2]=1.
A_AE=tfel.material.homogenization.computeAxisymmetricalEllipsoidLocalisationTensor(young,nu,young_i,nu_i,n,e)

# General ellipsoidal inclusion
a=10.
b=1.
c=3.

n_a=tfel.math.TVector3D()
n_b=tfel.math.TVector3D()
n_a[0]=0.
n_a[1]=0.
n_a[2]=1.
n_b[0]=0.
n_b[1]=1.
n_b[2]=0.

A_GE=tfel.material.homogenization.computeEllipsoidLocalisationTensor(young,nu,young_i,nu_i,n_a,a,n_b,b,c)

# Anisotropic matrix
max_it=12 #optional
C0_glob=tfel.math.ST2toST23D() # C0_glob is defined in the basis in which the localisation tensor is returned
for i in range(6):
    C0_glob.__setitem__(i,i,1e9)
C0_glob.__setitem__(0,2,0.1e9)
C0_glob.__setitem__(2,0,0.1e9)

Ci_loc=tmath.ST2toST23D() # Ci_loc is defined in the basis defined by 'n_a' and 'n_b'
for i in range(6):
    Ci_loc.__setitem__(i,i,1e9)

A_AN=tfel.material.homogenization.computeAnisotropicLocalisationTensor(C0_glob,Ci_loc,n_a,a,n_b,b,c,max_it)
~~~~


### Homogenization schemes

The following functions are available:

 - `computeSphereDiluteScheme`
 - `computeSphereMoriTanakaScheme`
 - `computeOrientedDiluteScheme`
 - `computeOrientedMoriTanakaScheme`
 - `computeIsotropicDiluteScheme`
 - `computeIsotropicMoriTanakaScheme`
 - `computeTransverseIsotropicDiluteScheme`
 - `computeTransverseIsotropicMoriTanakaScheme`
 
which are, in 3d, the same as those defined in `tfel::material::homogenization::elasticity`.
Here are some examples of computation:
 
~~~~{.py}
young=1e9
nu=0.2
young_i=100e9
nu_i=0.3
f=0.2

# Spherical inclusions
pairDS=tfel.material.homogenization.computeSphereDiluteScheme(young,nu,f,young_i,nu_i)
pairMT=tfel.material.homogenization.computeSphereMoriTanakaScheme(young,nu,f,young_i,nu_i)
young_hom=pairDS[0]
nu_hom=pairDS[1]

# Ellipsoidal inclusions
a=10.
b=1.
c=3.

## Isotropic distribution of orientations
pair_I_DS=tfel.material.homogenization.computeIsotropicDiluteScheme(young,nu,f,young_i,nu_i,a,b,c)
pair_I_MT=tfel.material.homogenization.computeIsotropicMoriTanakaScheme(young,nu,f,young_i,nu_i,a,b,c)
young_I=pair_I_DS[0]
nu_I=pair_I_DS[1]

n_a=tfel.math.TVector3D()
n_b=tfel.math.TVector3D()
n_a[0]=0.
n_a[1]=0.
n_a[2]=1.
n_b[0]=0.
n_b[1]=1.
n_b[2]=0.

## Ellipsoids which turn around their axis 'a'
C_TI_DS=tfel.material.homogenization.computeTransverseIsotropicDiluteScheme(young,nu,f,young_i,nu_i,n_a,a,b,c)
C_TI_MT=tfel.material.homogenization.computeTransverseIsotropicMoriTanakaScheme(young,nu,f,young_i,nu_i,n_a,a,b,c)

## Oriented ellipsoids
C_O_DS=tfel.material.homogenization.computeOrientedDiluteScheme(young,nu,f,young_i,nu_i,n_a,a,n_b,b,c)
C_O_MT=tfel.material.homogenization.computeOrientedMoriTanakaScheme(young,nu,f,young_i,nu_i,n_a,a,n_b,b,c)
~~~~


### Homogenization bounds

- `computeVoigtStiffness` corresponds to the function `computeVoigtStiffness` of
`tfel::material::homogenization::elasticity` for 2 phases in dimension 3.
- `computeReussStiffness` corresponds to the function `computeReussStiffness` of
`tfel::material::homogenization::elasticity` for 2 phases in dimension 3.
- `computeIsotropicHashinShtrikmanBounds` corresponds to the function `computeIsotropicHashinShtrikmanBounds` of
`tfel::material::homogenization::elasticity` for 2 phases in dimension 3.


<!-- Local IspellDict: english -->
