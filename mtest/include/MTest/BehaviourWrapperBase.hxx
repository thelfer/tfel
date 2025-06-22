/*!
 * \file   BehaviourWrapperBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08/11/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_BEHAVIOURWRAPPERBASE_HXX
#define LIB_MTEST_BEHAVIOURWRAPPERBASE_HXX

#include <memory>
#include "MTest/Behaviour.hxx"

namespace mtest {

  /*!
   * \brief a simple helper class to build wrapper around mechanical behaviours.
   *
   * Following the facade design pattern, the wrapper forwards the calls to the
   * wrapped behaviour.
   */
  struct MTEST_VISIBILITY_EXPORT BehaviourWrapperBase : public Behaviour {
    /*!
     * \brief constructor
     * \param[in] wb : wrapped behaviour
     */
    BehaviourWrapperBase(const std::shared_ptr<Behaviour>&);
    //
    void allocateCurrentState(CurrentState&) const override;
    std::string getBehaviourName() const override;
    unsigned short getSymmetryType() const override;
    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;
    std::vector<std::string> getMaterialPropertiesNames() const override;
    size_t getMaterialPropertiesSize() const override;
    std::vector<std::string> getOptionalMaterialProperties() const override;
    void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;
    std::vector<std::string> getParametersNames() const override;
    std::vector<std::string> getIntegerParametersNames() const override;
    std::vector<std::string> getUnsignedShortParametersNames() const override;
    double getRealParameterDefaultValue(const std::string&) const override;
    int getIntegerParameterDefaultValue(const std::string&) const override;
    unsigned short getUnsignedShortParameterDefaultValue(
        const std::string&) const override;
    void setOutOfBoundsPolicy(
        const tfel::material::OutOfBoundsPolicy) const override;
    bool hasBounds(const std::string&) const override;
    bool hasLowerBound(const std::string&) const override;
    bool hasUpperBound(const std::string&) const override;
    long double getLowerBound(const std::string&) const override;
    long double getUpperBound(const std::string&) const override;
    bool hasPhysicalBounds(const std::string&) const override;
    bool hasLowerPhysicalBound(const std::string&) const override;
    bool hasUpperPhysicalBound(const std::string&) const override;
    long double getLowerPhysicalBound(const std::string&) const override;
    long double getUpperPhysicalBound(const std::string&) const override;
    void setParameter(const std::string&, const real) const override;
    void setIntegerParameter(const std::string&, const int) const override;
    void setUnsignedIntegerParameter(const std::string&,
                                     const unsigned short) const override;
    //! \brief destructor
    ~BehaviourWrapperBase() override;

   protected:
    //! \brief wrapped behaviour
    std::shared_ptr<Behaviour> b;
  };  // end of struct BehaviourWrapperBase

}  // end of namespace mtest

#endif /* LIB_MTEST_BEHAVIOURWRAPPERBASE_HXX */
