/*!
 * \file   Glossary.cxx
 * \author Thomas Helfer
 * \date   09/06/14
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

namespace tfel {
  namespace glossary {

    const char* Glossary::names[106] = {"AxialDeformationGradient",
                                        "AxialGrowth",
                                        "AxialStrain",
                                        "AxialStress",
                                        "B10BurnUp",
                                        "Broken",
                                        "BulkModulus",
                                        "BurnUp (at.%)",
                                        "BurnUp (MWJ/tm)",
                                        "CohesiveForce",
                                        "ConvectiveHeatTransferCoefficient",
                                        "CrossSectionArea",
                                        "CylindricalStress",
                                        "Damage",
                                        "DeformationGradient",
                                        "Displacement",
                                        "DualStress",
                                        "ElasticStrain",
                                        "Emissivity",
                                        "EquivalentPlasticStrain",
                                        "EquivalentStrain",
                                        "EquivalentViscoplasticStrain",
                                        "FastNeutronFluence (>0.1 MeV)",
                                        "FastNeutronFluence (>1 MeV)",
                                        "FastNeutronFlux (>0.1 MeV)",
                                        "FastNeutronFlux (>1 MeV)",
                                        "FirstAxisSecondMomentArea",
                                        "FirstLameCoefficient",
                                        "FissionDensity",
                                        "GaseousSwelling",
                                        "GrainSize",
                                        "HeatFlux",
                                        "HeatTransferCoefficient",
                                        "HillStress",
                                        "HydrostaticPressure",
                                        "IrradiationDamage",
                                        "IrradiationInducedSwelling",
                                        "IrradiationSwelling",
                                        "IrradiationTemperature",
                                        "KelvinTemperature",
                                        "MassDensity",
                                        "MeanBurnUp (at.%)",
                                        "MeanBurnUp (MWJ/tm)",
                                        "MeanIrradiationTemperature",
                                        "MeanTemperature",
                                        "NeutronFluence",
                                        "NeutronFlux",
                                        "NormalStiffness",
                                        "NumberOfMoles",
                                        "OpeningDisplacement",
                                        "OrthotropicAxisX1",
                                        "OrthotropicAxisX2",
                                        "OrthotropicAxisY1",
                                        "OrthotropicAxisY2",
                                        "OrthotropicAxisZ1",
                                        "OrthotropicAxisZ2",
                                        "PlasticStrain",
                                        "PlateWidth",
                                        "PoissonRatio",
                                        "PoissonRatio12",
                                        "PoissonRatio13",
                                        "PoissonRatio23",
                                        "Porosity",
                                        "PorosityIncreaseDueToInelasticFlow",
                                        "PorosityIncreaseDueToNucleation",
                                        "PowerDensity",
                                        "Pressure",
                                        "PrincipalStress1",
                                        "PrincipalStress2",
                                        "PrincipalStress3",
                                        "SecondAxisSecondMomentArea",
                                        "ShearModulus",
                                        "ShearModulus12",
                                        "ShearModulus13",
                                        "ShearModulus23",
                                        "SolidSwelling",
                                        "SpecificHeat",
                                        "SphericalStress",
                                        "Strain",
                                        "StrainMeasure",
                                        "Stress",
                                        "Swelling",
                                        "TangentialStiffness",
                                        "Temperature",
                                        "TemperatureGradient",
                                        "ThermalConductivity",
                                        "ThermalConductivity1",
                                        "ThermalConductivity2",
                                        "ThermalConductivity3",
                                        "ThermalExpansion",
                                        "ThermalExpansion1",
                                        "ThermalExpansion2",
                                        "ThermalExpansion3",
                                        "TorsionConstant",
                                        "TrescaStress",
                                        "UltimateTensileStress",
                                        "UltimateTensileStrength",
                                        "ViscoplasticStrain",
                                        "VolumetricStrain",
                                        "VonMisesStress",
                                        "YieldStress",
                                        "YieldStrength",
                                        "YoungModulus",
                                        "YoungModulus1",
                                        "YoungModulus2",
                                        "YoungModulus3"};
    const GlossaryEntry Glossary::AxialDeformationGradient(
        "AxialDeformationGradient",
        "AxialDeformationGradient",
        "",
        "scalar",
        "la composante axiale du gradient de la transformation (cette grandeur "
        "n'a de sens que pour les calculs en contraintes planes)",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::AxialGrowth("AxialGrowth",
                                              "AxialGrowth",
                                              "",
                                              "scalar",
                                              "axial growth under irradiation",
                                              "" /* no 'description' defined */,
                                              "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::AxialStrain(
        "AxialStrain",
        "AxialStrain",
        "",
        "scalar",
        "la déformation axiale (cette grandeur n'a de sens que pour les "
        "calculs en contraintes planes)",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::AxialStress(
        "AxialStress",
        "AxialStress",
        "",
        "scalar",
        "la contrainte axiale",
        "" /* no 'description' defined */,
        "Cette grandeur est utilisée dans les calculs 1Daxisymétrique en "
        "contraintes planes généralisées");

    const GlossaryEntry Glossary::B10BurnUp(
        "B10BurnUp",
        "B10BurnUp",
        "m^{-3}",
        "scalar",
        "le taux d'usure en \\(\\mbox{}^{10}B\\)",
        "Ce nombre décrit le nombre d'atomes de \\(\\mbox{}^{10}B\\) consommé "
        "par unité de volume au cours de l'irradiation.",
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Broken("Broken",
                                         "Broken",
                                         "none",
                                         "scalar",
                                         "material failure indicator",
                                         "" /* no 'description' defined */,
                                         "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::BulkModulus(
        "BulkModulus",
        "BulkModulus",
        "Pa",
        "scalar",
        "The bulk modulus of an isotropic material",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::BurnUp_AtPercent(
        "BurnUp_AtPercent",
        "BurnUp (at.%)",
        "at./100",
        "scalar",
        "le taux de combustion en atome pour cent",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::BurnUp_MWJperTm(
        "BurnUp_MWJperTm",
        "BurnUp (MWJ/tm)",
        "MWJ/tm",
        "scalar",
        "le taux de combustion en MegaWattJour par tonne (métal)",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::CohesiveForce(
        "CohesiveForce",
        "CohesiveForce",
        "Newton",
        "vector",
        "cohesive force for cohesize zone models",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ConvectiveHeatTransferCoefficient(
        "ConvectiveHeatTransferCoefficient",
        "ConvectiveHeatTransferCoefficient",
        "W.m^{-2}.K^{-1}",
        "scalar",
        "le coefficient d'échange par convection",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::CrossSectionArea(
        "CrossSectionArea",
        "CrossSectionArea",
        "??",
        "scalar",
        "??",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::CylindricalStress(
        "CylindricalStress",
        "CylindricalStress",
        "Pa",
        "tensor",
        "les contraintes exprimées dans le repère cylindrique",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Damage("Damage",
                                         "Damage",
                                         "",
                                         "scalar",
                                         "l'endommagement d'un matériau",
                                         "" /* no 'description' defined */,
                                         "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::DeformationGradient(
        "DeformationGradient",
        "DeformationGradient",
        "",
        "tensor",
        "gradient of the transformation",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Displacement(
        "Displacement",
        "Displacement",
        "m",
        "vector",
        "le déplacement",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::DualStress(
        "DualStress",
        "DualStress",
        "stress",
        "tensor",
        "dual stress of the strain measure",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ElasticStrain(
        "ElasticStrain",
        "ElasticStrain",
        "",
        "tensor",
        "The elastic strain",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Emissivity(
        "Emissivity",
        "Emissivity",
        "",
        "scalar",
        "Emissivity of the surface of a material is its effectiveness in "
        "emitting energy as thermal radiation",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::EquivalentPlasticStrain(
        "EquivalentPlasticStrain",
        "EquivalentPlasticStrain",
        "",
        "scalar",
        "The equivalent plastic strain",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::EquivalentStrain(
        "EquivalentStrain",
        "EquivalentStrain",
        "",
        "scalar",
        "la somme des déformations plastiques ou viscoplastiques équivalentes",
        "" /* no 'description' defined */,
        "Cette quantité n'a pas de sens physique directe.");

    const GlossaryEntry Glossary::EquivalentViscoplasticStrain(
        "EquivalentViscoplasticStrain",
        "EquivalentViscoplasticStrain",
        "",
        "scalar",
        "The equivalent viscoplastic strain",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::FastNeutronFluence_01MeV(
        "FastNeutronFluence_01MeV",
        "FastNeutronFluence (>0.1 MeV)",
        "n.m^{-2}",
        "scalar",
        "la fluence rapide",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::FastNeutronFluence_1MeV(
        "FastNeutronFluence_1MeV",
        "FastNeutronFluence (>1 MeV)",
        "n.m^{-2}",
        "scalar",
        "la fluence rapide",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::FastNeutronFlux_01MeV(
        "FastNeutronFlux_01MeV",
        "FastNeutronFlux (>0.1 MeV)",
        "n.m^{-2}.s^{-1}",
        "scalar",
        "le flux de neutron rapide",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::FastNeutronFlux_1MeV(
        "FastNeutronFlux_1MeV",
        "FastNeutronFlux (>1 MeV)",
        "n.m^{-2}.s^{-1}",
        "scalar",
        "le flux de neutron rapide",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::FirstAxisSecondMomentArea(
        "FirstAxisSecondMomentArea",
        "FirstAxisSecondMomentArea",
        "??",
        "scalar",
        "??",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::FirstLameCoefficient(
        "FirstLameCoefficient",
        "FirstLameCoefficient",
        "Pa",
        "scalar",
        "First Lamé coefficient of an isotropic material",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::FissionDensity(
        "FissionDensity",
        "FissionDensity",
        "m^{-3}",
        "scalar",
        "la densité de fission",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::GaseousSwelling(
        "GaseousSwelling",
        "GaseousSwelling",
        "",
        "scalar",
        "un gonflement imposé dû à des produits de fission gazeux",
        "" /* no 'description' defined */,
        "L'utilisation de ce nom pour imposer un gonflement n'est pas "
        "obligatoire");

    const GlossaryEntry Glossary::GrainSize("GrainSize",
                                            "GrainSize",
                                            "m",
                                            "scalar",
                                            "la taille de grain",
                                            "" /* no 'description' defined */,
                                            "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::HeatFlux("HeatFlux",
                                           "HeatFlux",
                                           "J.m^{-2}.s^{-1}",
                                           "vector",
                                           "le flux de chaleur.",
                                           "" /* no 'description' defined */,
                                           "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::HeatTransferCoefficient(
        "HeatTransferCoefficient",
        "HeatTransferCoefficient",
        "W.m^{-2}.K^{-1}",
        "scalar",
        "The heat transfer coefficient is the proportionality constant between "
        "the heat flux and the temperature difference",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::HillStress(
        "HillStress",
        "HillStress",
        "Pa",
        "tensor",
        "la contrainte équivalente au sens de Hill",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::HydrostaticPressure(
        "HydrostaticPressure",
        "HydrostaticPressure",
        "Pa",
        "tensor",
        "la pression hydrostatique (égale au tiers de la trace du tenseur des "
        "contraintes)",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::IrradiationDamage(
        "IrradiationDamage",
        "IrradiationDamage",
        "dpa",
        "scalar",
        "le dommage due à l'irradiation, mesuré par le nombre de déplacement "
        "moyen de chaque atome (dpa)",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::IrradiationInducedSwelling(
        "IrradiationInducedSwelling",
        "IrradiationInducedSwelling",
        "",
        "scalar",
        "un gonflement imposé  dû à des dégâts d'irradiation",
        "" /* no 'description' defined */,
        "L'utilisation de ce nom pour imposer un gonflement n'est pas "
        "obligatoire");

    const GlossaryEntry Glossary::IrradiationSwelling(
        "IrradiationSwelling",
        "IrradiationSwelling",
        "",
        "scalar",
        "un gonflement imposé  dû à des dégâts d'irradiation",
        "" /* no 'description' defined */,
        "L'utilisation de ce nom pour imposer un gonflement n'est pas "
        "obligatoire");

    const GlossaryEntry Glossary::IrradiationTemperature(
        "IrradiationTemperature",
        "IrradiationTemperature",
        "K",
        "scalar",
        "la température moyenne (dans le temps) au cours de l'irradiation",
        "Cette température est définie ainsi :"
        "@^separator^@"
        "\\["
        "@^separator^@"
        "\\average{T}\\paren{t,\\vec{r}}  = "
        "\\Frac{1}{t-t_{0}}\\int_{t_{0}}^{t}T\\paren{u,\\vec{r}}\\,\\dtot\\, u"
        "@^separator^@"
        "\\]"
        "@^separator^@"
        "où"
        "@^separator^@"
        ""
        "@^separator^@"
        "* \\(T\\paren{t,\\vec{r}}\\) est la valeur à un instant \\(t\\) de la "
        "température au point \\(\\vec{r}\\) ;"
        "@^separator^@"
        "* \\(t_{0}\\) est la date du début de calcul ;"
        "@^separator^@"
        "* \\(t\\) est la date courante."
        "@^separator^@"
        ""
        "@^separator^@"
        "En pratique, l'intégrale temporelle est évaluée de manière "
        "incrémentale ainsi :"
        "@^separator^@"
        "\\["
        "@^separator^@"
        "\\average{T}\\paren{t+dt,\\vec{r}} \\approx "
        "\\Frac{1}{t+dt-t_{0}}\\left[\\paren{t-t_{0}}\\,\\average{T}\\paren{t,"
        "\\vec{r}}+\\Frac{dt}{2}\\left[T\\paren{t,\\vec{r}}+T\\paren{t+dt,"
        "\\vec{r}}\\right]\\right]"
        "@^separator^@"
        "\\]",
        "Ce mode de calcul peut conduire à de légères erreurs numériques");

    const GlossaryEntry Glossary::KelvinTemperature(
        "KelvinTemperature",
        "KelvinTemperature",
        "K",
        "scalar",
        "The temperature",
        "" /* no 'description' defined */,
        "Cette entrée a été rajoutée par compatibilité avec les choix "
        "d'implantation des lois de comportements mécaniques utilisés par le "
        "code Germinal");

    const GlossaryEntry Glossary::MassDensity("MassDensity",
                                              "MassDensity",
                                              "kg.m^{-3}",
                                              "scalar",
                                              "The mass density",
                                              "" /* no 'description' defined */,
                                              "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::MeanBurnUp_AtPercent(
        "MeanBurnUp_AtPercent",
        "MeanBurnUp (at.%)",
        "at./100",
        "scalar",
        "le taux de combustion en atome pour cent moyen (au sens spatial) pour "
        "un matériau donné",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::MeanBurnUp_MWJperTm(
        "MeanBurnUp_MWJperTm",
        "MeanBurnUp (MWJ/tm)",
        "MWJ/tm",
        "scalar",
        "le taux de combustion en MegaWattJour par tonne (métal) moyen (au "
        "sens spatial) pour un matériau donné",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::MeanIrradiationTemperature(
        "MeanIrradiationTemperature",
        "MeanIrradiationTemperature",
        "K",
        "scalar",
        "la température moyenne dans le temps et dans un domaine \\(\\Omega\\) "
        "donné",
        "Cette température est définie ainsi :\\[\\average{T}\\paren{t} "
        "=\\Frac{1}{t-t_{0}}\\Frac{1}{\\int_{\\Omega}\\dtot\\,V}\\int_{t_{0}}^{"
        "t}\\paren{\\int_{\\Omega}T\\paren{u,\\vec{r}}\\,\\dtot\\,V}\\]où "
        "\\(T\\paren{t,\\vec{r}}\\) est la valeur à un instant \\(t\\) de la "
        "température au point \\(\\vec{r}\\).",
        "En pratique, l'intégrale spatiale est calculée à l'aide de la "
        "discrétisation par éléments finis et l'intégrale temporelle est "
        "calculée de manière incrémentale.");

    const GlossaryEntry Glossary::MeanTemperature(
        "MeanTemperature",
        "MeanTemperature",
        "K",
        "scalar",
        "la température moyenne dans un domaine \\(\\Omega\\) donné. ",
        "Cette température est définie ainsi :\\[\\average{T}\\paren{t}  = "
        "\\Frac{1}{\\int_{\\Omega}\\dtot\\,V}\\int_{\\Omega}T\\paren{t,\\vec{r}"
        "}\\,\\dtot\\, V\\]où \\(T\\paren{t,\\vec{r}}\\) est la valeur à un "
        "instant \\(t\\) de latempérature au point \\(\\vec{r}\\).",
        "En pratique, l'intégrale spatiale est calculée à l'aide de la "
        "discrétisation par éléments finis.");

    const GlossaryEntry Glossary::NeutronFluence(
        "NeutronFluence",
        "NeutronFluence",
        "n.m^{-2}",
        "scalar",
        "la fluence",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::NeutronFlux("NeutronFlux",
                                              "NeutronFlux",
                                              "n.m^{-2}.s^{-1}",
                                              "scalar",
                                              "le flux de neutron",
                                              "" /* no 'description' defined */,
                                              "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::NormalStiffness(
        "NormalStiffness",
        "NormalStiffness",
        "Pa.m^{-1}",
        "scalar",
        "la raideur normale pour un modèle de zone cohésive",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::NumberOfMoles(
        "NumberOfMoles",
        "NumberOfMoles",
        "mol",
        "scalar",
        "le nombre de moles d'une substance",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::OpeningDisplacement(
        "OpeningDisplacement",
        "OpeningDisplacement",
        "m",
        "vector",
        "opening displacement in cohesive zone models",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::OrthotropicAxisX1(
        "OrthotropicAxisX1",
        "OrthotropicAxisX1",
        "",
        "scalar",
        "la première coordonnée du premier axe d'orthotropie",
        "Cette quantité est automatiquementcalculée par Licos à partir de la "
        "définition du matériau",
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::OrthotropicAxisX2(
        "OrthotropicAxisX2",
        "OrthotropicAxisX2",
        "",
        "scalar",
        "la première coordonnée du second axe d'orthotropie",
        "Cette quantité est automatiquementcalculée par Licos à partir de la "
        "définition du matériau",
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::OrthotropicAxisY1(
        "OrthotropicAxisY1",
        "OrthotropicAxisY1",
        "",
        "scalar",
        "la deuxième coordonnée du premier axe d'orthotropie",
        "Cette quantité est automatiquementcalculée par Licos à partir de la "
        "définition du matériau",
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::OrthotropicAxisY2(
        "OrthotropicAxisY2",
        "OrthotropicAxisY2",
        "",
        "scalar",
        "la deuxième coordonnée du second axe d'orthotropie",
        "Cette quantité est automatiquement calculée par Licos à partir de la "
        "définition du matériau",
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::OrthotropicAxisZ1(
        "OrthotropicAxisZ1",
        "OrthotropicAxisZ1",
        "",
        "scalar",
        "la troisième coordonnée du premier axe d'orthotropie",
        "Cette quantité est automatiquementcalculée par Licos à partir de la "
        "définition du matériau",
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::OrthotropicAxisZ2(
        "OrthotropicAxisZ2",
        "OrthotropicAxisZ2",
        "",
        "scalar",
        "la troisième coordonnée du second axe d'orthotropie",
        "Cette quantité est automatiquementcalculée par Licos à partir de la "
        "définition du matériau",
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PlasticStrain(
        "PlasticStrain",
        "PlasticStrain",
        "",
        "tensor",
        "The plastic strain",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PlateWidth("PlateWidth",
                                             "PlateWidth",
                                             "??",
                                             "scalar",
                                             "??",
                                             "" /* no 'description' defined */,
                                             "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PoissonRatio(
        "PoissonRatio",
        "PoissonRatio",
        "",
        "scalar",
        "The Poisson ratio of an isotropic material",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PoissonRatio12(
        "PoissonRatio12",
        "PoissonRatio12",
        "",
        "scalar",
        "le coefficient de Poisson d'un matériau orthotrope relatif aux "
        "première et deuxième directions d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PoissonRatio13(
        "PoissonRatio13",
        "PoissonRatio13",
        "",
        "scalar",
        "le coefficient de Poisson d'un matériau orthotrope relatif aux "
        "première et troisième directions d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PoissonRatio23(
        "PoissonRatio23",
        "PoissonRatio23",
        "",
        "scalar",
        "le coefficient de Poisson d'un matériau orthotrope relatif aux "
        "deuxième et troisième directions d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Porosity("Porosity",
                                           "Porosity",
                                           "",
                                           "scalar",
                                           "Porosity of the material",
                                           "" /* no 'description' defined */,
                                           "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PorosityIncreaseDueToInelasticFlow(
        "PorosityIncreaseDueToInelasticFlow",
        "PorosityIncreaseDueToInelasticFlow",
        "",
        "scalar",
        "Part of the porosity increase du to inelastic flow",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PorosityIncreaseDueToNucleation(
        "PorosityIncreaseDueToNucleation",
        "PorosityIncreaseDueToNucleation",
        "",
        "scalar",
        "Part of the porosity increase du to nucleation",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PowerDensity(
        "PowerDensity",
        "PowerDensity",
        "W.m^{-3}",
        "scalar",
        "la densité de puissance",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Pressure("Pressure",
                                           "Pressure",
                                           "Pa",
                                           "scalar",
                                           "la pression d'un gaz",
                                           "" /* no 'description' defined */,
                                           "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::PrincipalStress1(
        "PrincipalStress1",
        "PrincipalStress1",
        "Pa",
        "scalar",
        "la première contrainte principale",
        "" /* no 'description' defined */,
        "Son sens est précisé dans la notice de l'opérateur "
        "['PRIN'](http://www-cast3m.cea.fr/index.php?page=notices&notice=PRIN) "
        "de [Cast3M](http://www-cast3m.cea.fr)");

    const GlossaryEntry Glossary::PrincipalStress2(
        "PrincipalStress2",
        "PrincipalStress2",
        "Pa",
        "scalar",
        "la deuxième contrainte principale",
        "" /* no 'description' defined */,
        "Son sens est précisé dans la notice de l'opérateur "
        "['PRIN'](http://www-cast3m.cea.fr/index.php?page=notices&notice=PRIN) "
        "de [Cast3M](http://www-cast3m.cea.fr)");

    const GlossaryEntry Glossary::PrincipalStress3(
        "PrincipalStress3",
        "PrincipalStress3",
        "Pa",
        "scalar",
        "la troisième contrainte principale",
        "" /* no 'description' defined */,
        "Son sens est précisé dans la notice de l'opérateur "
        "['PRIN'](http://www-cast3m.cea.fr/index.php?page=notices&notice=PRIN) "
        "de [Cast3M](http://www-cast3m.cea.fr)");

    const GlossaryEntry Glossary::SecondAxisSecondMomentArea(
        "SecondAxisSecondMomentArea",
        "SecondAxisSecondMomentArea",
        "??",
        "scalar",
        "??",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ShearModulus(
        "ShearModulus",
        "ShearModulus",
        "Pa",
        "scalar",
        "The shear modulus of an isotropic material",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ShearModulus12(
        "ShearModulus12",
        "ShearModulus12",
        "Pa",
        "scalar",
        "le module de cisaillement d'un matériau orthotrope relatif aux "
        "première et deuxième directions d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ShearModulus13(
        "ShearModulus13",
        "ShearModulus13",
        "Pa",
        "scalar",
        "le module de cisaillement d'un matériau orthotrope relatif aux "
        "première et troisième directions d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ShearModulus23(
        "ShearModulus23",
        "ShearModulus23",
        "Pa",
        "scalar",
        "le module de cisaillement d'un matériau orthotrope relatif aux "
        "deuxième et troisième directions d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::SolidSwelling(
        "SolidSwelling",
        "SolidSwelling",
        "",
        "scalar",
        "un gonflement imposé dû à des produits de fission solides",
        "" /* no 'description' defined */,
        "L'utilisation de ce nom pour imposer un gonflement n'est pas "
        "obligatoire");

    const GlossaryEntry Glossary::SpecificHeat(
        "SpecificHeat",
        "SpecificHeat",
        "J.kg^{-1}.K^{-1}",
        "scalar",
        "la chaleur spécifique",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::SphericalStress(
        "SphericalStress",
        "SphericalStress",
        "Pa",
        "tensor",
        "les contraintes exprimées dans le repère sphérique",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Strain("Strain",
                                         "Strain",
                                         "",
                                         "tensor",
                                         "la déformation",
                                         "" /* no 'description' defined */,
                                         "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::StrainMeasure(
        "StrainMeasure",
        "StrainMeasure",
        "none",
        "tensor",
        "strain measure",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Stress("Stress",
                                         "Stress",
                                         "Pa",
                                         "tensor",
                                         "les contraintes",
                                         "" /* no 'description' defined */,
                                         "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Swelling(
        "Swelling",
        "Swelling",
        "",
        "scalar",
        "un gonflement imposé",
        "" /* no 'description' defined */,
        "l'utilisation de ce nom pour imposer un gonflement n'est "
        "pasobligatoire.");

    const GlossaryEntry Glossary::TangentialStiffness(
        "TangentialStiffness",
        "TangentialStiffness",
        "Pa.m^{-1}",
        "scalar",
        "la raideur tangentielle pour un modèle de zone cohésive",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::Temperature("Temperature",
                                              "Temperature",
                                              "K",
                                              "scalar",
                                              "The temperature",
                                              "" /* no 'description' defined */,
                                              "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::TemperatureGradient(
        "TemperatureGradient",
        "TemperatureGradient",
        "T.m^{-1}",
        "vector",
        "le gradient de température",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ThermalConductivity(
        "ThermalConductivity",
        "ThermalConductivity",
        "W.m^{-1}.K^{-1}",
        "scalar",
        "la conductivité thermique d'un matériau isotrope",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ThermalConductivity1(
        "ThermalConductivity1",
        "ThermalConductivity1",
        "W.m^{-1}.K^{-1}",
        "scalar",
        "la conductivité thermique d'un matériau orthotrope suivant la "
        "première direction d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ThermalConductivity2(
        "ThermalConductivity2",
        "ThermalConductivity2",
        "W.m^{-1}.K^{-1}",
        "scalar",
        "la conductivité thermique d'un matériau orthotrope suivant la "
        "deuxième direction d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ThermalConductivity3(
        "ThermalConductivity3",
        "ThermalConductivity3",
        "W.m^{-1}.K^{-1}",
        "scalar",
        "la conductivité thermique d'un matériau orthotrope suivant la "
        "troisième direction d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ThermalExpansion(
        "ThermalExpansion",
        "ThermalExpansion",
        "K^{-1}",
        "scalar",
        "le coefficient de dilatation linéique d'un matériau isotrope",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ThermalExpansion1(
        "ThermalExpansion1",
        "ThermalExpansion1",
        "K^{-1}",
        "scalar",
        "le coefficient de dilatation linéique d'un matériau orthotrope "
        "suivant la première direction d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ThermalExpansion2(
        "ThermalExpansion2",
        "ThermalExpansion2",
        "K^{-1}",
        "scalar",
        "le coefficient de dilatation linéique d'un matériau orthotrope "
        "suivant la deuxième direction d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ThermalExpansion3(
        "ThermalExpansion3",
        "ThermalExpansion3",
        "K^{-1}",
        "scalar",
        "le coefficient de dilatation linéique d'un matériau orthotrope "
        "suivant la troisième direction d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::TorsionConstant(
        "TorsionConstant",
        "TorsionConstant",
        "??",
        "scalar",
        "??",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::TrescaStress(
        "TrescaStress",
        "TrescaStress",
        "Pa",
        "scalar",
        "la contrainte équivalente au sens de Tresca",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::UltimateTensileStrength(
        "UltimateTensileStrength",
        Glossary::names + 95,
        Glossary::names + 97,
        "Pa",
        "scalar",
        "la valeur maximale de la contrainte qu'un materiau peut supporter",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::ViscoplasticStrain(
        "ViscoplasticStrain",
        "ViscoplasticStrain",
        "",
        "tensor",
        "The viscoplatic strain",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::VolumetricStrain(
        "VolumetricStrain",
        "VolumetricStrain",
        "",
        "scalar",
        "la dilatation volumique",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::VonMisesStress(
        "VonMisesStress",
        "VonMisesStress",
        "Pa",
        "scalar",
        "la contrainte équivalente au sens de Von Mises",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::YieldStrength(
        "YieldStrength",
        Glossary::names + 100,
        Glossary::names + 102,
        "Pa",
        "scalar",
        "la limite d'élasticité",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::YoungModulus(
        "YoungModulus",
        "YoungModulus",
        "Pa",
        "scalar",
        "The Young modulus of an isotropic material",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::YoungModulus1(
        "YoungModulus1",
        "YoungModulus1",
        "Pa",
        "scalar",
        "le module d'Young d'un matériau orthotrope suivant la première "
        "direction d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::YoungModulus2(
        "YoungModulus2",
        "YoungModulus2",
        "Pa",
        "scalar",
        "le module d'Young d'un matériau orthotrope suivant la deuxième "
        "direction d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    const GlossaryEntry Glossary::YoungModulus3(
        "YoungModulus3",
        "YoungModulus3",
        "Pa",
        "scalar",
        "le module d'Young d'un matériau orthotrope suivant la troisième "
        "direction d'orthotropie",
        "" /* no 'description' defined */,
        "" /* no 'notes' defined */);

    Glossary& Glossary::getGlossary() {
      static Glossary glossary;
      return glossary;
    }  // end of Glossary::getGlossary

    Glossary::Glossary() {
      this->keys.reserve(104);
      this->insert(Glossary::AxialDeformationGradient);
      this->insert(Glossary::AxialGrowth);
      this->insert(Glossary::AxialStrain);
      this->insert(Glossary::AxialStress);
      this->insert(Glossary::B10BurnUp);
      this->insert(Glossary::Broken);
      this->insert(Glossary::BulkModulus);
      this->insert(Glossary::BurnUp_AtPercent);
      this->insert(Glossary::BurnUp_MWJperTm);
      this->insert(Glossary::CohesiveForce);
      this->insert(Glossary::ConvectiveHeatTransferCoefficient);
      this->insert(Glossary::CrossSectionArea);
      this->insert(Glossary::CylindricalStress);
      this->insert(Glossary::Damage);
      this->insert(Glossary::DeformationGradient);
      this->insert(Glossary::Displacement);
      this->insert(Glossary::DualStress);
      this->insert(Glossary::ElasticStrain);
      this->insert(Glossary::Emissivity);
      this->insert(Glossary::EquivalentPlasticStrain);
      this->insert(Glossary::EquivalentStrain);
      this->insert(Glossary::EquivalentViscoplasticStrain);
      this->insert(Glossary::FastNeutronFluence_01MeV);
      this->insert(Glossary::FastNeutronFluence_1MeV);
      this->insert(Glossary::FastNeutronFlux_01MeV);
      this->insert(Glossary::FastNeutronFlux_1MeV);
      this->insert(Glossary::FirstAxisSecondMomentArea);
      this->insert(Glossary::FirstLameCoefficient);
      this->insert(Glossary::FissionDensity);
      this->insert(Glossary::GaseousSwelling);
      this->insert(Glossary::GrainSize);
      this->insert(Glossary::HeatFlux);
      this->insert(Glossary::HeatTransferCoefficient);
      this->insert(Glossary::HillStress);
      this->insert(Glossary::HydrostaticPressure);
      this->insert(Glossary::IrradiationDamage);
      this->insert(Glossary::IrradiationInducedSwelling);
      this->insert(Glossary::IrradiationSwelling);
      this->insert(Glossary::IrradiationTemperature);
      this->insert(Glossary::KelvinTemperature);
      this->insert(Glossary::MassDensity);
      this->insert(Glossary::MeanBurnUp_AtPercent);
      this->insert(Glossary::MeanBurnUp_MWJperTm);
      this->insert(Glossary::MeanIrradiationTemperature);
      this->insert(Glossary::MeanTemperature);
      this->insert(Glossary::NeutronFluence);
      this->insert(Glossary::NeutronFlux);
      this->insert(Glossary::NormalStiffness);
      this->insert(Glossary::NumberOfMoles);
      this->insert(Glossary::OpeningDisplacement);
      this->insert(Glossary::OrthotropicAxisX1);
      this->insert(Glossary::OrthotropicAxisX2);
      this->insert(Glossary::OrthotropicAxisY1);
      this->insert(Glossary::OrthotropicAxisY2);
      this->insert(Glossary::OrthotropicAxisZ1);
      this->insert(Glossary::OrthotropicAxisZ2);
      this->insert(Glossary::PlasticStrain);
      this->insert(Glossary::PlateWidth);
      this->insert(Glossary::PoissonRatio);
      this->insert(Glossary::PoissonRatio12);
      this->insert(Glossary::PoissonRatio13);
      this->insert(Glossary::PoissonRatio23);
      this->insert(Glossary::Porosity);
      this->insert(Glossary::PorosityIncreaseDueToInelasticFlow);
      this->insert(Glossary::PorosityIncreaseDueToNucleation);
      this->insert(Glossary::PowerDensity);
      this->insert(Glossary::Pressure);
      this->insert(Glossary::PrincipalStress1);
      this->insert(Glossary::PrincipalStress2);
      this->insert(Glossary::PrincipalStress3);
      this->insert(Glossary::SecondAxisSecondMomentArea);
      this->insert(Glossary::ShearModulus);
      this->insert(Glossary::ShearModulus12);
      this->insert(Glossary::ShearModulus13);
      this->insert(Glossary::ShearModulus23);
      this->insert(Glossary::SolidSwelling);
      this->insert(Glossary::SpecificHeat);
      this->insert(Glossary::SphericalStress);
      this->insert(Glossary::Strain);
      this->insert(Glossary::StrainMeasure);
      this->insert(Glossary::Stress);
      this->insert(Glossary::Swelling);
      this->insert(Glossary::TangentialStiffness);
      this->insert(Glossary::Temperature);
      this->insert(Glossary::TemperatureGradient);
      this->insert(Glossary::ThermalConductivity);
      this->insert(Glossary::ThermalConductivity1);
      this->insert(Glossary::ThermalConductivity2);
      this->insert(Glossary::ThermalConductivity3);
      this->insert(Glossary::ThermalExpansion);
      this->insert(Glossary::ThermalExpansion1);
      this->insert(Glossary::ThermalExpansion2);
      this->insert(Glossary::ThermalExpansion3);
      this->insert(Glossary::TorsionConstant);
      this->insert(Glossary::TrescaStress);
      this->insert(Glossary::UltimateTensileStrength);
      this->insert(Glossary::ViscoplasticStrain);
      this->insert(Glossary::VolumetricStrain);
      this->insert(Glossary::VonMisesStress);
      this->insert(Glossary::YieldStrength);
      this->insert(Glossary::YoungModulus);
      this->insert(Glossary::YoungModulus1);
      this->insert(Glossary::YoungModulus2);
      this->insert(Glossary::YoungModulus3);
    }  // end of Glossary::Glossary

    void Glossary::insert(const GlossaryEntry& e) {
      tfel::raise_if(!this->entries.insert(e).second,
                     "Glossary::insert: "
                     "'" +
                         e.getKey() + "' already declared");
      this->keys.push_back(e.getKey());
    }  // end of Glossary::insert

    bool Glossary::contains(const std::string& n) const {
      return this->findGlossaryEntry(n) != this->entries.end();
    }  // end of Glossary::contains

    const GlossaryEntry& Glossary::getGlossaryEntry(
        const std::string& n) const {
      const auto p = this->findGlossaryEntry(n);
      tfel::raise_if(
          p == this->entries.end(),
          "Glossary::getGlossaryEntry: no glossary entry matching '" + n + "'");
      return *p;
    }  // end of Glossary::contains

    const std::vector<std::string>& Glossary::getKeys() const {
      return this->keys;
    }  // end of Glossary::contains

    std::set<GlossaryEntry>::const_iterator Glossary::findGlossaryEntry(
        const std::string& n) const {
      using namespace std;
      set<GlossaryEntry>::const_iterator p;
      for (p = this->entries.begin(); p != this->entries.end(); ++p) {
        if (p->getKey() == n) {
          return p;
        }
        const auto& enames = p->getNames();
        if (find(enames.begin(), enames.end(), n) != enames.end()) {
          return p;
        }
      }
      return this->entries.end();
    }  // end of Glossary::findGlossaryEntry

  }  // end of namespace glossary

}  // end of namespace tfel
