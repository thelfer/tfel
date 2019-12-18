% Implementing a simple finite strain single crystal behaviour, comparison with Huang's implementation
% Thomas Helfer, Jürgen Almanstoetter
% October 2017

This document describes how to implement a phenomenological crystal
plasticity model used in Huang's UMAT subroutine (See
@huang_user-material_1991).

Such an example illustrates:

- The usage of `FiniteStrainSingleCrystal` brick (see
  [this page](finitestrainsinglecrystal.html)).
- The usage of the `@CristalStructure` and `@SlidingSystem`
  keywords. See [this page](singlecrystal.html) for details.

The whole implementation is available
[here](./gallery/finitestrainsinglecrystal/FiniteStrainSingleCrystalHuang.mfront).

# Constitutive equations

The shear rate \(\dot{g}^\alpha\) on each slip system \(\alpha\) is
determined by the resolved shear stress \(\tau^\alpha\) and the slip
system strength \(\tau_c^\alpha\) according to a power law
relationship,

\[
\dot{g}^\alpha=\dot a\left|\frac{\tau^\alpha}{\tau_c^\alpha}\right|^m sgn(\tau^\alpha).
\]

\(\dot a\) denotes the reference shear strain rate on slip system
\(\alpha\) and the exponent \(m\) the strain rate sensitivity. Strain
hardening is described by the evolution law

\[
\dot\tau_c^\alpha=\sum_{\beta=1}^{n} h_{\alpha\beta}\,\left|\dot{g}^\beta\right|
\]

where \(h_{\alpha\beta}\) is the matrix of strain hardening
coeffcients. The diagonal terms \(h_{\alpha\alpha}\) indicate self
hardening, while the off-diagonal entries
\(h_{\alpha\beta}\,(\alpha\neq\beta)\) represent latent hardening. The
sum extends over all \(n\) slip systems. The latent hardening is
assumed to scale with \(h_{\alpha\alpha}\) by the latent hardening
coefficient \(r\),

\[
h_{\alpha\beta}=r\,h_{\alpha\alpha}.
\]

Self hardening is expressed using

\[
h_{\alpha\alpha}=h(g_{c})=h_0\,\text{sech}^2\left|\frac{h_0g_{c}}{\tau_{sat}-\tau_0}\right|.
\]

Here \(h_0\) is the initial hardening modulus, \(\tau_0\) the initial
yield stress, \(\tau_{sat}\) the saturation stress of hardening and
\(g_{c}\) the cumulative shear strain, that is given by

\[
g_{c}=\sum_{\alpha=1}^{n} \int_0^t \left|\dot{g}^{\alpha}\right|\, dt
\]

# `MFront` Implementation

## `DSL` type

~~~~{.cpp}
@DSL ImplicitFiniteStrain;
~~~~

## Some meta-data

~~~~{.cpp}
@Behaviour FiniteStrainSingleCrystalBrickHuang;
@Author    Juergen Almanstoetter, Thomas Helfer;
@Date      25/09/2017;
@Description{
  "Implementation of the Huang's model."
}
~~~~

## Default numerical parameters

~~~~{.cpp}
@Algorithm NewtonRaphson;
@Theta   1;
@Epsilon 1.e-11;
~~~~

## The `FiniteStrainSingleCrystal` brick

~~~~{.cpp}
@Brick FiniteStrainSingleCrystal{
 shifted_elastic_deformation_gradient : true
};
~~~~

## Description of the crystal structure and the slip systems

~~~~{.cpp}
@CrystalStructure FCC;
@SlidingSystem <0,1,-1>{1,1,1};
~~~~

## Material properties

~~~~{.cpp}
@MaterialProperty stress h0;
@MaterialProperty stress tau_sat;
@MaterialProperty stress tau0;
@MaterialProperty real m;
@MaterialProperty strainrate da;
@MaterialProperty real y0;
@MaterialProperty real r;
~~~~

## Auxiliary state variables

### Cumulative shear strain on slip systems

