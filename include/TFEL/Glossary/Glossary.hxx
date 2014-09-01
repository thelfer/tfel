/*!
 *\file   Glossary.hxx
 *\author Helfer Thomas
 *\date   09/06/14
*/

#ifndef __TFEL_GLOSSARY_GLOSSARY_HH__
#define __TFEL_GLOSSARY_GLOSSARY_HH__

#include"TFEL/Config/TFELConfig.hxx"
#include<set>
#include<vector>
#include<string>

#include"TFEL/Glossary/GlossaryEntry.hxx"

namespace tfel
{
namespace glossary
{

/*!
 * \brief Structure in charge of handling the TFEL Glossary
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */
struct TFELGLOSSARY_VISIBILITY_EXPORT Glossary
{

/*!
 * \return the uniq instance of this class (singleton pattern)
 */
static Glossary&
getGlossary();

static const GlossaryEntry AxialStrain;
static const GlossaryEntry AxialStress;
static const GlossaryEntry B10BurnUp;
static const GlossaryEntry BurnUp;
static const GlossaryEntry ConvectiveHeatTransferCoefficient;
static const GlossaryEntry CrossSectionArea;
static const GlossaryEntry CylindricalStress;
static const GlossaryEntry Damage;
static const GlossaryEntry Displacement;
static const GlossaryEntry ElasticStrain;
static const GlossaryEntry EquivalentPlasticStrain;
static const GlossaryEntry EquivalentStrain;
static const GlossaryEntry EquivalentViscoplasticStrain;
static const GlossaryEntry FastNeutronFluence;
static const GlossaryEntry FastNeutronFlux;
static const GlossaryEntry FirstAxisSecondMomentArea;
static const GlossaryEntry FissionDensity;
static const GlossaryEntry GaseousSwelling;
static const GlossaryEntry GrainSize;
static const GlossaryEntry HeatFlux;
static const GlossaryEntry HillStress;
static const GlossaryEntry HydrostaticPressure;
static const GlossaryEntry IrradiationDamage;
static const GlossaryEntry IrradiationInducedSwelling;
static const GlossaryEntry IrradiationSwelling;
static const GlossaryEntry IrradiationTemperature;
static const GlossaryEntry KelvinTemperature;
static const GlossaryEntry MassDensity;
static const GlossaryEntry MeanIrradiationTemperature;
static const GlossaryEntry MeanTemperature;
static const GlossaryEntry NeutronFluence;
static const GlossaryEntry NeutronFlux;
static const GlossaryEntry NormalStiffness;
static const GlossaryEntry NumberOfMoles;
static const GlossaryEntry OrthotropicAxisX1;
static const GlossaryEntry OrthotropicAxisX2;
static const GlossaryEntry OrthotropicAxisY1;
static const GlossaryEntry OrthotropicAxisY2;
static const GlossaryEntry OrthotropicAxisZ1;
static const GlossaryEntry OrthotropicAxisZ2;
static const GlossaryEntry PlasticStrain;
static const GlossaryEntry PlateWidth;
static const GlossaryEntry PoissonRatio;
static const GlossaryEntry PoissonRatio12;
static const GlossaryEntry PoissonRatio13;
static const GlossaryEntry PoissonRatio23;
static const GlossaryEntry Porosity;
static const GlossaryEntry PowerDensity;
static const GlossaryEntry Pressure;
static const GlossaryEntry PrincipalStress1;
static const GlossaryEntry PrincipalStress2;
static const GlossaryEntry PrincipalStress3;
static const GlossaryEntry SecondAxisSecondMomentArea;
static const GlossaryEntry ShearModulus12;
static const GlossaryEntry ShearModulus13;
static const GlossaryEntry ShearModulus23;
static const GlossaryEntry SolidSwelling;
static const GlossaryEntry SpecificHeat;
static const GlossaryEntry SphericalStress;
static const GlossaryEntry Strain;
static const GlossaryEntry Stress;
static const GlossaryEntry Swelling;
static const GlossaryEntry TangentialStiffness;
static const GlossaryEntry Temperature;
static const GlossaryEntry TemperatureGradient;
static const GlossaryEntry ThermalConductivity;
static const GlossaryEntry ThermalConductivity1;
static const GlossaryEntry ThermalConductivity2;
static const GlossaryEntry ThermalConductivity3;
static const GlossaryEntry ThermalExpansion;
static const GlossaryEntry ThermalExpansion1;
static const GlossaryEntry ThermalExpansion2;
static const GlossaryEntry ThermalExpansion3;
static const GlossaryEntry TorsionConstant;
static const GlossaryEntry TrescaStress;
static const GlossaryEntry UltimateTensileStrength;
static const GlossaryEntry ViscoplasticStrain;
static const GlossaryEntry VolumetricStrain;
static const GlossaryEntry VonMisesStress;
static const GlossaryEntry YieldStrength;
static const GlossaryEntry YoungModulus;
static const GlossaryEntry YoungModulus1;
static const GlossaryEntry YoungModulus2;
static const GlossaryEntry YoungModulus3;

/*!
 * \return true if the glossary contains the given name
 * \param[in] n : name
 */
bool
contains(const std::string&) const;
/*!
 * \return all the registred keys
 */
const std::vector<std::string>&
getKeys(void) const;

protected :
//! all glossary names (to initialise glossary entries)
static const char * names[86];
/*!
 * \brief insert a new entry
 */
void
insert(const GlossaryEntry&);

Glossary();

Glossary(const Glossary&);

Glossary&
operator=(const Glossary&);

std::set<GlossaryEntry> entries;

std::vector<std::string> keys;

}; // end of struct Glossary

} // end of namespace glossary

} // end of namespace tfel

#endif /* __TFEL_GLOSSARY_GLOSSARY_HH__ */
