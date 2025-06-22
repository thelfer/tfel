/*!
 * \file   include/TFEL/Material/MechanicalBehaviour.hxx
 * \brief  This file declares the MechanicalBehaviour class
 * \author Thomas Helfer
 * \date   22 Sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MECHANICALBEHAVIOUR_HXX
#define LIB_TFEL_MECHANICALBEHAVIOUR_HXX

#include <utility>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperatorBase.hxx"

namespace tfel {

  namespace material {

    /*!
     * \class MechanicalBehaviourBase
     * \brief This class declares enums for mechanical behaviours.
     * \author Thomas Helfer
     * \date   30 Juillet 2013
     */
    struct MechanicalBehaviourBase {
      /*!
       * An indication of the type of the behaviour treated
       * If the behaviour is a small strain standard behaviour, then:
       * - the only driving variable is the total strain 'eto' (symmetric
       * tensor)
       * - the only thermodynamic force is the stress    'sig' (symmetric
       * tensor) If the behaviour is a cohesive zone model, then:
       * - the only driving variable is the opening displacement (tvector)
       * - the only thermodynamic force is the traction (tvector)
       */
      enum BehaviourType {
        GENERALBEHAVIOUR = 0,
        STANDARDSTRAINBASEDBEHAVIOUR = 1,
        STANDARDFINITESTRAINBEHAVIOUR = 2,
        COHESIVEZONEMODEL = 3
      };  // end of enum MFrontBehaviourType
      /*!
       * An indication of the kinematic of the behaviour treated
       */
      enum Kinematic {
        UNDEFINEDKINEMATIC = 0,
        SMALLSTRAINKINEMATIC = 1,
        COHESIVEZONEKINEMATIC = 2,
        FINITESTRAINKINEMATIC_F_CAUCHY = 3,
        FINITESTRAINKINEMATIC_ETO_PK1 = 4
      };  // end of enum MFrontBehaviourType
      /*!
       * \brief return values of the integrate method
       */
      enum IntegrationResult {
        SUCCESS,            //<! Integration is a success
        FAILURE,            //<! Integration failed
        UNRELIABLE_RESULTS  //<! Integration succeed, but one or more
                            //   internal criteria show that the
                            //   results may be inaccurate
      };                    // end of enum IntegrationResult
      /*!
       * list of possible tangent operator type
       */
      enum SMType {
        ELASTIC,
        SECANTOPERATOR,
        TANGENTOPERATOR,
        CONSISTENTTANGENTOPERATOR,
        NOSTIFFNESSREQUESTED
      };  // end of enum SMType
    };    // end of struct MechanicalBehaviourBase

    /*!
     * a trait class describing which tangent operators can be
     * computed by a mechanical behaviour.
     */
    template <MechanicalBehaviourBase::BehaviourType btype>
    struct TangentOperatorTraits {
      /*!
       * Most mechanical behaviour can only compute one tangent
       * operator. Finite strain beaviours are a noticeable exception.
       */
      enum SMFlag { STANDARDTANGENTOPERATOR };  // end of enum Flag
    };                                          // end of struct

    /*!
     * a trait class describing which tangent operators can be
     * computed by a mechanical behaviour.
     */
    template <>
    struct TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR>
        : public FiniteStrainBehaviourTangentOperatorBase {
      /*!
       * Most mechanical behaviour can only compute one tangent
       * operator. Finite strain beaviours are a noticeable exception.
       */
      typedef FiniteStrainBehaviourTangentOperatorBase::Flag SMFlag;
    };  // end of struct

    /*!
     * \class MechanicalBehaviour
     * \brief This class declares an interface for mechanical behaviours.
     * \param H, modelling hypothesis
     * \param NumType, numerical type.
     * \param bool use_qt, says if one shall use quantities.
     * \author Thomas Helfer
     * \date   28 Jul 2006
     */
    template <MechanicalBehaviourBase::BehaviourType btype,
              ModellingHypothesis::Hypothesis H,
              typename NumType,
              bool use_qt>
    struct MechanicalBehaviour : public TangentOperatorTraits<btype>,
                                 public MechanicalBehaviourBase {
      /*!
       * available tangent operator
       */
      using SMFlag = typename TangentOperatorTraits<btype>::SMFlag;
      /*!
       * dimension of the space for the the given modelling hypothesis
       */
      static constexpr unsigned short N =
          ModellingHypothesisToSpaceDimension<H>::value;
      /*!
       * \brief only compute a prediction stiffness matrix.
       * The result shall be retrieved through the
       * "getTangeOperator" method
       * \param[in] smflag : expected tangent operator
       * \param[in] smt    : expected tangent operator
       * \return SUCCESS if the integration is successfull.
       */
      virtual IntegrationResult computePredictionOperator(const SMFlag,
                                                          const SMType) = 0;
      /*!
       * \return the minimal scaling factor to be used. This scaling
       * factor is used to decrease the time step if the integration
       * failed.
       */
      virtual NumType getMinimalTimeStepScalingFactor() const = 0;
      /*!
       * \param[in] dt: time step scaling factor proposed by the calling code
       * \return a pair containing:
       * - a boolean syaing if the behaviour integration shall be
       *   performed
       * - a scaling factor that can be used to:
       *     - increase the time step if the integration was successfull
       *     - decrease the time step if the integration failed or if the
       *       results were not reliable (time step too large).
       *
       * Interfaces to behaviour law shall use the
       * hasAPrioriTimeStepScalingFactor of the traits class
       * MechanicalBehaviourTraits to know if the behaviour is able to
       * give such a time step scaling factor. If not, behaviours
       * may return the NumType(1) value.
       */
      virtual std::pair<bool, NumType> computeAPrioriTimeStepScalingFactor(
          const NumType) const = 0;
      /*!
       * \brief determine the value of the internal state variables at
       * the end of the time step
       * \param[in] smflag : expected tangent operator
       * \param[in] smt    : expected tangent operator
       * \return SUCCESS if the integration is successfull.
       */
      virtual IntegrationResult integrate(const SMFlag, const SMType) = 0;
      /*!
       * \param[in] dt: current time step scaling factor
       * \return a pair containing:
       * - a boolean syaing if the behaviour integration shall be
       *   considered a success or a failure
       * - a scaling factor that can be used to:
       *     - increase the time step if the integration was successfull
       *     - decrease the time step if the integration failed or if the
       *       results were not reliable (time step too large).
       *
       * Interfaces to behaviour law shall use the
       * hasAPosterioriTimeStepScalingFactor of the traits class
       * MechanicalBehaviourTraits to know if the behaviour is able to
       * give such a time step scaling factor. If not, behaviours
       * may return the NumType(1) value.
       */
      virtual std::pair<bool, NumType> computeAPosterioriTimeStepScalingFactor(
          const NumType) const = 0;
      /*!
       * destructor
       */
      virtual ~MechanicalBehaviour() {}
    };

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MECHANICALBEHAVIOUR_HXX */
