/*!
 * \file   mfront/include/MFront/BehaviourBrick/HookeStressPotentialBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIALBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIALBASE_HXX

#include "MFront/BehaviourBrick/StressPotential.hxx"

namespace mfront {

  // forward declaration
  struct LocalDataStructure;

  namespace bbrick {

    /*!
     * \brief class describing the computation of the stress through the
     * standard Hooke law.
     */
    struct HookeStressPotentialBase : StressPotential {
      HookeStressPotentialBase();
      std::string getName() const override;
      std::vector<OptionDescription> getOptions() const override;
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const DataMap&) override;
      std::vector<Hypothesis> getSupportedModellingHypotheses(
          const BehaviourDescription&,
          const AbstractBehaviourDSL&) const override;
      void completeVariableDeclaration(
          BehaviourDescription&, const AbstractBehaviourDSL&) const override;
      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&) const override;
      void writeStressDerivatives(BehaviourDescription&) const override;
      std::string computeDerivatives(const BehaviourDescription& bd,
                                     const std::string&,
                                     const std::string&,
                                     const std::string&) const override;
      void computeElasticPrediction(BehaviourDescription&) const override;
      std::string getStressNormalisationFactor(
          const BehaviourDescription&) const override;
      std::string getEquivalentStressLowerBound(
          const BehaviourDescription&) const override;
      //! destructor
      ~HookeStressPotentialBase() override;

     protected:
      /*!
       * \brief declared the computeStress and computeFinalStress when the
       * requiresStiffnessTensor attribute has been set.
       */
      virtual void declareComputeStressWhenStiffnessTensorIsDefined(
          BehaviourDescription&) const;
      /*!
       * \brief treat the case of isotropic behaviours
       * \param[in] d: local data structure
       */
      virtual void treatIsotropicBehaviour(BehaviourDescription&,
                                           LocalDataStructure&) const;
      /*!
       * \brief treat the case of isotropic behaviours
       */
      virtual void treatOrthotropicBehaviour(BehaviourDescription&) const;
      /*!
       * \brief add support for the AXISYMMETRICALGENERALISEDPLANESTRESS
       * modelling hypothesis
       */
      virtual void addAxisymmetricalGeneralisedPlaneStressSupport(
          BehaviourDescription&, const AbstractBehaviourDSL&) const;
      /*!
       * \brief add support for the PLANESTRESS modelling hypothesis
       */
      virtual void addPlaneStressSupport(BehaviourDescription&,
                                         const AbstractBehaviourDSL&) const;
      /*!
       * \brief add the generic tangent operator computation
       */
      virtual void addGenericTangentOperatorSupport(
          BehaviourDescription&, const AbstractBehaviourDSL&) const;
      /*!
       * \brief add the generic prediction operator computation
       */
      virtual void addGenericPredictionOperatorSupport(
          BehaviourDescription&) const;
      /*!
       * \brief declare the compute elastic prediction method
       */
      virtual void declareComputeElasticPredictionMethod(
          BehaviourDescription&) const;
      //! \return true if a derived potential wants to add an isotropic damage.
      virtual bool handleIsotropicDamage() const = 0;

      //! plane stress support;
      bool pss = true;
      //! generic prediction operator support
      bool gto = true;
      //! generic tangent operator support
      bool gpo = true;
    };  // end of struct HookeStressPotentialBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIALBASE_HXX */
