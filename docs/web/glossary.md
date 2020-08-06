% TFEL Glossary description
% Helfer Thomas; Bernaud Stéphane

\newcommand{\dtot}{\mathrm{d}}
\newcommand{\paren}[1]{{\displaystyle \left(#1\right)}}
\newcommand{\average}[1]{{\displaystyle \left\langle#1\right\rangle}}
\newcommand{\Frac}[2]{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}

# L'entrée AxialDeformationGradient

Cette entrée décrit la composante axiale du gradient de la transformation (cette grandeur n'a de sens que pour les calculs en contraintes planes).

* noms : AxialDeformationGradient
* unité: sans unité
* type: scalaire 

# L'entrée AxialGrowth

Cette entrée décrit axial growth under irradiation.

* noms : AxialGrowth
* unité: sans unité
* type: scalaire 

# L'entrée AxialStrain

Cette entrée décrit la déformation axiale (cette grandeur n'a de sens que pour les calculs en contraintes planes).

* noms : AxialStrain
* unité: sans unité
* type: scalaire 

# L'entrée AxialStress

Cette entrée décrit la contrainte axiale.

* noms : AxialStress
* unité: sans unité
* type: scalaire 

## Notes 

Cette grandeur est utilisée dans les calculs 1Daxisymétrique en contraintes planes généralisées

# L'entrée B10BurnUp

Cette entrée décrit le taux d'usure en \(\mbox{}^{10}B\).

* noms : B10BurnUp
* unité: \(m^{-3}\)
* type: scalaire 

## Description

Ce nombre décrit le nombre d'atomes de \(\mbox{}^{10}B\) consommé par unité de volume au cours de l'irradiation.

# L'entrée Broken

Cette entrée décrit material failure indicator.

* noms : Broken
* unité: \(none\)
* type: scalaire 

# L'entrée BulkModulus

Cette entrée décrit The bulk modulus of an isotropic material.

* noms : BulkModulus
* unité: \(Pa\)
* type: scalaire 

# L'entrée BurnUp_AtPercent

Cette entrée décrit le taux de combustion en atome pour cent.

* noms : BurnUp (at.%)
* unité: \(at./100\)
* type: scalaire 

# L'entrée BurnUp_MWJperTm

Cette entrée décrit le taux de combustion en MegaWattJour par tonne (métal).

* noms : BurnUp (MWJ/tm)
* unité: \(MWJ/tm\)
* type: scalaire 

# L'entrée CohesiveForce

Cette entrée décrit cohesive force for cohesize zone models.

* noms : CohesiveForce
* unité: \(Newton\)
* type: vecteur 

# L'entrée ConvectiveHeatTransferCoefficient

Cette entrée décrit le coefficient d'échange par convection.

* noms : ConvectiveHeatTransferCoefficient
* unité: \(W.m^{-2}.K^{-1}\)
* type: scalaire 

# L'entrée CrossSectionArea

Cette entrée décrit ??.

* noms : CrossSectionArea
* unité: \(??\)
* type: scalaire 

# L'entrée CylindricalStress

Cette entrée décrit les contraintes exprimées dans le repère cylindrique.

* noms : CylindricalStress
* unité: \(Pa\)
* type: tenseur symétrique

# L'entrée Damage

Cette entrée décrit l'endommagement d'un matériau.

* noms : Damage
* unité: sans unité
* type: scalaire 

# L'entrée DeformationGradient

Cette entrée décrit gradient of the transformation.

* noms : DeformationGradient
* unité: sans unité
* type: tenseur symétrique

# L'entrée Displacement

Cette entrée décrit le déplacement.

* noms : Displacement
* unité: \(m\)
* type: vecteur 

# L'entrée ElasticStrain

Cette entrée décrit The elastic strain.

* noms : ElasticStrain
* unité: sans unité
* type: tenseur symétrique

# L'entrée Emissivity

Cette entrée décrit Emissivity of the surface of a material is its effectiveness in emitting energy as thermal radiation.

* noms : Emissivity
* unité: \(kg.m.T^{−3}\)
* type: scalaire 

# L'entrée EquivalentPlasticStrain

Cette entrée décrit The equivalent plastic strain.

* noms : EquivalentPlasticStrain
* unité: sans unité
* type: scalaire 

# L'entrée EquivalentStrain

Cette entrée décrit la somme des déformations plastiques ou viscoplastiques équivalentes.

* noms : EquivalentStrain
* unité: sans unité
* type: scalaire 

## Notes 

Cette quantité n'a pas de sens physique directe.

# L'entrée EquivalentViscoplasticStrain

Cette entrée décrit The equivalent viscoplastic strain.

* noms : EquivalentViscoplasticStrain
* unité: sans unité
* type: scalaire 

# L'entrée FastNeutronFluence_01MeV

Cette entrée décrit la fluence rapide.

* noms : FastNeutronFluence (>0.1 MeV)
* unité: \(n.m^{-2}\)
* type: scalaire 

# L'entrée FastNeutronFluence_1MeV

Cette entrée décrit la fluence rapide.

* noms : FastNeutronFluence (>1 MeV)
* unité: \(n.m^{-2}\)
* type: scalaire 

# L'entrée FastNeutronFlux_01MeV

Cette entrée décrit le flux de neutron rapide.

* noms : FastNeutronFlux (>0.1 MeV)
* unité: \(n.m^{-2}.s^{-1}\)
* type: scalaire 

# L'entrée FastNeutronFlux_1MeV

Cette entrée décrit le flux de neutron rapide.

* noms : FastNeutronFlux (>1 MeV)
* unité: \(n.m^{-2}.s^{-1}\)
* type: scalaire 

# L'entrée FirstAxisSecondMomentArea

Cette entrée décrit ??.

* noms : FirstAxisSecondMomentArea
* unité: \(??\)
* type: scalaire 

# L'entrée FirstLameCoefficient

Cette entrée décrit First Lamé coefficient of an isotropic material.

* noms : FirstLameCoefficient
* unité: \(Pa\)
* type: scalaire 

# L'entrée FissionDensity

Cette entrée décrit la densité de fission.

* noms : FissionDensity
* unité: \(m^{-3}\)
* type: scalaire 

# L'entrée GaseousSwelling

Cette entrée décrit un gonflement imposé dû à des produits de fission gazeux.

* noms : GaseousSwelling
* unité: sans unité
* type: scalaire 

## Notes 

L'utilisation de ce nom pour imposer un gonflement n'est pas obligatoire

# L'entrée GrainSize

Cette entrée décrit la taille de grain.

* noms : GrainSize
* unité: \(m\)
* type: scalaire 

# L'entrée HeatFlux

Cette entrée décrit le flux de chaleur..

* noms : HeatFlux
* unité: \(J.m^{-2}.s^{-1}\)
* type: vecteur 

# L'entrée HeatTransferCoefficient

Cette entrée décrit The heat transfer coefficient is the proportionality constant between the heat flux and the temperature difference.

* noms : HeatTransferCoefficient
* unité: \(W.m^{-2}.K^{-1}\)
* type: scalaire 

# L'entrée HillStress

Cette entrée décrit la contrainte équivalente au sens de Hill.

* noms : HillStress
* unité: \(Pa\)
* type: tenseur symétrique

# L'entrée HydrostaticPressure

Cette entrée décrit la pression hydrostatique (égale au tiers de la trace du tenseur des contraintes).

* noms : HydrostaticPressure
* unité: \(Pa\)
* type: tenseur symétrique

# L'entrée IrradiationDamage

Cette entrée décrit le dommage due à l'irradiation, mesuré par le nombre de déplacement moyen de chaque atome (dpa).

* noms : IrradiationDamage
* unité: \(dpa\)
* type: scalaire 

# L'entrée IrradiationInducedSwelling

Cette entrée décrit un gonflement imposé  dû à des dégâts d'irradiation.

* noms : IrradiationInducedSwelling
* unité: sans unité
* type: scalaire 

## Notes 

L'utilisation de ce nom pour imposer un gonflement n'est pas obligatoire

# L'entrée IrradiationSwelling

Cette entrée décrit un gonflement imposé  dû à des dégâts d'irradiation.

* noms : IrradiationSwelling
* unité: sans unité
* type: scalaire 

## Notes 

L'utilisation de ce nom pour imposer un gonflement n'est pas obligatoire

# L'entrée IrradiationTemperature

Cette entrée décrit la température moyenne (dans le temps) au cours de l'irradiation.

* noms : IrradiationTemperature
* unité: \(K\)
* type: scalaire 

## Description

Cette température est définie ainsi :
\[
\average{T}\paren{t,\vec{r}}  = \Frac{1}{t-t_{0}}\int_{t_{0}}^{t}T\paren{u,\vec{r}}\,\dtot\, u
\]
où

* \(T\paren{t,\vec{r}}\) est la valeur à un instant \(t\) de la température au point \(\vec{r}\) ;
* \(t_{0}\) est la date du début de calcul ;
* \(t\) est la date courante.

En pratique, l'intégrale temporelle est évaluée de manière incrémentale ainsi :
\[
\average{T}\paren{t+dt,\vec{r}} \approx \Frac{1}{t+dt-t_{0}}\left[\paren{t-t_{0}}\,\average{T}\paren{t,\vec{r}}+\Frac{dt}{2}\left[T\paren{t,\vec{r}}+T\paren{t+dt,\vec{r}}\right]\right]
\]

## Notes 

Ce mode de calcul peut conduire à de légères erreurs numériques

# L'entrée KelvinTemperature

Cette entrée décrit The temperature.

* noms : KelvinTemperature
* unité: \(K\)
* type: scalaire 

## Notes 

Cette entrée a été rajoutée par compatibilité avec les choix d'implantation des lois de comportements mécaniques utilisés par le code Germinal

# L'entrée MassDensity

Cette entrée décrit The mass density.

* noms : MassDensity
* unité: \(kg.m^{-3}\)
* type: scalaire 

# L'entrée MeanBurnUp_AtPercent

Cette entrée décrit le taux de combustion en atome pour cent moyen (au sens spatial) pour un matériau donné.

* noms : MeanBurnUp (at.%)
* unité: \(at./100\)
* type: scalaire 

# L'entrée MeanBurnUp_MWJperTm

Cette entrée décrit le taux de combustion en MegaWattJour par tonne (métal) moyen (au sens spatial) pour un matériau donné.

* noms : MeanBurnUp (MWJ/tm)
* unité: \(MWJ/tm\)
* type: scalaire 

# L'entrée MeanIrradiationTemperature

Cette entrée décrit la température moyenne dans le temps et dans un domaine \(\Omega\) donné.

* noms : MeanIrradiationTemperature
* unité: \(K\)
* type: scalaire 

## Description

Cette température est définie ainsi :\[\average{T}\paren{t} =\Frac{1}{t-t_{0}}\Frac{1}{\int_{\Omega}\dtot\,V}\int_{t_{0}}^{t}\paren{\int_{\Omega}T\paren{u,\vec{r}}\,\dtot\,V}\]où \(T\paren{t,\vec{r}}\) est la valeur à un instant \(t\) de la température au point \(\vec{r}\).

## Notes 

En pratique, l'intégrale spatiale est calculée à l'aide de la discrétisation par éléments finis et l'intégrale temporelle est calculée de manière incrémentale.

# L'entrée MeanTemperature

Cette entrée décrit la température moyenne dans un domaine \(\Omega\) donné. .

* noms : MeanTemperature
* unité: \(K\)
* type: scalaire 

## Description

Cette température est définie ainsi :\[\average{T}\paren{t}  = \Frac{1}{\int_{\Omega}\dtot\,V}\int_{\Omega}T\paren{t,\vec{r}}\,\dtot\, V\]où \(T\paren{t,\vec{r}}\) est la valeur à un instant \(t\) de latempérature au point \(\vec{r}\).

## Notes 

En pratique, l'intégrale spatiale est calculée à l'aide de la discrétisation par éléments finis.

# L'entrée NeutronFluence

Cette entrée décrit la fluence.

* noms : NeutronFluence
* unité: \(n.m^{-2}\)
* type: scalaire 

# L'entrée NeutronFlux

Cette entrée décrit le flux de neutron.

* noms : NeutronFlux
* unité: \(n.m^{-2}.s^{-1}\)
* type: scalaire 

# L'entrée NormalStiffness

Cette entrée décrit la raideur normale pour un modèle de zone cohésive.

* noms : NormalStiffness
* unité: \(Pa.m^{-1}\)
* type: scalaire 

# L'entrée NumberOfMoles

Cette entrée décrit le nombre de moles d'une substance.

* noms : NumberOfMoles
* unité: \(mol\)
* type: scalaire 

# L'entrée OpeningDisplacement

Cette entrée décrit opening displacement in cohesive zone models.

* noms : OpeningDisplacement
* unité: \(m\)
* type: vecteur 

# L'entrée OrthotropicAxisX1

Cette entrée décrit la première coordonnée du premier axe d'orthotropie.

* noms : OrthotropicAxisX1
* unité: sans unité
* type: scalaire 

## Description

Cette quantité est automatiquementcalculée par Licos à partir de la définition du matériau

# L'entrée OrthotropicAxisX2

Cette entrée décrit la première coordonnée du second axe d'orthotropie.

* noms : OrthotropicAxisX2
* unité: sans unité
* type: scalaire 

## Description

Cette quantité est automatiquementcalculée par Licos à partir de la définition du matériau

# L'entrée OrthotropicAxisY1

Cette entrée décrit la deuxième coordonnée du premier axe d'orthotropie.

* noms : OrthotropicAxisY1
* unité: sans unité
* type: scalaire 

## Description

Cette quantité est automatiquementcalculée par Licos à partir de la définition du matériau

# L'entrée OrthotropicAxisY2

Cette entrée décrit la deuxième coordonnée du second axe d'orthotropie.

* noms : OrthotropicAxisY2
* unité: sans unité
* type: scalaire 

## Description

Cette quantité est automatiquement calculée par Licos à partir de la définition du matériau

# L'entrée OrthotropicAxisZ1

Cette entrée décrit la troisième coordonnée du premier axe d'orthotropie.

* noms : OrthotropicAxisZ1
* unité: sans unité
* type: scalaire 

## Description

Cette quantité est automatiquementcalculée par Licos à partir de la définition du matériau

# L'entrée OrthotropicAxisZ2

Cette entrée décrit la troisième coordonnée du second axe d'orthotropie.

* noms : OrthotropicAxisZ2
* unité: sans unité
* type: scalaire 

## Description

Cette quantité est automatiquementcalculée par Licos à partir de la définition du matériau

# L'entrée PlasticStrain

Cette entrée décrit The plastic strain.

* noms : PlasticStrain
* unité: sans unité
* type: tenseur symétrique

# L'entrée PlateWidth

Cette entrée décrit ??.

* noms : PlateWidth
* unité: \(??\)
* type: scalaire 

# L'entrée PoissonRatio

Cette entrée décrit The Poisson ratio of an isotropic material.

* noms : PoissonRatio
* unité: sans unité
* type: scalaire 

# L'entrée PoissonRatio12

Cette entrée décrit le coefficient de Poisson d'un matériau orthotrope relatif aux première et deuxième directions d'orthotropie.

* noms : PoissonRatio12
* unité: sans unité
* type: scalaire 

# L'entrée PoissonRatio13

Cette entrée décrit le coefficient de Poisson d'un matériau orthotrope relatif aux première et troisième directions d'orthotropie.

* noms : PoissonRatio13
* unité: sans unité
* type: scalaire 

# L'entrée PoissonRatio23

Cette entrée décrit le coefficient de Poisson d'un matériau orthotrope relatif aux deuxième et troisième directions d'orthotropie.

* noms : PoissonRatio23
* unité: sans unité
* type: scalaire 

# L'entrée Porosity

Cette entrée décrit Porosity of the material.

* noms : Porosity
* unité: sans unité
* type: scalaire 

# L'entrée PorosityIncreaseDueToInelasticFlow

Cette entrée décrit Part of the porosity increase du to inelastic flow.

* noms : PorosityIncreaseDueToInelasticFlow
* unité: sans unité
* type: scalaire 

# L'entrée PorosityIncreaseDueToNucleation

Cette entrée décrit Part of the porosity increase du to nucleation.

* noms : PorosityIncreaseDueToNucleation
* unité: sans unité
* type: scalaire 

# L'entrée PowerDensity

Cette entrée décrit la densité de puissance.

* noms : PowerDensity
* unité: \(W.m^{-3}\)
* type: scalaire 

# L'entrée Pressure

Cette entrée décrit la pression d'un gaz.

* noms : Pressure
* unité: \(Pa\)
* type: scalaire 

# L'entrée PrincipalStress1

Cette entrée décrit la première contrainte principale.

* noms : PrincipalStress1
* unité: \(Pa\)
* type: scalaire 

## Notes 

Son sens est précisé dans la notice de l'opérateur ['PRIN'](http://www-cast3m.cea.fr/index.php?page=notices&notice=PRIN) de [Cast3M](http://www-cast3m.cea.fr)

# L'entrée PrincipalStress2

Cette entrée décrit la deuxième contrainte principale.

* noms : PrincipalStress2
* unité: \(Pa\)
* type: scalaire 

## Notes 

Son sens est précisé dans la notice de l'opérateur ['PRIN'](http://www-cast3m.cea.fr/index.php?page=notices&notice=PRIN) de [Cast3M](http://www-cast3m.cea.fr)

# L'entrée PrincipalStress3

Cette entrée décrit la troisième contrainte principale.

* noms : PrincipalStress3
* unité: \(Pa\)
* type: scalaire 

## Notes 

Son sens est précisé dans la notice de l'opérateur ['PRIN'](http://www-cast3m.cea.fr/index.php?page=notices&notice=PRIN) de [Cast3M](http://www-cast3m.cea.fr)

# L'entrée SecondAxisSecondMomentArea

Cette entrée décrit ??.

* noms : SecondAxisSecondMomentArea
* unité: \(??\)
* type: scalaire 

# L'entrée ShearModulus

Cette entrée décrit The shear modulus of an isotropic material.

* noms : ShearModulus
* unité: \(Pa\)
* type: scalaire 

# L'entrée ShearModulus12

Cette entrée décrit le module de cisaillement d'un matériau orthotrope relatif aux première et deuxième directions d'orthotropie.

* noms : ShearModulus12
* unité: \(Pa\)
* type: scalaire 

# L'entrée ShearModulus13

Cette entrée décrit le module de cisaillement d'un matériau orthotrope relatif aux première et troisième directions d'orthotropie.

* noms : ShearModulus13
* unité: \(Pa\)
* type: scalaire 

# L'entrée ShearModulus23

Cette entrée décrit le module de cisaillement d'un matériau orthotrope relatif aux deuxième et troisième directions d'orthotropie.

* noms : ShearModulus23
* unité: \(Pa\)
* type: scalaire 

# L'entrée SolidSwelling

Cette entrée décrit un gonflement imposé dû à des produits de fission solides.

* noms : SolidSwelling
* unité: sans unité
* type: scalaire 

## Notes 

L'utilisation de ce nom pour imposer un gonflement n'est pas obligatoire

# L'entrée SpecificHeat

Cette entrée décrit la chaleur spécifique.

* noms : SpecificHeat
* unité: \(J.kg^{-1}.K^{-1}\)
* type: scalaire 

# L'entrée SphericalStress

Cette entrée décrit les contraintes exprimées dans le repère sphérique.

* noms : SphericalStress
* unité: \(Pa\)
* type: tenseur symétrique

# L'entrée Strain

Cette entrée décrit la déformation.

* noms : Strain
* unité: sans unité
* type: tenseur symétrique

# L'entrée Stress

Cette entrée décrit les contraintes.

* noms : Stress
* unité: \(Pa\)
* type: tenseur symétrique

# L'entrée Swelling

Cette entrée décrit un gonflement imposé.

* noms : Swelling
* unité: sans unité
* type: scalaire 

## Notes 

l'utilisation de ce nom pour imposer un gonflement n'est pasobligatoire.

# L'entrée TangentialStiffness

Cette entrée décrit la raideur tangentielle pour un modèle de zone cohésive.

* noms : TangentialStiffness
* unité: \(Pa.m^{-1}\)
* type: scalaire 

# L'entrée Temperature

Cette entrée décrit The temperature.

* noms : Temperature
* unité: \(K\)
* type: scalaire 

# L'entrée TemperatureGradient

Cette entrée décrit le gradient de température.

* noms : TemperatureGradient
* unité: \(T.m^{-1}\)
* type: vecteur 

# L'entrée ThermalConductivity

Cette entrée décrit la conductivité thermique d'un matériau isotrope.

* noms : ThermalConductivity
* unité: \(W.m^{-1}\)
* type: scalaire 

# L'entrée ThermalConductivity1

Cette entrée décrit la conductivité thermique d'un matériau orthotrope suivant la première direction d'orthotropie.

* noms : ThermalConductivity1
* unité: \(W.m^{-1}\)
* type: scalaire 

# L'entrée ThermalConductivity2

Cette entrée décrit la conductivité thermique d'un matériau orthotrope suivant la deuxième direction d'orthotropie.

* noms : ThermalConductivity2
* unité: \(W.m^{-1}\)
* type: scalaire 

# L'entrée ThermalConductivity3

Cette entrée décrit la conductivité thermique d'un matériau orthotrope suivant la troisième direction d'orthotropie.

* noms : ThermalConductivity3
* unité: \(W.m^{-1}\)
* type: scalaire 

# L'entrée ThermalExpansion

Cette entrée décrit le coefficient de dilatation linéique d'un matériau isotrope.

* noms : ThermalExpansion
* unité: \(K^{-1}\)
* type: scalaire 

# L'entrée ThermalExpansion1

Cette entrée décrit le coefficient de dilatation linéique d'un matériau orthotrope suivant la première direction d'orthotropie.

* noms : ThermalExpansion1
* unité: \(K^{-1}\)
* type: scalaire 

# L'entrée ThermalExpansion2

Cette entrée décrit le coefficient de dilatation linéique d'un matériau orthotrope suivant la deuxième direction d'orthotropie.

* noms : ThermalExpansion2
* unité: \(K^{-1}\)
* type: scalaire 

# L'entrée ThermalExpansion3

Cette entrée décrit le coefficient de dilatation linéique d'un matériau orthotrope suivant la troisième direction d'orthotropie.

* noms : ThermalExpansion3
* unité: \(K^{-1}\)
* type: scalaire 

# L'entrée TorsionConstant

Cette entrée décrit ??.

* noms : TorsionConstant
* unité: \(??\)
* type: scalaire 

# L'entrée TrescaStress

Cette entrée décrit la contrainte équivalente au sens de Tresca.

* noms : TrescaStress
* unité: \(Pa\)
* type: scalaire 

# L'entrée UltimateTensileStrength

Cette entrée décrit la valeur maximale de la contrainte qu'un materiau peut supporter.

* noms : UltimateTensileStress, UltimateTensileStrength
* unité: \(Pa\)
* type: scalaire 

# L'entrée ViscoplasticStrain

Cette entrée décrit The viscoplatic strain.

* noms : ViscoplasticStrain
* unité: sans unité
* type: tenseur symétrique

# L'entrée VolumetricStrain

Cette entrée décrit la dilatation volumique.

* noms : VolumetricStrain
* unité: sans unité
* type: scalaire 

# L'entrée VonMisesStress

Cette entrée décrit la contrainte équivalente au sens de Von Mises.

* noms : VonMisesStress
* unité: \(Pa\)
* type: scalaire 

# L'entrée YieldStrength

Cette entrée décrit la limite d'élasticité.

* noms : YieldStress, YieldStrength
* unité: \(Pa\)
* type: scalaire 

# L'entrée YoungModulus

Cette entrée décrit The Young modulus of an isotropic material.

* noms : YoungModulus
* unité: \(Pa\)
* type: scalaire 

# L'entrée YoungModulus1

Cette entrée décrit le module d'Young d'un matériau orthotrope suivant la première direction d'orthotropie.

* noms : YoungModulus1
* unité: \(Pa\)
* type: scalaire 

# L'entrée YoungModulus2

Cette entrée décrit le module d'Young d'un matériau orthotrope suivant la deuxième direction d'orthotropie.

* noms : YoungModulus2
* unité: \(Pa\)
* type: scalaire 

# L'entrée YoungModulus3

Cette entrée décrit le module d'Young d'un matériau orthotrope suivant la troisième direction d'orthotropie.

* noms : YoungModulus3
* unité: \(Pa\)
* type: scalaire 
<!-- Local IspellDict: english -->
