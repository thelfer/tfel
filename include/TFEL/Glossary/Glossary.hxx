/*!
 *\file   Glossary.hxx
 *\author Helfer Thomas
 *\date   09/06/14
*/

#ifndef __TFEL_GLOSSARY_GLOSSARY_HH__
#define __TFEL_GLOSSARY_GLOSSARY_HH__

#include"TFEL/Config/TFELConfig.hxx"
#include<set>
#include<string>

#include"TFEL/Glossary/GlossaryEntry.hxx"

namespace tfel
{
namespace glossary
{

/*!
 * \brief Structure in charge of handling the TFEL Glossary
 */
struct TFELGLOSSARY_VISIBILITY_EXPORT Glossary
{

/*!
 * \return the uniq instance of this class (singleton pattern)
 */
static Glossary&
getGlossary();


/*!
 * \return true if the glossary contains the given name
 * \param[in] n : name
 */

bool
contains(const std::string&) const;

static const GlossaryEntry AxialStrain;
static const GlossaryEntry ConvectiveHeatTransferCoefficient;
static const GlossaryEntry CylindricalStress;
static const GlossaryEntry Displacement;
static const GlossaryEntry ExternalDisplacement;
static const GlossaryEntry ExternalPressure;
static const GlossaryEntry ExternalTemperature;
static const GlossaryEntry ExternalTraction;
static const GlossaryEntry FirstMaterialRadius;
static const GlossaryEntry FirstMaterialTemperature;
static const GlossaryEntry GaseousSwelling;
static const GlossaryEntry HeatFlux;
static const GlossaryEntry HydrostaticPressure;
static const GlossaryEntry IrradiationInducedSwelling;
static const GlossaryEntry IrradiationTemperature;
static const GlossaryEntry KelvinTemperature;
static const GlossaryEntry MassDensity;
static const GlossaryEntry MeanIrradiationTemperature;
static const GlossaryEntry MeanTemperature;
static const GlossaryEntry OrthotropicAxisX1;
static const GlossaryEntry OrthotropicAxisX2;
static const GlossaryEntry OrthotropicAxisY1;
static const GlossaryEntry OrthotropicAxisY2;
static const GlossaryEntry OrthotropicAxisZ1;
static const GlossaryEntry OrthotropicAxisZ2;
static const GlossaryEntry PoissonRatio;
static const GlossaryEntry PoissonRatio12;
static const GlossaryEntry PoissonRatio13;
static const GlossaryEntry PoissonRatio23;
static const GlossaryEntry PowerDensity;
static const GlossaryEntry Pressure;
static const GlossaryEntry PrincipalStress1;
static const GlossaryEntry PrincipalStress2;
static const GlossaryEntry PrincipalStress3;
static const GlossaryEntry SecondMaterialRadius;
static const GlossaryEntry SecondMaterialTemperature;
static const GlossaryEntry ShearModulus12;
static const GlossaryEntry ShearModulus13;
static const GlossaryEntry ShearModulus23;
static const GlossaryEntry SolidSwelling;
static const GlossaryEntry SpecificHeat;
static const GlossaryEntry SphericalStress;
static const GlossaryEntry Strain;
static const GlossaryEntry Stress;
static const GlossaryEntry Swelling;
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
static const GlossaryEntry Trescastress;
static const GlossaryEntry UpdatedFirstCoordinate;
static const GlossaryEntry UpdatedSecondCoordinate;
static const GlossaryEntry UpdatedThirdCoordinate;
static const GlossaryEntry VolumetricStrain;
static const GlossaryEntry VonMisesStress;
static const GlossaryEntry YoungModulus;
static const GlossaryEntry YoungModulus1;
static const GlossaryEntry YoungModulus2;
static const GlossaryEntry YoungModulus3;

private :
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

}; // end of struct Glossary

} // end of namespace glossary

} // end of namespace tfel

#endif /* __TFEL_GLOSSARY_GLOSSARY_HH__ */