In our implementation, we store the cumulative shear strain \(g_{c}\)
in an auxiliary state variable.

~~~~{.cpp}
@AuxiliaryStateVariable real gc;
~~~~

For convenience, a variable to store the increment of \(g_{c}\) over
the time step will be needed. This increment is defined as a local
variable, to be able to access it from the `@Integrator` and
`@UpdateAuxiliaryStateVariables` blocks of our implementation.

~~~~{.cpp}
@LocalVariable real dgc; 
~~~~

### Slip system strength 

In this implementation, we store the strength of each slip system in
an array of auxiliary state variables \(y).

~~~~{.cpp}
@AuxiliaryStateVariable real y[Nss];
~~~~

~~~~{.cpp}
@LocalVariable real dy[Nss];
~~~~

## Integration of the constitutive equations

### Computation of \(\Delta\,g_{c}\)

~~~~{.cpp}
dgc = real{};
for(unsigned short i=0;i!=Nss;++i){
  dgc+=abs(dg[i]);
}
~~~~

### Computation of the self-hardening

~~~~{.cpp}
const auto hb = h0/power<2>(std::cosh((h0*(gc+dgc))/(tau_sat-tau0)));
~~~~

### Computation of the hardening strength and the plastic slip rate of the \(i\)th system

#### Hardening strength

~~~~{.cpp}
dy[i] = real{};
for(unsigned short j=0;j!=Nss;++j){
  const auto qab = (ss.areCoplanar(i,j)) ? 1 : r;
  dy[i] += qab*hb*abs(dg[j]);
}
~~~~

#### Plastic slip rate

~~~~{.cpp}
const auto tau = ss.mu[i] | M ;
if (abs(tau)>0){
  const auto y_   = y0+y[i]+dy[i];
  const auto sgn  = tau/abs(tau);
  const auto hsi  = dt*da*pow(abs(tau/y_),m-1)/y_;
  fg[i] -= hsi*abs(tau)*sgn;
  const auto Hsi  = hsi*m;
  dfg_ddeel(i)    = -Hsi*(ss.mu[i]|dM_ddeel);
  dfg_ddg(i,i)   -=  m*fg[i]*hb/y_;
}
~~~~

## Update auxiliary state variables

If the Newton-Raphson algorithm converges, the evolution of \(g\) is
known. The only thing left is to update the values of the auxiliary
state variables \(g_{c}\) and \(\Delta\,y\). Here, for convenience, a
litte approximation is made: the values of \(\Delta\,g_{c}\) and
\(\Delta\,y\) as computed at the last iteration of the Newton-Raphson
algorithm, are used for that purpose:

~~~~{.cpp}
@UpdateAuxiliaryStateVariables{
  gc+=dgc;
  y+=dy;
} // end of @UpdateAuxiliaryStateVariables
~~~~

Indeed, a more rigorous approach would be to re-evaluate
\(\Delta\,g_{c}\) and \(\Delta\,y\) using the values of \(\Delta\,g\)
at convergence.

# Comparison to Huang's implementation

The `MFront` solution is verified using `ABAQUS` by comparing to
results obtained using Huang's `UMAT` subroutine. In both cases a
single crystal copper bar of 100 mm x 10 mm x 10 mm is subjected to a
tensile load of 200 MPa within 1s.  The required materials parameters
are taken from the reference [1], with elastic moduli \(C_{11} =
168400\) MPa, \(C_{12} =121400\) MPa, \(C_{44} =75400\) MPa, initial
flow stress \(\tau_0=60.8\) MPa, initial hardening \(h_0=541.5\) MPa,
rate sensitivity exponent \(m=10\), latent hardening \(r=1.0\),
reference strain rate \(\dot a=0.001\) and saturation strength
\(\tau_{sat}=109.8\) MPa.  Symmetric boundary conditions are applied
in all three directions to allow for uniform deformation. The copper
material has an fcc crystal structure with [110]\)\langle 111\rangle\)
slip systems. The grain is oriented using zeros for all Euler angles
and represented by a single higher order C3D20R hex element.
