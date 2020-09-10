/*!
 * \file   mfront/include/MFront/BehaviourBrick/HookeStressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX

#include "MFront/BehaviourBrick/HookeStressPotentialBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing the computation of the stress through the
     * standard Hooke law.
     */
    struct HookeStressPotential : HookeStressPotentialBase{
      //! \brief default constructor
      HookeStressPotential();
      std::vector<std::tuple<std::string,
                             std::string,
                             mfront::SupportedTypes::TypeFlag>>
      getStressDerivatives(const BehaviourDescription&) const override;
      std::string generateImplicitEquationDerivatives(
          const BehaviourDescription&,
          const std::string&,
          const std::string&,
          const std::string&,
          const bool) const override;
      //! \brief destructor
      ~HookeStressPotential() override;

     protected:
      void declareComputeElasticPredictionMethod(
          BehaviourDescription&) const override;
      void declareComputeStressWhenStiffnessTensorIsDefined(
          BehaviourDescription&) const override;
      void declareComputeStressForIsotropicBehaviour(
          BehaviourDescription&, LocalDataStructure&) const override;
      void addGenericTangentOperatorSupport(
          BehaviourDescription&, const AbstractBehaviourDSL&) const override;
      void addGenericPredictionOperatorSupport(
          BehaviourDescription&) const override;
    };  // end of struct HookeStressPotential

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX */
