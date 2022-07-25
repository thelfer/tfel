---
title: Description of the TFEL glossary
author: Helfer Thomas, Maxence Wangermez, Bernaud Stéphane
date: 21/11/2017
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\dtot}{\mathrm{d}}
\newcommand{\paren}[1]{{\displaystyle \left(#1\right)}}
\newcommand{\average}[1]{{\displaystyle \left\langle#1\right\rangle}}
\newcommand{\Frac}[2]{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}

# The `AxialDeformationGradient` entry

This entry describes the axial component of the deformation gradient.


* units:
no units specified
* type: scalar

## Notes 

This quantity is only meaningful under on of the plane stress modelling hypotheses.

# The `AxialGrowth` entry

This entry describes axial growth under irradiation.


* units:
no units specified
* type: scalar

# The `AxialStrain` entry

This entry describes the axial strain.


* units:
no units specified
* type: scalar

## Notes 

This quantity is only meaningful under on of the plane stress modelling hypotheses.

# The `AxialStress` entry

This entry describes the axial stress.


* units:
no units specified
* type: scalar

## Notes 

This quantity is only meaningful under the axisymmetrical generalized plane stress modelling hypothesis.

# The `B10BurnUp` entry

This entry describes the burn-up of an absorant material containing \(\mbox{}^{10}B\).


* units:
  *SI: m^{-3}
* type: scalar

# The `Broken` entry

This entry describes a material failure indicator.


* units:
no units specified
* type: scalar

# The `BulkModulus` entry

This entry describes the bulk modulus of an isotropic material.


* units:
  *SI: Pa
* type: scalar

# The `BurnUp_AtPercent` entry

This entry describes the burn-up in at.%.


* units:
  *SI: at./100
* type: scalar

# The `BurnUp_MWJperTm` entry

This entry describes the burn-up in MegaWattJour per tons of metals.


* units:
  *SI: MWJ/tm
* type: scalar

# The `CohesiveForce` entry

This entry describes cohesive force for cohesize zone models.


* units:
  *SI: Newton
* type: vector

# The `ConvectiveHeatTransferCoefficient` entry

This entry describes the heat transfer coefficient by convection.


* units:
  *SI: W.m^{-2}.K^{-1}
* type: scalar

# The `CrossSectionArea` entry

This entry describes ??.


* units:
  *SI: m^{2}
* type: scalar

# The `CylindricalStress` entry

This entry describes the stress in the cylindrical frame.


* units:
  *SI: Pa
* type: tensor

# The `Damage` entry

This entry describes the damage, generally between 0 (sound material) and 1 (broken material).


* units:
no units specified
* type: scalar

# The `DeformationGradient` entry

This entry describes gradient of the transformation.


* units:
no units specified
* type: tensor

# The `Displacement` entry

This entry describes the displacement.


* units:
  *SI: m
* type: vector

# The `DualStress` entry

This entry describes the dual stress of the strain measure.


* units:
  *SI: stress
* type: tensor

# The `ElasticStrain` entry

This entry describes The elastic strain.


* units:
no units specified
* type: tensor

# The `Emissivity` entry

This entry describes the emissivity of the surface of a material is its effectiveness in emitting energy as thermal radiation.


* units:
  *SI: kg.m.T^{−3}
* type: scalar

# The `EquivalentPlasticStrain` entry

This entry describes the equivalent plastic strain.


* units:
no units specified
* type: scalar

# The `EquivalentStrain` entry

This entry describes the sum of all plastic and viscoplastic equivalent strains.


* units:
no units specified
* type: scalar

## Notes 

This quantity has no direct physical meaning.

# The `EquivalentViscoplasticStrain` entry

This entry describes the equivalent viscoplastic strain.


* units:
no units specified
* type: scalar

# The `FastNeutronFluence_01MeV` entry

This entry describes the fast neutron fluence, where the limit for fast neutron is 0.1 MeV.


* units:
  *SI: n.m^{-2}
* type: scalar

# The `FastNeutronFluence_1MeV` entry

This entry describes the fast neutron fluence, where the limit for fast neutron is 1 MeV.


* units:
  *SI: n.m^{-2}
* type: scalar

# The `FastNeutronFlux_01MeV` entry

This entry describes the fast neutron fluence.


* units:
  *SI: n.m^{-2}.s^{-1}
* type: scalar

# The `FastNeutronFlux_1MeV` entry

This entry describes the fast neutron fluence.


* units:
  *SI: n.m^{-2}.s^{-1}
* type: scalar

# The `FirstAxisSecondMomentArea` entry

This entry describes ??.


* units:
  *SI: m^{4}
* type: scalar

# The `FirstLameCoefficient` entry

This entry describes the first Lamé's coefficient of an isotropic material.


* units:
  *SI: Pa
* type: scalar

# The `FissionDensity` entry

This entry describes the fission density.


* units:
  *SI: m^{-3}
* type: scalar

# The `GaseousSwelling` entry

This entry describes swelling du to gazeous fission products.


* units:
no units specified
* type: scalar

# The `GrainSize` entry

This entry describes the grain size.


* units:
  *SI: m
* type: scalar

# The `HeatFlux` entry

This entry describes the heat flux, generally in the current configuration..


* units:
  *SI: J.m^{-2}.s^{-1}
* type: vector

# The `HeatTransferCoefficient` entry

This entry describes the heat transfer coefficient is the proportionality constant between the heat flux and the temperature difference.


* units:
  *SI: W.m^{-2}.K^{-1}
* type: scalar

# The `HillStress` entry

This entry describes the Hill equivalent stress.


* units:
  *SI: Pa
* type: tensor

# The `HydrostaticPressure` entry

This entry describes the hydrostatic pressure, defined as the third of the trace of the stress tensor.


* units:
  *SI: Pa
* type: tensor

# The `IrradiationDamage` entry

This entry describes the irradiation damage, measure by the mean number of displacements of each atoms.


* units:
  *SI: dpa
* type: scalar

# The `IrradiationInducedSwelling` entry

This entry describes swelling du to irradiation damage.


* units:
no units specified
* type: scalar

# The `IrradiationSwelling` entry

This entry describes swelling du to irradiation damage.


* units:
no units specified
* type: scalar

# The `IrradiationTemperature` entry

This entry describes the mean temperature (in time) of the temperature during the irradiation.


* units:
  *SI: K
* type: scalar

## Description

This temperature is defined as follows:
\[
\average{T}\paren{t,\vec{r}}  = \Frac{1}{t-t_{0}}\int_{t_{0}}^{t}T\paren{u,\vec{r}}\,\dtot\, u
\]
where

* \(T\paren{t,\vec{r}}\) is the value of the temperature at time \(t\) and position \(\vec{r}\) ;
* \(t_{0}\) is the reference time;
* \(t\) is the current time.

In pratice, this integral is computed incrementally as follows: 
\[
\average{T}\paren{t+dt,\vec{r}} \approx \Frac{1}{t+dt-t_{0}}\left[\paren{t-t_{0}}\,\average{T}\paren{t,\vec{r}}+\Frac{dt}{2}\left[T\paren{t,\vec{r}}+T\paren{t+dt,\vec{r}}\right]\right]
\]

## Notes 

The approximation made when computing the time integral may lead to (small) numerical errors.

# The `KelvinTemperature` entry

This entry describes the temperature.


* units:
  *SI: K
* type: scalar

## Notes 

This entry has been introduced by compatibility with implantation choices made by the Germinal fuel performance code

# The `MassDensity` entry

This entry describes the mass density.


* units:
  *SI: kg.m^{-3}
* type: scalar

# The `MeanBurnUp_AtPercent` entry

This entry describes the spatial average of the  burn-up in at.%.


* units:
  *SI: at./100
* type: scalar

# The `MeanBurnUp_MWJperTm` entry

This entry describes the spatial average of the  burn-up in MegaWattJour per tons of metals.


* units:
  *SI: MWJ/tm
* type: scalar

# The `MeanIrradiationTemperature` entry

This entry describes The mean temperature  in time over a given domain \(\Omega\).


* units:
  *SI: K
* type: scalar

## Description

This temperature is defined as follows:\[\average{T}\paren{t} =\Frac{1}{t-t_{0}}\Frac{1}{\int_{\Omega}\dtot\,V}\int_{t_{0}}^{t}\paren{\int_{\Omega}T\paren{u,\vec{r}}\,\dtot\,V}\]where \(T\paren{t,\vec{r}}\) is the value of the temperature at time \(t\) and at position \(\vec{r}\).

## Notes 

In pratice, the computation of the spatial integral is done using standard finite element operations and the time integral is performed incrementally using a trapezoidal rule.

# The `MeanTemperature` entry

This entry describes The mean temperature over a given domain \(\Omega\).


* units:
  *SI: K
* type: scalar

## Description

This temperature is defined as follows:\[\average{T}\paren{t}  = \Frac{1}{\int_{\Omega}\dtot\,V}\int_{\Omega}T\paren{t,\vec{r}}\,\dtot\, V\]where \(T\paren{t,\vec{r}}\) is the value of the temperature at time \(t\) and at position \(\vec{r}\).

## Notes 

In pratice, the computation of this integral is done using standard finite element operations.

# The `NeutronFluence` entry

This entry describes the neutron fluence.


* units:
  *SI: n.m^{-2}
* type: scalar

# The `NeutronFlux` entry

This entry describes the neutron flux.


* units:
  *SI: n.m^{-2}.s^{-1}
* type: scalar

# The `NormalStiffness` entry

This entry describes the normal elastic stiffness for a cohesive zone model.


* units:
  *SI: Pa.m^{-1}
* type: scalar

# The `NumberOfMoles` entry

This entry describes the amount of substance.


* units:
  *SI: mol
* type: scalar

# The `OpeningDisplacement` entry

This entry describes opening displacement in cohesive zone models.


* units:
  *SI: m
* type: vector

# The `OrthotropicAxisX1` entry

This entry describes the first coordinate of the vector giving the first axis of orthotropy.


* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisX2` entry

This entry describes the first coordinate of the vector giving the second axis of orthotropy.


* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisY1` entry

This entry describes the second coordinate of the vector giving the first axis of orthotropy.


* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisY2` entry

This entry describes the second coordinate of the vector giving the second axis of orthotropy.


* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisZ1` entry

This entry describes the third coordinate of the vector giving the first axis of orthotropy.


* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisZ2` entry

This entry describes the third coordinate of the vector giving the second axis of orthotropy.


* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `PlasticStrain` entry

This entry describes The plastic strain.


* units:
no units specified
* type: tensor

# The `PlateWidth` entry

This entry describes ??.


* units:
  *SI: m
* type: scalar

# The `PoissonRatio` entry

This entry describes the Poisson ratio of an isotropic material.


* units:
no units specified
* type: scalar

# The `PoissonRatio12` entry

This entry describes the Poisson's coefficient between the first and second directions of orthotropy.


* units:
no units specified
* type: scalar

# The `PoissonRatio13` entry

This entry describes the Poisson's coefficient between the first and third directions of orthotropy.


* units:
no units specified
* type: scalar

# The `PoissonRatio23` entry

This entry describes the Poisson's coefficient between the second and third directions of orthotropy.


* units:
no units specified
* type: scalar

# The `Porosity` entry

This entry describes Porosity of the material.


* units:
no units specified
* type: scalar

# The `PorosityIncreaseDueToInelasticFlow` entry

This entry describes Part of the porosity increase du to inelastic flow.


* units:
no units specified
* type: scalar

# The `PorosityIncreaseDueToNucleation` entry

This entry describes Part of the porosity increase du to nucleation.


* units:
no units specified
* type: scalar

# The `PowerDensity` entry

This entry describes the power density, generally in the current configuration.


* units:
  *SI: W.m^{-3}
* type: scalar

# The `Pressure` entry

This entry describes the pressure of a gaz.


* units:
  *SI: Pa
* type: scalar

# The `PrincipalStress1` entry

This entry describes the first principal stress.


* units:
  *SI: Pa
* type: scalar

# The `PrincipalStress2` entry

This entry describes the third principal stress.


* units:
  *SI: Pa
* type: scalar

# The `PrincipalStress3` entry

This entry describes the third principal stress.


* units:
  *SI: Pa
* type: scalar

# The `SecondAxisSecondMomentArea` entry

This entry describes ??.


* units:
  *SI: m^{4}
* type: scalar

# The `ShearModulus` entry

This entry describes the shear modulus of an isotropic material.


* units:
  *SI: Pa
* type: scalar

# The `ShearModulus12` entry

This entry describes the shear moduls between the first and second directions of orthotropy.


* units:
  *SI: Pa
* type: scalar

# The `ShearModulus13` entry

This entry describes the shear moduls between the first and third directions of orthotropy.


* units:
  *SI: Pa
* type: scalar

# The `ShearModulus23` entry

This entry describes the shear moduls between the second and third directions of orthotropy.


* units:
  *SI: Pa
* type: scalar

# The `SolidSwelling` entry

This entry describes swelling du to solid fission products.


* units:
no units specified
* type: scalar

# The `SpecificHeat` entry

This entry describes the specific heat.


* units:
  *SI: J.kg^{-1}.K^{-1}
* type: scalar

# The `SphericalStress` entry

This entry describes the stress in a spherical frame.


* units:
  *SI: Pa
* type: tensor

# The `Strain` entry

This entry describes the strain tensor.


* units:
no units specified
* type: tensor

# The `StrainMeasure` entry

This entry describes a generic entry for a strain measure (for instance, the Henky strain or the Green-Lagrange strain).


* units:
no units specified
* type: tensor

# The `Stress` entry

This entry describes the stress tensor.


* units:
  *SI: Pa
* type: tensor

# The `Swelling` entry

This entry describes an imposed swelling.


* units:
no units specified
* type: scalar

# The `TangentialStiffness` entry

This entry describes the tangential elastic stiffness for a cohesive zone model.


* units:
  *SI: Pa.m^{-1}
* type: scalar

# The `Temperature` entry

This entry describes the temperature.


* units:
  *SI: K
* type: scalar

# The `TemperatureGradient` entry

This entry describes the temperature gradient, generally in the current configuration.


* units:
  *SI: T.m^{-1}
* type: vector

# The `ThermalConductivity` entry

This entry describes the thermal conductivity of an isotropic material.


* units:
  *SI: W.m^{-1}
* type: scalar

# The `ThermalConductivity1` entry

This entry describes the thermal conductivity of an orthotropic material along the first axis of orthotropy.


* units:
  *SI: W.m^{-1}
* type: scalar

# The `ThermalConductivity2` entry

This entry describes the thermal conductivity of an orthotropic material along the second axis of orthotropy.


* units:
  *SI: W.m^{-1}
* type: scalar

# The `ThermalConductivity3` entry

This entry describes the thermal conductivity of an orthotropic material along the third axis of orthotropy.


* units:
  *SI: W.m^{-1}
* type: scalar

# The `ThermalExpansion` entry

This entry describes the mean thermal expansion coefficient.


* units:
  *SI: K^{-1}
* type: scalar

## Notes 

This entry shall have be named MeanThermalExpansionCoefficient.

# The `ThermalExpansion1` entry

This entry describes the mean thermal expansion coefficient along the first orthotropy direction.


* units:
  *SI: K^{-1}
* type: scalar

## Notes 

This entry shall have be named MeanThermalExpansionCoefficient1.

# The `ThermalExpansion2` entry

This entry describes the mean thermal expansion coefficient along the second orthotropy direction.


* units:
  *SI: K^{-1}
* type: scalar

## Notes 

This entry shall have be named MeanThermalExpansionCoefficient2.

# The `ThermalExpansion3` entry

This entry describes the mean thermal expansion coefficient along the third orthotropy direction.


* units:
  *SI: K^{-1}
* type: scalar

## Notes 

This entry shall have be named MeanThermalExpansionCoefficient3.

# The `TorsionConstant` entry

This entry describes ??.


* units:
  *SI: ??
* type: scalar

# The `TrescaStress` entry

This entry describes the Tresca equivalent stress.


* units:
  *SI: Pa
* type: scalar

# The `UltimateTensileStrength` entry

This entry describes the maximum stress that a material can withstand while being stretched or pulled before breaking.

* alternative names: UltimateTensileStress, UltimateTensileStrength
* units:
  *SI: Pa
* type: scalar

# The `ViscoplasticStrain` entry

This entry describes The viscoplatic strain.


* units:
no units specified
* type: tensor

# The `VolumetricStrain` entry

This entry describes the volumetric strain, defined as the trace of the strain tensor.


* units:
no units specified
* type: scalar

# The `VonMisesStress` entry

This entry describes the von Mises equivalent stress.


* units:
  *SI: Pa
* type: scalar

# The `YieldStrength` entry

This entry describes the stress corresponding to the yield point at which the material begins to deform plastically.

* alternative names: YieldStress, YieldStrength
* units:
  *SI: Pa
* type: scalar

## Notes 

When this limit is difficult to identify experimentally, the offset yield point is taken as the stress at which 0.2% plastic deformation occurs

# The `YoungModulus` entry

This entry describes the Young's modulus of an isotropic material.


* units:
  *SI: Pa
* type: scalar

# The `YoungModulus1` entry

This entry describes the Young's modulus of an isotropic material along the first direction of orthotropy.


* units:
  *SI: Pa
* type: scalar

# The `YoungModulus2` entry

This entry describes the Young's modulus of an isotropic material along the second direction of orthotropy.


* units:
  *SI: Pa
* type: scalar

# The `YoungModulus3` entry

This entry describes the Young's modulus of an isotropic material along the third direction of orthotropy.


* units:
  *SI: Pa
* type: scalar
<!-- Local IspellDict: english -->
