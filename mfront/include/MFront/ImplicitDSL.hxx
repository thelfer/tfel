/*!
 * \file  mfront/include/MFront/ImplicitDSL.hxx
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

#ifndef LIB_MFRONT_MFRONTIMPLICITPARSER_HXX
#define LIB_MFRONT_MFRONTIMPLICITPARSER_HXX

#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  /*!
   * \brief a DSL handling strain based behaviours integrated using an implicit
   * scheme.
   *
   * \note This DSL automatically defines the elastic strain as the first state
   * variable (which is the main difference with the `ImplicitII` DSL).
   */
  struct ImplicitDSL : public ImplicitDSLBase {
    //! \brief return the name of the DSL
    static std::string getName();
    //! \brief return a short description of the DSL
    static std::string getDescription();
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    ImplicitDSL(const DSLOptions&);
    //
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    //! \brief  destructor
    ~ImplicitDSL() override;
  };  // end of struct ImplicitDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTIMPLICITPARSER_HXX */
