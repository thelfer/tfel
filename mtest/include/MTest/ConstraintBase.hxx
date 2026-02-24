/*!
 * \file  mtest/include/MTest/ConstraintBase.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTCONSTRAINTBASE_HXX
#define LIB_MTEST_MTESTCONSTRAINTBASE_HXX

#include "MTest/Constraint.hxx"

namespace mtest {

  /*!
   * \brief Base class for the constraints imposed to a single mechanical point
   * which handles all the details about the active state and associated events.
   */
  struct MTEST_VISIBILITY_EXPORT ConstraintBase : public Constraint,
                                                  private ConstraintOptions {
    /*!
     * \brief treat the given event
     * \param[in] e :event
     */
    bool treatEvent(const std::string&) override;
    /*!
     * \brief set the activating events
     * \param[in] evs: list of events
     */
    void setActivatingEvents(const std::vector<std::string>&) override;
    /*!
     * \brief set the desactivating events
     * \param[in] evs: list of events
     */
    void setDesactivatingEvents(const std::vector<std::string>&) override;
    /*!
     * \brief activate or desactivate the constraint
     * \param[in] b: state
     */
    void setActive(const bool) override;
    //! \return if the constraint is active
    bool isActive() const override;
    //! destructor
    ~ConstraintBase() override;
  };  // end of struct ConstraintBase

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTCONSTRAINTBASE_HXX */
