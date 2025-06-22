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

namespace tfel::glossary {

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
      Glossary::names,
      Glossary::names + 1,
      "",
      "scalar",
      "the axial component of the deformation gradient",
      "" /* no 'description' defined */,
      "This quantity is only meaningful under on of the plane stress modelling "
      "hypotheses.",
      "",
      "");

  const GlossaryEntry Glossary::AxialGrowth("AxialGrowth",
                                            Glossary::names + 1,
                                            Glossary::names + 2,
                                            "",
                                            "scalar",
                                            "axial growth under irradiation",
                                            "" /* no 'description' defined */,
                                            "" /* no 'notes' defined */,
                                            "",
                                            "");

  const GlossaryEntry Glossary::AxialStrain(
      "AxialStrain",
      Glossary::names + 2,
      Glossary::names + 3,
      "",
      "scalar",
      "the axial strain",
      "" /* no 'description' defined */,
      "This quantity is only meaningful under on of the plane stress modelling "
      "hypotheses.",
      "",
      "");

  const GlossaryEntry Glossary::AxialStress(
      "AxialStress",
      Glossary::names + 3,
      Glossary::names + 4,
      "",
      "scalar",
      "the axial stress",
      "" /* no 'description' defined */,
      "This quantity is only meaningful under the axisymmetrical generalized "
      "plane stress modelling hypothesis.",
      "",
      "");

  const GlossaryEntry Glossary::B10BurnUp(
      "B10BurnUp",
      Glossary::names + 4,
      Glossary::names + 5,
      "SI:m^{-3}",
      "scalar",
      "the burn-up of an absorant material containing \\(\\mbox{}^{10}B\\)",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::Broken("Broken",
                                       Glossary::names + 5,
                                       Glossary::names + 6,
                                       "",
                                       "scalar",
                                       "a material failure indicator",
                                       "" /* no 'description' defined */,
                                       "" /* no 'notes' defined */,
                                       "",
                                       "");

  const GlossaryEntry Glossary::BulkModulus(
      "BulkModulus",
      Glossary::names + 6,
      Glossary::names + 7,
      "SI:Pa",
      "scalar",
      "the bulk modulus of an isotropic material",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::BurnUp_AtPercent(
      "BurnUp_AtPercent",
      Glossary::names + 7,
      Glossary::names + 8,
      "SI:at./100",
      "scalar",
      "the burn-up in at.%",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::BurnUp_MWJperTm(
      "BurnUp_MWJperTm",
      Glossary::names + 8,
      Glossary::names + 9,
      "SI:MWJ/tm",
      "scalar",
      "the burn-up in MegaWattJour per tons of metals",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::CohesiveForce(
      "CohesiveForce",
      Glossary::names + 9,
      Glossary::names + 10,
      "SI:Newton",
      "vector",
      "cohesive force for cohesize zone models",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::ConvectiveHeatTransferCoefficient(
      "ConvectiveHeatTransferCoefficient",
      Glossary::names + 10,
      Glossary::names + 11,
      "SI:W.m^{-2}.K^{-1}",
      "scalar",
      "the heat transfer coefficient by convection",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::CrossSectionArea(
      "CrossSectionArea",
      Glossary::names + 11,
      Glossary::names + 12,
      "SI:m^{2}",
      "scalar",
      "??",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::CylindricalStress(
      "CylindricalStress",
      Glossary::names + 12,
      Glossary::names + 13,
      "SI:Pa",
      "tensor",
      "the stress in the cylindrical frame",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::Damage("Damage",
                                       Glossary::names + 13,
                                       Glossary::names + 14,
                                       "",
                                       "scalar",
                                       "the damage, generally between 0 (sound "
                                       "material) and 1 (broken material)",
                                       "" /* no 'description' defined */,
                                       "" /* no 'notes' defined */,
                                       "",
                                       "");

  const GlossaryEntry Glossary::DeformationGradient(
      "DeformationGradient",
      Glossary::names + 14,
      Glossary::names + 15,
      "",
      "tensor",
      "gradient of the transformation",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::Displacement("Displacement",
                                             Glossary::names + 15,
                                             Glossary::names + 16,
                                             "SI:m",
                                             "vector",
                                             "the displacement",
                                             "" /* no 'description' defined */,
                                             "" /* no 'notes' defined */,
                                             "",
                                             "");

  const GlossaryEntry Glossary::DualStress(
      "DualStress",
      Glossary::names + 16,
      Glossary::names + 17,
      "SI:stress",
      "tensor",
      "the dual stress of the strain measure",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::ElasticStrain("ElasticStrain",
                                              Glossary::names + 17,
                                              Glossary::names + 18,
                                              "",
                                              "tensor",
                                              "The elastic strain",
                                              "" /* no 'description' defined */,
                                              "" /* no 'notes' defined */,
                                              "",
                                              "");

  const GlossaryEntry Glossary::Emissivity(
      "Emissivity",
      Glossary::names + 18,
      Glossary::names + 19,
      "",
      "scalar",
      "the emissivity of the surface of a material is its effectiveness in "
      "emitting energy as thermal radiation",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "SI:1");

  const GlossaryEntry Glossary::EquivalentPlasticStrain(
      "EquivalentPlasticStrain",
      Glossary::names + 19,
      Glossary::names + 20,
      "",
      "scalar",
      "the equivalent plastic strain",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::EquivalentStrain(
      "EquivalentStrain",
      Glossary::names + 20,
      Glossary::names + 21,
      "",
      "scalar",
      "the sum of all plastic and viscoplastic equivalent strains",
      "" /* no 'description' defined */,
      "This quantity has no direct physical meaning.",
      "",
      "");

  const GlossaryEntry Glossary::EquivalentViscoplasticStrain(
      "EquivalentViscoplasticStrain",
      Glossary::names + 21,
      Glossary::names + 22,
      "",
      "scalar",
      "the equivalent viscoplastic strain",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::FastNeutronFluence_01MeV(
      "FastNeutronFluence_01MeV",
      Glossary::names + 22,
      Glossary::names + 23,
      "SI:n.m^{-2}",
      "scalar",
      "the fast neutron fluence, where the limit for fast neutron is 0.1 MeV",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::FastNeutronFluence_1MeV(
      "FastNeutronFluence_1MeV",
      Glossary::names + 23,
      Glossary::names + 24,
      "SI:n.m^{-2}",
      "scalar",
      "the fast neutron fluence, where the limit for fast neutron is 1 MeV",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::FastNeutronFlux_01MeV(
      "FastNeutronFlux_01MeV",
      Glossary::names + 24,
      Glossary::names + 25,
      "SI:n.m^{-2}.s^{-1}",
      "scalar",
      "the fast neutron fluence",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::FastNeutronFlux_1MeV(
      "FastNeutronFlux_1MeV",
      Glossary::names + 25,
      Glossary::names + 26,
      "SI:n.m^{-2}.s^{-1}",
      "scalar",
      "the fast neutron fluence",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::FirstAxisSecondMomentArea(
      "FirstAxisSecondMomentArea",
      Glossary::names + 26,
      Glossary::names + 27,
      "SI:m^{4}",
      "scalar",
      "??",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::FirstLameCoefficient(
      "FirstLameCoefficient",
      Glossary::names + 27,
      Glossary::names + 28,
      "SI:Pa",
      "scalar",
      "the first Lamé's coefficient of an isotropic material",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::FissionDensity(
      "FissionDensity",
      Glossary::names + 28,
      Glossary::names + 29,
      "SI:m^{-3}",
      "scalar",
      "the fission density",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::GaseousSwelling(
      "GaseousSwelling",
      Glossary::names + 29,
      Glossary::names + 30,
      "",
      "scalar",
      "swelling du to gazeous fission products",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::GrainSize("GrainSize",
                                          Glossary::names + 30,
                                          Glossary::names + 31,
                                          "SI:m",
                                          "scalar",
                                          "the grain size",
                                          "" /* no 'description' defined */,
                                          "" /* no 'notes' defined */,
                                          "SI:0",
                                          "");

  const GlossaryEntry Glossary::HeatFlux(
      "HeatFlux",
      Glossary::names + 31,
      Glossary::names + 32,
      "SI:J.m^{-2}.s^{-1}",
      "vector",
      "the heat flux, generally in the current configuration.",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::HeatTransferCoefficient(
      "HeatTransferCoefficient",
      Glossary::names + 32,
      Glossary::names + 33,
      "SI:W.m^{-2}.K^{-1}",
      "scalar",
      "the heat transfer coefficient is the proportionality constant between "
      "the heat flux and the temperature difference",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::HillStress("HillStress",
                                           Glossary::names + 33,
                                           Glossary::names + 34,
                                           "SI:Pa",
                                           "tensor",
                                           "the Hill equivalent stress",
                                           "" /* no 'description' defined */,
                                           "" /* no 'notes' defined */,
                                           "SI:0",
                                           "");

  const GlossaryEntry Glossary::HydrostaticPressure(
      "HydrostaticPressure",
      Glossary::names + 34,
      Glossary::names + 35,
      "SI:Pa",
      "tensor",
      "the hydrostatic pressure, defined as the third of the trace of the "
      "stress tensor",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::IrradiationDamage(
      "IrradiationDamage",
      Glossary::names + 35,
      Glossary::names + 36,
      "SI:dpa",
      "scalar",
      "the irradiation damage, measure by the mean number of displacements of "
      "each atoms",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::IrradiationInducedSwelling(
      "IrradiationInducedSwelling",
      Glossary::names + 36,
      Glossary::names + 37,
      "",
      "scalar",
      "swelling du to irradiation damage",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::IrradiationSwelling(
      "IrradiationSwelling",
      Glossary::names + 37,
      Glossary::names + 38,
      "",
      "scalar",
      "swelling du to irradiation damage",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::IrradiationTemperature(
      "IrradiationTemperature",
      Glossary::names + 38,
      Glossary::names + 39,
      "SI:K",
      "scalar",
      "the mean temperature (in time) of the temperature during the "
      "irradiation",
      "This temperature is defined as follows:"
      "@^separator^@"
      "\\["
      "@^separator^@"
      "\\average{T}\\paren{t,\\vec{r}}  = "
      "\\Frac{1}{t-t_{0}}\\int_{t_{0}}^{t}T\\paren{u,\\vec{r}}\\,\\dtot\\, u"
      "@^separator^@"
      "\\]"
      "@^separator^@"
      "where"
      "@^separator^@"
      ""
      "@^separator^@"
      "* \\(T\\paren{t,\\vec{r}}\\) is the value of the temperature at time "
      "\\(t\\) and position \\(\\vec{r}\\) ;"
      "@^separator^@"
      "* \\(t_{0}\\) is the reference time;"
      "@^separator^@"
      "* \\(t\\) is the current time."
      "@^separator^@"
      ""
      "@^separator^@"
      "In pratice, this integral is computed incrementally as follows: "
      "@^separator^@"
      "\\["
      "@^separator^@"
      "\\average{T}\\paren{t+dt,\\vec{r}} \\approx "
      "\\Frac{1}{t+dt-t_{0}}\\left[\\paren{t-t_{0}}\\,\\average{T}\\paren{t,"
      "\\vec{r}}+\\Frac{dt}{2}\\left[T\\paren{t,\\vec{r}}+T\\paren{t+dt,\\vec{"
      "r}}\\right]\\right]"
      "@^separator^@"
      "\\]",
      "The approximation made when computing the time integral may lead to "
      "(small) numerical errors.",
      "SI:0",
      "");

  const GlossaryEntry Glossary::KelvinTemperature(
      "KelvinTemperature",
      Glossary::names + 39,
      Glossary::names + 40,
      "SI:K",
      "scalar",
      "the temperature",
      "" /* no 'description' defined */,
      "This entry has been introduced by compatibility with implantation "
      "choices made by the Germinal fuel performance code",
      "SI:0",
      "");

  const GlossaryEntry Glossary::MassDensity("MassDensity",
                                            Glossary::names + 40,
                                            Glossary::names + 41,
                                            "SI:kg.m^{-3}",
                                            "scalar",
                                            "the mass density",
                                            "" /* no 'description' defined */,
                                            "" /* no 'notes' defined */,
                                            "SI:0",
                                            "");

  const GlossaryEntry Glossary::MeanBurnUp_AtPercent(
      "MeanBurnUp_AtPercent",
      Glossary::names + 41,
      Glossary::names + 42,
      "SI:at./100",
      "scalar",
      "the spatial average of the  burn-up in at.%",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::MeanBurnUp_MWJperTm(
      "MeanBurnUp_MWJperTm",
      Glossary::names + 42,
      Glossary::names + 43,
      "SI:MWJ/tm",
      "scalar",
      "the spatial average of the  burn-up in MegaWattJour per tons of metals",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::MeanIrradiationTemperature(
      "MeanIrradiationTemperature",
      Glossary::names + 43,
      Glossary::names + 44,
      "SI:K",
      "scalar",
      "The mean temperature  in time over a given domain \\(\\Omega\\)",
      "This temperature is defined as follows:\\[\\average{T}\\paren{t} "
      "=\\Frac{1}{t-t_{0}}\\Frac{1}{\\int_{\\Omega}\\dtot\\,V}\\int_{t_{0}}^{t}"
      "\\paren{\\int_{\\Omega}T\\paren{u,\\vec{r}}\\,\\dtot\\,V}\\]where "
      "\\(T\\paren{t,\\vec{r}}\\) is the value of the temperature at time "
      "\\(t\\) and at position \\(\\vec{r}\\).",
      "In pratice, the computation of the spatial integral is done using "
      "standard finite element operations and the time integral is performed "
      "incrementally using a trapezoidal rule.",
      "SI:0",
      "");

  const GlossaryEntry Glossary::MeanTemperature(
      "MeanTemperature",
      Glossary::names + 44,
      Glossary::names + 45,
      "SI:K",
      "scalar",
      "The mean temperature over a given domain \\(\\Omega\\)",
      "This temperature is defined as follows:\\[\\average{T}\\paren{t}  = "
      "\\Frac{1}{\\int_{\\Omega}\\dtot\\,V}\\int_{\\Omega}T\\paren{t,\\vec{r}}"
      "\\,\\dtot\\, V\\]where \\(T\\paren{t,\\vec{r}}\\) is the value of the "
      "temperature at time \\(t\\) and at position \\(\\vec{r}\\).",
      "In pratice, the computation of this integral is done using standard "
      "finite element operations.",
      "SI:0",
      "");

  const GlossaryEntry Glossary::NeutronFluence(
      "NeutronFluence",
      Glossary::names + 45,
      Glossary::names + 46,
      "SI:n.m^{-2}",
      "scalar",
      "the neutron fluence",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::NeutronFlux("NeutronFlux",
                                            Glossary::names + 46,
                                            Glossary::names + 47,
                                            "SI:n.m^{-2}.s^{-1}",
                                            "scalar",
                                            "the neutron flux",
                                            "" /* no 'description' defined */,
                                            "" /* no 'notes' defined */,
                                            "",
                                            "");

  const GlossaryEntry Glossary::NormalStiffness(
      "NormalStiffness",
      Glossary::names + 47,
      Glossary::names + 48,
      "SI:Pa.m^{-1}",
      "scalar",
      "the normal elastic stiffness for a cohesive zone model",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::NumberOfMoles("NumberOfMoles",
                                              Glossary::names + 48,
                                              Glossary::names + 49,
                                              "SI:mol",
                                              "scalar",
                                              "the amount of substance",
                                              "" /* no 'description' defined */,
                                              "" /* no 'notes' defined */,
                                              "SI:0",
                                              "");

  const GlossaryEntry Glossary::OpeningDisplacement(
      "OpeningDisplacement",
      Glossary::names + 49,
      Glossary::names + 50,
      "SI:m",
      "vector",
      "opening displacement in cohesive zone models",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::OrthotropicAxisX1(
      "OrthotropicAxisX1",
      Glossary::names + 50,
      Glossary::names + 51,
      "",
      "scalar",
      "the first coordinate of the vector giving the first axis of orthotropy",
      "" /* no 'description' defined */,
      "This quantity is defined internally by the Licos fuel performance code",
      "",
      "");

  const GlossaryEntry Glossary::OrthotropicAxisX2(
      "OrthotropicAxisX2",
      Glossary::names + 51,
      Glossary::names + 52,
      "",
      "scalar",
      "the first coordinate of the vector giving the second axis of orthotropy",
      "" /* no 'description' defined */,
      "This quantity is defined internally by the Licos fuel performance code",
      "",
      "");

  const GlossaryEntry Glossary::OrthotropicAxisY1(
      "OrthotropicAxisY1",
      Glossary::names + 52,
      Glossary::names + 53,
      "",
      "scalar",
      "the second coordinate of the vector giving the first axis of orthotropy",
      "" /* no 'description' defined */,
      "This quantity is defined internally by the Licos fuel performance code",
      "",
      "");

  const GlossaryEntry Glossary::OrthotropicAxisY2(
      "OrthotropicAxisY2",
      Glossary::names + 53,
      Glossary::names + 54,
      "",
      "scalar",
      "the second coordinate of the vector giving the second axis of "
      "orthotropy",
      "" /* no 'description' defined */,
      "This quantity is defined internally by the Licos fuel performance code",
      "",
      "");

  const GlossaryEntry Glossary::OrthotropicAxisZ1(
      "OrthotropicAxisZ1",
      Glossary::names + 54,
      Glossary::names + 55,
      "",
      "scalar",
      "the third coordinate of the vector giving the first axis of orthotropy",
      "" /* no 'description' defined */,
      "This quantity is defined internally by the Licos fuel performance code",
      "",
      "");

  const GlossaryEntry Glossary::OrthotropicAxisZ2(
      "OrthotropicAxisZ2",
      Glossary::names + 55,
      Glossary::names + 56,
      "",
      "scalar",
      "the third coordinate of the vector giving the second axis of orthotropy",
      "" /* no 'description' defined */,
      "This quantity is defined internally by the Licos fuel performance code",
      "",
      "");

  const GlossaryEntry Glossary::PlasticStrain("PlasticStrain",
                                              Glossary::names + 56,
                                              Glossary::names + 57,
                                              "",
                                              "tensor",
                                              "The plastic strain",
                                              "" /* no 'description' defined */,
                                              "" /* no 'notes' defined */,
                                              "",
                                              "");

  const GlossaryEntry Glossary::PlateWidth("PlateWidth",
                                           Glossary::names + 57,
                                           Glossary::names + 58,
                                           "SI:m",
                                           "scalar",
                                           "??",
                                           "" /* no 'description' defined */,
                                           "" /* no 'notes' defined */,
                                           "",
                                           "");

  const GlossaryEntry Glossary::PoissonRatio(
      "PoissonRatio",
      Glossary::names + 58,
      Glossary::names + 59,
      "",
      "scalar",
      "the Poisson ratio of an isotropic material",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:-1",
      "SI:0.5");

  const GlossaryEntry Glossary::PoissonRatio12(
      "PoissonRatio12",
      Glossary::names + 59,
      Glossary::names + 60,
      "",
      "scalar",
      "the Poisson's coefficient between the first and second directions of "
      "orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:-1",
      "SI:0.5");

  const GlossaryEntry Glossary::PoissonRatio13(
      "PoissonRatio13",
      Glossary::names + 60,
      Glossary::names + 61,
      "",
      "scalar",
      "the Poisson's coefficient between the first and third directions of "
      "orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:-1",
      "SI:0.5");

  const GlossaryEntry Glossary::PoissonRatio23(
      "PoissonRatio23",
      Glossary::names + 61,
      Glossary::names + 62,
      "",
      "scalar",
      "the Poisson's coefficient between the second and third directions of "
      "orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:-1",
      "SI:0.5");

  const GlossaryEntry Glossary::Porosity("Porosity",
                                         Glossary::names + 62,
                                         Glossary::names + 63,
                                         "",
                                         "scalar",
                                         "Porosity of the material",
                                         "" /* no 'description' defined */,
                                         "" /* no 'notes' defined */,
                                         "SI:0",
                                         "SI:1");

  const GlossaryEntry Glossary::PorosityIncreaseDueToInelasticFlow(
      "PorosityIncreaseDueToInelasticFlow",
      Glossary::names + 63,
      Glossary::names + 64,
      "",
      "scalar",
      "Part of the porosity increase du to inelastic flow",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "SI:1");

  const GlossaryEntry Glossary::PorosityIncreaseDueToNucleation(
      "PorosityIncreaseDueToNucleation",
      Glossary::names + 64,
      Glossary::names + 65,
      "",
      "scalar",
      "Part of the porosity increase du to nucleation",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "SI:1");

  const GlossaryEntry Glossary::PowerDensity(
      "PowerDensity",
      Glossary::names + 65,
      Glossary::names + 66,
      "SI:W.m^{-3}",
      "scalar",
      "the power density, generally in the current configuration",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::Pressure("Pressure",
                                         Glossary::names + 66,
                                         Glossary::names + 67,
                                         "SI:Pa",
                                         "scalar",
                                         "the pressure of a gaz",
                                         "" /* no 'description' defined */,
                                         "" /* no 'notes' defined */,
                                         "",
                                         "");

  const GlossaryEntry Glossary::PrincipalStress1(
      "PrincipalStress1",
      Glossary::names + 67,
      Glossary::names + 68,
      "SI:Pa",
      "scalar",
      "the first principal stress",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::PrincipalStress2(
      "PrincipalStress2",
      Glossary::names + 68,
      Glossary::names + 69,
      "SI:Pa",
      "scalar",
      "the third principal stress",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::PrincipalStress3(
      "PrincipalStress3",
      Glossary::names + 69,
      Glossary::names + 70,
      "SI:Pa",
      "scalar",
      "the third principal stress",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::SecondAxisSecondMomentArea(
      "SecondAxisSecondMomentArea",
      Glossary::names + 70,
      Glossary::names + 71,
      "SI:m^{4}",
      "scalar",
      "??",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::ShearModulus(
      "ShearModulus",
      Glossary::names + 71,
      Glossary::names + 72,
      "SI:Pa",
      "scalar",
      "the shear modulus of an isotropic material",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::ShearModulus12(
      "ShearModulus12",
      Glossary::names + 72,
      Glossary::names + 73,
      "SI:Pa",
      "scalar",
      "the shear moduls between the first and second directions of orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::ShearModulus13(
      "ShearModulus13",
      Glossary::names + 73,
      Glossary::names + 74,
      "SI:Pa",
      "scalar",
      "the shear moduls between the first and third directions of orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::ShearModulus23(
      "ShearModulus23",
      Glossary::names + 74,
      Glossary::names + 75,
      "SI:Pa",
      "scalar",
      "the shear moduls between the second and third directions of orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::SolidSwelling(
      "SolidSwelling",
      Glossary::names + 75,
      Glossary::names + 76,
      "",
      "scalar",
      "swelling du to solid fission products",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::SpecificHeat("SpecificHeat",
                                             Glossary::names + 76,
                                             Glossary::names + 77,
                                             "SI:J.kg^{-1}.K^{-1}",
                                             "scalar",
                                             "the specific heat",
                                             "" /* no 'description' defined */,
                                             "" /* no 'notes' defined */,
                                             "SI:0",
                                             "");

  const GlossaryEntry Glossary::SphericalStress(
      "SphericalStress",
      Glossary::names + 77,
      Glossary::names + 78,
      "SI:Pa",
      "tensor",
      "the stress in a spherical frame",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::Strain("Strain",
                                       Glossary::names + 78,
                                       Glossary::names + 79,
                                       "",
                                       "tensor",
                                       "the strain tensor",
                                       "" /* no 'description' defined */,
                                       "" /* no 'notes' defined */,
                                       "",
                                       "");

  const GlossaryEntry Glossary::StrainMeasure(
      "StrainMeasure",
      Glossary::names + 79,
      Glossary::names + 80,
      "",
      "tensor",
      "a generic entry for a strain measure (for instance, the Henky strain or "
      "the Green-Lagrange strain)",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::Stress("Stress",
                                       Glossary::names + 80,
                                       Glossary::names + 81,
                                       "SI:Pa",
                                       "tensor",
                                       "the stress tensor",
                                       "" /* no 'description' defined */,
                                       "" /* no 'notes' defined */,
                                       "",
                                       "");

  const GlossaryEntry Glossary::Swelling("Swelling",
                                         Glossary::names + 81,
                                         Glossary::names + 82,
                                         "",
                                         "scalar",
                                         "an imposed swelling",
                                         "" /* no 'description' defined */,
                                         "" /* no 'notes' defined */,
                                         "",
                                         "");

  const GlossaryEntry Glossary::TangentialStiffness(
      "TangentialStiffness",
      Glossary::names + 82,
      Glossary::names + 83,
      "SI:Pa.m^{-1}",
      "scalar",
      "the tangential elastic stiffness for a cohesive zone model",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::Temperature("Temperature",
                                            Glossary::names + 83,
                                            Glossary::names + 84,
                                            "SI:K",
                                            "scalar",
                                            "the temperature",
                                            "" /* no 'description' defined */,
                                            "" /* no 'notes' defined */,
                                            "SI:0",
                                            "");

  const GlossaryEntry Glossary::TemperatureGradient(
      "TemperatureGradient",
      Glossary::names + 84,
      Glossary::names + 85,
      "SI:T.m^{-1}",
      "vector",
      "the temperature gradient, generally in the current configuration",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::ThermalConductivity(
      "ThermalConductivity",
      Glossary::names + 85,
      Glossary::names + 86,
      "SI:W.m^{-1}.K^{-1}",
      "scalar",
      "the thermal conductivity of an isotropic material",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::ThermalConductivity1(
      "ThermalConductivity1",
      Glossary::names + 86,
      Glossary::names + 87,
      "SI:W.m^{-1}.K^{-1}",
      "scalar",
      "the thermal conductivity of an orthotropic material along the first "
      "axis of orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::ThermalConductivity2(
      "ThermalConductivity2",
      Glossary::names + 87,
      Glossary::names + 88,
      "SI:W.m^{-1}.K^{-1}",
      "scalar",
      "the thermal conductivity of an orthotropic material along the second "
      "axis of orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::ThermalConductivity3(
      "ThermalConductivity3",
      Glossary::names + 88,
      Glossary::names + 89,
      "SI:W.m^{-1}.K^{-1}",
      "scalar",
      "the thermal conductivity of an orthotropic material along the third "
      "axis of orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::ThermalExpansion(
      "ThermalExpansion",
      Glossary::names + 89,
      Glossary::names + 90,
      "SI:K^{-1}",
      "scalar",
      "the mean thermal expansion coefficient",
      "" /* no 'description' defined */,
      "This entry shall have be named MeanThermalExpansionCoefficient.",
      "",
      "");

  const GlossaryEntry Glossary::ThermalExpansion1(
      "ThermalExpansion1",
      Glossary::names + 90,
      Glossary::names + 91,
      "SI:K^{-1}",
      "scalar",
      "the mean thermal expansion coefficient along the first orthotropy "
      "direction",
      "" /* no 'description' defined */,
      "This entry shall have be named MeanThermalExpansionCoefficient1.",
      "",
      "");

  const GlossaryEntry Glossary::ThermalExpansion2(
      "ThermalExpansion2",
      Glossary::names + 91,
      Glossary::names + 92,
      "SI:K^{-1}",
      "scalar",
      "the mean thermal expansion coefficient along the second orthotropy "
      "direction",
      "" /* no 'description' defined */,
      "This entry shall have be named MeanThermalExpansionCoefficient2.",
      "",
      "");

  const GlossaryEntry Glossary::ThermalExpansion3(
      "ThermalExpansion3",
      Glossary::names + 92,
      Glossary::names + 93,
      "SI:K^{-1}",
      "scalar",
      "the mean thermal expansion coefficient along the third orthotropy "
      "direction",
      "" /* no 'description' defined */,
      "This entry shall have be named MeanThermalExpansionCoefficient3.",
      "",
      "");

  const GlossaryEntry Glossary::TorsionConstant(
      "TorsionConstant",
      Glossary::names + 93,
      Glossary::names + 94,
      "SI:??",
      "scalar",
      "??",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::TrescaStress("TrescaStress",
                                             Glossary::names + 94,
                                             Glossary::names + 95,
                                             "SI:Pa",
                                             "scalar",
                                             "the Tresca equivalent stress",
                                             "" /* no 'description' defined */,
                                             "" /* no 'notes' defined */,
                                             "SI:0",
                                             "");

  const GlossaryEntry Glossary::UltimateTensileStrength(
      "UltimateTensileStrength",
      Glossary::names + 95,
      Glossary::names + 97,
      "SI:Pa",
      "scalar",
      "the maximum stress that a material can withstand while being stretched "
      "or pulled before breaking",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::ViscoplasticStrain(
      "ViscoplasticStrain",
      Glossary::names + 97,
      Glossary::names + 98,
      "",
      "tensor",
      "The viscoplatic strain",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::VolumetricStrain(
      "VolumetricStrain",
      Glossary::names + 98,
      Glossary::names + 99,
      "",
      "scalar",
      "the volumetric strain, defined as the trace of the strain tensor",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "",
      "");

  const GlossaryEntry Glossary::VonMisesStress(
      "VonMisesStress",
      Glossary::names + 99,
      Glossary::names + 100,
      "SI:Pa",
      "scalar",
      "the von Mises equivalent stress",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::YieldStrength(
      "YieldStrength",
      Glossary::names + 100,
      Glossary::names + 102,
      "SI:Pa",
      "scalar",
      "the stress corresponding to the yield point at which the material "
      "begins to deform plastically",
      "" /* no 'description' defined */,
      "When this limit is difficult to identify experimentally, the offset "
      "yield point is taken as the stress at which 0.2% plastic deformation "
      "occurs",
      "SI:0",
      "");

  const GlossaryEntry Glossary::YoungModulus(
      "YoungModulus",
      Glossary::names + 102,
      Glossary::names + 103,
      "SI:Pa",
      "scalar",
      "the Young's modulus of an isotropic material",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::YoungModulus1(
      "YoungModulus1",
      Glossary::names + 103,
      Glossary::names + 104,
      "SI:Pa",
      "scalar",
      "the Young's modulus of an isotropic material along the first direction "
      "of orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::YoungModulus2(
      "YoungModulus2",
      Glossary::names + 104,
      Glossary::names + 105,
      "SI:Pa",
      "scalar",
      "the Young's modulus of an isotropic material along the second direction "
      "of orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

  const GlossaryEntry Glossary::YoungModulus3(
      "YoungModulus3",
      Glossary::names + 105,
      Glossary::names + 106,
      "SI:Pa",
      "scalar",
      "the Young's modulus of an isotropic material along the third direction "
      "of orthotropy",
      "" /* no 'description' defined */,
      "" /* no 'notes' defined */,
      "SI:0",
      "");

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

  const GlossaryEntry& Glossary::getGlossaryEntry(const std::string& n) const {
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

}  // end of namespace tfel::glossary
