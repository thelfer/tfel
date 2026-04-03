/*!
 * \file   Glossary.cxx
 * \author Thomas Helfer
 * \date   18/12/2024
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

void declareGlossary(pybind11::module_&);

void declareGlossary(pybind11::module_& m) {
  using namespace tfel::glossary;
  pybind11::class_<Glossary>(m, "Glossary")
      .def_static("getGlossary", Glossary::getGlossary,
                  pybind11::return_value_policy::reference)
      .def("contains", &Glossary::contains)
      .def_readonly_static("AxialDeformationGradient",
                           &Glossary::AxialDeformationGradient)
      .def_readonly_static("AxialGrowth", &Glossary::AxialGrowth)
      .def_readonly_static("AxialStrain", &Glossary::AxialStrain)
      .def_readonly_static("AxialStress", &Glossary::AxialStress)
      .def_readonly_static("B10BurnUp", &Glossary::B10BurnUp)
      .def_readonly_static("Broken", &Glossary::Broken)
      .def_readonly_static("BulkModulus", &Glossary::BulkModulus)
      .def_readonly_static("BurnUp_AtPercent", &Glossary::BurnUp_AtPercent)
      .def_readonly_static("BurnUp_MWJperTm", &Glossary::BurnUp_MWJperTm)
      .def_readonly_static("CohesiveForce", &Glossary::CohesiveForce)
      .def_readonly_static("ConvectiveHeatTransferCoefficient",
                           &Glossary::ConvectiveHeatTransferCoefficient)
      .def_readonly_static("CrossSectionArea", &Glossary::CrossSectionArea)
      .def_readonly_static("CylindricalStress", &Glossary::CylindricalStress)
      .def_readonly_static("Damage", &Glossary::Damage)
      .def_readonly_static("DeformationGradient",
                           &Glossary::DeformationGradient)
      .def_readonly_static("Displacement", &Glossary::Displacement)
      .def_readonly_static("DualStress", &Glossary::DualStress)
      .def_readonly_static("ElasticStrain", &Glossary::ElasticStrain)
      .def_readonly_static("Emissivity", &Glossary::Emissivity)
      .def_readonly_static("EquivalentPlasticStrain",
                           &Glossary::EquivalentPlasticStrain)
      .def_readonly_static("EquivalentStrain", &Glossary::EquivalentStrain)
      .def_readonly_static("EquivalentViscoplasticStrain",
                           &Glossary::EquivalentViscoplasticStrain)
      .def_readonly_static("FastNeutronFluence_01MeV",
                           &Glossary::FastNeutronFluence_01MeV)
      .def_readonly_static("FastNeutronFluence_1MeV",
                           &Glossary::FastNeutronFluence_1MeV)
      .def_readonly_static("FastNeutronFlux_01MeV",
                           &Glossary::FastNeutronFlux_01MeV)
      .def_readonly_static("FastNeutronFlux_1MeV",
                           &Glossary::FastNeutronFlux_1MeV)
      .def_readonly_static("FirstAxisSecondMomentArea",
                           &Glossary::FirstAxisSecondMomentArea)
      .def_readonly_static("FirstLameCoefficient",
                           &Glossary::FirstLameCoefficient)
      .def_readonly_static("FissionDensity", &Glossary::FissionDensity)
      .def_readonly_static("GaseousSwelling", &Glossary::GaseousSwelling)
      .def_readonly_static("GrainSize", &Glossary::GrainSize)
      .def_readonly_static("HeatFlux", &Glossary::HeatFlux)
      .def_readonly_static("HeatTransferCoefficient",
                           &Glossary::HeatTransferCoefficient)
      .def_readonly_static("HillStress", &Glossary::HillStress)
      .def_readonly_static("HydrostaticPressure",
                           &Glossary::HydrostaticPressure)
      .def_readonly_static("IrradiationDamage", &Glossary::IrradiationDamage)
      .def_readonly_static("IrradiationInducedSwelling",
                           &Glossary::IrradiationInducedSwelling)
      .def_readonly_static("IrradiationSwelling",
                           &Glossary::IrradiationSwelling)
      .def_readonly_static("IrradiationTemperature",
                           &Glossary::IrradiationTemperature)
      .def_readonly_static("KelvinTemperature", &Glossary::KelvinTemperature)
      .def_readonly_static("MassDensity", &Glossary::MassDensity)
      .def_readonly_static("MeanBurnUp_AtPercent",
                           &Glossary::MeanBurnUp_AtPercent)
      .def_readonly_static("MeanBurnUp_MWJperTm",
                           &Glossary::MeanBurnUp_MWJperTm)
      .def_readonly_static("MeanIrradiationTemperature",
                           &Glossary::MeanIrradiationTemperature)
      .def_readonly_static("MeanTemperature", &Glossary::MeanTemperature)
      .def_readonly_static("NeutronFluence", &Glossary::NeutronFluence)
      .def_readonly_static("NeutronFlux", &Glossary::NeutronFlux)
      .def_readonly_static("NormalStiffness", &Glossary::NormalStiffness)
      .def_readonly_static("NumberOfMoles", &Glossary::NumberOfMoles)
      .def_readonly_static("OpeningDisplacement",
                           &Glossary::OpeningDisplacement)
      .def_readonly_static("OrthotropicAxisX1", &Glossary::OrthotropicAxisX1)
      .def_readonly_static("OrthotropicAxisX2", &Glossary::OrthotropicAxisX2)
      .def_readonly_static("OrthotropicAxisY1", &Glossary::OrthotropicAxisY1)
      .def_readonly_static("OrthotropicAxisY2", &Glossary::OrthotropicAxisY2)
      .def_readonly_static("OrthotropicAxisZ1", &Glossary::OrthotropicAxisZ1)
      .def_readonly_static("OrthotropicAxisZ2", &Glossary::OrthotropicAxisZ2)
      .def_readonly_static("PlasticStrain", &Glossary::PlasticStrain)
      .def_readonly_static("PlateWidth", &Glossary::PlateWidth)
      .def_readonly_static("PoissonRatio", &Glossary::PoissonRatio)
      .def_readonly_static("PoissonRatio12", &Glossary::PoissonRatio12)
      .def_readonly_static("PoissonRatio13", &Glossary::PoissonRatio13)
      .def_readonly_static("PoissonRatio23", &Glossary::PoissonRatio23)
      .def_readonly_static("Porosity", &Glossary::Porosity)
      .def_readonly_static("PorosityIncreaseDueToInelasticFlow",
                           &Glossary::PorosityIncreaseDueToInelasticFlow)
      .def_readonly_static("PorosityIncreaseDueToNucleation",
                           &Glossary::PorosityIncreaseDueToNucleation)
      .def_readonly_static("PowerDensity", &Glossary::PowerDensity)
      .def_readonly_static("Pressure", &Glossary::Pressure)
      .def_readonly_static("PrincipalStress1", &Glossary::PrincipalStress1)
      .def_readonly_static("PrincipalStress2", &Glossary::PrincipalStress2)
      .def_readonly_static("PrincipalStress3", &Glossary::PrincipalStress3)
      .def_readonly_static("SecondAxisSecondMomentArea",
                           &Glossary::SecondAxisSecondMomentArea)
      .def_readonly_static("ShearModulus", &Glossary::ShearModulus)
      .def_readonly_static("ShearModulus12", &Glossary::ShearModulus12)
      .def_readonly_static("ShearModulus13", &Glossary::ShearModulus13)
      .def_readonly_static("ShearModulus23", &Glossary::ShearModulus23)
      .def_readonly_static("SolidSwelling", &Glossary::SolidSwelling)
      .def_readonly_static("SpecificHeat", &Glossary::SpecificHeat)
      .def_readonly_static("SphericalStress", &Glossary::SphericalStress)
      .def_readonly_static("Strain", &Glossary::Strain)
      .def_readonly_static("StrainMeasure", &Glossary::StrainMeasure)
      .def_readonly_static("Stress", &Glossary::Stress)
      .def_readonly_static("Swelling", &Glossary::Swelling)
      .def_readonly_static("TangentialStiffness",
                           &Glossary::TangentialStiffness)
      .def_readonly_static("Temperature", &Glossary::Temperature)
      .def_readonly_static("TemperatureGradient",
                           &Glossary::TemperatureGradient)
      .def_readonly_static("ThermalConductivity",
                           &Glossary::ThermalConductivity)
      .def_readonly_static("ThermalConductivity1",
                           &Glossary::ThermalConductivity1)
      .def_readonly_static("ThermalConductivity2",
                           &Glossary::ThermalConductivity2)
      .def_readonly_static("ThermalConductivity3",
                           &Glossary::ThermalConductivity3)
      .def_readonly_static("ThermalExpansion", &Glossary::ThermalExpansion)
      .def_readonly_static("ThermalExpansion1", &Glossary::ThermalExpansion1)
      .def_readonly_static("ThermalExpansion2", &Glossary::ThermalExpansion2)
      .def_readonly_static("ThermalExpansion3", &Glossary::ThermalExpansion3)
      .def_readonly_static("TorsionConstant", &Glossary::TorsionConstant)
      .def_readonly_static("TrescaStress", &Glossary::TrescaStress)
      .def_readonly_static("UltimateTensileStrength",
                           &Glossary::UltimateTensileStrength)
      .def_readonly_static("ViscoplasticStrain", &Glossary::ViscoplasticStrain)
      .def_readonly_static("VolumetricStrain", &Glossary::VolumetricStrain)
      .def_readonly_static("VonMisesStress", &Glossary::VonMisesStress)
      .def_readonly_static("YieldStrength", &Glossary::YieldStrength)
      .def_readonly_static("YoungModulus", &Glossary::YoungModulus)
      .def_readonly_static("YoungModulus1", &Glossary::YoungModulus1)
      .def_readonly_static("YoungModulus2", &Glossary::YoungModulus2)
      .def_readonly_static("YoungModulus3", &Glossary::YoungModulus3);
}
