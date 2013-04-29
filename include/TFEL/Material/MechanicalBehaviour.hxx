/*!
 * \file   MechanicalBehaviour.hxx
 * \brief  This file declares the MechanicalBehaviour class
 * \author Helfer Thomas
 * \date   22 Sep 2006
 */

#ifndef _LIB_TFEL_MECHANICALBEHAVIOUR_H_
#define _LIB_TFEL_MECHANICALBEHAVIOUR_H_ 

#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Material/MechanicalBehaviourData.hxx"

namespace tfel{

  namespace material{

    /*!
     * \class MechanicalBehaviour
     * \brief This class declares an interface for mechanical behaviours.
     * \param H, modelling hypothesis
     * \param NumType, numerical type.
     * \param bool use_qt, says if one shall use quantities.
     * \author Helfer Thomas
     * \date   28 Jul 2006
     */
    template<ModellingHypothesis::Hypothesis H,
	     typename NumType, bool use_qt>
    struct MechanicalBehaviour
    {
      /*!
       * \brief return values of the integrate method
       */
      enum IntegrationResult{
	SUCCESS,            //<! Integration is a success
	FAILURE,            //<! Integration failed
	UNRELIABLE_RESULTS  //<! Integration succeed, but one or more
			    //   internal criteria show that the
			    //   results may be inaccurate
      }; // end of enum IntegrationResult
      /*!
       * list of possible stiffness matrix type
       */
      enum SMType{
	ELASTIC,
	SECANTOPERATOR,
	TANGENTOPERATOR,
	CONSISTANTTANGENTOPERATOR,
	NOSTIFFNESSREQUESTED
      }; // end of enum StiffnessMatrixType
      /*!
       * dimension of the space for the the given modelling hypothesis
       */
      static const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	return std::string("MechanicalBehaviour");
      }
      /*!
       * \brief only compute a prediction stiffness matrix.
       * The result shall be retrieved through the "
       * "getTangeOperator method
       * \param[in] smt : stiffness matrix type
       * \return SUCCESS if the integration is successfull.
       */
      virtual IntegrationResult
      computePredictionOperator(const SMType) = 0;
      /*!
       * \brief determine the value of the internal state variables at
       * the end of the time step
       * \param[in] b : compute the stiffness matrix
       * \return SUCCESS if the integration is successfull.
       */
      virtual IntegrationResult
      integrate(const SMType) = 0;
      /*!
       * \return the tangent operator This shall be called afer the
       * integration.  Normally this operator has been computed at the
       * end of the integration or after a call to the
       * getTangentOperator, this is a why we return a reference.
       *
       * Interfaces to behaviour law shall use the
       * hasConsistantTangentOperator of the traits class
       * MechanicalBehaviourTraits to know if the behaviour provides a
       * consistent tangent operator. If not, behaviours shall throw
       * an exception.
       *
       * The behaviour indicates wheter the constistent operator is
       * symmetric through the isConsistantTangentOperatorSymmetric
       * member of the traits class MechanicalBehaviourTraits
       */
      virtual const typename tfel::config::Types<N,NumType,use_qt>::StiffnessTensor&
      getTangentOperator(void) const = 0;
      /*!
       * This method returns a scaling factor that can be used to:
       * - increase the time step if the integration was successfull
       * - decrease the time step if the integration failed or if the
       *   results were not reliable (time step too large).
       *
       * Interfaces to behaviour law shall use the
       * hasTimeStepScalingFactor of the traits class
       * MechanicalBehaviourTraits to know if the behaviour is able to
       * give such a time step scaling factor. If not, behaviours
       * may return the NumType(1) value.
       */
      virtual NumType
      getTimeStepScalingFactor(void) const = 0;
      /*!
       * destructor
       */
      virtual ~MechanicalBehaviour(){};
    };

  } // end of namespace material 

} // end of namespace tfel

#endif /* _LIB_TFEL_MECHANICALBEHAVIOUR_H */

