/*!
 * \file   StudyCurrentState.hxx
 * \brief
 * \author Thomas Helfer
 * \date   11/11/2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_STUDYCURRENTSTATE_HXX
#define LIB_MTEST_STUDYCURRENTSTATE_HXX

#include <map>
#include <string>
#include <memory>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/vector.hxx"
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/StudyParameter.hxx"

namespace mtest {

  // forward declaration
  struct Evolution;
  // forward declaration
  struct StructureCurrentState;

  /*!
   * structure describing the current state of a study
   */
  struct MTEST_VISIBILITY_EXPORT StudyCurrentState {
    //! a simple alias
    using size_type = tfel::math::vector<real>::size_type;
    //! default constructor
    StudyCurrentState();
    StudyCurrentState(const StudyCurrentState&);
    StudyCurrentState(StudyCurrentState&&);
    StudyCurrentState& operator=(const StudyCurrentState&);
    StudyCurrentState& operator=(StudyCurrentState&&);
    ~StudyCurrentState();
    // vector of unknows at
    // the beginning of the
    // previous time step.
    tfel::math::vector<real> u_1;
    // vector of unknows at the beginning of the
    // time step.
    tfel::math::vector<real> u0;
    // vector of unknows at the end of the
    // time step
    tfel::math::vector<real> u1;
    // vector of unknows at the previous iteration (end of the
    // time step)
    tfel::math::vector<real> u10;
    // current period number
    unsigned int period = 1u;
    // number of iterations made so far
    unsigned int iterations = 0u;
    // number of sub-steppings so far
    unsigned int subSteps = 0u;
    // previous time step
    real dt_1 = real{};
    /*!
     * \brief allocate memory
     * \param[out] psz: problem size
     */
    void initialize(const StudyCurrentState::size_type);
    /*!
     * \brief update the state: values at the end of the time step
     * becomes the values at the beginning of the next time step
     * \param[in] dt: time increment
     */
    void update(const real);
    /*!
     * \brief revert the state: values at the end of the time step are
     * set equal to the values at the beginning of the next time step
     */
    void revert();
    /*!
     * \return the state associated with a structure
     * \param[in] n: structure name
     */
    StructureCurrentState& getStructureCurrentState(const std::string&);
    /*!
     * \return the state associated with a structure
     * \param[in] n: structure name
     */
    const StructureCurrentState& getStructureCurrentState(
        const std::string&) const;
    /*!
     * \param[in] n: parameter name
     */
    bool containsParameter(const std::string&) const;
    /*!
     * \return true if the study parameter with the specified name has the
     * specified type
     * \tparam T    : requested type
     * \param[in] n : the requested name
     * \note an exception is thrown if no data of the specified name
     * is found or if the object type has not the requested type
     */
    template <typename T>
    bool checkParameterType(const std::string&) const;
    /*!
     * \return the object holded by the study parameter with the
     * specified name
     * \tparam T    : requested type
     * \param[in] n : the requested name
     * \param[in] b : create the object if it does not exists
     * \note an exception is thrown if no data of the specified name
     * is found or if the object type has not the requested type
     */
    template <typename T>
    T& getParameter(const std::string&, const bool = false);
    /*!
     * \return the object holded by the study parameter with the
     * specified name
     * \tparam T     : requested type
     * \param[in] n : the requested name
     * \note an exception is thrown if no data of the specified name
     * is found or if the object type has not the requested type
     */
    template <typename T>
    const T& getParameter(const std::string&) const;
    /*!
     * \brief set a parameter
     * \tparam T    : type of the parameter
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     * \note an exception is thrown if a data of the specified name
     * is found
     */
    template <typename T>
    T& setParameter(const std::string&, const T&);
    /*!
     * \param[in] n : name of the evolution
     */
    bool containsEvolution(const std::string&) const;
    /*!
     * \brief add a new evolution
     * \param[in] n : name of the evolution
     * \param[in] e : evolution
     */
    void addEvolution(const std::string&, std::shared_ptr<Evolution>);
    /*!
     * \return the evolution with the given name
     * \param[in] n  : name of the evolution
     */
    Evolution& getEvolution(const std::string&);
    /*!
     * \return the evolution with the given name
     * \param[in] n  : name of the evolution
     */
    const Evolution& getEvolution(const std::string&) const;
    //! \return if any of the failure criterion is true
    bool getFailureStatus() const;
    /*!
     * \return the status of a failure criterion
     * \param[in] i: index of the failure criterion
     */
    bool getFailureCriterionStatus(const std::size_t) const;
    /*!
     * \brief set the status of a failure criterion
     * \param[in] i: index of the failure criterion
     * \param[in] status: new status
     */
    void setFailureCriterionStatus(const std::size_t, const bool);
    /*!
     * \brief set the number of failure criterion status
     * \param[in] n: number of the failure criteria
     * \note the newly allocated status are initialized to `false`
     */
    void setNumberOfFailureCriterionStatus(const std::size_t);
    //! \return the number of failure criterion status
    std::size_t getNumberOfFailureCriterionStatus() const noexcept;

   protected:
    /*!
     * \brief throw an exception stating that no parameter with the
     * given name has been found
     * \param[in] n: parameter name
     */
    [[noreturn]] static void throwUnknownParameterException(const std::string&);
    /*!
     * \brief throw an exception stating that a parameter with the
     * given name has been found
     * \param[in] n: parameter name
     */
    [[noreturn]] static void throwAlreadyDeclaredParameterException(
        const std::string&);
    //! \brief study parameters
    std::map<std::string, StudyParameter> parameters;
    /*!
     * \brief current state of the structures indexed by the structure's
     * names
     */
    std::map<std::string, std::shared_ptr<StructureCurrentState>> s;
    //! \brief evolutions
    std::map<std::string, std::shared_ptr<Evolution>> evs;
    //! \brief failure criteria status
    std::vector<bool> failure_criterion_status;
  };  // end of struct StudyCurrentState

}  // end of namespace mtest

#include "MTest/StudyCurrentState.ixx"

#endif /* LIB_MTEST_STUDYCURRENTSTATE_HXX */
