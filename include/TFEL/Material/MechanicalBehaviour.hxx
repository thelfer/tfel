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

#ifndef LIB_TFEL_MATERIAL_MECHANICALBEHAVIOUR_HXX
#define LIB_TFEL_MATERIAL_MECHANICALBEHAVIOUR_HXX

#include <utility>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperatorBase.hxx"

namespace tfel::material {

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
     * - the only driving variable is the total strain 'eto' (symmetric tensor)
     * - the only thermodynamic force is the stress    'sig' (symmetric tensor)
     * If the behaviour is a cohesive zone model, then:
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
    enum ExitStatus {
      FAILURE = 0,        //<! Integration failed
      SUCCESS = 1,        //<! Integration is a success
      UNRELIABLE_RESULTS  //<! Integration succeed, but one or more
                          //   internal criteria show that the
                          //   results may be inaccurate
    };
    /*!
     * \brief a small structure representing the result of the behaviour
     * integration
     * See Issue #296 for the rationale behind this class.
     * <https://github.com/thelfer/tfel/issues/296>
     */
    struct IntegrationResult {
      //! \brief default constructor
      constexpr IntegrationResult() noexcept : status(SUCCESS) {}
#ifndef __clang__
      //! \brief move constructor
      constexpr IntegrationResult(IntegrationResult&&) noexcept = default;
      //! \brief default constructor
      constexpr IntegrationResult(const IntegrationResult&) noexcept = default;
      //! \brief move assignement
      constexpr IntegrationResult& operator=(IntegrationResult&&) noexcept =
          default;
      //! \brief standard assignement
      constexpr IntegrationResult& operator=(
          const IntegrationResult&) noexcept = default;
#else
      //! \brief move constructor
      IntegrationResult(IntegrationResult&& src) noexcept
          : status(src.status) {}
      //! \brief default constructor
      IntegrationResult(const IntegrationResult& src) noexcept
          : status(src.status) {}
      //! \brief move assignement
      IntegrationResult& operator=(IntegrationResult&& src) noexcept {
        this->status = src.status;
        return *this;
      }
      //! \brief standard assignement
      IntegrationResult& operator=(const IntegrationResult& src) noexcept {
        this->status = src.status;
        return *this;
      }
#endif
      /*!
       * \brief constructor from an ExitStatus
       */
      constexpr IntegrationResult(const ExitStatus s) noexcept
          : status(s) {}  // end of IntegrationResult
      /*!
       * \brief constructor from a boolean value
       */
      constexpr IntegrationResult(const bool b) noexcept
          : status(b ? SUCCESS : FAILURE) {}  // end of IntegrationResult

      //! \brief convertion operator to the ExistStatus enumeration
      constexpr operator ExitStatus() const noexcept { return this->status; }
      //! \brief destructor
      inline ~IntegrationResult() noexcept = default;

     private:
      ExitStatus status;
    };  // end of enum IntegrationResult
    //! \brief list of possible tangent operator type
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
    //! \brief available tangent operator
    using SMFlag = typename TangentOperatorTraits<btype>::SMFlag;
    /*!
     * \brief dimension of the space for the the given modelling hypothesis
     */
    static constexpr unsigned short N =
        ModellingHypothesisToSpaceDimension<H>::value;
    //! \brief a simple alias
    using TFELTypes = tfel::config::Types<N, NumType, use_qt>;
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
    virtual typename TFELTypes::real getMinimalTimeStepScalingFactor()
        const = 0;
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
    virtual std::pair<bool, typename TFELTypes::real>
    computeAPrioriTimeStepScalingFactor(
        const typename TFELTypes::real) const = 0;
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
    virtual std::pair<bool, typename TFELTypes::real>
    computeAPosterioriTimeStepScalingFactor(
        const typename TFELTypes::real) const = 0;
    //! destructor
    virtual ~MechanicalBehaviour() noexcept = default;
  };

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_MECHANICALBEHAVIOUR_HXX */
