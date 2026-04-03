/*!
 * \file   mfront/include/MFront/ImplicitCZMDSL.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTIMPLICITCZMDSL_HXX
#define LIB_MFRONT_MFRONTIMPLICITCZMDSL_HXX

#include <string>
#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  /*!
   * \brief DSL associated with Cohesive Zone Models
   */
  struct ImplicitCZMDSL : public ImplicitDSLBase {
    //! \brief return the name of the DSL
    static std::string getName();
    //! \brief return a short description of the DSL
    static std::string getDescription();
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    ImplicitCZMDSL(const DSLOptions&);
    //
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    //! \brief  destructor
    ~ImplicitCZMDSL() override;

    //    protected:
    //     std::string getLocalVariablesInitializers(const Hypothesis) const
    //     override;
    //
    //     void writeBehaviourParserSpecificIncludes(std::ostream&) const
    //     override;

  };  // end of struct ImplicitCZMDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTIMPLICITCZMDSL_HXX */
