/*!
 * \file  mfront/include/MFront/ImplicitModelDSL.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_IMPLICITMODELDSL_HXX
#define LIB_MFRONT_IMPLICITMODELDSL_HXX

#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  /*!
   * \brief Implementation of a generic model
   */
  struct ImplicitModelDSL : public ImplicitDSLBase {
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    ImplicitModelDSL(const DSLOptions&);
    //! \return the name of the DSL
    static std::string getName();
    //! \return a description of the DSL
    static std::string getDescription();
    //! \return a description of the DSL
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    //! \brief destructor
    ~ImplicitModelDSL() noexcept override;
  };  // end of struct ImplicitModelDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_IMPLICITMODELDSL_HXX */
