/*!
 * \file   mfront/include/MFront/BehaviourBrick/HookeStressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX

#include "MFront/BehaviourBrick/StressPotential.hxx"

namespace mfront {

  // forward declaration
  struct LocalDataStructure;

  namespace bbrick {

    /*!
     * \brief class describing the computation of the stress through the
     * standard Hooke law.
     */
    struct HookeStressPotential : StressPotential{
      HookeStressPotential();
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
      ~HookeStressPotential() override;

     protected:
      /*!
       * \brief declared the computeStress and computeFinalStress when the
       * requiresStiffnessTensor attribute has been set.
       */
      virtual void declareComputeStressWhenStiffnessTensorIsDefined(
          BehaviourDescription&) const;
      /*!
       * treat the case of isotropic behaviours
       * \param[in] d: local data structure
       */
      virtual void treatIsotropicBehaviour(BehaviourDescription&,
                                           LocalDataStructure&) const;
      /*!
       * treat the case of isotropic behaviours
       */
      virtual void treatOrthotropicBehaviour(BehaviourDescription&) const;
      /*!
       * \brief add support for the AXISYMMETRICALGENERALISEDPLANESTRESS
       * modelling hypothesis
       */
      virtual void addAxisymmetricalGeneralisedPlaneStressSupport(
          AbstractBehaviourDSL&, BehaviourDescription&) const;
      /*!
       * \brief add support for the PLANESTRESS modelling hypothesis
       */
      virtual void addPlaneStressSupport(AbstractBehaviourDSL&,
                                         BehaviourDescription&) const;
      /*!
       * \brief add the generic tangent operator computation
       */
      virtual void addGenericTangentOperatorSupport(
          AbstractBehaviourDSL&, BehaviourDescription&) const;
      /*!
       * \brief add the generic prediction operator computation
       */
      virtual void addGenericPredictionOperatorSupport(
          AbstractBehaviourDSL&, BehaviourDescription&) const;
      /*!
       * \brief declare the compute elastic prediction method
       */
      virtual void declareComputeElasticPredictionMethod(
          BehaviourDescription&) const;
      //! plane stress support;
      bool pss = true;
      //! generic prediction operator support
      bool gto = true;
      //! generic tangent operator support
      bool gpo = true;
    };  // end of struct HookeStressPotential

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX */
