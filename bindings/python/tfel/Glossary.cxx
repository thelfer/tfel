/*!
 * \file   PythonGlossary.cxx
 * \author Thomas Helfer
 * \date   09/06/14
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
      .def_readonly("AxialDeformationGradient",
                    &Glossary::AxialDeformationGradient,
                    "the axial component of the deformation gradient")
      .def_readonly("AxialGrowth", &Glossary::AxialGrowth,
                    "axial growth under irradiation")
      .def_readonly("AxialStrain", &Glossary::AxialStrain, "the axial strain")
      .def_readonly("AxialStress", &Glossary::AxialStress, "the axial stress")
      .def_readonly(
          "B10BurnUp", &Glossary::B10BurnUp,
          "the burn-up of an absorant material containing \\(\\mbox{}^{10}B\\)")
      .def_readonly("Broken", &Glossary::Broken, "a material failure indicator")
      .def_readonly("BulkModulus", &Glossary::BulkModulus,
                    "the bulk modulus of an isotropic material")
      .def_readonly("BurnUp_AtPercent", &Glossary::BurnUp_AtPercent,
                    "the burn-up in at.%")
      .def_readonly("BurnUp_MWJperTm", &Glossary::BurnUp_MWJperTm,
                    "the burn-up in MegaWattJour per tons of metals")
      .def_readonly("CohesiveForce", &Glossary::CohesiveForce,
                    "cohesive force for cohesize zone models")
      .def_readonly("ConvectiveHeatTransferCoefficient",
                    &Glossary::ConvectiveHeatTransferCoefficient,
                    "the heat transfer coefficient by convection")
      .def_readonly("CrossSectionArea", &Glossary::CrossSectionArea, "??")
      .def_readonly("CylindricalStress", &Glossary::CylindricalStress,
                    "the stress in the cylindrical frame")
      .def_readonly("Damage", &Glossary::Damage,
                    "the damage, generally between 0 (sound material) and 1 "
                    "(broken material)")
      .def_readonly("DeformationGradient", &Glossary::DeformationGradient,
                    "gradient of the transformation")
      .def_readonly("Displacement", &Glossary::Displacement, "the displacement")
      .def_readonly("DualStress", &Glossary::DualStress,
                    "the dual stress of the strain measure")
      .def_readonly("ElasticStrain", &Glossary::ElasticStrain,
                    "The elastic strain")
      .def_readonly("Emissivity", &Glossary::Emissivity,
                    "the emissivity of the surface of a material is its "
                    "effectiveness in emitting energy as thermal radiation")
      .def_readonly("EquivalentPlasticStrain",
                    &Glossary::EquivalentPlasticStrain,
                    "the equivalent plastic strain")
      .def_readonly(
          "EquivalentStrain", &Glossary::EquivalentStrain,
          "the sum of all plastic and viscoplastic equivalent strains")
      .def_readonly("EquivalentViscoplasticStrain",
                    &Glossary::EquivalentViscoplasticStrain,
                    "the equivalent viscoplastic strain")
      .def_readonly("FastNeutronFluence_01MeV",
                    &Glossary::FastNeutronFluence_01MeV,
                    "the fast neutron fluence, where the limit for fast "
                    "neutron is 0.1 MeV")
      .def_readonly(
          "FastNeutronFluence_1MeV", &Glossary::FastNeutronFluence_1MeV,
          "the fast neutron fluence, where the limit for fast neutron is 1 MeV")
      .def_readonly("FastNeutronFlux_01MeV", &Glossary::FastNeutronFlux_01MeV,
                    "the fast neutron fluence")
      .def_readonly("FastNeutronFlux_1MeV", &Glossary::FastNeutronFlux_1MeV,
                    "the fast neutron fluence")
      .def_readonly("FirstAxisSecondMomentArea",
                    &Glossary::FirstAxisSecondMomentArea, "??")
      .def_readonly("FirstLameCoefficient", &Glossary::FirstLameCoefficient,
                    "the first Lamé's coefficient of an isotropic material")
      .def_readonly("FissionDensity", &Glossary::FissionDensity,
                    "the fission density")
      .def_readonly("GaseousSwelling", &Glossary::GaseousSwelling,
                    "swelling du to gazeous fission products")
      .def_readonly("GrainSize", &Glossary::GrainSize, "the grain size")
      .def_readonly("HeatFlux", &Glossary::HeatFlux,
                    "the heat flux, generally in the current configuration.")
      .def_readonly(
          "HeatTransferCoefficient", &Glossary::HeatTransferCoefficient,
          "the heat transfer coefficient is the proportionality constant "
          "between the heat flux and the temperature difference")
      .def_readonly("HillStress", &Glossary::HillStress,
                    "the Hill equivalent stress")
      .def_readonly("HydrostaticPressure", &Glossary::HydrostaticPressure,
                    "the hydrostatic pressure, defined as the third of the "
                    "trace of the stress tensor")
      .def_readonly("IrradiationDamage", &Glossary::IrradiationDamage,
                    "the irradiation damage, measure by the mean number of "
                    "displacements of each atoms")
      .def_readonly("IrradiationInducedSwelling",
                    &Glossary::IrradiationInducedSwelling,
                    "swelling du to irradiation damage")
      .def_readonly("IrradiationSwelling", &Glossary::IrradiationSwelling,
                    "swelling du to irradiation damage")
      .def_readonly("IrradiationTemperature", &Glossary::IrradiationTemperature,
                    "the mean temperature (in time) of the temperature during "
                    "the irradiation")
      .def_readonly("KelvinTemperature", &Glossary::KelvinTemperature,
                    "the temperature")
      .def_readonly("MassDensity", &Glossary::MassDensity, "the mass density")
      .def_readonly("MeanBurnUp_AtPercent", &Glossary::MeanBurnUp_AtPercent,
                    "the spatial average of the  burn-up in at.%")
      .def_readonly("MeanBurnUp_MWJperTm", &Glossary::MeanBurnUp_MWJperTm,
                    "the spatial average of the  burn-up in MegaWattJour per "
                    "tons of metals")
      .def_readonly(
          "MeanIrradiationTemperature", &Glossary::MeanIrradiationTemperature,
          "The mean temperature  in time over a given domain \\(\\Omega\\)")
      .def_readonly("MeanTemperature", &Glossary::MeanTemperature,
                    "The mean temperature over a given domain \\(\\Omega\\)")
      .def_readonly("NeutronFluence", &Glossary::NeutronFluence,
                    "the neutron fluence")
      .def_readonly("NeutronFlux", &Glossary::NeutronFlux, "the neutron flux")
      .def_readonly("NormalStiffness", &Glossary::NormalStiffness,
                    "the normal elastic stiffness for a cohesive zone model")
      .def_readonly("NumberOfMoles", &Glossary::NumberOfMoles,
                    "the amount of substance")
      .def_readonly("OpeningDisplacement", &Glossary::OpeningDisplacement,
                    "opening displacement in cohesive zone models")
      .def_readonly("OrthotropicAxisX1", &Glossary::OrthotropicAxisX1,
                    "the first coordinate of the vector giving the first axis "
                    "of orthotropy")
      .def_readonly("OrthotropicAxisX2", &Glossary::OrthotropicAxisX2,
                    "the first coordinate of the vector giving the second axis "
                    "of orthotropy")
      .def_readonly("OrthotropicAxisY1", &Glossary::OrthotropicAxisY1,
                    "the second coordinate of the vector giving the first axis "
                    "of orthotropy")
      .def_readonly("OrthotropicAxisY2", &Glossary::OrthotropicAxisY2,
                    "the second coordinate of the vector giving the second "
                    "axis of orthotropy")
      .def_readonly("OrthotropicAxisZ1", &Glossary::OrthotropicAxisZ1,
                    "the third coordinate of the vector giving the first axis "
                    "of orthotropy")
      .def_readonly("OrthotropicAxisZ2", &Glossary::OrthotropicAxisZ2,
                    "the third coordinate of the vector giving the second axis "
                    "of orthotropy")
      .def_readonly("PlasticStrain", &Glossary::PlasticStrain,
                    "The plastic strain")
      .def_readonly("PlateWidth", &Glossary::PlateWidth, "??")
      .def_readonly("PoissonRatio", &Glossary::PoissonRatio,
                    "the Poisson ratio of an isotropic material")
      .def_readonly("PoissonRatio12", &Glossary::PoissonRatio12,
                    "the Poisson's coefficient between the first and second "
                    "directions of orthotropy")
      .def_readonly("PoissonRatio13", &Glossary::PoissonRatio13,
                    "the Poisson's coefficient between the first and third "
                    "directions of orthotropy")
      .def_readonly("PoissonRatio23", &Glossary::PoissonRatio23,
                    "the Poisson's coefficient between the second and third "
                    "directions of orthotropy")
      .def_readonly("Porosity", &Glossary::Porosity, "Porosity of the material")
      .def_readonly("PorosityIncreaseDueToInelasticFlow",
                    &Glossary::PorosityIncreaseDueToInelasticFlow,
                    "Part of the porosity increase du to inelastic flow")
      .def_readonly("PorosityIncreaseDueToNucleation",
                    &Glossary::PorosityIncreaseDueToNucleation,
                    "Part of the porosity increase du to nucleation")
      .def_readonly("PowerDensity", &Glossary::PowerDensity,
                    "the power density, generally in the current configuration")
      .def_readonly("Pressure", &Glossary::Pressure, "the pressure of a gaz")
      .def_readonly("PrincipalStress1", &Glossary::PrincipalStress1,
                    "the first principal stress")
      .def_readonly("PrincipalStress2", &Glossary::PrincipalStress2,
                    "the third principal stress")
      .def_readonly("PrincipalStress3", &Glossary::PrincipalStress3,
                    "the third principal stress")
      .def_readonly("SecondAxisSecondMomentArea",
                    &Glossary::SecondAxisSecondMomentArea, "??")
      .def_readonly("ShearModulus", &Glossary::ShearModulus,
                    "the shear modulus of an isotropic material")
      .def_readonly("ShearModulus12", &Glossary::ShearModulus12,
                    "the shear moduls between the first and second directions "
                    "of orthotropy")
      .def_readonly("ShearModulus13", &Glossary::ShearModulus13,
                    "the shear moduls between the first and third directions "
                    "of orthotropy")
      .def_readonly("ShearModulus23", &Glossary::ShearModulus23,
                    "the shear moduls between the second and third directions "
                    "of orthotropy")
      .def_readonly("SolidSwelling", &Glossary::SolidSwelling,
                    "swelling du to solid fission products")
      .def_readonly("SpecificHeat", &Glossary::SpecificHeat,
                    "the specific heat")
      .def_readonly("SphericalStress", &Glossary::SphericalStress,
                    "the stress in a spherical frame")
      .def_readonly("Strain", &Glossary::Strain, "the strain tensor")
      .def_readonly("StrainMeasure", &Glossary::StrainMeasure,
                    "a generic entry for a strain measure (for instance, the "
                    "Henky strain or the Green-Lagrange strain)")
      .def_readonly("Stress", &Glossary::Stress, "the stress tensor")
      .def_readonly("Swelling", &Glossary::Swelling, "an imposed swelling")
      .def_readonly(
          "TangentialStiffness", &Glossary::TangentialStiffness,
          "the tangential elastic stiffness for a cohesive zone model")
      .def_readonly("Temperature", &Glossary::Temperature, "the temperature")
      .def_readonly(
          "TemperatureGradient", &Glossary::TemperatureGradient,
          "the temperature gradient, generally in the current configuration")
      .def_readonly("ThermalConductivity", &Glossary::ThermalConductivity,
                    "the thermal conductivity of an isotropic material")
      .def_readonly("ThermalConductivity1", &Glossary::ThermalConductivity1,
                    "the thermal conductivity of an orthotropic material along "
                    "the first axis of orthotropy")
      .def_readonly("ThermalConductivity2", &Glossary::ThermalConductivity2,
                    "the thermal conductivity of an orthotropic material along "
                    "the second axis of orthotropy")
      .def_readonly("ThermalConductivity3", &Glossary::ThermalConductivity3,
                    "the thermal conductivity of an orthotropic material along "
                    "the third axis of orthotropy")
      .def_readonly("ThermalExpansion", &Glossary::ThermalExpansion,
                    "the mean thermal expansion coefficient")
      .def_readonly("ThermalExpansion1", &Glossary::ThermalExpansion1,
                    "the mean thermal expansion coefficient along the first "
                    "orthotropy direction")
      .def_readonly("ThermalExpansion2", &Glossary::ThermalExpansion2,
                    "the mean thermal expansion coefficient along the second "
                    "orthotropy direction")
      .def_readonly("ThermalExpansion3", &Glossary::ThermalExpansion3,
                    "the mean thermal expansion coefficient along the third "
                    "orthotropy direction")
      .def_readonly("TorsionConstant", &Glossary::TorsionConstant, "??")
      .def_readonly("TrescaStress", &Glossary::TrescaStress,
                    "the Tresca equivalent stress")
      .def_readonly("UltimateTensileStrength",
                    &Glossary::UltimateTensileStrength,
                    "the maximum stress that a material can withstand while "
                    "being stretched or pulled before breaking")
      .def_readonly("ViscoplasticStrain", &Glossary::ViscoplasticStrain,
                    "The viscoplatic strain")
      .def_readonly(
          "VolumetricStrain", &Glossary::VolumetricStrain,
          "the volumetric strain, defined as the trace of the strain tensor")
      .def_readonly("VonMisesStress", &Glossary::VonMisesStress,
                    "the von Mises equivalent stress")
      .def_readonly("YieldStrength", &Glossary::YieldStrength,
                    "the stress corresponding to the yield point at which the "
                    "material begins to deform plastically")
      .def_readonly("YoungModulus", &Glossary::YoungModulus,
                    "the Young's modulus of an isotropic material")
      .def_readonly("YoungModulus1", &Glossary::YoungModulus1,
                    "the Young's modulus of an isotropic material along the "
                    "first direction of orthotropy")
      .def_readonly("YoungModulus2", &Glossary::YoungModulus2,
                    "the Young's modulus of an isotropic material along the "
                    "second direction of orthotropy")
      .def_readonly("YoungModulus3", &Glossary::YoungModulus3,
                    "the Young's modulus of an isotropic material along the "
                    "third direction of orthotropy");
}
