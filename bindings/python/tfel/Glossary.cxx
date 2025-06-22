/*!
 * \file   PythonGlossary.cxx
 * \author Helfer Thomas
 * \date   09/06/14
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

void declareGlossary() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::glossary;
  class_<Glossary, noncopyable>("Glossary", no_init)
      .def("getGlossary", Glossary::getGlossary,
           return_value_policy<reference_existing_object>())
      .staticmethod("getGlossary")
      .def("contains", &Glossary::contains)
      .def_readonly("AxialGrowth", &Glossary::AxialGrowth,
                    "axial growth under irradiation")
      .def_readonly("AxialStrain", &Glossary::AxialStrain,
                    "la déformation axiale (cette grandeur n'a de sens que "
                    "pour les calculs en contraintes planes)")
      .def_readonly("AxialStress", &Glossary::AxialStress,
                    "la contrainte axiale")
      .def_readonly("B10BurnUp", &Glossary::B10BurnUp,
                    "le taux d'usure en \\(\\mbox{}^{10}B\\)")
      .def_readonly("BulkModulus", &Glossary::BulkModulus,
                    "the bulk modulus of an isotropic material")
      .def_readonly("BurnUp_AtPercent", &Glossary::BurnUp_AtPercent,
                    "le taux de combustion en atome pour cent")
      .def_readonly("BurnUp_MWJperTm", &Glossary::BurnUp_MWJperTm,
                    "le taux de combustion en MegaWattJour par tonne (métal)")
      .def_readonly("ConvectiveHeatTransferCoefficient",
                    &Glossary::ConvectiveHeatTransferCoefficient,
                    "le coefficient d'échange par convection")
      .def_readonly("CrossSectionArea", &Glossary::CrossSectionArea, "??")
      .def_readonly("CylindricalStress", &Glossary::CylindricalStress,
                    "les contraintes exprimées dans le repère cylindrique")
      .def_readonly("Damage", &Glossary::Damage,
                    "l'endommagement d'un matériau")
      .def_readonly("Displacement", &Glossary::Displacement, "le déplacement")
      .def_readonly("ElasticStrain", &Glossary::ElasticStrain,
                    "la déformation élastique")
      .def_readonly("Emissivity", &Glossary::Emissivity,
                    "Emissivity of the surface of a material is its "
                    "effectiveness in emitting energy as thermal radiation")
      .def_readonly("EquivalentPlasticStrain",
                    &Glossary::EquivalentPlasticStrain,
                    "la déformation plastique équivalente")
      .def_readonly("EquivalentStrain", &Glossary::EquivalentStrain,
                    "la somme des déformations plastiques ou viscoplastiques "
                    "équivalentes")
      .def_readonly("EquivalentViscoplasticStrain",
                    &Glossary::EquivalentViscoplasticStrain,
                    "la déformation viscoplastique équivalente")
      .def_readonly("FastNeutronFluence_01MeV",
                    &Glossary::FastNeutronFluence_01MeV, "la fluence rapide")
      .def_readonly("FastNeutronFluence_1MeV",
                    &Glossary::FastNeutronFluence_1MeV, "la fluence rapide")
      .def_readonly("FastNeutronFlux_01MeV", &Glossary::FastNeutronFlux_01MeV,
                    "le flux de neutron rapide")
      .def_readonly("FastNeutronFlux_1MeV", &Glossary::FastNeutronFlux_1MeV,
                    "le flux de neutron rapide")
      .def_readonly("FirstAxisSecondMomentArea",
                    &Glossary::FirstAxisSecondMomentArea, "??")
      .def_readonly("FirstLameCoefficient", &Glossary::FirstLameCoefficient,
                    "le premier coefficient de Lamé d'un matériau isotrope")
      .def_readonly("FissionDensity", &Glossary::FissionDensity,
                    "la densité de fission")
      .def_readonly("GaseousSwelling", &Glossary::GaseousSwelling,
                    "un gonflement imposé dû à des produits de fission gazeux")
      .def_readonly("GrainSize", &Glossary::GrainSize, "la taille de grain")
      .def_readonly("HeatFlux", &Glossary::HeatFlux, "le flux de chaleur.")
      .def_readonly(
          "HeatTransferCoefficient", &Glossary::HeatTransferCoefficient,
          "The heat transfer coefficient is the proportionality constant "
          "between the heat flux and the temperature difference")
      .def_readonly("HillStress", &Glossary::HillStress,
                    "la contrainte équivalente au sens de Hill")
      .def_readonly("HydrostaticPressure", &Glossary::HydrostaticPressure,
                    "la pression hydrostatique (égale au tiers de la trace du "
                    "tenseur des contraintes)")
      .def_readonly("IrradiationDamage", &Glossary::IrradiationDamage,
                    "le dommage due à l'irradiation, mesuré par le nombre de "
                    "déplacement moyen de chaque atome (dpa)")
      .def_readonly("IrradiationInducedSwelling",
                    &Glossary::IrradiationInducedSwelling,
                    "un gonflement imposé  dû à des dégâts d'irradiation")
      .def_readonly("IrradiationSwelling", &Glossary::IrradiationSwelling,
                    "un gonflement imposé  dû à des dégâts d'irradiation")
      .def_readonly(
          "IrradiationTemperature", &Glossary::IrradiationTemperature,
          "la température moyenne (dans le temps) au cours de l'irradiation")
      .def_readonly("KelvinTemperature", &Glossary::KelvinTemperature,
                    "la température")
      .def_readonly("MassDensity", &Glossary::MassDensity,
                    "la densité massique")
      .def_readonly("MeanBurnUp_AtPercent", &Glossary::MeanBurnUp_AtPercent,
                    "le taux de combustion en atome pour cent moyen (au sens "
                    "spatial) pour un matériau donné")
      .def_readonly("MeanBurnUp_MWJperTm", &Glossary::MeanBurnUp_MWJperTm,
                    "le taux de combustion en MegaWattJour par tonne (métal) "
                    "moyen (au sens spatial) pour un matériau donné")
      .def_readonly("MeanIrradiationTemperature",
                    &Glossary::MeanIrradiationTemperature,
                    "la température moyenne dans le temps et dans un domaine "
                    "\\(\\Omega\\) donné")
      .def_readonly(
          "MeanTemperature", &Glossary::MeanTemperature,
          "la température moyenne dans un domaine \\(\\Omega\\) donné. ")
      .def_readonly("NeutronFluence", &Glossary::NeutronFluence, "la fluence")
      .def_readonly("NeutronFlux", &Glossary::NeutronFlux, "le flux de neutron")
      .def_readonly("NormalStiffness", &Glossary::NormalStiffness,
                    "la raideur normale pour un modèle de zone cohésive")
      .def_readonly("NumberOfMoles", &Glossary::NumberOfMoles,
                    "le nombre de moles d'une substance")
      .def_readonly("OrthotropicAxisX1", &Glossary::OrthotropicAxisX1,
                    "la première coordonnée du premier axe d'orthotropie")
      .def_readonly("OrthotropicAxisX2", &Glossary::OrthotropicAxisX2,
                    "la première coordonnée du second axe d'orthotropie")
      .def_readonly("OrthotropicAxisY1", &Glossary::OrthotropicAxisY1,
                    "la deuxième coordonnée du premier axe d'orthotropie")
      .def_readonly("OrthotropicAxisY2", &Glossary::OrthotropicAxisY2,
                    "la deuxième coordonnée du second axe d'orthotropie")
      .def_readonly("OrthotropicAxisZ1", &Glossary::OrthotropicAxisZ1,
                    "la troisième coordonnée du premier axe d'orthotropie")
      .def_readonly("OrthotropicAxisZ2", &Glossary::OrthotropicAxisZ2,
                    "la troisième coordonnée du second axe d'orthotropie")
      .def_readonly("PlasticStrain", &Glossary::PlasticStrain,
                    "la déformation plastique")
      .def_readonly("PlateWidth", &Glossary::PlateWidth, "??")
      .def_readonly("PoissonRatio", &Glossary::PoissonRatio,
                    "the Poisson ratio of an isotropic material")
      .def_readonly("PoissonRatio12", &Glossary::PoissonRatio12,
                    "le coefficient de Poisson d'un matériau orthotrope "
                    "relatif aux première et deuxième directions d'orthotropie")
      .def_readonly(
          "PoissonRatio13", &Glossary::PoissonRatio13,
          "le coefficient de Poisson d'un matériau orthotrope relatif aux "
          "première et troisième directions d'orthotropie")
      .def_readonly(
          "PoissonRatio23", &Glossary::PoissonRatio23,
          "le coefficient de Poisson d'un matériau orthotrope relatif aux "
          "deuxième et troisième directions d'orthotropie")
      .def_readonly("Porosity", &Glossary::Porosity, "la porosité du matériau")
      .def_readonly("PowerDensity", &Glossary::PowerDensity,
                    "la densité de puissance")
      .def_readonly("Pressure", &Glossary::Pressure, "la pression d'un gaz")
      .def_readonly("PrincipalStress1", &Glossary::PrincipalStress1,
                    "la première contrainte principale")
      .def_readonly("PrincipalStress2", &Glossary::PrincipalStress2,
                    "la deuxième contrainte principale")
      .def_readonly("PrincipalStress3", &Glossary::PrincipalStress3,
                    "la troisième contrainte principale")
      .def_readonly("SecondAxisSecondMomentArea",
                    &Glossary::SecondAxisSecondMomentArea, "??")
      .def_readonly("ShearModulus", &Glossary::ShearModulus,
                    "the shear modulus of an isotropic material")
      .def_readonly("ShearModulus12", &Glossary::ShearModulus12,
                    "le module de cisaillement d'un matériau orthotrope "
                    "relatif aux première et deuxième directions d'orthotropie")
      .def_readonly(
          "ShearModulus13", &Glossary::ShearModulus13,
          "le module de cisaillement d'un matériau orthotrope relatif aux "
          "première et troisième directions d'orthotropie")
      .def_readonly(
          "ShearModulus23", &Glossary::ShearModulus23,
          "le module de cisaillement d'un matériau orthotrope relatif aux "
          "deuxième et troisième directions d'orthotropie")
      .def_readonly("SolidSwelling", &Glossary::SolidSwelling,
                    "un gonflement imposé dû à des produits de fission solides")
      .def_readonly("SpecificHeat", &Glossary::SpecificHeat,
                    "la chaleur spécifique")
      .def_readonly("SphericalStress", &Glossary::SphericalStress,
                    "les contraintes exprimées dans le repère sphérique")
      .def_readonly("Strain", &Glossary::Strain, "la déformation")
      .def_readonly("Stress", &Glossary::Stress, "les contraintes")
      .def_readonly("Swelling", &Glossary::Swelling, "un gonflement imposé")
      .def_readonly("TangentialStiffness", &Glossary::TangentialStiffness,
                    "la raideur tangentielle pour un modèle de zone cohésive")
      .def_readonly("Temperature", &Glossary::Temperature, "la température")
      .def_readonly("TemperatureGradient", &Glossary::TemperatureGradient,
                    "le gradient de température")
      .def_readonly("ThermalConductivity", &Glossary::ThermalConductivity,
                    "la conductivité thermique d'un matériau isotrope")
      .def_readonly("ThermalConductivity1", &Glossary::ThermalConductivity1,
                    "la conductivité thermique d'un matériau orthotrope "
                    "suivant la première direction d'orthotropie")
      .def_readonly("ThermalConductivity2", &Glossary::ThermalConductivity2,
                    "la conductivité thermique d'un matériau orthotrope "
                    "suivant la deuxième direction d'orthotropie")
      .def_readonly("ThermalConductivity3", &Glossary::ThermalConductivity3,
                    "la conductivité thermique d'un matériau orthotrope "
                    "suivant la troisième direction d'orthotropie")
      .def_readonly(
          "ThermalExpansion", &Glossary::ThermalExpansion,
          "le coefficient de dilatation linéique d'un matériau isotrope")
      .def_readonly("ThermalExpansion1", &Glossary::ThermalExpansion1,
                    "le coefficient de dilatation linéique d'un matériau "
                    "orthotrope suivant la première direction d'orthotropie")
      .def_readonly("ThermalExpansion2", &Glossary::ThermalExpansion2,
                    "le coefficient de dilatation linéique d'un matériau "
                    "orthotrope suivant la deuxième direction d'orthotropie")
      .def_readonly("ThermalExpansion3", &Glossary::ThermalExpansion3,
                    "le coefficient de dilatation linéique d'un matériau "
                    "orthotrope suivant la troisième direction d'orthotropie")
      .def_readonly("TorsionConstant", &Glossary::TorsionConstant, "??")
      .def_readonly("TrescaStress", &Glossary::TrescaStress,
                    "la contrainte équivalente au sens de Tresca")
      .def_readonly(
          "UltimateTensileStrength", &Glossary::UltimateTensileStrength,
          "la valeur maximale de la contrainte qu'un materiau peut supporter")
      .def_readonly("ViscoplasticStrain", &Glossary::ViscoplasticStrain,
                    "la déformation viscoplastique")
      .def_readonly("VolumetricStrain", &Glossary::VolumetricStrain,
                    "la dilatation volumique")
      .def_readonly("VonMisesStress", &Glossary::VonMisesStress,
                    "la contrainte équivalente au sens de Von Mises")
      .def_readonly("YieldStrength", &Glossary::YieldStrength,
                    "la limite d'élasticité")
      .def_readonly("YoungModulus", &Glossary::YoungModulus,
                    "the Young modulus of an isotropic material")
      .def_readonly("YoungModulus1", &Glossary::YoungModulus1,
                    "le module d'Young d'un matériau orthotrope suivant la "
                    "première direction d'orthotropie")
      .def_readonly("YoungModulus2", &Glossary::YoungModulus2,
                    "le module d'Young d'un matériau orthotrope suivant la "
                    "deuxième direction d'orthotropie")
      .def_readonly("YoungModulus3", &Glossary::YoungModulus3,
                    "le module d'Young d'un matériau orthotrope suivant la "
                    "troisième direction d'orthotropie");
}
