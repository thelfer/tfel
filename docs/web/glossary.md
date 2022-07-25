---
title: Implementation of a perfect plastic behaviour using the Hosford equivalent stress
author: Helfer Thomas, Maxence Wangermez Bernaud Stéphane
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

This entry describes la composante axiale du gradient de la transformation (cette grandeur n'a de sens que pour les calculs en contraintes planes).

* alternative names: AxialDeformationGradient
* units:
no units specified
* type: scalar

# The `AxialGrowth` entry

This entry describes axial growth under irradiation.

* alternative names: AxialGrowth
* units:
no units specified
* type: scalar

# The `AxialStrain` entry

This entry describes la déformation axiale (cette grandeur n'a de sens que pour les calculs en contraintes planes).

* alternative names: AxialStrain
* units:
no units specified
* type: scalar

# The `AxialStress` entry

This entry describes la contrainte axiale.

* alternative names: AxialStress
* units:
no units specified
* type: scalar

## Notes 

Cette grandeur est utilisée dans les calculs 1Daxisymétrique en contraintes planes généralisées

# The `B10BurnUp` entry

This entry describes le taux d'usure en \(\mbox{}^{10}B\).

* alternative names: B10BurnUp
* units:
  *SI: m^{-3}
* type: scalar

## Description

Ce nombre décrit le nombre d'atomes de \(\mbox{}^{10}B\) consommé par unité de volume au cours de l'irradiation.

# The `Broken` entry

This entry describes a material failure indicator.

* alternative names: Broken
* units:
no units specified
* type: scalar

# The `BulkModulus` entry

This entry describes the bulk modulus of an isotropic material.

* alternative names: BulkModulus
* units:
  *SI: Pa
* type: scalar

# The `BurnUp_AtPercent` entry

This entry describes le taux de combustion en atome pour cent.

* alternative names: BurnUp (at.%)
* units:
  *SI: at./100
* type: scalar

# The `BurnUp_MWJperTm` entry

This entry describes le taux de combustion en MegaWattJour par tonne (métal).

* alternative names: BurnUp (MWJ/tm)
* units:
  *SI: MWJ/tm
* type: scalar

# The `CohesiveForce` entry

This entry describes cohesive force for cohesize zone models.

* alternative names: CohesiveForce
* units:
  *SI: Newton
* type: vector

# The `ConvectiveHeatTransferCoefficient` entry

This entry describes the heat transfer coefficient by convection.

* alternative names: ConvectiveHeatTransferCoefficient
* units:
  *SI: W.m^{-2}.K^{-1}
* type: scalar

# The `CrossSectionArea` entry

This entry describes ??.

* alternative names: CrossSectionArea
* units:
  *SI: m^{2}
* type: scalar

# The `CylindricalStress` entry

This entry describes les contraintes exprimées dans le repère cylindrique.

* alternative names: CylindricalStress
* units:
  *SI: Pa
* type: tensor

# The `Damage` entry

This entry describes l'endommagement d'un matériau.

* alternative names: Damage
* units:
no units specified
* type: scalar

# The `DeformationGradient` entry

This entry describes gradient of the transformation.

* alternative names: DeformationGradient
* units:
no units specified
* type: tensor

# The `Displacement` entry

This entry describes le déplacement.

* alternative names: Displacement
* units:
  *SI: m
* type: vector

# The `DualStress` entry

This entry describes the dual stress of the strain measure.

* alternative names: DualStress
* units:
  *SI: stress
* type: tensor

# The `ElasticStrain` entry

This entry describes The elastic strain.

* alternative names: ElasticStrain
* units:
no units specified
* type: tensor

# The `Emissivity` entry

This entry describes the emissivity of the surface of a material is its effectiveness in emitting energy as thermal radiation.

* alternative names: Emissivity
* units:
  *SI: kg.m.T^{−3}
* type: scalar

# The `EquivalentPlasticStrain` entry

This entry describes The equivalent plastic strain.

* alternative names: EquivalentPlasticStrain
* units:
no units specified
* type: scalar

# The `EquivalentStrain` entry

This entry describes la somme des déformations plastiques ou viscoplastiques équivalentes.

* alternative names: EquivalentStrain
* units:
no units specified
* type: scalar

## Notes 

Cette quantité n'a pas de sens physique directe.

# The `EquivalentViscoplasticStrain` entry

This entry describes The equivalent viscoplastic strain.

* alternative names: EquivalentViscoplasticStrain
* units:
no units specified
* type: scalar

# The `FastNeutronFluence_01MeV` entry

This entry describes la fluence rapide.

* alternative names: FastNeutronFluence (>0.1 MeV)
* units:
  *SI: n.m^{-2}
* type: scalar

# The `FastNeutronFluence_1MeV` entry

This entry describes la fluence rapide.

* alternative names: FastNeutronFluence (>1 MeV)
* units:
  *SI: n.m^{-2}
* type: scalar

# The `FastNeutronFlux_01MeV` entry

This entry describes le flux de neutron rapide.

* alternative names: FastNeutronFlux (>0.1 MeV)
* units:
  *SI: n.m^{-2}.s^{-1}
* type: scalar

# The `FastNeutronFlux_1MeV` entry

This entry describes le flux de neutron rapide.

* alternative names: FastNeutronFlux (>1 MeV)
* units:
  *SI: n.m^{-2}.s^{-1}
* type: scalar

# The `FirstAxisSecondMomentArea` entry

This entry describes ??.

* alternative names: FirstAxisSecondMomentArea
* units:
  *SI: m^{4}
* type: scalar

# The `FirstLameCoefficient` entry

This entry describes the first Lamé's coefficient of an isotropic material.

* alternative names: FirstLameCoefficient
* units:
  *SI: Pa
* type: scalar

# The `FissionDensity` entry

This entry describes la densité de fission.

* alternative names: FissionDensity
* units:
  *SI: m^{-3}
* type: scalar

# The `GaseousSwelling` entry

This entry describes un gonflement imposé dû à des produits de fission gazeux.

* alternative names: GaseousSwelling
* units:
no units specified
* type: scalar

## Notes 

L'utilisation de ce nom pour imposer un gonflement n'est pas obligatoire

# The `GrainSize` entry

This entry describes la taille de grain.

* alternative names: GrainSize
* units:
  *SI: m
* type: scalar

# The `HeatFlux` entry

This entry describes the heat flux, generally in the current configuration..

* alternative names: HeatFlux
* units:
  *SI: J.m^{-2}.s^{-1}
* type: vector

# The `HeatTransferCoefficient` entry

This entry describes the heat transfer coefficient is the proportionality constant between the heat flux and the temperature difference.

* alternative names: HeatTransferCoefficient
* units:
  *SI: W.m^{-2}.K^{-1}
* type: scalar

# The `HillStress` entry

This entry describes la contrainte équivalente au sens de Hill.

* alternative names: HillStress
* units:
  *SI: Pa
* type: tensor

# The `HydrostaticPressure` entry

This entry describes la pression hydrostatique (égale au tiers de la trace du tenseur des contraintes).

* alternative names: HydrostaticPressure
* units:
  *SI: Pa
* type: tensor

# The `IrradiationDamage` entry

This entry describes le dommage due à l'irradiation, mesuré par le nombre de déplacement moyen de chaque atome (dpa).

* alternative names: IrradiationDamage
* units:
  *SI: dpa
* type: scalar

# The `IrradiationInducedSwelling` entry

This entry describes un gonflement imposé  dû à des dégâts d'irradiation.

* alternative names: IrradiationInducedSwelling
* units:
no units specified
* type: scalar

## Notes 

L'utilisation de ce nom pour imposer un gonflement n'est pas obligatoire

# The `IrradiationSwelling` entry

This entry describes un gonflement imposé  dû à des dégâts d'irradiation.

* alternative names: IrradiationSwelling
* units:
no units specified
* type: scalar

## Notes 

L'utilisation de ce nom pour imposer un gonflement n'est pas obligatoire

# The `IrradiationTemperature` entry

This entry describes the mean temperature (in time) of the temperature during the irradiation.

* alternative names: IrradiationTemperature
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

* alternative names: KelvinTemperature
* units:
  *SI: K
* type: scalar

## Notes 

This entry has been introduced by compatibility with implantation choices made by the Germinal fuel performance code

# The `MassDensity` entry

This entry describes the mass density.

* alternative names: MassDensity
* units:
  *SI: kg.m^{-3}
* type: scalar

# The `MeanBurnUp_AtPercent` entry

This entry describes le taux de combustion en atome pour cent moyen (au sens spatial) pour un matériau donné.

* alternative names: MeanBurnUp (at.%)
* units:
  *SI: at./100
* type: scalar

# The `MeanBurnUp_MWJperTm` entry

This entry describes le taux de combustion en MegaWattJour par tonne (métal) moyen (au sens spatial) pour un matériau donné.

* alternative names: MeanBurnUp (MWJ/tm)
* units:
  *SI: MWJ/tm
* type: scalar

# The `MeanIrradiationTemperature` entry

This entry describes The mean temperature  in time over a given domain \(\Omega\).

* alternative names: MeanIrradiationTemperature
* units:
  *SI: K
* type: scalar

## Description

This temperature is defined as follows:\[\average{T}\paren{t} =\Frac{1}{t-t_{0}}\Frac{1}{\int_{\Omega}\dtot\,V}\int_{t_{0}}^{t}\paren{\int_{\Omega}T\paren{u,\vec{r}}\,\dtot\,V}\]where \(T\paren{t,\vec{r}}\) is the value of the temperature at time \(t\) and at position \(\vec{r}\).

## Notes 

In pratice, the computation of the spatial integral is done using standard finite element operations and the time integral is performed incrementally using a trapezoidal rule.

# The `MeanTemperature` entry

This entry describes The mean temperature over a given domain \(\Omega\).

* alternative names: MeanTemperature
* units:
  *SI: K
* type: scalar

## Description

This temperature is defined as follows:\[\average{T}\paren{t}  = \Frac{1}{\int_{\Omega}\dtot\,V}\int_{\Omega}T\paren{t,\vec{r}}\,\dtot\, V\]where \(T\paren{t,\vec{r}}\) is the value of the temperature at time \(t\) and at position \(\vec{r}\).

## Notes 

In pratice, the computation of this integral is done using standard finite element operations.

# The `NeutronFluence` entry

This entry describes la fluence.

* alternative names: NeutronFluence
* units:
  *SI: n.m^{-2}
* type: scalar

# The `NeutronFlux` entry

This entry describes le flux de neutron.

* alternative names: NeutronFlux
* units:
  *SI: n.m^{-2}.s^{-1}
* type: scalar

# The `NormalStiffness` entry

This entry describes la raideur normale pour un modèle de zone cohésive.

* alternative names: NormalStiffness
* units:
  *SI: Pa.m^{-1}
* type: scalar

# The `NumberOfMoles` entry

This entry describes le nombre de moles d'une substance.

* alternative names: NumberOfMoles
* units:
  *SI: mol
* type: scalar

# The `OpeningDisplacement` entry

This entry describes opening displacement in cohesive zone models.

* alternative names: OpeningDisplacement
* units:
  *SI: m
* type: vector

# The `OrthotropicAxisX1` entry

This entry describes the first coordinate of the vector giving the first axis of orthotropy.

* alternative names: OrthotropicAxisX1
* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisX2` entry

This entry describes the first coordinate of the vector giving the second axis of orthotropy.

* alternative names: OrthotropicAxisX2
* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisY1` entry

This entry describes the second coordinate of the vector giving the first axis of orthotropy.

* alternative names: OrthotropicAxisY1
* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisY2` entry

This entry describes the second coordinate of the vector giving the second axis of orthotropy.

* alternative names: OrthotropicAxisY2
* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisZ1` entry

This entry describes the third coordinate of the vector giving the first axis of orthotropy.

* alternative names: OrthotropicAxisZ1
* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `OrthotropicAxisZ2` entry

This entry describes the third coordinate of the vector giving the second axis of orthotropy.

* alternative names: OrthotropicAxisZ2
* units:
no units specified
* type: scalar

## Notes 

This quantity is defined internally by the Licos fuel performance code

# The `PlasticStrain` entry

This entry describes The plastic strain.

* alternative names: PlasticStrain
* units:
no units specified
* type: tensor

# The `PlateWidth` entry

This entry describes ??.

* alternative names: PlateWidth
* units:
  *SI: m
* type: scalar

# The `PoissonRatio` entry

This entry describes the Poisson ratio of an isotropic material.

* alternative names: PoissonRatio
* units:
no units specified
* type: scalar

# The `PoissonRatio12` entry

This entry describes le coefficient de Poisson d'un matériau orthotrope relatif aux première et deuxième directions d'orthotropie.

* alternative names: PoissonRatio12
* units:
no units specified
* type: scalar

# The `PoissonRatio13` entry

This entry describes le coefficient de Poisson d'un matériau orthotrope relatif aux première et troisième directions d'orthotropie.

* alternative names: PoissonRatio13
* units:
no units specified
* type: scalar

# The `PoissonRatio23` entry

This entry describes le coefficient de Poisson d'un matériau orthotrope relatif aux deuxième et troisième directions d'orthotropie.

* alternative names: PoissonRatio23
* units:
no units specified
* type: scalar

# The `Porosity` entry

This entry describes Porosity of the material.

* alternative names: Porosity
* units:
no units specified
* type: scalar

# The `PorosityIncreaseDueToInelasticFlow` entry

This entry describes Part of the porosity increase du to inelastic flow.

* alternative names: PorosityIncreaseDueToInelasticFlow
* units:
no units specified
* type: scalar

# The `PorosityIncreaseDueToNucleation` entry

This entry describes Part of the porosity increase du to nucleation.

* alternative names: PorosityIncreaseDueToNucleation
* units:
no units specified
* type: scalar

# The `PowerDensity` entry

This entry describes the power density, generally in the current configuration.

* alternative names: PowerDensity
* units:
  *SI: W.m^{-3}
* type: scalar

# The `Pressure` entry

This entry describes la pression d'un gaz.

* alternative names: Pressure
* units:
  *SI: Pa
* type: scalar

# The `PrincipalStress1` entry

This entry describes la première contrainte principale.

* alternative names: PrincipalStress1
* units:
  *SI: Pa
* type: scalar

## Notes 

Son sens est précisé dans la notice de l'opérateur ['PRIN'](http://www-cast3m.cea.fr/index.php?page=notices&notice=PRIN) de [Cast3M](http://www-cast3m.cea.fr)

# The `PrincipalStress2` entry

This entry describes la deuxième contrainte principale.

* alternative names: PrincipalStress2
* units:
  *SI: Pa
* type: scalar

## Notes 

Son sens est précisé dans la notice de l'opérateur ['PRIN'](http://www-cast3m.cea.fr/index.php?page=notices&notice=PRIN) de [Cast3M](http://www-cast3m.cea.fr)

# The `PrincipalStress3` entry

This entry describes la troisième contrainte principale.

* alternative names: PrincipalStress3
* units:
  *SI: Pa
* type: scalar

## Notes 

Son sens est précisé dans la notice de l'opérateur ['PRIN'](http://www-cast3m.cea.fr/index.php?page=notices&notice=PRIN) de [Cast3M](http://www-cast3m.cea.fr)

# The `SecondAxisSecondMomentArea` entry

This entry describes ??.

* alternative names: SecondAxisSecondMomentArea
* units:
  *SI: m^{4}
* type: scalar

# The `ShearModulus` entry

This entry describes the shear modulus of an isotropic material.

* alternative names: ShearModulus
* units:
  *SI: Pa
* type: scalar

# The `ShearModulus12` entry

This entry describes le module de cisaillement d'un matériau orthotrope relatif aux première et deuxième directions d'orthotropie.

* alternative names: ShearModulus12
* units:
  *SI: Pa
* type: scalar

# The `ShearModulus13` entry

This entry describes le module de cisaillement d'un matériau orthotrope relatif aux première et troisième directions d'orthotropie.

* alternative names: ShearModulus13
* units:
  *SI: Pa
* type: scalar

# The `ShearModulus23` entry

This entry describes le module de cisaillement d'un matériau orthotrope relatif aux deuxième et troisième directions d'orthotropie.

* alternative names: ShearModulus23
* units:
  *SI: Pa
* type: scalar

# The `SolidSwelling` entry

This entry describes un gonflement imposé dû à des produits de fission solides.

* alternative names: SolidSwelling
* units:
no units specified
* type: scalar

## Notes 

L'utilisation de ce nom pour imposer un gonflement n'est pas obligatoire

# The `SpecificHeat` entry

This entry describes the specific heat.

* alternative names: SpecificHeat
* units:
  *SI: J.kg^{-1}.K^{-1}
* type: scalar

# The `SphericalStress` entry

This entry describes les contraintes exprimées dans le repère sphérique.

* alternative names: SphericalStress
* units:
  *SI: Pa
* type: tensor

# The `Strain` entry

This entry describes la déformation.

* alternative names: Strain
* units:
no units specified
* type: tensor

# The `StrainMeasure` entry

This entry describes a generic entry for a strain measure (for instance, the Henky strain or the Green-Lagrange strain).

* alternative names: StrainMeasure
* units:
no units specified
* type: tensor

# The `Stress` entry

This entry describes les contraintes.

* alternative names: Stress
* units:
  *SI: Pa
* type: tensor

# The `Swelling` entry

This entry describes un gonflement imposé.

* alternative names: Swelling
* units:
no units specified
* type: scalar

## Notes 

l'utilisation de ce nom pour imposer un gonflement n'est pasobligatoire.

# The `TangentialStiffness` entry

This entry describes la raideur tangentielle pour un modèle de zone cohésive.

* alternative names: TangentialStiffness
* units:
  *SI: Pa.m^{-1}
* type: scalar

# The `Temperature` entry

This entry describes the temperature.

* alternative names: Temperature
* units:
  *SI: K
* type: scalar

# The `TemperatureGradient` entry

This entry describes the temperature gradient, generally in the current configuration.

* alternative names: TemperatureGradient
* units:
  *SI: T.m^{-1}
* type: vector

# The `ThermalConductivity` entry

This entry describes the thermal conductivity of an isotropic material.

* alternative names: ThermalConductivity
* units:
  *SI: W.m^{-1}
* type: scalar

# The `ThermalConductivity1` entry

This entry describes the thermal conductivity of an orthotropic material along the first axis of orthotropy.

* alternative names: ThermalConductivity1
* units:
  *SI: W.m^{-1}
* type: scalar

# The `ThermalConductivity2` entry

This entry describes the thermal conductivity of an orthotropic material along the second axis of orthotropy.

* alternative names: ThermalConductivity2
* units:
  *SI: W.m^{-1}
* type: scalar

# The `ThermalConductivity3` entry

This entry describes the thermal conductivity of an orthotropic material along the third axis of orthotropy.

* alternative names: ThermalConductivity3
* units:
  *SI: W.m^{-1}
* type: scalar

# The `ThermalExpansion` entry

This entry describes le coefficient de dilatation linéique d'un matériau isotrope.

* alternative names: ThermalExpansion
* units:
  *SI: K^{-1}
* type: scalar

# The `ThermalExpansion1` entry

This entry describes le coefficient de dilatation linéique d'un matériau orthotrope suivant la première direction d'orthotropie.

* alternative names: ThermalExpansion1
* units:
  *SI: K^{-1}
* type: scalar

# The `ThermalExpansion2` entry

This entry describes le coefficient de dilatation linéique d'un matériau orthotrope suivant la deuxième direction d'orthotropie.

* alternative names: ThermalExpansion2
* units:
  *SI: K^{-1}
* type: scalar

# The `ThermalExpansion3` entry

This entry describes le coefficient de dilatation linéique d'un matériau orthotrope suivant la troisième direction d'orthotropie.

* alternative names: ThermalExpansion3
* units:
  *SI: K^{-1}
* type: scalar

# The `TorsionConstant` entry

This entry describes ??.

* alternative names: TorsionConstant
* units:
  *SI: ??
* type: scalar

# The `TrescaStress` entry

This entry describes la contrainte équivalente au sens de Tresca.

* alternative names: TrescaStress
* units:
  *SI: Pa
* type: scalar

# The `UltimateTensileStrength` entry

This entry describes la valeur maximale de la contrainte qu'un materiau peut supporter.

* alternative names: UltimateTensileStress, UltimateTensileStrength
* units:
  *SI: Pa
* type: scalar

# The `ViscoplasticStrain` entry

This entry describes The viscoplatic strain.

* alternative names: ViscoplasticStrain
* units:
no units specified
* type: tensor

# The `VolumetricStrain` entry

This entry describes la dilatation volumique.

* alternative names: VolumetricStrain
* units:
no units specified
* type: scalar

# The `VonMisesStress` entry

This entry describes la contrainte équivalente au sens de Von Mises.

* alternative names: VonMisesStress
* units:
  *SI: Pa
* type: scalar

# The `YieldStrength` entry

This entry describes la limite d'élasticité.

* alternative names: YieldStress, YieldStrength
* units:
  *SI: Pa
* type: scalar

# The `YoungModulus` entry

This entry describes the Young's modulus of an isotropic material.

* alternative names: YoungModulus
* units:
  *SI: Pa
* type: scalar

# The `YoungModulus1` entry

This entry describes the Young's modulus of an isotropic material along the first direction of orthotropy.

* alternative names: YoungModulus1
* units:
  *SI: Pa
* type: scalar

# The `YoungModulus2` entry

This entry describes the Young's modulus of an isotropic material along the second direction of orthotropy.

* alternative names: YoungModulus2
* units:
  *SI: Pa
* type: scalar

# The `YoungModulus3` entry

This entry describes the Young's modulus of an isotropic material along the third direction of orthotropy.

* alternative names: YoungModulus3
* units:
  *SI: Pa
* type: scalar
<!-- Local IspellDict: english -->
