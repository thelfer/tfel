/*!
 * \file   mfront/include/MFront/BehaviourBrick/DDIF2StressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_DDIF2STRESSPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_DDIF2STRESSPOTENTIAL_HXX

#include <array>
#include <memory>
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MFront/BehaviourBrick/HookeStressPotential.hxx"

namespace mfront {

  // forward declaration
  struct MaterialPropertyDescription;
  // forward declaration
  struct LocalDataStructure;

  namespace bbrick {

    /*!
     * \brief class describing the computation of the stress through the
     * standard DDIF2 law.
     */
    struct DDIF2StressPotential : HookeStressPotential{
      //! \brief constructor
      DDIF2StressPotential();
      std::string getName() const override;
      std::vector<OptionDescription> getOptions() const override;
      void initialize(AbstractBehaviourDSL&,
                      BehaviourDescription&,
                      const DataMap&) override;
      std::vector<Hypothesis> getSupportedModellingHypotheses(
          AbstractBehaviourDSL&, BehaviourDescription&) const override;
      void completeVariableDeclaration(AbstractBehaviourDSL&,
                                       BehaviourDescription&) const override;
      void endTreatment(AbstractBehaviourDSL&,
                        BehaviourDescription&) const override;
      //! destructor
      ~DDIF2StressPotential() override;

     protected:
      //! fracture stress
      std::array<MaterialProperty, 3u> sr;
      //! softening slopes
      std::array<MaterialProperty, 3u> rp;
      //! fracture energy
      std::array<MaterialProperty, 3u> gc;
      //! handle pressure on crack surface
      bool pr = false;
    };  // end of struct DDIF2StressPotential

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_DDIF2STRESSPOTENTIAL_HXX */
