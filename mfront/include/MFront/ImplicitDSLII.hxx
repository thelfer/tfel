/*!
 * \file  mfront/include/MFront/ImplicitDSLII.hxx
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

#ifndef LIB_MFRONT_MFRONTIMPLICITPARSERII_HXX
#define LIB_MFRONT_MFRONTIMPLICITPARSERII_HXX

#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  /*!
   * \brief a DSLII handling strain base behaviours integrated using an
   * implicit scheme.
   */
  struct ImplicitDSLII : public ImplicitDSLBase {
    //! \brief return the name of the DSLII
    static std::string getName();
    //! \brief return a short description of the DSLII
    static std::string getDescription();
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    ImplicitDSLII(const DSLOptions&);
    //
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    //! \brief  destructor
    ~ImplicitDSLII() override;
  };  // end of struct ImplicitDSLII

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTIMPLICITPARSERII_HXX */
