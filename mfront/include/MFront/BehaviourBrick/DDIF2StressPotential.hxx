/*!
 * \file   mfront/include/MFront/BehaviourBrick/DDIF2StressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
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
      /*!
       * \param[in] dsl_: abstract behaviour dsl
       * \param[in] bd_: behaviour description
       * \param[in] p: parameters of the stress potential
       */
      DDIF2StressPotential(AbstractBehaviourDSL&,
                           BehaviourDescription&,
                           const DataMap&);
      std::string getName() const override;
      std::vector<Hypothesis> getSupportedModellingHypotheses() const override;
      void completeVariableDeclaration() const override;
      void endTreatment() const override;
      //! destructor
      ~DDIF2StressPotential() override;

     protected:
      //! a simple alias
      using FractureStress =
          tfel::utilities::GenType<std::shared_ptr<MaterialPropertyDescription>,
                                   double>;
      //! a simple alias
      using SofteningSlope =
          tfel::utilities::GenType<std::shared_ptr<MaterialPropertyDescription>,
                                   double>;
      //! a simple alias
      using FractureEnergy =
          tfel::utilities::GenType<std::shared_ptr<MaterialPropertyDescription>,
                                   double>;
      //! fracture stress
      std::array<FractureStress, 3u> sr;
      //! softening slopes
      std::array<SofteningSlope, 3u> rp;
      //! fracture energy
      std::array<FractureEnergy, 3u> gc;
      //! handle pressure on crack surface
      bool pr = false;
    };  // end of struct DDIF2StressPotential

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_DDIF2STRESSPOTENTIAL_HXX */
