/*!
 * \file   mfront/include/MFront/BehaviourBrick/HookeStressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX

#include "MFront/BehaviourBrick/StressPotentialBase.hxx"

namespace mfront {

  // forward declaration
  struct LocalDataStructure;

  namespace bbrick {

    /*!
     * \brief class describing the computation of the stress through the
     * standard Hooke law.
     */
    struct HookeStressPotential : StressPotentialBase {
      /*!
       * \param[in] dsl_: abstract behaviour dsl
       * \param[in] bd_: behaviour description
       * \param[in] p: parameters of the stress potential
       */
      HookeStressPotential(AbstractBehaviourDSL&,
                           BehaviourDescription&,
                           const DataMap&);
      std::string getName() const override;
      std::vector<Hypothesis> getSupportedModellingHypotheses() const override;
      void completeVariableDeclaration() const override;
      void endTreatment() const override;
      //! destructor
      ~HookeStressPotential() override;

     protected:
      /*!
       * \brief declared the computeStress and computeFinalStress when the
       * requiresStiffnessTensor attribute has been set.
       */
      virtual void declareComputeStressWhenStiffnessTensorIsDefined() const;
      /*!
       * treat the case of isotropic behaviours
       * \param[in] d: local data structure
       */
      virtual void treatIsotropicBehaviour(LocalDataStructure&) const;
      /*!
       * treat the case of isotropic behaviours
       */
      virtual void treatOrthotropicBehaviour() const;
      /*!
       * \brief add support for the AXISYMMETRICALGENERALISEDPLANESTRESS
       * modelling hypothesis
       */
      virtual void addAxisymmetricalGeneralisedPlaneStressSupport() const;
      /*!
       * \brief add support for the PLANESTRESS modelling hypothesis
       */
      virtual void addPlaneStressSupport() const;
      /*!
       * \brief add the generic tangent operator computation
       */
      virtual void addGenericTangentOperatorSupport() const;
      /*!
       * \brief add the generic prediction operator computation
       */
      virtual void addGenericPredictionOperatorSupport() const;
      /*!
       * \brief declare the compute elastic prediction method
       */
      virtual void declareComputeElasticPredictionMethod() const;
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
