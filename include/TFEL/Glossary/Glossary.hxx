/*!
 * \file   Glossary.hxx
 * \author Thomas Helfer
 * \date   09/06/14
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_GLOSSARY_GLOSSARY_HXX
#define TFEL_GLOSSARY_GLOSSARY_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include <set>
#include <vector>
#include <string>

#include "TFEL/Glossary/Forward/Glossary.hxx"

namespace tfel {
  namespace glossary {

    /*!
     * \brief Structure in charge of handling the TFEL Glossary
     */
    struct TFELGLOSSARY_VISIBILITY_EXPORT Glossary {
      /*!
       * \return the uniq instance of this class (singleton pattern)
       */
      static Glossary& getGlossary();

      static const GlossaryEntry AxialGrowth;
      static const GlossaryEntry AxialStrain;
      static const GlossaryEntry AxialStress;
      static const GlossaryEntry B10BurnUp;
      static const GlossaryEntry BulkModulus;
      static const GlossaryEntry BurnUp_AtPercent;
      static const GlossaryEntry BurnUp_MWJperTm;
      static const GlossaryEntry ConvectiveHeatTransferCoefficient;
      static const GlossaryEntry CrossSectionArea;
      static const GlossaryEntry CylindricalStress;
      static const GlossaryEntry Damage;
      static const GlossaryEntry Displacement;
      static const GlossaryEntry ElasticStrain;
      static const GlossaryEntry Emissivity;
      static const GlossaryEntry EquivalentPlasticStrain;
      static const GlossaryEntry EquivalentStrain;
      static const GlossaryEntry EquivalentViscoplasticStrain;
      static const GlossaryEntry FastNeutronFluence_01MeV;
      static const GlossaryEntry FastNeutronFluence_1MeV;
      static const GlossaryEntry FastNeutronFlux_01MeV;
      static const GlossaryEntry FastNeutronFlux_1MeV;
      static const GlossaryEntry FirstAxisSecondMomentArea;
      static const GlossaryEntry FirstLameCoefficient;
      static const GlossaryEntry FissionDensity;
      static const GlossaryEntry GaseousSwelling;
      static const GlossaryEntry GrainSize;
      static const GlossaryEntry HeatFlux;
      static const GlossaryEntry HeatTransferCoefficient;
      static const GlossaryEntry HillStress;
      static const GlossaryEntry HydrostaticPressure;
      static const GlossaryEntry IrradiationDamage;
      static const GlossaryEntry IrradiationInducedSwelling;
      static const GlossaryEntry IrradiationSwelling;
      static const GlossaryEntry IrradiationTemperature;
      static const GlossaryEntry KelvinTemperature;
      static const GlossaryEntry MassDensity;
      static const GlossaryEntry MeanBurnUp_AtPercent;
      static const GlossaryEntry MeanBurnUp_MWJperTm;
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
      static const GlossaryEntry ShearModulus;
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
       * \return true if the glossary contains the given name or key
       * \param[in] n: name
       */
      bool contains(const std::string&) const;
      /*!
       * \return the glossary entry associated with the given name or key
       * \param[in] n: name
       */
      const GlossaryEntry& getGlossaryEntry(const std::string&) const;
      /*!
       * \return all the registred keys
       */
      const std::vector<std::string>& getKeys() const;

     protected:
      //! all glossary names (to initialise glossary entries)
      static const char* names[97];
      /*!
       * \brief insert a new entry
       */
      void insert(const GlossaryEntry&);

      Glossary();

      Glossary(const Glossary&);

      Glossary& operator=(const Glossary&);

      /*!
      \return an iterator to the glossary associated with the given name or key.
      Return this->entries.end() if no matching entry is found.
      \param[in] n: name or key.
      */
      std::set<GlossaryEntry>::const_iterator findGlossaryEntry(
          const std::string&) const;

      //! list of all registred entries
      std::set<GlossaryEntry> entries;

      //! list of all registred keys
      std::vector<std::string> keys;

    };  // end of struct Glossary

  }  // end of namespace glossary

}  // end of namespace tfel

#endif /* TFEL_GLOSSARY_GLOSSARY_HXX */
